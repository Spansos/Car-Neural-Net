#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.h>
#include <lines.h>
#include <car.h>

int main() {
    sfRenderWindow *window = sfRenderWindow_create((sfVideoMode){800, 600}, "Car Neural Net", sfClose, NULL);

    Map map;

    FILE *file = fopen("maplines.txt", "r");
    char buffer[255];

    fgets(buffer, 255, file);
    int linec = atoi(buffer);
    map.linec = linec;
    map.lines = calloc(sizeof(Line), linec);

    for (int i=0; i < linec; i++) {
        fgets(buffer, 255, file);
        
        // int i=-1;
        // char intbuffer[255];
        // do {
        //     i++;
        //     intbuffer[i] = buffer[i];
        // } while (intbuffer[i] != '\n');  ??? need sleep
        map.lines[i] = (Line){.p1 = (Point){.x = 100, .y = 100}, .p2 = (Point){.x = 800, .y = 300}};

    }
    fclose(file);
    return;

    Line goal_lines[2];
    goal_lines[0] = (Line){.p1 = (Point){.x = 400, .y = 100}, .p2 = (Point){.x = 500, .y = 350}};
    goal_lines[1] = (Line){.p1 = (Point){.x = 600, .y = 235}, .p2 = (Point){.x = 675, .y = 325}};
    map.goal_lines = goal_lines;
    map.goal_linec = 2;

    Car *car = create_car();
    car->pos = (sfVector2f){300, 250};
    car->rotation = 242;

    sfRenderWindow_setFramerateLimit(window, 60);
    sfEvent event;
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
        update_car(car, &map, sfKeyboard_isKeyPressed(sfKeyA), sfKeyboard_isKeyPressed(sfKeyD), sfKeyboard_isKeyPressed(sfKeyW), sfKeyboard_isKeyPressed(sfKeyS));

        sfRenderWindow_clear(window, sfBlack);
        render_map(&map, window);
        render_car(car, window);
        sfRenderWindow_display(window);
    }
}