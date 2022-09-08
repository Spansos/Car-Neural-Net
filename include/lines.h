#ifndef LINES_H
#define LINES_H

#include <stdbool.h>

typedef struct point_t {
    int x;
    int y;
} Point;

typedef struct line_t {
    Point p1;
    Point p2;
} Line;

typedef struct map_t {
    Line *lines;
    int linec;
    Line *goal_lines;
    int goal_linec;
} Map;

typedef struct sfRenderWindow sfRenderWindow;

Map *create_map(Line *lines, int linec);
void render(Map *map, sfRenderWindow *window);
bool lines_intersect(Line line1, Line line2, Point *point);

#endif