#include <cstddef>
#include <cstdint>
#include <glaze/glaze.hpp>
#include <iostream>

#include "struct_details.hh"

int main(void) {
    details2::UD u1;

    std::string json = glz::write_json(u1);

    std::cout << json << std::endl;

    return EXIT_SUCCESS;
}
