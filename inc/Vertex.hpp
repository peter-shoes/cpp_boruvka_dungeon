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

        bool operator==(const Vertex& other) const {
            return (x == other.x && y == other.y && id == other.id);
        }

        bool operator!=(const Vertex& other) const {
            return !(*this == other);
        }

        bool operator<(const Vertex& other) const {
            if (x < other.x) {
                return true;
            } else if (x == other.x) {
                if (y < other.y) {
                    return true;
                } else if (y == other.y) {
                    if (id < other.id) {
                        return true;
                    }
                }
            }
            return false;
        }

        bool operator>(const Vertex& other) const {
            return !(*this < other);
        }

        bool operator<=(const Vertex& other) const {
            return (*this < other || *this == other);
        }

        bool operator>=(const Vertex& other) const {
            return (*this > other || *this == other);
        }

        void print() {
            std::cout << "Vertex: " << x << ", " << y << ", " << id << std::endl;
        }
};
#endif //VERTEX_HPP