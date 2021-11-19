#ifndef CELLWORLD_H
#define CELLWORLD_H
#include "cell.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<unordered_set>
#include <ctime>
#include<QObject>
#include <chrono>

using namespace std;
struct SimpleHash {
    size_t operator()(const std::pair<int, int>& p) const {
        return p.first ^ p.second;
    }
};

unordered_set<pair<int,int>, SimpleHash> randomAlivePopulation(int xLimit, int yLimit);


class CellWorld : public QObject
{
public:
    CellWorld();
    CellWorld(int rows, int columns, bool randomGeneration);
    CellWorld(string loadingPath);

    CellWorld& operator= (const CellWorld& cw);

    void initialize(int rows, int columns);
    void initialize(int rows, int columns, unordered_set<pair<int,int>, SimpleHash> alive);

    void setAging(bool b);

    void clear();
    void reset();
    void modify(int posx, int posy);
    void update();
    int neighborsSum(int posx, int posy);

    int width();
    int height();
    int alivePopulation();
    int overallDeads();
    int updateTime();
    bool isAlive(int row, int col);
    vector<vector<Cell>> getState();
    unordered_set<pair<int,int>, SimpleHash> getChanged();
    unordered_set<pair<int,int>, SimpleHash> getAliveCells();

    void saveToFile(CellWorld* cw, string path);
    void loadFromFile(string path, CellWorld* cw);

protected:
    void addChange(int posx, int posy);

private:
    bool valid_position(int x, int y);
    vector<vector<Cell>> world;
    unordered_set<pair<int,int>, SimpleHash> changed;
    unordered_set<pair<int,int>, SimpleHash> aliveCells;
    unordered_set<pair<int,int>, SimpleHash> initialState;

    int dimensions[2] = { 0, 0 };
    int lastUpdateTime;
    int deadCellCounter = 0;
    //Flags
    bool agingFlag = true;
};

#endif // CELLWORLD_H

