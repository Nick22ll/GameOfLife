#include "cell.h"

Cell::Cell() {
    this->alive = false;
    this->age = 0;
}

void Cell::aging()
{
    this->age++;
}

int Cell::getAge()
{
    return this->age;
}

void Cell::die()
{
    this->alive = false;

}

void Cell::live()
{
    this->alive = true;
    age = 0;
}

bool Cell::isAlive() {
    return alive;
}
