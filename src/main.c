#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SFML/Graphics.h>
#include <lines.h>
#include <car.h>

#define NUM_CARS 20
#define NUM_SURVIVE 10

int read_lines_file(char *file_name, Line **lines);
void sort_cars(Car **cars);
void new_cars(Car **cars, Map *map);

int main() {
    sfVector2i res = {800, 600};
    sfRenderWindow *window = sfRenderWindow_create((sfVideoMode){.width=res.x, .height=res.y}, "Car Neural Net", sfClose, NULL);

    Map map;
    map.linec = read_lines_file("maplines.txt", &map.lines);
    map.goal_linec = read_lines_file("goallines.txt", &map.goal_lines);

    Car *cars[NUM_CARS];
    for (int i=0; i<NUM_CARS; i++) {
        cars[i] = create_car(&map, NULL, true);
    }

    int t=0;
    bool speed = false;
    sfRenderWindow_setFramerateLimit(window, 60);
    sfVector2f cam_pos;
    sfClock *clock = sfClock_create();
    sfEvent event;
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
            if ((event.type == sfEvtKeyPressed) && (event.key.code == sfKeySpace)) {
                speed = !speed;
                if (speed) {
                    sfRenderWindow_setFramerateLimit(window, 0);
                }
                else {
                    sfRenderWindow_setFramerateLimit(window, 60);
                }
            }
        }
        double max_fit = 0;
        Car *max_fit_car;
        for (int i=0; i<NUM_CARS; i++) {
            update_car(cars[i], &map);
            if (cars[i]->fitness > max_fit) {
                max_fit = cars[i]->fitness;
                max_fit_car = cars[i];
            }
        }
        cam_pos = (sfVector2f){.x=max_fit_car->pos.x - res.x/2, .y=max_fit_car->pos.y - res.y/2};

        if (!speed || sfTime_asMilliseconds(sfClock_getElapsedTime(clock)) > 16) {
            sfClock_restart(clock);
            sfRenderWindow_clear(window, sfBlack);
            render_map(&map, window, cam_pos);
            for (int i=0; i<NUM_CARS; i++) {
                render_car(cars[i], window, cam_pos);
            }
            sfRenderWindow_display(window);
        }

        t++;
        if (t> 16 * 60) {
            new_cars(cars, &map);
            t=0;
        }
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

void sort_cars(Car **cars) {
    Car *temp;
    for (int i=0; i < NUM_CARS; i++) {
        int max_fit_index = 0;
        double max_fit = 0;
        for (int j=i; j < NUM_CARS; j++) {
            if (cars[j]->fitness > max_fit) {
                max_fit = cars[j]->fitness;
                max_fit_index = j;
            }
        }
        temp = cars[i];
        cars[i] = cars[max_fit_index];
        cars[max_fit_index] = temp;
    }
}

void new_cars(Car **cars, Map *map) {
    sort_cars(cars);
    for (int i=0; i < NUM_CARS; i++) {
        int j = i % NUM_SURVIVE;
        Car *new_car = create_car(map, cars[j]->net, i>=NUM_SURVIVE);
        free_car(cars[i], i>=NUM_SURVIVE);
        cars[i] = new_car;
    }
}