#include <stdlib.h>
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

Point lines_intersect(Line line1, Line line2) {
    
}