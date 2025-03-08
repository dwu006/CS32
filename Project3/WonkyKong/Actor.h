#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// Actor 

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld* playerWorld, int imageID, int startX, int startY);
    virtual void doSomething() = 0;
    bool isAlive();
    void kill();
    
    StudentWorld* getWorld() const;
    
    virtual bool isFloor() const { return false; }
    virtual bool isLadder() const { return false; }
    virtual bool isDestructiveEntity() const { return false; }
    virtual bool isAttackable() const { return false;}
    
    virtual void beAttacked() {};
    
private:
    StudentWorld* playerWorld;
    bool aliveStatus;
};

// Floor

class Floor : public Actor {
public:
    Floor(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool isFloor() const { return true; } // override
private:
};

// Ladder

class Ladder : public Actor {
public:
    Ladder(StudentWorld* world, int startX, int startY);
    virtual void doSomething() override;
    virtual bool isLadder() const override { return true; }
private:
};

// Kong

class Kong : public Actor {
public:
    Kong(StudentWorld* world, int startX, int startY, bool orient); // Note* orient is TRUE if RIGHT, FALSE if LEFT
    virtual void doSomething();
private:
    bool m_fleeing;
    int m_tickCount;
    int m_ticksSinceLastBarrel;
};

// Fireball

class Fireball : public Actor {
public:
    Fireball(StudentWorld* world, int startX, int startY);
    virtual void doSomething() override;
    virtual void beAttacked() override;
    virtual bool isAttackable() const override { return true;}
private:
    int m_tickCount;
    int m_climbState;
};

// Koopa

class Koopa : public Actor {
public:
    Koopa(StudentWorld* world, int startX, int startY);
    virtual void doSomething() override;
    virtual void beAttacked() override;
    virtual bool isAttackable() const override { return true;}
private:
    int m_freezeCooldown;
    int m_tickCount;
};

// Barrel

class Barrel : public Actor {
public:
    Barrel(StudentWorld* world, int startX, int startY, int direction);
    virtual void doSomething() override;
    virtual void beAttacked() override;
    virtual bool isAttackable() const override { return true;}
private:
    int m_tickCount;
};


// Bonfire

class Bonfire : public Actor {
public:
    Bonfire(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool isDestructiveEntity() const { return true; }
private:
};

// Extra Life

class ExtraLife : public Actor {
public:
    ExtraLife(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
private:
    // other implementations to be added later
};

// Garlic

class Garlic : public Actor {
public:
    Garlic(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
private:
    // other implementations to be added later
};

// Burp

class Burp : public Actor {
public:
    Burp(StudentWorld* world, int startX, int startY, int direction);
    virtual void doSomething();
private:
    int m_lifetime;
};

// Player

class Player : public Actor {
public:
    Player(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    void fall();
    void executeJump();
    
    virtual void beAttacked();
    virtual void beFrozen();
    void addBurps();
    void clearBurps();
    int numBurps() const;
    
private:
    int jumpTick;
    int burpCount;
    int m_freezeTimer;
};


#endif



