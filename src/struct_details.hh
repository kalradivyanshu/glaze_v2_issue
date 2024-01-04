#include <cstdint>
#include <glaze/glaze.hpp>
#include <map>
#include <refl.hpp>
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

struct SD {
    uint8_t t;
    std::string sid;
    uint8_t sn;
    uint8_t ln;
    uint8_t sln;
    bool k;
    uint64_t sn_;
    uint8_t ffs;
    uint64_t fls;
    uint64_t fsn;
    uint64_t fgn;
    uint64_t ct;
    uint64_t t_;
    uint64_t fn_;
    uint64_t d_s;
    uint64_t pt;
    uint64_t fst;
    size_t dl;
    std::vector<uint8_t> d;
};

REFL_TYPE(SD)
REFL_FIELD(t)
REFL_FIELD(sid)
REFL_FIELD(sn)
REFL_FIELD(ln)
REFL_FIELD(sln)
REFL_FIELD(k)
REFL_FIELD(sn_)
REFL_FIELD(ffs)
REFL_FIELD(fls)
REFL_FIELD(fsn)
REFL_FIELD(fgn)
REFL_FIELD(ct)
REFL_FIELD(t_)
REFL_FIELD(fn_)
REFL_FIELD(d_s)
REFL_FIELD(pt)
REFL_FIELD(fst)
REFL_FIELD(dl)
REFL_FIELD(d)
REFL_END