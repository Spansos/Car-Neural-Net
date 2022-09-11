#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SFML/Graphics.h>
#include <stdio.h>
#include <lines.h>
#include <car.h>

Car *create_car() {
    Car *car = calloc(1, sizeof(Car));
    car->linec = 4;
    car->lines = calloc(sizeof(Line), 4);
    car->lines[0] = (Line){.p1 = (Point){.x = -30, .y = -20}, .p2 = (Point){.x = 30, .y = -20}};
    car->lines[1] = (Line){.p1 = (Point){.x = 30, .y = -20}, .p2 = (Point){.x = 30, .y = 20}};
    car->lines[2] = (Line){.p1 = (Point){.x = 30, .y = 20}, .p2 = (Point){.x = -30, .y = 20}};
    car->lines[3] = (Line){.p1 = (Point){.x = -30, .y = 20}, .p2 = (Point){.x = -30, .y = -20}};
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

void render_car(Car *car, sfRenderWindow *window) {
    sfColor color;
    if (car->is_dead) {
        color = sfRed;
    }
    else {
        color = sfWhite;
    }
    for (int i=0; i < car->linec; i++) {
        Line line = calc_car_line(car, i);
        render_line(window, line, color);
    }
}

void update_car(Car *car, Map *map, bool go_left, bool go_right, bool accelerate, bool brake) {
    // check if dead
    if (car->is_dead) {
        return;
    }

    // rotate
    if (go_left) {
        car->rotation_change -= .5;
    }
    if (go_right) {
        car->rotation_change += .5;
    }
    car->rotation += car->rotation_change;
    car->rotation_change *= .9;

    // move
    if (accelerate) {
        car->vel.x += cos(2 * M_PI * car->rotation / 360);
        car->vel.y += sin(2 * M_PI * car->rotation / 360);
    }
    if (brake) {
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