#ifndef CELLWORLD_H
#define CELLWORLD_H
#include <vector>
#include<unordered_set>

#include "cell.h"

using namespace std;
struct SimpleHash {
    size_t operator()(const std::pair<int, int>& p) const {
        return p.first ^ p.second;
    }
};


class CellWorld
{
public:
    CellWorld();
    CellWorld(int rows, int columns);

    void initialize(int rows, int columns);
    void clear();
    void modify(int posx, int posy);
    void update();
    int neighborsSum(int posx, int posy);
    int width();
    int height();
    vector<vector<Cell>> getState();
    unordered_set<pair<int,int>, SimpleHash> getChanged();
protected:
    void addChange(int posx, int posy);

private:

    bool valid_position(int x, int y);
    vector<vector<Cell>> world;
    unordered_set<pair<int,int>, SimpleHash> changed;
    int dimensions[2] = { 0, 0 };
    int aliveCells = 0;
};

#endif // CELLWORLD_H

