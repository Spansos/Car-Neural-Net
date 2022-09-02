#ifndef LINES_H
#define LINES_H

typedef struct point_t {
    int x;
    int y;
} Point;

typedef struct line_t {
    Point *p1;
    Point *p2;
} Line;

typedef struct map_t {
    Line **lines;
    int linec;
    Line **goal_lines;
    int goal_linec;
} Map;



#endif