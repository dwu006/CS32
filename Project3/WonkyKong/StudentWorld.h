#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

// StudentWorld

class Player;
class Floor;

class StudentWorld : public GameWorld
{
  public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    bool checkFloor(int x, int y);
    bool checkLadder(int x, int y);
    bool checkDestructiveEntity(int x, int y);
    vector<Actor*> returnActors();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isPlayerHere(Actor* a) const;
    void attackPlayer();
    void addPlayerBurps();
    void freezePlayer(); // koopa logic
    void addActor(Actor* actor);
    int distanceToPlayerSquared(Actor* actor) const;
    void setLevelFinished();
  private:
    Player* player;
    vector<Actor*> actorContainer;
    int posInfo(int x, int y);
    void setDisplayText();
    std::string generateStats(int score, int level, int livesLeft, unsigned int burps) const;
    int getCurrentPlayerBurps() const;
    bool levelFinished;
};

#endif
