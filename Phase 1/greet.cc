#include "hello.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc > 1) {
        std::cout << hello(argv[1]) << std::endl;
    } else {
        std::cout << "Usage: " << argv[0] << " [name]" << std::endl;
    }
    return 0;
}
