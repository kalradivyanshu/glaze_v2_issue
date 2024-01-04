
#include <cstdint>
#include <refl.hpp>
#include <tuple>
#include <vector>

#pragma once

namespace writer {

/**
    gets the byte length it will take to store the value
  */
size_t varint_len(uint64_t value);

/*
  Gets the length of the varint in bytes
*/
size_t varint_parse_len(uint8_t first);

template <typename T>
    requires std::is_integral_v<T>
size_t len(T value) {
    return varint_len(static_cast<uint64_t>(value));
}
size_t len(bool);

template <typename T, typename F>
size_t len(const std::pair<T, F>& value) {
    return len(value.first) + len(value.second);
}

template <typename T>
size_t len(const std::vector<T>& value) {
    size_t length = 0;
    length += len(value.size());
    for (const auto& val : value) {
        length += len(val);
    }
    return length;
}

size_t len(const std::string& val);

class Writer {
   private:
    size_t offset = 0;

   public:
    size_t max_len;
    bool is_read_only = false;
    std::vector<uint8_t> data;

    Writer(size_t max_len);

    // delete copy constructors
    Writer(const Writer&) = delete;
    Writer& operator=(const Writer&) = delete;

    /// @brief Resets the offset to 0
    void reset_offset() {
        this->offset = 0;
    }

    /**
     * @brief Reads and returns nth byte
     *
     */
    uint8_t read(size_t n);

    /**
     * @brief sets nth byte
     *
     */
    uint8_t set(size_t n, uint64_t val);

    /**
      Sets a value slice into writer at the current offset.
      @param value: the u64 value to write into data.
    */
    void put_varint(uint64_t value);
    template <typename T>
        requires std::is_integral_v<T>
    void put(T value) {
        this->put_varint(value);
    }
    void put(bool value) {
        this->put_varint((uint64_t)value);
    }
    void put(std::string& t) {
        this->put_varint(t.size());
        for (size_t i = 0; i < t.size(); i++) {
            this->data[this->off() + i] = t[i];
        }
        this->offset += t.size();
    }

    /// @brief Serializes a vector which has pure value, must be supported by writer
    /// @tparam T
    /// @param t
    template <typename T>
    void put(std::vector<T>& t) {
        this->put_varint(t.size());
        for (auto& val : t) {
            this->put(val);
        }
    }

    template <typename T>
    void get(std::vector<T>& t) {
        size_t size = this->get_varint();
        t.resize(size);

        for (size_t i = 0; i < size; i++) {
            this->get(t[i]);
        }
    }

    template <typename T>
        requires std::is_integral_v<T>
    void get(T& t) {
        t = this->get_varint();
    }

    void get(bool& t) {
        t = (bool)this->get_varint();
    }
    void get(std::string& s) {
        size_t size = this->get_varint();
        s.resize(size);
        for (size_t i = 0; i < size; i++) {
            s[i] = (char)this->data[this->off() + i];
        }
        this->offset += size;
    }

    /// @brief returns the current offset
    /// @return
    size_t off() {
        return this->offset;
    }

    uint64_t get_varint();
};

/// @brief Iterates over all types of a value using reflection and writes them to an writer
/// @tparam T the type to write
/// @param o the writer to write to
/// @param value value of type T
template <typename T>
void serialize(Writer& o, T& value) {
    for_each(refl::reflect(value).members, [&](auto member) {
        // SPDLOG_INFO("{}", get_display_name(member));

        // is_readable checks if the member is a non-const field
        // or a 0-arg const-qualified function marked with property attribute
        if constexpr (!is_readable(member)) {
            return;
        }

        o.put(member(value));
    });
}

// @brief Iterates over all types of a value using reflection and get their serialized length
/// @tparam T the type to write
/// @param value value of type T
template <typename T>
size_t length(const T& value) {
    size_t length_ = 0;
    for_each(refl::reflect(value).members, [&](const auto member) {
        // SPDLOG_INFO("{}", get_display_name(member));

        // is_readable checks if the member is a non-const field
        // or a 0-arg const-qualified function marked with property attribute
        if constexpr (!is_readable(member)) {
            return;
        }

        length_ += len(member(value));
    });
    return length_;
}

/// @brief Iterates over all types of a value using reflection and writes them to an writer
/// @tparam T the type to write
/// @param o the writer to write to
/// @param value value of type T
template <typename T>
void deserialize(Writer& o, T& value) {
    for_each(refl::reflect(value).members, [&](auto member) {
        // SPDLOG_INFO("{}", get_display_name(member));

        // is_readable checks if the member is a non-const field
        // or a 0-arg const-qualified function marked with property attribute
        if constexpr (!is_readable(member)) {
            return;
        }

        o.get(member(value));
    });
}

};  // namespace writer