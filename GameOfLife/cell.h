#ifndef CELL_H
#define CELL_H

//This class represents a cell of a CellWorld, it can born, die and age.
class Cell
{
    public:
        Cell();
        void aging();
        int getAge();
        void die();
        void live();
        bool isAlive();
        template<class Archive>
            void serialize(Archive& ar, const unsigned int version) {
                ar & age;
                ar & alive;
            }
    private:
        int age;
        bool alive;
};

#endif // CELL_H
