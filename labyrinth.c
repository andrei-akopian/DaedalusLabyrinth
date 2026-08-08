#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>

struct Room{
    struct RoomSide *top;
    struct RoomSide *top_right;
    struct RoomSide *right;
    struct RoomSide *bottom_right;
    struct RoomSide *top_left;
    struct RoomSide *left;
    struct RoomSide *bottom_left;
    struct RoomSide *bottom;
};

struct RoomSide{
    int connected;
    struct RoomSide *next_room;
    int string_layers;
    int stones;
    int shape;
};

struct Sign{
    struct parent_room *Room;
    char text[11];
};

struct Inventory{
    int stones;
    int wood_scraps;
    int ariadne_string;
};

int main(){

}