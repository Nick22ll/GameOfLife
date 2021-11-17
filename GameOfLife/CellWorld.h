#ifndef CELLWORLD_H
#define CELLWORLD_H
#include <vector>
#include<unordered_set>

#include "cell.h"
#include<QObject>

using namespace std;
struct SimpleHash {
    size_t operator()(const std::pair<int, int>& p) const {
        return p.first ^ p.second;
    }
};


class CellWorld : public QObject
{
public:
    CellWorld();
    CellWorld(int rows, int columns);

    void initialize(int rows, int columns);
    void clear();
    void modify(int posx, int posy);
    void update();
    void setAging(bool b);
    int neighborsSum(int posx, int posy);
    int width();
    int height();
    int alivePopulation();
    int overallDeads();
    bool isAlive(int row, int col);
    vector<vector<Cell>> getState();
    unordered_set<pair<int,int>, SimpleHash> getChanged();
    unordered_set<pair<int,int>, SimpleHash> getAliveCells();

protected:
    void addChange(int posx, int posy);

private:

    bool valid_position(int x, int y);
    vector<vector<Cell>> world;
    unordered_set<pair<int,int>, SimpleHash> changed;
    unordered_set<pair<int,int>, SimpleHash> aliveCells;

    int dimensions[2] = { 0, 0 };
    int deadCellCounter = 0;
    //Flags
    bool agingFlag = true;
};

#endif // CELLWORLD_H

