// Daniel Wu 706382792


#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, int startX, int startY) : GraphObject(imageID, startX, startY) {}

Floor::Floor(int startX, int startY) : Actor(IID_FLOOR, startX, startY) {}

void Floor::doSomething() {
    
}

Player::Player(int startX, int startY, StudentWorld* playerWorld) : Actor(IID_PLAYER, startX, startY) {
    this->playerWorld = playerWorld;
}

void Player::doSomething()
{
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {

            case KEY_PRESS_RIGHT:
                if (getDirection() != 0)
                    setDirection(0);
                else if (playerWorld->checkFloor(getX()+1, getY()))
                    moveTo(getX()+1, getY()); 
                break;

            case KEY_PRESS_LEFT: 
            if (getDirection() != 180)
                setDirection(180);
            else if (playerWorld->checkFloor(getX()-1, getY()))
                moveTo(getX()-1, getY());
            break;
                
        }
    }
}

StudentWorld* Player::getWorld() {return playerWorld;}