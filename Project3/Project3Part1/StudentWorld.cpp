// Daniel Wu 706382792

#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld() {}

int StudentWorld::init()
{
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel("level01.txt");
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find level01.txt data file\n";
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr << "Your level was improperly formatted\n";
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level\n";
        for (int x = 0; x < 20; x++) {
            for (int y = 0; y < 20; y++) {
                Level::MazeEntry p = lev.getContentsOf(x, y);
                switch (p)
                {
                    case Level::floor:
                        floorVec.push_back(new Floor(x, y));
                        break;
                    case Level::player:
                        player = new Player(x, y, this);
                        break;
                }
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    setGameStatText("Game will end when you type q");

    player->doSomething();
    for(Floor* floor : floorVec)
    {
        floor->doSomething();
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (player != nullptr) {
        delete player; 
        player = nullptr;
    }

    while (!floorVec.empty()){
        Floor* temp = floorVec.back();
        floorVec.pop_back();
        delete temp;
    }
}
bool StudentWorld::checkFloor(int x, int y)
{
    for(Floor* floor : floorVec)
    {
        if(floor->getX() == x && floor->getY() == y) {return false;}
    }
    return true;
}
