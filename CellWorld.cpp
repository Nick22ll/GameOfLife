#include "CellWorld.h"
using namespace std;

//Standard Cell World constructor
CellWorld::CellWorld() {
    initialize(60, 40);
}

//Custom size Cell World constructor
CellWorld::CellWorld(int rows, int columns, bool randomGeneration) {
    if(randomGeneration)
        initialize(rows, columns, randomAlivePopulation(rows, columns));
    else
        initialize(rows, columns);
}

//Constructor using an external file path
CellWorld::CellWorld(string loadingPath) {
    loadFromFile(loadingPath, this);
}

CellWorld& CellWorld::operator= (const CellWorld& cw){
    // self-assignment guard
        if (this == &cw)
            return *this;

        world = cw.world;
        changed = cw.changed;
        aliveCells = cw.aliveCells;
        dimensions[0] = cw.dimensions[0];
        dimensions[1] = cw.dimensions[1];
        agingFlag = cw.agingFlag;
        deadCellCounter = cw.deadCellCounter;

        // return the existing object so we can chain this operator
        return *this;
}

void CellWorld::initialize(int rows, int columns)
{
    if (!world.empty()){
        clear();
    }
    for (int i = 0; i < rows; i++)
        world.push_back(vector<Cell>(columns));

    dimensions[0] = rows;
    dimensions[1] = columns;

    initialState.clear();
}

void CellWorld::initialize(int rows, int columns, unordered_set<pair<int, int>, SimpleHash> alive)
{
    if (!world.empty())
        clear();
    for (int i = 0; i < rows; i++)
        world.push_back(vector<Cell>(columns));

    dimensions[0] = rows;
    dimensions[1] = columns;

    aliveCells = initialState = alive;
    for(const auto &iter:aliveCells){
        if(valid_position(iter.first,iter.second)){
            world[iter.first][iter.second].live();
            addChange(iter.first,iter.second);
        }
    }

}

int CellWorld::alivePopulation(){
    return aliveCells.size();
}

int CellWorld::overallDeads(){
    return deadCellCounter;
}

bool CellWorld::isAlive(int row, int col){
    if(valid_position(row,col))
        return world[row][col].isAlive();
    return false;
}

int CellWorld::updateTime(){
    return lastUpdateTime;
}

void CellWorld::clear()
{
    changed.clear();
    aliveCells.clear();
    deadCellCounter = 0;
    world.clear();
}

void CellWorld::reset(){

    clear();
    initialize(dimensions[0], dimensions[1], initialState);
}

void CellWorld::modify(int posx, int posy)
{
    if (valid_position(posx, posy)) {
        if(world[posx][posy].isAlive()){
            world[posx][posy].die();
            deadCellCounter++;
            aliveCells.erase({posx, posy});
        }
        else{
             world[posx][posy].live();
             aliveCells.insert({posx, posy});
        }
        addChange(posx, posy);
    }
}

//This function applies GameOfLife rules to update the world (to increase the computation speed, the function applies the rules of the t-th step only on the cells changed on the (t-1)-th step)
void CellWorld::update()
{
    auto start = chrono::high_resolution_clock::now();
    vector<vector<Cell>> newWorld = world;
    unordered_set<pair<int,int>, SimpleHash> oldChanged = changed;
    changed.clear();

    //Aging of the curent population
    if(agingFlag){
        for (const auto& elem : aliveCells) {
            int posx = elem.first;
            int posy = elem.second;
            newWorld[posx][posy].aging();
        }
    }

    //Calculate the births and deaths
    for (const auto& elem : oldChanged){
        int posx = elem.first;
        int posy = elem.second;
        int n_neighbors = neighborsSum(posx, posy);
        if (world[posx][posy].isAlive() && (n_neighbors >= 4 || n_neighbors <= 1)) {  //die from loneliness or overpopulation
            newWorld[posx][posy].die();
            deadCellCounter++;
            aliveCells.erase({posx, posy});
            addChange(posx, posy);
        }
        if (!world[posx][posy].isAlive() && n_neighbors == 3) {  //live from 3 neighbors
            newWorld[posx][posy].live();
            aliveCells.insert({posx, posy});
            addChange(posx, posy);
        }
    }
    world = newWorld;
    auto stop = chrono::high_resolution_clock::now();
    lastUpdateTime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    int a;
}

void CellWorld::setAging(bool b){
    agingFlag = b;
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

unordered_set<pair<int,int>, SimpleHash> CellWorld::getAliveCells(){
    return aliveCells;
}

void CellWorld::saveToFile(CellWorld* cw, string path){
    ofstream file;
    file.open(path, ios::out);
    //Save the matrix dimensions
    file<<"DIMENSIONS "<<cw->width()<<" "<<cw->height()<<"\n";

    //Save alive cells
    for(const auto &iter:cw->getAliveCells()){
        file<<"ALIVE "<<iter.first<<" "<<iter.second<<"\n";
    }
    file.close();
}

//This function load a CellWorld state from an external file
void CellWorld::loadFromFile(string path, CellWorld* cw){
    int dimensions[2] = {0,0};
    int tempArray[2] = {0,0};
     std::unordered_set<pair<int,int>, SimpleHash> loadAliveCells;
     string line;
     ifstream file;
     file.open(path, ios::in);
     string delimiter = " ";
     if (file.is_open())
       {
         while(file)
         {
             string keyWord;
             file>>keyWord;
             if(keyWord == "DIMENSIONS")
                 file>>dimensions[0]>>dimensions[1];
             if(keyWord == "ALIVE"){
                 file>>tempArray[0]>>tempArray[1];
                 loadAliveCells.insert(make_pair(tempArray[0], tempArray[1]));
             }
         }
         cw->initialize(dimensions[0], dimensions[1], loadAliveCells);
         file.close();
       }
}

//This function populate the world randomly
unordered_set<pair<int,int>, SimpleHash> randomAlivePopulation(int xLimit, int yLimit)
{
    unordered_set<pair<int,int>, SimpleHash> result;
    srand(time(NULL));
    float aliveProbability = 10.0; //in percentage 10%
    for(int i=0;i<xLimit;i++)
        for(int j=0; j<yLimit; j++)
            if((rand()%100+1)<=aliveProbability)
                result.insert(make_pair(i,j));
    return result;
}

