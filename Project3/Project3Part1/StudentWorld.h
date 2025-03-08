// Daniel Wu 706382792

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>
using namespace std;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Player;
class Floor;

class StudentWorld : public GameWorld
{
  public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    bool checkFloor(int x, int y);
    virtual int init();
    virtual int move();
    virtual void cleanUp();

  private:
    Player* player;
    vector<Floor*> floorVec;
    int pos(int x, int y);
};

#endif // STUDENTWORLD_H_
