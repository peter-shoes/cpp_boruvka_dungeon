#ifndef ROOM_HPP
#define ROOM_HPP

#include <iostream>

class Room {
    public:
        int lx;
        int ly;
        int sx;
        int sy;

        Room() {
            lx = 0;
            ly = 0;
            sx = 0;
            sy = 0;
        }

        Room(int lx, int ly, int sx, int sy) {
            this->lx = lx;
            this->ly = ly;
            this->sx = sx;
            this->sy = sy;
        }

        void print() {
            std::cout << "Room: " << lx << ", " << ly << ", " << sx << ", " << sy << std::endl;
        }
};

#endif //ROOM_HPP