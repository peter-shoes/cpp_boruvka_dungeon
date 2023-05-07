#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>

class Vertex {
    public:
        int x;
        int y;
        int id;

        Vertex() {
            x = 0;
            y = 0;
            id = 0;
        }

        Vertex(int x, int y, int id) {
            this->x = x;
            this->y = y;
            this->id = id;
        }

        void print() {
            std::cout << "Vertex: " << x << ", " << y << ", " << id << std::endl;
        }
};
#endif //VERTEX_HPP