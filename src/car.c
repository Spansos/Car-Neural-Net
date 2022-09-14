#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SFML/Graphics.h>
#include <stdio.h>
#include <lines.h>
#include <neuralnet.h>
#include <car.h>

Car *create_car(Map *map, Network *net) {
    Car *car = calloc(1, sizeof(Car));
    car->linec = 4;
    car->lines = calloc(sizeof(Line), 4);
    car->lines[0] = (Line){.p1 = (Point){.x = -30, .y = -20}, .p2 = (Point){.x = 30, .y = -20}};
    car->lines[1] = (Line){.p1 = (Point){.x = 30, .y = -20}, .p2 = (Point){.x = 30, .y = 20}};
    car->lines[2] = (Line){.p1 = (Point){.x = 30, .y = 20}, .p2 = (Point){.x = -30, .y = 20}};
    car->lines[3] = (Line){.p1 = (Point){.x = -30, .y = 20}, .p2 = (Point){.x = -30, .y = -20}};

    Line first_goal = map->goal_lines[0];
    int avr_x = (first_goal.p1.x + first_goal.p2.x) / 2;
    int avr_y = (first_goal.p1.y + first_goal.p2.y) / 2;
    car->pos.x = avr_x;
    car->pos.y = avr_y;

    Line sec_goal = map->goal_lines[1];
    int avr_x2 = (sec_goal.p1.x + sec_goal.p2.x) / 2;
    int avr_y2 = (sec_goal.p1.y + sec_goal.p2.y) / 2;
    double dir_co = (avr_x-avr_x2)/(avr_x-avr_x2);
    double degs = atan(dir_co);
    car->rotation = degs;

    int net_size[] = {9, 8, 8, 4};
    Network *rand_net = create_network(net_size, 4, NULL);
    init_random(rand_net, 2, 2);
    if (net) {
        car->net = add_networks(net, rand_net);
        free_network(rand_net);
    }
    else {
        car->net = rand_net;
    }
    return car;
}

void free_car(Car *car) {
    free(car);
}

Line calc_car_line(Car *car, int linen) {
    // get line
    Line line = car->lines[linen];

    // rotate
    line = rot_line(line, car->rotation);

    // add car pos
    line.p1.x += car->pos.x; line.p1.y += car->pos.y;
    line.p2.x += car->pos.x; line.p2.y += car->pos.y;

    return line;
}

bool collide_line(Car *car, Line line) {
    for (int i=0; i < car->linec; i++) {
        Line car_line = calc_car_line(car, i);
        if (lines_intersect(car_line, line, NULL)) {
            return true;
        }
    }
    return false;
}

bool collide_lines(Car *car, Line *lines, int linec){
    for (int i=0; i < linec; i++) {
        if (collide_line(car, lines[i])) {
            return true;
        }
    }
    return false;
}

void render_car(Car *car, sfRenderWindow *window, sfVector2f cam_pos) {
    sfColor color;
    if (car->is_dead) {
        color = sfRed;
    }
    else {
        color = sfWhite;
    }
    for (int i=0; i < car->linec; i++) {
        Line line = calc_car_line(car, i);
        render_line(window, line, color, cam_pos);
    }
}

void update_car(Car *car, Map *map) {
    // check if dead
    if (car->is_dead) {
        return;
    }

    Point end_point;
    Line see_line;
    double dists[9];
    for (int i=0; i<9; i++) {
        double deg = (i-4) * 10;
        deg += car->rotation;

        end_point= rot_point((Point){.x=25, .y=0}, deg);

        see_line = (Line){(Point){.x=(int)car->pos.x, .y=(int)car->pos.y}, end_point};
        double dist = 25*25;
        for (int j=0; j < map->linec; j++) {
            Point intersect;
            if (lines_intersect(map->lines[j], see_line, &intersect)) {
                double cur_dist = intersect.x*intersect.x + intersect.y*intersect.y;
            }
        }
    }
    bool out[4];

    // rotate
    if (out[0]) {
        car->rotation_change -= .5;
    }
    if (out[1]) {
        car->rotation_change += .5;
    }
    car->rotation += car->rotation_change;
    car->rotation_change *= .9;

    // move
    if (out[2]) {
        car->vel.x += cos(2 * M_PI * car->rotation / 360);
        car->vel.y += sin(2 * M_PI * car->rotation / 360);
    }
    if (out[3]) {
        car->vel.x -= cos(2 * M_PI * car->rotation / 360);
        car->vel.y -= sin(2 * M_PI * car->rotation / 360);
    }
    car->pos.x += car->vel.x;
    car->pos.y += car->vel.y;
    car->vel.x *= .95;
    car->vel.y *= .95;

    // collision
    if (collide_lines(car, map->lines, map->linec)) {
        car->is_dead = true;
    }

    // calc cur goal + some goal data
    int cur_goaln = car->goals % map->goal_linec;
    int next_goaln = (cur_goaln + 1) % map->goal_linec;
    if (collide_line(car, map->goal_lines[next_goaln])) {
        car->goals++;
        cur_goaln = car->goals % map->goal_linec;
        next_goaln = (cur_goaln + 1) % map->goal_linec;
    }

    // calc dist to next goal
    Line next_goal = map->goal_lines[next_goaln];
    Point goal_avrg = {
        (next_goal.p1.x + next_goal.p2.x)/2,    // avrg x
        (next_goal.p1.y + next_goal.p2.y)/2     // avrg y
    };
    double goal_dist = sqrt(
        pow((car->pos.x - goal_avrg.x), 2)
        + pow((car->pos.y - goal_avrg.y), 2)
    );

    // calc fitness
    car->fitness = car->goals + (1/(goal_dist + 1));
}