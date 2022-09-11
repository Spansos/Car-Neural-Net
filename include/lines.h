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

void render_line(sfRenderWindow *window, Line line, sfColor color);
void render_map(Map *map, sfRenderWindow *window);
bool lines_intersect(Line line1, Line line2, Point *point);
Point rot_point(Point point, double deg);
Line rot_line(Line line, double deg);

#endif