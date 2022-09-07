#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.h>

typedef struct line_t Line;

typedef struct car_t {
    sfVector2f size;
    sfVector2f rect;

    sfVector2f pos;
    sfVector2f vel;
} Car;

Car *create_car();
void free_car(Car *car);
int collide_line(Car *car, Line line);

#endif