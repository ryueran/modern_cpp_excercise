#include <osmium/io/reader.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <iostream>

class OSMHandler : public osmium::handler::Handler {
public:
    void node(const osmium::Node& node) {
        std::cout << "Node ID: " << node.id() 
                  << " Location: " << node.location() << std::endl;
    }
    void way(const osmium::Way& way) {
        std::cout << "Way ID: " << way.id() << std::endl;
    }
};

int main() {
    osmium::io::Reader reader("berlin.osm.pbf");
    OSMHandler handler;
    osmium::apply(reader, handler);
    reader.close();
    return 0;
}
