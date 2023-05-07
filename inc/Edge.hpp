#ifndef EDGE_HPP
#define EDGE_HPP

#include <iostream>
#include "Vertex.hpp"

class Edge {
    public:
        Vertex t;
        Vertex f;
        int weight;

        Edge() {
            t = Vertex();
            f = Vertex();
            weight = -1;
        }

        Edge(Vertex t, Vertex f) {
            // lower id is always first
            if (t.id > f.id) {
                this->t = t;
                this->f = f;
            } else {
                this->t = f;
                this->f = t;
            }
            this->weight = calc_weight();
        }

        void print() {
            std::cout << "Edge: "<<std::endl;
            t.print();
            f.print();
            std::cout << std::endl;
        }

        bool operator==(const Edge& other) const {
            return (t.id == other.t.id && f.id == other.f.id) || (t.id == other.f.id && f.id == other.t.id);
        }

        bool operator!=(const Edge& other) const {
            return !(*this == other);
        }

        bool operator<(const Edge& other) const {
            return weight < other.weight;
        }

        bool operator>(const Edge& other) const {
            return weight > other.weight;
        }

        bool operator<=(const Edge& other) const {
            return weight <= other.weight;
        }

        bool operator>=(const Edge& other) const {
            return weight >= other.weight;
        }

        int calc_weight() {
            return abs(t.x - f.x) + abs(t.y - f.y);
        }
};
#endif //EDGE_HPP