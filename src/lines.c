#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <SFML/Graphics.h>
#include <lines.h>

Map *create_map(Line *lines, int linec) {
    Map *map = calloc(1, sizeof(Map));

    map->lines = lines;
    map->linec = linec;
    
    return map;
}


void render(Map *map, sfRenderWindow *window) {
    for (int i=0; i < map->linec; i++) {
        sfVertex line[2];

        line[0] = (sfVertex){
            .position = (sfVector2f){
                .x = map->lines[i].p1.x,
                .y = map->lines[i].p1.y
            },
            .color = sfWhite,
        };
        line[1] = (sfVertex){
            .position = (sfVector2f){
                .x = map->lines[i].p2.x,
                .y = map->lines[i].p2.y
            },
            .color = sfWhite,
        };

        sfRenderWindow_drawPrimitives(window, line, 2, sfLines, NULL);
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
        y = c4 / b4;
        x = (c1+b1*y) / a1;
    }
    point->x = (int)x;
    point->y = (int)y;
    return true;
}