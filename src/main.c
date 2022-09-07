#include <stdio.h>
#include <SFML/Graphics.h>
#include <lines.h>

int main() {
    sfRenderWindow *window = sfRenderWindow_create((sfVideoMode){800, 600}, "Car Neural Net", sfClose, NULL);
    printf("-2\n");

    
    Line lines[4];
    lines[0] = (Line){.p1 = (Point){.x = 100, .y = 100}, .p2 = (Point){.x = 700, .y = 100}};
    lines[1] = (Line){.p1 = (Point){.x = 700, .y = 100}, .p2 = (Point){.x = 700, .y = 500}};
    lines[2] = (Line){.p1 = (Point){.x = 700, .y = 500}, .p2 = (Point){.x = 100, .y = 500}};
    lines[3] = (Line){.p1 = (Point){.x = 100, .y = 500}, .p2 = (Point){.x = 100, .y = 100}};

    Map *map = create_map(lines, 4);

    sfEvent event;
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
        sfRenderWindow_clear(window, sfBlack);
        render(map, window);
        sfRenderWindow_display(window);
    }
}