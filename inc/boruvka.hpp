#ifndef BORUVKA_HPP
#define BORUVKA_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "Room.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Component.hpp"

// CLASS DEFINITIONS
class Vector2 {
    public:
        int x;
        int y;
        Vector2(int x, int y) {
            this->x = x;
            this->y = y;
        }
};

// MAIN PROGRAM
int generate(Vector2 map_size, 
            int num_rooms, 
            Vector2 room_size);

std::vector<Edge> boruvka(Room rooms[], int num_rooms);

void do_a_star(int** dungeon, int rows, int cols, std::vector<Edge> edges);

bool is_preferred_over(Edge uv, Edge wx);

void print_dungeon(int** array, int rows, int cols);

#endif //BORUVKA_HPP

