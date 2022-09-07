#include <stdlib.h>
#include <SFML/Graphics.h>
#include <lines.h>
#include <car.h>

Car *create_car() {
    Car *car = calloc(1, sizeof(Car));
    return car;
}

void free_car(Car *car) {
    free(car);
}

int collide_line(Car *car, Line line);