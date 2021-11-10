#ifndef CELL_H
#define CELL_H


class Cell
{
    public:
        Cell();
        void aging();
        int getAge();
        void die();
        void live();
        bool isAlive();

    private:
        int age;
        bool alive;
};

#endif // CELL_H
