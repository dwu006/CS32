// Daniel Wu 706382792

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
    public:
        Actor(int imageID, int startX, int startY);
        virtual void doSomething() = 0;
    private:
};

class Floor : public Actor 
{
    public:
        Floor(int startX, int startY);
        virtual void doSomething();
    private:
};

class StudentWorld;

class Player : public Actor 
{
    public:
        Player(int startX, int startY, StudentWorld* playerWorld);
        virtual void doSomething();
        StudentWorld* getWorld();
    
    private:
        StudentWorld* playerWorld;
};
#endif // ACTOR_H_
