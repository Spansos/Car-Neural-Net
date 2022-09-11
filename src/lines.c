#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <SFML/Graphics.h>
#include <lines.h>

Map *create_map(Line *lines, int linec) {
    Map *map = calloc(1, sizeof(Map));

    map->lines = lines;
    map->linec = linec;
    
    return map;
}

void render_line(sfRenderWindow *window, Line line, sfColor color) {
    sfVertex line_vertices[2];

    line_vertices[0] = (sfVertex){
        .position = (sfVector2f){
            .x = line.p1.x,
            .y = line.p1.y
        },
        .color = color,
    };
    line_vertices[1] = (sfVertex){
        .position = (sfVector2f){
            .x = line.p2.x,
            .y = line.p2.y
        },
        .color = color,
    };

    sfRenderWindow_drawPrimitives(window, line_vertices, 2, sfLines, NULL);
}

void render_map(Map *map, sfRenderWindow *window) {
    for (int i=0; i < map->goal_linec; i++) {
        render_line(window, map->goal_lines[i], sfGreen);
    }
    for (int i=0; i < map->linec; i++) {
        render_line(window, map->lines[i], sfWhite);
    }
}

bool lines_intersect(Line line1, Line line2, Point *point) {
    // make formula representation for lines
    int a1 = line1.p1.y - line1.p2.y;
    int b1 = line1.p1.x - line1.p2.x;
    int c1 = a1*line1.p1.x - b1*line1.p1.y;

    int a2 = line2.p1.y - line2.p2.y;
    int b2 = line2.p1.x - line2.p2.x;
    int c2 = a2*line2.p1.x - b2*line2.p1.y;

    // multiply formula of line1 to make either
    // a1 or b1 the same as a2 or b2
    double mult_factor;
    if (a1) {
        mult_factor = (double)a2 / a1;
    }
    else {
        mult_factor = (double)b2 / b1;
    }
    double a3 = a1 * mult_factor;
    double b3 = b1 * mult_factor;
    double c3 = c1 * mult_factor;

    // subtract formulas to eliminate variable
    double a4 = a2 - a3;
    double b4 = b2 - b3;
    double c4 = c2 - c3;

    // get x or y from new formula and then get missing
    double x, y;
    if (abs(a4) > abs(b4)) {
        x = c4 / a4;
        y = -(c1-a1*x) / b1;
    }
    else {
        y = -c4 / b4;
        x = (c1+b1*y) / a1;
    }

    // check if point is on both lines
    double min_l1_x = fmin(line1.p1.x, line1.p2.x);
    double min_l1_y = fmin(line1.p1.y, line1.p2.y);
    double max_l1_x = fmax(line1.p1.x, line1.p2.x);
    double max_l1_y = fmax(line1.p1.y, line1.p2.y);
    double min_l2_x = fmin(line2.p1.x, line2.p2.x);
    double min_l2_y = fmin(line2.p1.y, line2.p2.y);
    double max_l2_x = fmax(line2.p1.x, line2.p2.x);
    double max_l2_y = fmax(line2.p1.y, line2.p2.y);
    if (x >= min_l1_x && x <= max_l1_x && y >= min_l1_y && y <= max_l1_y && x >= min_l2_x && x <= max_l2_x && y >= min_l2_y && y <= max_l2_y) {
        if (point) {
            point->x = x;
            point->y = y;
        }
        return true;
    }
    return false;
}

Point rot_point(Point point, double deg) {
    // calc properties of point
    double dist = sqrt(point.x*point.x + point.y*point.y);
    double angle = atan2(point.y, point.x);

    // change calculated properties of point
    angle += 2*M_PI*deg/360;

    // recalculate point with new properties
    Point r_point;
    r_point.x = cos(angle) * dist;
    r_point.y = sin(angle) * dist;

    return r_point;
}


Line rot_line(Line line, double deg) {
    line.p1 = rot_point(line.p1, deg);
    line.p2 = rot_point(line.p2, deg);
    return line;
}