#pragma once

#include <string>
#include <vector>
#include <map>
#include "ast/Ast.hpp"

using namespace ast;

struct TableColumn {
    std::string key;
    std::string language_token;
    double order;

    bool is_sortable = true;
    bool is_exportable = true;
    std::string filter_type = "search";

    std::map<std::string, JSONValue> extra_props;
    std::map<std::string, JSONValue> filter_props;


    void AddColumnProp(const std::string& name, const JSONValue& value) {
        extra_props[name] = value;
    }

    void AddFilterProp(const std::string& name, const JSONValue& value) {
        filter_props[name] = value;
    }

    [[nodiscard]] JSONValue ToJSON() const {
        // TODO: фильтр надо будет расширять
        JSONObject filter;

        // Базовые фильтры
        filter["type"] = filter_type;

        // Дополнительные фильтры
        for (const auto& [k, v] : filter_props) {
            filter[k] = v;
        }

        JSONObject column_props = {
            {"name", language_token},
            {"filter", filter},
            {"order", order},
            {"sort", is_sortable},
            {"export", is_exportable}
        };

        // Дополнительные свойства колонки
        for (const auto& [k, v] : extra_props) {
            column_props[k] = v;
        }

        return column_props;
    }
};

class TableBuilder {
public:
    TableBuilder(const std::string& table_name) : _table_name(table_name) {}

    void AddColumn(const TableColumn& column) {
        _columns[column.key] = column.ToJSON();
    }

    void AddRow(const std::map<std::string, JSONValue>& row_data) {
        JSONObject row;
        for (const auto& [key, value] : row_data) {
            row[key] = value;
        }
        _data.emplace_back(row);
    }

    void SetIdColumn(const std::string& id_column) {
        _id_column = id_column;
    }

    void SetOrderBy(const std::string& column, const std::string& order = "DESC") {
        _order_by = {column, order};
    }

    void SetTotalData(const JSONArray& total_data) {
        _total_data = total_data;
    }

    void SetTotalDataTitle(const std::string& total_data_title) {
        _total_data_title = total_data_title;
    }

    void EnableExportButton(const bool& enabled = true) {
        _show_export_button = enabled;
    }

    void EnableRefreshButton(const bool& enabled = true) {
        _show_refresh_button = enabled;
    }

    void EnableBookmarksButton(const bool& enabled = true) {
        _show_export_button = enabled;
    }

    void EnableTotal(const bool& enabled = true) {
        _show_total = enabled;
    }

    [[nodiscard]] JSONObject CreateTableProps() const {
        JSONObject structure;
        for (const auto& [key, value] : _columns) {
            structure[key] = value;
        }

        JSONObject table_props = {
            {"name", _table_name},
            {"idCol", _id_column},
            {"data", _data},
            {"orderBy", JSONArray{_order_by[0], _order_by[1]}},
            {"showRefreshBtn", _show_refresh_button},
            {"showBookmarksBtn", _show_bookmarks_button},
            {"showTotal", _show_total},
            {"totalDataTitle", _total_data_title},
            {"showExportBtn", _show_export_button},
            {"structure", structure},
        };

        if (!_total_data.empty()) {
            table_props["totalData"] = _total_data;
        }

        return table_props;
    }

private:
    std::string _table_name;
    std::string _id_column;
    std::map<std::string, JSONValue> _columns;
    JSONArray _data;
    JSONArray _total_data;
    std::vector <std::string> _order_by = {"id", "DESC"};
    bool _show_refresh_button = true;
    bool _show_bookmarks_button = true;
    bool _show_export_button = true;
    bool _show_total = false;
    std::string _total_data_title;
};

