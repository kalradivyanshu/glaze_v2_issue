#include "writer.hh"

writer::Writer::Writer(size_t max_len) {
    this->data = std::vector<uint8_t>(max_len);
    this->max_len = max_len;
}

size_t writer::len(bool) {
    return 1;
}
size_t writer::len(const std::string& val) {
    return val.size() + 1;
}

/**
 * @brief Reads and returns nth byte
 *
 */
uint8_t writer::Writer::read(size_t n) {
    auto ptr = this->data.data() + n;
    if (!ptr) {
        return 0;
    }
    return this->data[n];
}

/**
 * @brief sets nth byte
 *
 */
uint8_t writer::Writer::set(size_t n, uint64_t val) {
    auto ptr = this->data.data() + n;
    if (!ptr) {
        return 0;
    }
    this->data[n] = val;
    return this->data[n];
}

size_t writer::varint_len(uint64_t v) {
    if (v <= 63) {
        return 1;
    } else if (v <= 16383) {
        return 2;
    } else if (v <= 1073741823) {
        return 4;
    } else if (v <= 4611686018427387903) {
        return 8;
    } else {
        throw std::runtime_error("invalid varint");
        return 8;
    }
}

size_t writer::varint_parse_len(uint8_t first) {
    switch (first >> 6) {
        case 0:
            return 1;
        case 1:
            return 2;
        case 2:
            return 4;
        case 3:
            return 8;
        default:
            throw std::runtime_error("unreachable");
    }
}

void writer::Writer::put_varint(uint64_t value) {
    if (this->is_read_only) return;
    size_t len = varint_len(value);

    if (len + this->off() > this->max_len) {
        throw std::invalid_argument("buffertooshort");
    }

    auto offset = this->off();

    if (len == 1) {
        this->set(offset, value);
    } else if (len == 2) {
        this->set(offset, (value >> 8) & 0xff);
        this->set(offset + 1, value & 0xff);

        auto data = this->read(offset);
        this->set(offset, data | 0x40);

    } else if (len == 4) {
        this->data[this->off()] = (value >> 24) & 0xff;
        this->set(offset, (value >> 24) & 0xff);
        this->set(offset + 1, (value >> 16) & 0xff);
        this->set(offset + 2, (value >> 8) & 0xff);
        this->set(offset + 3, value & 0xff);

        auto data = this->read(offset);
        this->set(offset, data | 0x80);
    } else if (len == 8) {
        this->set(this->off(), (value >> 56) & 0xff);
        this->set(this->off() + 1, (value >> 48) & 0xff);
        this->set(this->off() + 2, (value >> 40) & 0xff);
        this->set(this->off() + 3, (value >> 32) & 0xff);
        this->set(this->off() + 4, (value >> 24) & 0xff);
        this->set(this->off() + 5, (value >> 16) & 0xff);
        this->set(this->off() + 6, (value >> 8) & 0xff);
        this->set(this->off() + 7, value & 0xff);

        auto data = this->read(this->off());
        this->set(this->off(), data | 0xc0);
    } else {
        throw std::runtime_error("unreachable");
    }
    this->offset += len;
    return;
}

uint64_t writer::Writer::get_varint() {
    uint8_t first = this->read(this->off());

    size_t len = varint_parse_len(first);

    if (len + this->off() > this->max_len) {
        throw std::invalid_argument("buffertooshort");
    }

    if (len == 1) {
        this->offset += 1;
        return first & 0x3f;
    } else if (len == 2) {
        uint16_t value = this->read(this->off()) & 0x3f;
        value <<= 8;
        value |= this->read(this->off() + 1);
        this->offset += 2;
        return value;
    } else if (len == 4) {
        uint32_t value = this->read(this->off()) & 0x3f;
        value <<= 8;
        value |= this->read(this->off() + 1);
        value <<= 8;
        value |= this->read(this->off() + 2);
        value <<= 8;
        value |= this->read(this->off() + 3);
        this->offset += 4;
        return value;
    } else if (len == 8) {
        uint64_t value = this->read(this->off()) & 0x3f;
        value <<= 8;
        value |= this->read(this->off() + 1);
        value <<= 8;
        value |= this->read(this->off() + 2);
        value <<= 8;
        value |= this->read(this->off() + 3);
        value <<= 8;
        value |= this->read(this->off() + 4);
        value <<= 8;
        value |= this->read(this->off() + 5);
        value <<= 8;
        value |= this->read(this->off() + 6);
        value <<= 8;
        value |= this->read(this->off() + 7);
        this->offset += 8;
        return value;
    } else {
        throw std::runtime_error("unreachable");
    }
}