#pragma once

#include <string>
#include <vector>
#include <utility>
#include <optional>
#include "ast/Ast.hpp"

using namespace ast;

// Тип фильтра - определяет внешний вид и поведение фильтра на UI
enum class FilterType {
    Search,              // Текстовый поиск
    Select,              // Выпадающий список
    Date,                // Дата
    DateTime,            // Дата + время
    DateTimeSec,         // Дата + время с секундами
    DateInput,           // Дата + поле ввода
    DateTimeInput,       // Дата + время + поле ввода
    DateTimeSecInput     // Дата + время с секундами + поле ввода
};

// Тип операции поиска, применяемой фильтром
enum class SearchType {
    Like,                // Частичное совпадение
    Equal,               // Равно
    NotEqual,            // Не равно
    Between,             // В диапазоне
    Outside,             // Вне диапазона
    Below,               // Меньше
    BelowOrEqual,        // Меньше или равно
    Above,               // Больше
    AboveOrEqual,        // Больше или равно
    Select,              // Выбор указанных значений
    SelectExcept         // Выбор всех, кроме указанных
};

// Тип данных, с которыми работает фильтр
enum class FilterMode {
    String,              // Строковые значения
    Number               // Числовые значения
};

// Опция для фильтра с типом Select
struct FilterOption {
    std::string text;   // Отображаемый текст
    std::string value;  // Значение, используемое в логике
};

// Конфигурация фильтра колонки
struct FilterConfig {
    // Тип фильтра (обязательное поле)
    FilterType type = FilterType::Search;

    // Тип операции поиска
    std::optional<SearchType> search_type;

    // Режим обработки значений (строка / число)
    std::optional<FilterMode> mode;

    // Список опций для select-фильтра
    std::vector<FilterOption> options;

    // Ключ значения для поиска в объекте опции
    std::optional<std::string> search_option_key;

    // Настройки виртуализации списка опций
    std::optional<bool> is_virtualized_options;
    std::optional<double> virtualized_options_height;
    std::optional<double> virtualized_option_height;

    // Дополнительные флаги поведения фильтра
    std::optional<bool> is_exact;        // Точное совпадение
    std::optional<bool> is_return_unix;  // Возвращать дату в UNIX-формате
};

// Конфигурация одной колонки таблицы
struct TableColumn {
    std::string key;                    // Ключ колонки
    std::string language_token;         // Языковой токен
    double order = 0.0;                 // Дефолтное значение для порядка отображения колонки
    std::optional<FilterConfig> filter; // Конфигурация фильтра (может отсутствовать)
    bool is_exported = true;            // Участие в экспорте (может отсутствовать)
    bool is_sorted = true;              // Доступна ли сортировка (может отсутствовать)
};

// Основной класс для пошаговой сборки JSON-описания таблицы
class TableBuilder {
public:
    explicit TableBuilder(const std::string& table_name)
        : _table_name(table_name) {}

    void AddColumn(const TableColumn& column) {
        _column_order_by_keys.push_back(column.key);

        JSONObject column_obj;
        column_obj["name"] = column.language_token;
        column_obj["order"] = column.order;
        column_obj["export"] = column.is_exported;
        column_obj["sort"] = column.is_sorted;

        if (column.filter) {
            column_obj["filter"] = ConvertFilterToJson(*column.filter);
        }

        _structure[column.key] = std::move(column_obj);
    }

    void AddRow(const std::vector<JSONValue>& row_values) {
        JSONArray json_row;
        json_row.reserve(row_values.size());

        for (const auto& val : row_values) {
            json_row.push_back(val);
        }

        _rows.push_back(std::move(json_row));
    }

    void SetIdColumn(const std::string& id_column) { _id_column = id_column; }

    void SetOrderBy(const std::string& column, const std::string& order = "DESC") {
        _order_by = {column, order};
    }

    void EnableAutoSave(const bool& enabled) { _is_total_row_enabled = enabled; }

    void EnableRefreshButton(const bool& enabled) { _is_refresh_button_enabled = enabled; }

    void EnableBookmarksButton(const bool& enabled) { _is_bookmarks_button_enabled = enabled; }

    void EnableExportButton(const bool& enabled) { _is_export_button_enabled = enabled; }

    void EnableTotal(const bool& enabled) { _is_total_row_enabled = enabled; }

    void SetTotalDataTitle(const std::string& title) { _total_data_title = title; }

    void SetTotalData(const JSONArray& total_data) { _total_data = total_data; }

    [[nodiscard]] JSONObject CreateTableProps() const {
        JSONObject table_props;
        table_props["name"] = _table_name;
        table_props["idCol"] = _id_column;
        table_props["orderBy"] = JSONArray{_order_by.first, _order_by.second};
        table_props["autoSave"] = _is_auto_save_enabled;
        table_props["showRefreshBtn"] = _is_refresh_button_enabled;
        table_props["showBookmarksBtn"] = _is_bookmarks_button_enabled;
        table_props["showExportBtn"] = _is_export_button_enabled;
        table_props["showTotal"] = _is_total_row_enabled;
        table_props["totalDataTitle"] = _total_data_title;

        if (!_total_data.empty()) {
            table_props["totalData"] = _total_data;
        }

        JSONObject data_obj;
        JSONArray json_rows;
        json_rows.reserve(_rows.size());

        for (const auto& row : _rows) {
            json_rows.emplace_back(row);
        }

        data_obj["rows"] = std::move(json_rows);


        JSONArray structure_keys;
        structure_keys.reserve(_column_order_by_keys.size());

        for (const auto& key : _column_order_by_keys) {
            structure_keys.emplace_back(key);
        }

        data_obj["structure"] = std::move(structure_keys);
        table_props["data"] = std::move(data_obj);
        table_props["structure"] = _structure;

        return table_props;
    }

private:
    std::string _table_name;
    std::string _id_column;
    std::vector<std::string> _column_order_by_keys;
    std::vector<JSONArray> _rows;
    JSONObject _structure;
    std::pair<std::string, std::string> _order_by{"id", "DESC"};
    bool _is_auto_save_enabled = false;
    bool _is_refresh_button_enabled = true;
    bool _is_bookmarks_button_enabled = true;
    bool _is_export_button_enabled = true;
    bool _is_total_row_enabled = false;
    std::string _total_data_title;
    JSONArray _total_data;

    static JSONObject ConvertFilterToJson(const FilterConfig& filter_config) {
        JSONObject json_object;
        json_object["type"] = ConvertFilterTypeToString(filter_config.type);

        if (filter_config.search_type) {
            json_object["search_type"] = ConvertSearchTypeToString(static_cast<SearchType>(*filter_config.search_type));
        }

        if (filter_config.mode) {
            json_object["mode"] = (*filter_config.mode == FilterMode::Number ? "number" : "string");
        }

        if (!filter_config.options.empty()) {
            JSONArray options_array;
            for (const auto& option : filter_config.options) {
                JSONObject option_obj;
                option_obj["text"] = option.text;
                option_obj["value"] = option.value;
                options_array.emplace_back(option_obj);
            }
            json_object["options"] = std::move(options_array);
        }

        if (filter_config.search_option_key) json_object["search_option_key"] = *filter_config.search_option_key;
        if (filter_config.is_virtualized_options) json_object["is_virtualized_options"] = *filter_config.is_virtualized_options;
        if (filter_config.virtualized_options_height) json_object["virtualized_options_height"] = *filter_config.virtualized_options_height;
        if (filter_config.virtualized_option_height) json_object["virtualized_option_height"] = *filter_config.virtualized_option_height;
        if (filter_config.is_exact) json_object["is_exact"] = *filter_config.is_exact;
        if (filter_config.is_return_unix) json_object["is_return_unix"] = *filter_config.is_return_unix;

        return json_object;
    }

    static std::string ConvertFilterTypeToString(const FilterType filter_type) {
        switch (filter_type) {
            case FilterType::Search: return "search";
            case FilterType::Select: return "select";
            case FilterType::Date: return "date";
            case FilterType::DateTime: return "date-time";
            case FilterType::DateTimeSec: return "date-time-sec";
            case FilterType::DateInput: return "date-input";
            case FilterType::DateTimeInput: return "date-time-input";
            case FilterType::DateTimeSecInput: return "date-time-sec-input";
        }
        return "search";
    }

    static std::string ConvertSearchTypeToString(const SearchType search_type) {
        switch (search_type) {
            case SearchType::Like: return "like";
            case SearchType::Equal: return "equal";
            case SearchType::NotEqual: return "not_equal";
            case SearchType::Between: return "between";
            case SearchType::Outside: return "outside";
            case SearchType::Below: return "below";
            case SearchType::BelowOrEqual: return "below_or_equal";
            case SearchType::Above: return "above";
            case SearchType::AboveOrEqual: return "above_or_equal";
            case SearchType::Select: return "select";
            case SearchType::SelectExcept: return "select_except";
        }
        return "like";
    }
};
