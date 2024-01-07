#include <cstddef>
#include <cstdint>
#include <glaze/glaze.hpp>
#include <iostream>
#include <map>
#include <variant>

#include "struct_details.hh"
#include "writer.hh"

struct A {
    uint8_t a;
};

struct A1 {
    std::map<uint8_t, uint64_t> a;
};

struct B {
    uint8_t b;
    A1 a;
};

struct C {
    bool is_a;
    std::map<uint8_t, std::variant<A, B>> a;
};

class D {
   public:
    C c;
};

template <>
struct glz::meta<A> {
    using T = A;
    static constexpr auto value = object("a", &T::a);
};

template <>
struct glz::meta<A1> {
    using T = A1;
    static constexpr auto value = object("a", &T::a);
};

template <>
struct glz::meta<B> {
    using T = B;
    static constexpr auto value = object("b", &T::b, "a", &T::a);
};

template <>
struct glz::meta<C> {
    using T = C;
    static constexpr auto value = object("is_a", &T::is_a, "a", &T::a);
};

template <>
struct glz::meta<D> {
    using T = D;
    static constexpr auto value = object("c", &T::c);
};

int main(void) {
    using Ty = D;
    Ty d;

    std::vector<uint8_t> out;
    glz::write<glz::opts{.format = glz::binary, .structs_as_arrays = true}>(d, out);

    Ty c1;
    auto _ = glz::read_binary_untagged(c1, out);

    std::cout << "written: " << glz::write_json(out) << std::endl;

    std::cout << "recovered: " << glz::write_json(c1) << std::endl;
}
