#include <stdio.h>
#include <SFML/Graphics.h>
#include <lines.h>

int main() {
    sfRenderWindow *window = sfRenderWindow_create((sfVideoMode){800, 600}, "Car Neural Net", sfClose, NULL);

    
    Line lines[2];
    lines[0] = (Line){.p1 = (Point){.x = 100, .y = 100}, .p2 = (Point){.x = 700, .y = 100}};
    lines[1] = (Line){.p1 = (Point){.x = 700, .y = 100}, .p2 = (Point){.x = 700, .y = 500}};

    Point intersect;
    lines_intersect(lines[0], lines[1], &intersect);
    printf("%i, %i\n", intersect.x, intersect.y);

    Map *map = create_map(lines, 2);

    sfEvent event;
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
        sfRenderWindow_clear(window, sfBlack);
        render(map, window);
        sfCircleShape *circle = sfCircleShape_create();
        sfCircleShape_setPosition(circle, (sfVector2f){(float)intersect.x-6, (float)intersect.y-6});
        sfCircleShape_setFillColor(circle, sfRed);
        sfCircleShape_setRadius(circle, 6);
        sfRenderWindow_drawCircleShape(window, circle, NULL);
        sfCircleShape_destroy(circle);
        sfRenderWindow_display(window);
    }
}