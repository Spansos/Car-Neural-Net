#ifndef CAR_H
#define CAR_H

#include <stdbool.h>
#include <lines.h>
#include <SFML/Graphics.h>
#include <neuralnet.h>

typedef struct car_t {
    Line *lines;
    int linec;

    double rotation_change;
    double rotation;
    sfVector2f pos;
    sfVector2f vel;

    int goals;
    bool is_dead;
    double fitness;
    double mem[8];
    Network *net;
} Car;

Car *create_car(Map *map, Network *net, bool add_rand);
void free_car(Car *car, bool free_net);
bool collide_line(Car *car, Line line);
bool collide_lines(Car *car, Line *lines, int linec);
void render_car(Car *car, sfRenderWindow *window, sfVector2f cam_pos);
void update_car(Car *car, Map *map);

#endif