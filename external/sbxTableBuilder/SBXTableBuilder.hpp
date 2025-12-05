#pragma once

#include <string>
#include <vector>
#include <map>
#include "ast/Ast.hpp"

using namespace ast;

struct TableColumn {
    std::string key;
    std::string language_token;
    bool is_sortable = true;
    bool is_exportable = true;
    std::string filter_type = "search";

    [[nodiscard]] JSONValue ToJSON() const {
        // TODO: фильтр надо будет расширять
        JSONObject filter = {{"type", filter_type}};
        JSONObject column_props = {
            {"name", language_token},
            {"filter", filter},
            {"sort", is_sortable},
            {"export", is_exportable}
        };

        return column_props;
    }
};

class TableBuilder {
public:
    TableBuilder(const std::string& table_name) : _table_name(table_name) {}

    void AddColumn(const TableColumn& column) {
        _columns[column.key] = column.ToJSON();
    }

    void AddRow(const std::map<std::string, std::string>& row_data) {
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

    void EnableExportButton(const bool& enabled = true) {
        _show_export_button = enabled;
    }

    void EnableRefreshButton(const bool& enabled = true) {
        _show_refresh_button = enabled;
    }

    void EnableBookmarksButton(const bool& enabled = true) {
        _show_export_button = enabled;
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
            {"showExportBtn", _show_export_button},
            {"structure", structure}
        };

        return table_props;
    }

private:
    std::string _table_name;
    std::string _id_column;
    std::map<std::string, JSONValue> _columns;
    JSONArray _data;
    std::vector <std::string> _order_by = {"id", "DESC"};
    bool _show_refresh_button = true;
    bool _show_bookmarks_button = true;
    bool _show_export_button = true;
};

