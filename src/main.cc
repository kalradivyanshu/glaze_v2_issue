#include <cstddef>
#include <cstdint>
#include <glaze/glaze.hpp>
#include <iostream>

#include "struct_details.hh"
#include "writer.hh"

int main(void) {
    auto sd1 = SD{
        .t = 20,
        .sid = "abcd1",
        .sn = 12,
        .ln = 1,
        .sln = 10,
        .k = true,
        .sn_ = 121,
        .ffs = 10,
        .fls = 1,
        .fgn = 10,
        .ct = 15313,
        .t_ = 1351231,
        .fn_ = 121,
        .d_s = 121,
        .pt = 1413134,
        .fst = 1354134,
        .dl = 1032,
    };

    auto buffer = glz::write_binary_untagged(sd1);

    auto writer = writer::Writer(200);

    writer::serialize(writer, sd1);

    std::cout << "glaze untagged: " << buffer.size()
              << " writer: " << writer.off() << std::endl;
}
