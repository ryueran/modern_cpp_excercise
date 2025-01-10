#include <mapnik/projection.hpp>
#include <iostream>

#define MAPNIK_USE_PROJ 1

int main() {
    std::cout << "Projection initialized successfully." << std::endl;
    try {
        mapnik::projection proj("epsg:4269");
        std::cout << "Projection initialized successfully." << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Projection error: " << ex.what() << std::endl;
    }
    return 0;
}