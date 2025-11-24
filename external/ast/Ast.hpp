#pragma once

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <utility>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace ast {

    using namespace rapidjson;

    // ====================== JSONValue ======================

    struct JSONValue;
    using JSONArray  = std::vector<JSONValue>;
    using JSONObject = std::map<std::string, JSONValue>;

    /**
     * Represents a dynamic JSON-like value that can store:
     * - string
     * - double
     * - bool
     * - array (JSONArray)
     * - object (JSONObject)
     */
    struct JSONValue {
        std::variant<std::string, double, bool, JSONArray, JSONObject> value;

        JSONValue() = default;
        JSONValue(const char* s) : value(std::string(s)) {}
        JSONValue(const std::string& s) : value(s) {}
        JSONValue(double d) : value(d) {}
        JSONValue(bool b) : value(b) {}
        JSONValue(const JSONArray& arr) : value(arr) {}
        JSONValue(const JSONObject& obj) : value(obj) {}
    };

    // Recursive serialization for JSONValue
    inline void to_json_value(const JSONValue& jv, Value& out, Document::AllocatorType& alloc) {
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>)
                out.SetString(arg.c_str(), alloc);
            else if constexpr (std::is_same_v<T, double>)
                out.SetDouble(arg);
            else if constexpr (std::is_same_v<T, bool>)
                out.SetBool(arg);
            else if constexpr (std::is_same_v<T, JSONArray>) {
                out.SetArray();
                for (const auto& el : arg) {
                    Value item;
                    to_json_value(el, item, alloc);
                    out.PushBack(item, alloc);
                }
            } else if constexpr (std::is_same_v<T, JSONObject>) {
                out.SetObject();
                for (const auto& [k, v] : arg) {
                    Value key(k.c_str(), alloc);
                    Value val;
                    to_json_value(v, val, alloc);
                    out.AddMember(key, val, alloc);
                }
            }
        }, jv.value);
    }

    // ====================== Node AST ======================

    struct Node {
        std::string type;
        JSONObject props;
        std::vector<Node> children;
    };

    // ---------- Constructors ----------

    inline Node element(
        const std::string& type,
        std::vector<Node> children = {},
        JSONObject props = {}
    ) {
        return Node{type, std::move(props), std::move(children)};
    }

    inline Node text(const std::string& value) {
        return Node{"#text", {{"value", JSONValue(value)}}, {}};
    }

    // ---------- TAG macro ----------

    #define TAG(name) \
    inline Node name(std::vector<Node> children = {}, JSONObject props = {}) { \
        return element(#name, std::move(children), std::move(props)); \
    }

        // ---------- basic HTML tags ----------

        TAG(div)
        TAG(span)
        TAG(p)
        TAG(h1)
        TAG(h2)
        TAG(h3)
        TAG(h4)
        TAG(h5)
        TAG(h6)
        TAG(ul)
        TAG(ol)
        TAG(li)
        TAG(table)
        TAG(thead)
        TAG(tfoot)
        TAG(tbody)
        TAG(tr)
        TAG(td)
        TAG(th)
        TAG(img)
        TAG(a)
        TAG(button)
        TAG(input)
        TAG(label)
        TAG(form)
        TAG(select)
        TAG(option)
        TAG(section)
        TAG(article)
        TAG(header)
        TAG(footer)
        TAG(main)
        TAG(nav)
        TAG(svg)
        TAG(path)
        TAG(rect)
        TAG(circle)
        TAG(line)
        TAG(g)

        // ---------- Recharts tags ----------

        TAG(ResponsiveContainer)
        TAG(LineChart)
        TAG(BarChart)
        TAG(PieChart)
        TAG(AreaChart)
        TAG(XAxis)
        TAG(YAxis)
        TAG(ZAxis)
        TAG(Tooltip)
        TAG(Legend)
        TAG(Line)
        TAG(Bar)
        TAG(Pie)
        TAG(Area)
        TAG(Cell)
        TAG(CartesianGrid)
        TAG(Brush)
        TAG(ReferenceLine)
        TAG(ReferenceDot)
        TAG(ComposedChart)
        TAG(ScatterChart)
        TAG(Scatter)
        TAG(RadarChart)
        TAG(Radar)
        TAG(PolarGrid)
        TAG(PolarAngleAxis)
        TAG(PolarRadiusAxis)

    // ---------- Props helper ----------

    inline JSONObject props(std::initializer_list<std::pair<const std::string, JSONValue>> kv) {
        return JSONObject(kv);
    }

    // ---------- Serialization ----------

    inline void to_json(const Node& node, Value& out, Document::AllocatorType& alloc) {
        out.SetObject();
        out.AddMember("type", Value(node.type.c_str(), alloc), alloc);

        if (!node.props.empty()) {
            Value propsObj(kObjectType);
            for (auto& [k, v] : node.props) {
                Value key(k.c_str(), alloc);
                Value val;
                to_json_value(v, val, alloc);
                propsObj.AddMember(key, val, alloc);
            }
            out.AddMember("props", propsObj, alloc);
        }

        if (!node.children.empty()) {
            Value childrenArr(kArrayType);
            for (auto& c : node.children) {
                Value child(kObjectType);
                to_json(c, child, alloc);
                childrenArr.PushBack(child, alloc);
            }
            out.AddMember("children", childrenArr, alloc);
        }
    }

    // ---------- stringify ----------

    inline std::string stringify(const Node& node) {
        Document doc;
        doc.SetObject();
        auto& alloc = doc.GetAllocator();
        to_json(node, doc, alloc);
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        doc.Accept(writer);
        return buffer.GetString();
    }

    // ---------- none helper ----------

    inline std::vector<Node> none() { return {}; }
}