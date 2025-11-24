#include "PluginInterface.hpp"

#include <iomanip>

using namespace ast;

extern "C" void AboutReport(rapidjson::Value& request,
                            rapidjson::Value& response,
                            rapidjson::Document::AllocatorType& allocator,
                            CServerInterface* server) {
    response.AddMember("version", 1, allocator);
    response.AddMember("name", Value().SetString("Daily Trades report", allocator), allocator);
    response.AddMember("description",
        Value().SetString("Trading operations of selected trader groups for the selected day. "
                           "Includes profit and loss graphs and detailed information about all performed deals and open positions.",
             allocator), allocator);
    response.AddMember("type", REPORT_DAILY_GROUP_TYPE, allocator);
}

extern "C" void DestroyReport() {}

extern "C" void CreateReport(rapidjson::Value& request,
                             rapidjson::Value& response,
                             rapidjson::Document::AllocatorType& allocator,
                             CServerInterface* server) {
    // Структура накопления итогов
    struct Total {
        double balance;
        std::string currency;
    };

    std::unordered_map<std::string, Total> totals_map;

    std::string group_mask;
    int from;
    int to;
    if (request.HasMember("group") && request["group"].IsString()) {
        group_mask = request["group"].GetString();
    }
    if (request.HasMember("from") && request["from"].IsNumber()) {
        from = request["from"].GetInt();
    }
    if (request.HasMember("to") && request["to"].IsNumber()) {
        to = request["to"].GetInt();
    }

    std::cout << "Group mask: " << group_mask << std::endl;
    std::cout << "from: " << from << std::endl;
    std::cout << "to: " << to << std::endl;

    std::vector<TradeRecord> trades_vector;
    std::vector<GroupRecord> groups_vector;

    try {
        server->GetTransactionsByGroup(group_mask, from, to, &trades_vector);
        server->GetAllGroups(&groups_vector);
    } catch (const std::exception& e) {
        std::cout << "[DepositWithdrawalReportInterface]: " << e.what() << std::endl;
    }

    std::cout << "Trades size: " << trades_vector.size() << std::endl;
    std::cout << "Groups size: " << groups_vector.size() << std::endl;

    // Лямбда подготавливающая значения double для вставки в AST (округление до 2-х знаков)
    auto format_for_AST = [](double value) -> std::string {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        return oss.str();
    };

    auto make_table = [&](const std::vector<TradeRecord>& trades) -> Node {
        std::vector<Node> thead_rows;
        std::vector<Node> tbody_rows;
        std::vector<Node> tfoot_rows;

        // Thead
        thead_rows.push_back(tr({
            th({div({text("Order")})}),
            th({div({text("Login")})}),
            th({div({text("Name")})}),
            th({div({text("Time")})}),
            th({div({text("Comment")})}),
            th({div({text("Amount")})}),
            th({div({text("Currency")})}),
        }));

        for (const auto& trade : trades_vector) {
            if (trade.cmd == OP_BALANCE_IN || trade.cmd == OP_BALANCE_OUT) {
                tbody_rows.push_back(tr({
                    td({div({text(std::to_string(trade.order))})}),
                    td({div({text(std::to_string(trade.login))})}),
                    td({div({text("NAME")})}),
                    td({div({text(std::to_string(trade.timestamp))})}),
                    td({div({text(trade.comment)})}),
                    td({div({text(format_for_AST(trade.profit))})}),
                    td({div({text("CURRENCY")})}),
                }));
            }
        }

        return table({
            thead(thead_rows),
            tbody(tbody_rows),
            tfoot(tfoot_rows),
        }, props({{"className", "table"}}));
    };

    const Node report = div({
        h1({ text("Deposit Withdrawal Report") }),
        make_table(trades_vector),
    });

    utils::CreateUI(report, response, allocator);
}