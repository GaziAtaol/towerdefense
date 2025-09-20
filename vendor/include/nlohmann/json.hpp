#pragma once

// Minimal compatible subset of nlohmann::json implemented for offline use.
// Supports parsing of objects, arrays, strings, numbers, booleans and null.
// Provides interface used within the TowerDefense project.

#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace nlohmann {

class json {
public:
    using object_t = std::map<std::string, json>;
    using array_t = std::vector<json>;
    using string_t = std::string;
    using boolean_t = bool;
    using number_integer_t = std::int64_t;
    using number_unsigned_t = std::uint64_t;
    using number_float_t = double;
    using value_t = std::variant<std::nullptr_t, boolean_t, number_integer_t,
                                 number_float_t, string_t, array_t, object_t>;

    json() : data_(nullptr) {}
    json(std::nullptr_t) : data_(nullptr) {}
    json(boolean_t b) : data_(b) {}
    json(number_integer_t i) : data_(i) {}
    json(number_unsigned_t u) : data_(static_cast<number_integer_t>(u)) {}
    json(number_float_t d) : data_(d) {}
    json(const char* s) : data_(string_t(s)) {}
    json(string_t s) : data_(std::move(s)) {}
    json(array_t arr) : data_(std::move(arr)) {}
    json(object_t obj) : data_(std::move(obj)) {}

    json(std::initializer_list<std::pair<const std::string, json>> init)
        : data_(object_t{}) {
        object_t obj;
        for (const auto& [k, v] : init) {
            obj.emplace(k, v);
        }
        data_ = std::move(obj);
    }

    static json parse(const std::string& text) {
        std::size_t idx = 0;
        json result = parse_value(text, idx);
        skip_ws(text, idx);
        if (idx != text.size()) {
            throw std::runtime_error("json parse error: trailing characters");
        }
        return result;
    }

    template <typename T>
    T value(const std::string& key, T default_value) const {
        if (!is_object()) {
            return default_value;
        }
        const auto& obj = std::get<object_t>(data_);
        auto it = obj.find(key);
        if (it == obj.end()) {
            return default_value;
        }
        return it->second.get<T>();
    }

    bool contains(const std::string& key) const {
        if (!is_object()) return false;
        const auto& obj = std::get<object_t>(data_);
        return obj.find(key) != obj.end();
    }

    json& operator[](const std::string& key) {
        if (!is_object()) {
            data_ = object_t{};
        }
        return std::get<object_t>(data_)[key];
    }

    const json& operator[](const std::string& key) const {
        return at(key);
    }

    json& operator[](std::size_t idx) {
        if (!is_array()) {
            throw std::runtime_error("json not array");
        }
        auto& arr = std::get<array_t>(data_);
        if (idx >= arr.size()) {
            throw std::out_of_range("json index");
        }
        return arr[idx];
    }

    const json& operator[](std::size_t idx) const {
        if (!is_array()) {
            throw std::runtime_error("json not array");
        }
        const auto& arr = std::get<array_t>(data_);
        if (idx >= arr.size()) {
            throw std::out_of_range("json index");
        }
        return arr[idx];
    }

    const json& at(const std::string& key) const {
        if (!is_object()) {
            throw std::runtime_error("json not object");
        }
        const auto& obj = std::get<object_t>(data_);
        auto it = obj.find(key);
        if (it == obj.end()) {
            throw std::out_of_range("json key");
        }
        return it->second;
    }

    json& at(const std::string& key) {
        if (!is_object()) {
            throw std::runtime_error("json not object");
        }
        auto& obj = std::get<object_t>(data_);
        auto it = obj.find(key);
        if (it == obj.end()) {
            throw std::out_of_range("json key");
        }
        return it->second;
    }

    bool is_null() const { return std::holds_alternative<std::nullptr_t>(data_); }
    bool is_boolean() const { return std::holds_alternative<boolean_t>(data_); }
    bool is_number_integer() const { return std::holds_alternative<number_integer_t>(data_); }
    bool is_number_float() const { return std::holds_alternative<number_float_t>(data_); }
    bool is_string() const { return std::holds_alternative<string_t>(data_); }
    bool is_array() const { return std::holds_alternative<array_t>(data_); }
    bool is_object() const { return std::holds_alternative<object_t>(data_); }

    std::size_t size() const {
        if (is_array()) return std::get<array_t>(data_).size();
        if (is_object()) return std::get<object_t>(data_).size();
        return 0;
    }

    array_t::iterator begin() {
        if (!is_array()) throw std::runtime_error("json not array");
        return std::get<array_t>(data_).begin();
    }

    array_t::iterator end() {
        if (!is_array()) throw std::runtime_error("json not array");
        return std::get<array_t>(data_).end();
    }

    array_t::const_iterator begin() const {
        if (!is_array()) throw std::runtime_error("json not array");
        return std::get<array_t>(data_).begin();
    }

    array_t::const_iterator end() const {
        if (!is_array()) throw std::runtime_error("json not array");
        return std::get<array_t>(data_).end();
    }

    template <typename T>
    T get() const {
        if constexpr (std::is_same_v<T, json>) {
            return *this;
        } else if constexpr (std::is_same_v<T, string_t>) {
            if (!is_string()) throw std::runtime_error("json not string");
            return std::get<string_t>(data_);
        } else if constexpr (std::is_same_v<T, const char*>) {
            if (!is_string()) throw std::runtime_error("json not string");
            return std::get<string_t>(data_).c_str();
        } else if constexpr (std::is_same_v<T, boolean_t>) {
            if (!is_boolean()) throw std::runtime_error("json not bool");
            return std::get<boolean_t>(data_);
        } else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            if (is_number_integer()) return static_cast<T>(std::get<number_integer_t>(data_));
            if (is_number_float()) return static_cast<T>(std::get<number_float_t>(data_));
            throw std::runtime_error("json not integer");
        } else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>) {
            if (is_number_integer()) return static_cast<T>(std::get<number_integer_t>(data_));
            if (is_number_float()) return static_cast<T>(std::get<number_float_t>(data_));
            throw std::runtime_error("json not unsigned");
        } else if constexpr (std::is_floating_point_v<T>) {
            if (is_number_integer()) return static_cast<T>(std::get<number_integer_t>(data_));
            if (is_number_float()) return static_cast<T>(std::get<number_float_t>(data_));
            throw std::runtime_error("json not number");
        } else if constexpr (std::is_same_v<T, array_t>) {
            if (!is_array()) throw std::runtime_error("json not array");
            return std::get<array_t>(data_);
        } else if constexpr (std::is_same_v<T, object_t>) {
            if (!is_object()) throw std::runtime_error("json not object");
            return std::get<object_t>(data_);
        } else {
            static_assert(!sizeof(T), "Unsupported type for json::get");
        }
    }

    template <typename T>
    void get_to(T& value) const {
        value = get<T>();
    }

    template <typename T>
    T at(const std::string& key) const {
        return this->at(key).template get<T>();
    }

    std::string dump(int indent = -1) const {
        std::string out;
        dump_internal(out, indent, 0);
        return out;
    }

private:
    static void skip_ws(const std::string& s, std::size_t& idx) {
        while (idx < s.size() && std::isspace(static_cast<unsigned char>(s[idx]))) {
            ++idx;
        }
    }

    static json parse_value(const std::string& s, std::size_t& idx) {
        skip_ws(s, idx);
        if (idx >= s.size()) {
            throw std::runtime_error("json parse error: unexpected end");
        }
        char c = s[idx];
        if (c == 'n') {
            if (s.compare(idx, 4, "null") != 0) throw std::runtime_error("json parse error");
            idx += 4;
            return json(nullptr);
        }
        if (c == 't') {
            if (s.compare(idx, 4, "true") != 0) throw std::runtime_error("json parse error");
            idx += 4;
            return json(true);
        }
        if (c == 'f') {
            if (s.compare(idx, 5, "false") != 0) throw std::runtime_error("json parse error");
            idx += 5;
            return json(false);
        }
        if (c == '"') {
            return json(parse_string(s, idx));
        }
        if (c == '{') {
            return json(parse_object(s, idx));
        }
        if (c == '[') {
            return json(parse_array(s, idx));
        }
        return json(parse_number(s, idx));
    }

    static string_t parse_string(const std::string& s, std::size_t& idx) {
        if (s[idx] != '"') throw std::runtime_error("json string expected");
        ++idx;
        string_t result;
        while (idx < s.size()) {
            char c = s[idx++];
            if (c == '"') break;
            if (c == '\\') {
                if (idx >= s.size()) throw std::runtime_error("json string escape");
                char esc = s[idx++];
                switch (esc) {
                    case '"': result.push_back('"'); break;
                    case '\\': result.push_back('\\'); break;
                    case '/': result.push_back('/'); break;
                    case 'b': result.push_back('\b'); break;
                    case 'f': result.push_back('\f'); break;
                    case 'n': result.push_back('\n'); break;
                    case 'r': result.push_back('\r'); break;
                    case 't': result.push_back('\t'); break;
                    case 'u': {
                        if (idx + 4 > s.size()) throw std::runtime_error("json unicode escape");
                        unsigned code = 0;
                        for (int i = 0; i < 4; ++i) {
                            char h = s[idx++];
                            code <<= 4;
                            if (h >= '0' && h <= '9') code |= (h - '0');
                            else if (h >= 'a' && h <= 'f') code |= (h - 'a' + 10);
                            else if (h >= 'A' && h <= 'F') code |= (h - 'A' + 10);
                            else throw std::runtime_error("json unicode hex");
                        }
                        if (code <= 0x7F) result.push_back(static_cast<char>(code));
                        else if (code <= 0x7FF) {
                            result.push_back(static_cast<char>(0xC0 | (code >> 6)));
                            result.push_back(static_cast<char>(0x80 | (code & 0x3F)));
                        } else {
                            result.push_back(static_cast<char>(0xE0 | (code >> 12)));
                            result.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
                            result.push_back(static_cast<char>(0x80 | (code & 0x3F)));
                        }
                        break;
                    }
                    default:
                        throw std::runtime_error("json invalid escape");
                }
            } else {
                result.push_back(c);
            }
        }
        return result;
    }

    static number_float_t parse_number(const std::string& s, std::size_t& idx) {
        std::size_t start = idx;
        if (s[idx] == '-') ++idx;
        while (idx < s.size() && std::isdigit(static_cast<unsigned char>(s[idx]))) ++idx;
        if (idx < s.size() && s[idx] == '.') {
            ++idx;
            while (idx < s.size() && std::isdigit(static_cast<unsigned char>(s[idx]))) ++idx;
        }
        if (idx < s.size() && (s[idx] == 'e' || s[idx] == 'E')) {
            ++idx;
            if (idx < s.size() && (s[idx] == '+' || s[idx] == '-')) ++idx;
            while (idx < s.size() && std::isdigit(static_cast<unsigned char>(s[idx]))) ++idx;
        }
        number_float_t value = std::strtod(s.c_str() + start, nullptr);
        return value;
    }

    static array_t parse_array(const std::string& s, std::size_t& idx) {
        if (s[idx] != '[') throw std::runtime_error("json array expected");
        ++idx;
        array_t arr;
        skip_ws(s, idx);
        if (idx < s.size() && s[idx] == ']') {
            ++idx;
            return arr;
        }
        while (idx < s.size()) {
            arr.push_back(parse_value(s, idx));
            skip_ws(s, idx);
            if (idx >= s.size()) throw std::runtime_error("json array error");
            char c = s[idx++];
            if (c == ']') break;
            if (c != ',') throw std::runtime_error("json array comma");
        }
        return arr;
    }

    static object_t parse_object(const std::string& s, std::size_t& idx) {
        if (s[idx] != '{') throw std::runtime_error("json object expected");
        ++idx;
        object_t obj;
        skip_ws(s, idx);
        if (idx < s.size() && s[idx] == '}') {
            ++idx;
            return obj;
        }
        while (idx < s.size()) {
            skip_ws(s, idx);
            string_t key = parse_string(s, idx);
            skip_ws(s, idx);
            if (idx >= s.size() || s[idx] != ':') throw std::runtime_error("json object colon");
            ++idx;
            obj.emplace(std::move(key), parse_value(s, idx));
            skip_ws(s, idx);
            if (idx >= s.size()) throw std::runtime_error("json object error");
            char c = s[idx++];
            if (c == '}') break;
            if (c != ',') throw std::runtime_error("json object comma");
        }
        return obj;
    }

    void dump_internal(std::string& out, int indent, int depth) const {
        auto indent_if_needed = [&](int next_depth) {
            if (indent >= 0) {
                out.push_back('\n');
                out.append(next_depth * indent, ' ');
            }
        };

        if (is_null()) {
            out += "null";
        } else if (is_boolean()) {
            out += std::get<boolean_t>(data_) ? "true" : "false";
        } else if (is_number_integer()) {
            out += std::to_string(std::get<number_integer_t>(data_));
        } else if (is_number_float()) {
            out += std::to_string(std::get<number_float_t>(data_));
        } else if (is_string()) {
            out.push_back('"');
            out += std::get<string_t>(data_);
            out.push_back('"');
        } else if (is_array()) {
            out.push_back('[');
            const auto& arr = std::get<array_t>(data_);
            for (std::size_t i = 0; i < arr.size(); ++i) {
                if (i != 0) {
                    out.push_back(',');
                }
                indent_if_needed(depth + 1);
                arr[i].dump_internal(out, indent, depth + 1);
            }
            if (indent >= 0 && !arr.empty()) {
                indent_if_needed(depth);
            }
            out.push_back(']');
        } else if (is_object()) {
            out.push_back('{');
            const auto& obj = std::get<object_t>(data_);
            bool first = true;
            for (const auto& [k, v] : obj) {
                if (!first) {
                    out.push_back(',');
                }
                first = false;
                indent_if_needed(depth + 1);
                out.push_back('"');
                out += k;
                out += ":";
                if (indent >= 0) out.push_back(' ');
                v.dump_internal(out, indent, depth + 1);
            }
            if (indent >= 0 && !obj.empty()) {
                indent_if_needed(depth);
            }
            out.push_back('}');
        }
    }

    value_t data_;
};

inline json parse(const std::string& text) {
    return json::parse(text);
}

} // namespace nlohmann

