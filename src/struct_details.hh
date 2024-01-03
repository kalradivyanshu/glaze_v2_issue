#include <cstdint>
#include <glaze/glaze.hpp>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace details {

enum Q {
    A1 = 0,
    A2 = 1,
};

enum B {
    B1 = 0,
    B2 = 1,
};

struct V {
    Q v1;
    uint8_t v2;
    B v3;
    uint64_t v4;
    uint8_t v5;
    std::vector<uint8_t> v6;
    auto operator<=>(const V&) const = default;
};

struct VS {
    uint16_t w;
    uint16_t h;
    uint8_t f;
    auto operator<=>(const VS&) const = default;
};

struct VC {
    std::string c;
    bool l;
    bool s;
    uint8_t sn;
    std::string sid;
    uint64_t time;
    uint8_t p;
    uint64_t age;
    uint32_t gs;
    VS srs;
    std::map<uint8_t, V> layers;
    bool operator<=>(const VC&) const = default;
};

struct A {
    uint64_t b;
    std::vector<uint8_t> e;
    bool operator<=>(const A&) const = default;
};

struct ASS {
    uint32_t sr;
    uint8_t cc;
    bool operator<=>(const ASS&) const = default;
};

struct AC {
    std::string c;
    bool m;
    bool s;
    uint8_t sn;
    std::string sid;
    uint64_t time;
    uint8_t p;
    uint64_t age;
    ASS srs;
    std::map<uint8_t, A> layers;
    bool operator<=>(const AC&) const = default;
};

struct C {
    bool a;
    std::variant<VC, AC> Config;
};

};  // namespace details

namespace details2 {
class UD {
   public:
    std::string id;
    std::string n;
    std::string e;
    std::string aid;
    uint64_t o;
    bool ob;
    std::string ri;
    std::map<uint8_t, details::VC> v;
    std::map<uint8_t, details::AC> a;
    bool operator<=>(const UD&) const = default;
};
};  // namespace details2

template <>
struct glz::meta<details::Q> {
    using enum details::Q;
    static constexpr auto value = enumerate("0", A1, "1", A2);
};

template <>
struct glz::meta<details::B> {
    using enum details::B;
    static constexpr auto value = enumerate("0", B1, "1", B2);
};