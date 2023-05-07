#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iostream>
#include "Edge.hpp"

class Component {
    public:
        int id;
        Edge cheapest_edge;

    Component() {
        id = 0;
        cheapest_edge = Edge();
    }

    Component(int id) {
        this->id = id;
    }

    void print() {
        std::cout << "Component: " << id << std::endl;
    }
};
#endif //COMPONENT_HPP