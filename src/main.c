#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.h>
#include <lines.h>
#include <car.h>

int read_lines_file(char *file_name, Line **lines);

int main() {
    sfVector2i res = {800, 600};
    sfRenderWindow *window = sfRenderWindow_create((sfVideoMode){.width=res.x, .height=res.y}, "Car Neural Net", sfClose, NULL);

    Map map;
    map.linec = read_lines_file("maplines.txt", &map.lines);
    map.goal_linec = read_lines_file("goallines.txt", &map.goal_lines);

    Car *cars[55];
    for (int i=0; i<55; i++) {
        cars[i] = create_car(&map, NULL);
    }

    sfRenderWindow_setFramerateLimit(window, 60);
    sfVector2f cam_pos;
    sfEvent event;
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
        double max_fit = 0;
        Car *max_fit_car;
        for (int i=0; i<55; i++) {
            update_car(cars[i], &map);
            if (cars[i]->fitness > max_fit) {
                max_fit = cars[i]->fitness;
                max_fit_car = cars[i];
            }
        }
        cam_pos = (sfVector2f){.x=max_fit_car->pos.x - res.x/2, .y=max_fit_car->pos.y - res.y/2};

        sfRenderWindow_clear(window, sfBlack);
        render_map(&map, window, cam_pos);
        for (int i=0; i<55; i++) {
            render_car(cars[55], window, cam_pos);
        }
        sfRenderWindow_display(window);
    }
}

int read_lines_file(char *file_name, Line **lines) {
    FILE *file = fopen(file_name, "r");
    char buffer[255];

    fgets(buffer, 255, file);
    int linec = atoi(buffer);
    *lines = calloc(linec, sizeof(Line));

    for (int i=0; i < linec; i++) {
        int line_nums[4];
        for (int j=0; j < 4; j++) {
            fscanf(file, "%s", buffer);
            line_nums[j] = atoi(buffer);
        }
        (*lines)[i] = (Line){.p1 = (Point){.x = line_nums[0], .y = line_nums[1]}, .p2 = (Point){.x = line_nums[2], .y = line_nums[3]}};
    }
    fclose(file);
    return linec;
}