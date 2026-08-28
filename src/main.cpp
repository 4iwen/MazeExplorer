#include "app.h"

#include <iostream>

int main() {
    try {
        App::run();
    } catch (std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
