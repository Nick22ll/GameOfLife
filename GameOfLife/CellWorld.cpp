#include "CellWorld.h"



using namespace std;



CellWorld::CellWorld() {
    initialize(100, 100);
}

CellWorld::CellWorld(int rows, int columns) {
    initialize(rows, columns);
}


void CellWorld::initialize(int rows, int columns)
{
    if (!world.empty())
        world.clear();
    for (int i = 0; i < rows; i++)
        world.push_back(vector<Cell>(columns));

    dimensions[0] = rows;
    dimensions[1] = columns;

    world[20][20].live();
    addChange(20, 20);
    world[20][21].live();
    addChange(20, 21);
    world[21][20].live();
    addChange(21, 20);
    world[22][21].live();
    addChange(22, 21);
    world[23][20].live();
    addChange(23, 20);
    world[19][21].live();
    addChange(19, 21);
    world[19][22].live();
    addChange(19, 22);
    world[20][22].live();
    addChange(20, 22);
    world[19][19].live();
    addChange(20, 22);

    world[18][19].live();
    addChange(20, 22);

    world[23][22].live();
    addChange(20, 22);


}



void CellWorld::clear()
{
    this->initialize(world.size(), world[0].size());
}

void CellWorld::modify(int posx, int posy)
{
    if (valid_position(posx, posy)) {
        world[posx][posy].isAlive() ? world[posx][posy].die() : world[posx][posy].live();
        addChange(posx, posy);
    }
}

void CellWorld::update()
{
    vector<vector<Cell>> newWorld = world;
    unordered_set<pair<int,int>, SimpleHash> oldChanged = changed;
    changed.clear();

    for (const auto& elem : oldChanged) {
        int posx = elem.first;
        int posy = elem.second;
        int n_neighbors = neighborsSum(posx, posy);
        if (world[posx][posy].isAlive() && (n_neighbors >= 4 || n_neighbors <= 1)) {  //die from loneliness or overpopulation
            newWorld[posx][posy].die();
            addChange(posx, posy);
        }
        if (!world[posx][posy].isAlive() && n_neighbors == 3) {  //live from 3 neighbors
            newWorld[posx][posy].live();
            addChange(posx, posy);
        }
    }
    world = newWorld;
}

int CellWorld::neighborsSum(int posx, int posy)
{
    int neighbors = 0;
    neighbors += int((valid_position(posx - 1, posy - 1) && world[posx - 1][posy - 1].isAlive()));
    neighbors += int((valid_position(posx - 1, posy) && world[posx - 1][posy].isAlive()));
    neighbors += int((valid_position(posx - 1, posy + 1) && world[posx - 1][posy + 1].isAlive()));

    neighbors += int((valid_position(posx, posy - 1) && world[posx][posy - 1].isAlive()));
    neighbors += int((valid_position(posx, posy + 1) && world[posx][posy + 1].isAlive()));

    neighbors += int((valid_position(posx + 1, posy - 1) && world[posx + 1][posy - 1].isAlive()));
    neighbors += int((valid_position(posx + 1, posy) && world[posx + 1][posy].isAlive()));
    neighbors += int((valid_position(posx + 1, posy + 1) && world[posx + 1][posy + 1].isAlive()));
    return neighbors;
}

bool CellWorld::valid_position(int x, int y) {
    return bool(((0 <= x) && (x < dimensions[0]) && (0 <= y) && (y < dimensions[1])));
}

vector<vector<Cell>> CellWorld::getState()
{
    return world;
}

int CellWorld::width() {
    return dimensions[0];
}

int CellWorld::height() {
    return dimensions[1];
}

void CellWorld::addChange(int posx, int posy) {
    if (valid_position(posx, posy))
    {
        changed.insert({ posx,posy });
        if (valid_position(posx - 1, posy - 1)) { changed.insert({ posx - 1, posy - 1 }); }
        if (valid_position(posx - 1, posy)) { changed.insert({ posx - 1, posy }); }
        if (valid_position(posx - 1, posy + 1)) { changed.insert({ posx - 1, posy + 1 }); }
        if (valid_position(posx, posy - 1)) { changed.insert({ posx, posy - 1 }); }
        if (valid_position(posx, posy + 1)) { changed.insert({ posx, posy + 1 }); }
        if (valid_position(posx + 1, posy - 1)) { changed.insert({ posx + 1, posy - 1 }); }
        if (valid_position(posx + 1, posy)) { changed.insert({ posx + 1, posy }); }
        if (valid_position(posx + 1, posy + 1)) { changed.insert({ posx + 1, posy + 1 }); }
    }
}


unordered_set<pair<int,int>, SimpleHash> CellWorld::getChanged(){
    return changed;
}
