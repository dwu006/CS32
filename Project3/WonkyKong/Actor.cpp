#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor 

Actor::Actor(StudentWorld* playerWorld, int imageID, int startX, int startY) : GraphObject(imageID, startX, startY){
    // imageID will be the ID of whatever object (actor) we are spawning, i.e. floor is IID_FLOOR (which is a global constant found in GameConstants.h)
    // leave GraphObject initialized as 0, 0, 0 until I figure out what those numbers mean
    this->playerWorld = playerWorld;
    aliveStatus = true;
}

bool Actor::isAlive() { return aliveStatus; }
void Actor::kill() { aliveStatus = false; }

StudentWorld* Actor::getWorld() const {
    return playerWorld;
}

// Floor

Floor::Floor(StudentWorld* world, int startX, int startY) : Actor(world, IID_FLOOR, startX, startY) {

}

void Floor::doSomething() {
    
}

// Ladder

Ladder::Ladder(StudentWorld* world, int startX, int startY) : Actor(world, IID_LADDER, startX, startY) {

}

void Ladder::doSomething() {
    
}

// Kong

Kong::Kong(StudentWorld* world, int startX, int startY, bool orient) : Actor(world, IID_KONG, startX, startY) {
    if (orient)
        setDirection(0);
    else
        setDirection(180);
    
    m_fleeing = false;
    m_tickCount = 0;
    m_ticksSinceLastBarrel = 0;
}

void Kong::doSomething() {
    if (!isAlive())
            return;
            
        // Step 2: Increment Kong's animation.
        increaseAnimationNumber();
        
        // Step 3: Check the distance between Kong and the Player.
        int distance = getWorld()->distanceToPlayerSquared(this);
        if (distance <= 2.0)
        {
            m_fleeing = true;  // Transition into flee state.
        }
        
        // Step 4: If Kong is not fleeing, handle barrel throwing.
        if (!m_fleeing)
        {
            // Calculate the delay: N = max(200 - 50 * level, 50)
            int delay = 200 - 50 * getWorld()->getLevel();
            if (delay < 50)
                delay = 50;
            
            // If the required number of ticks have elapsed, throw a Barrel.
            if (m_ticksSinceLastBarrel >= delay)
            {
                int barrelX = (getDirection() == left ? getX() - 1 : getX() + 1);
                int barrelY = getY();
                // Create a new Barrel adjacent to Kong in the current facing direction.
                getWorld()->addActor(new Barrel(getWorld(), barrelX, barrelY, getDirection()));
                m_ticksSinceLastBarrel = 0;  // Reset the barrel timer.
            }
            else
            {
                m_ticksSinceLastBarrel++;  // Otherwise, increment the timer.
            }
        }
        
        // Step 5: Increment Kong's tick counter.
        m_tickCount++;
        
        // Step 6: Every 5 ticks, if Kong is fleeing, attempt to move up.
        if (m_tickCount % 5 == 0 && m_fleeing)
        {
            int newY = getY() + 1;  // Attempt to move up one square.
            // Check if moving up reaches (or exceeds) the top of the screen.
            // (Assume VIEW_HEIGHT is a defined constant representing the top; adjust as needed.)
            if (newY >= VIEW_HEIGHT)
            {
                getWorld()->increaseScore(1000);         // Award the Player 1000 points.
                getWorld()->playSound(SOUND_FINISHED_LEVEL); // Play the celebration sound.
                getWorld()->setLevelFinished();                 // Notify StudentWorld that the level is finished.
            }
            else
            {
                moveTo(getX(), newY);  // Otherwise, move up one square.
            }
        }
}

// Fireball 

Fireball::Fireball(StudentWorld* world, int startX, int startY) : Actor(world, IID_FIREBALL, startX, startY) {
    // other implementations
    m_tickCount = 0;
    m_climbState = 0;
}

void Fireball::doSomething() {
    if (!isAlive())
        return;
    
    if (getWorld()->isPlayerHere(this)) {
        getWorld()->attackPlayer();
        return;
    }
    
    m_tickCount++;
    
    if (m_tickCount % 10 == 0)
    {
        bool moved = false; // Flag to track if vertical (climbing) movement occurred.
        
        // --- Step 3a: Attempt to climb up ---
        // Conditions: current tile is a ladder, the tile above is not a floor (i.e., not blocked),
        // and the Fireball is not already in a climbing down state.
        if (getWorld()->checkLadder(getX(), getY()) &&
            !getWorld()->checkFloor(getX(), getY() + 1) &&
            m_climbState != -1)
        {
            // If already climbing up or a 1 in 3 chance occurs:
            if (m_climbState == 1 || (rand() % 3 == 0))
            {
                m_climbState = 1;               // Set state to climbing up.
                moveTo(getX(), getY() + 1);       // Move one tile up.
                moved = true;
            }
        }
        
        // --- Step 3b: Attempt to climb down ---
        // Only if no upward movement occurred.
        // Conditions: tile below is a ladder and the Fireball is not in a climbing up state.
        if (!moved &&
            getWorld()->checkLadder(getX(), getY() - 1) &&
            m_climbState != 1)
        {
            if (m_climbState == -1 || (rand() % 3 == 0))
            {
                m_climbState = -1;              // Set state to climbing down.
                moveTo(getX(), getY() - 1);       // Move one tile down.
                moved = true;
            }
        }
        
        // --- Step 3c: If in a climbing state but cannot continue climbing, reset state ---
        if (m_climbState == 1) // Climbing up.
        {
            if (getWorld()->checkFloor(getX(), getY() + 1) ||
                !getWorld()->checkLadder(getX(), getY() + 1))
            {
                m_climbState = 0;             // Reset to not climbing.
            }
        }
        else if (m_climbState == -1) // Climbing down.
        {
            if (getWorld()->checkFloor(getX(), getY() - 1) ||
                !getWorld()->checkLadder(getX(), getY() - 1))
            {
                m_climbState = 0;
            }
        }
        
        // --- Step 3d: Attempt horizontal movement if no vertical move occurred ---
        if (!moved)
        {
            int newX = (getDirection() == left ? getX() - 1 : getX() + 1);
            
            // Check if the tile in front is a floor (i.e., a block) OR there is no support (floor or ladder)
            // beneath the target tile.
            if (getWorld()->checkFloor(newX, getY()) ||
                (!getWorld()->checkFloor(newX, getY() - 1) && !getWorld()->checkLadder(newX, getY() - 1)))
            {
                // Reverse direction if movement is blocked or unsupported.
                setDirection(getDirection() == left ? right : left);
            }
            else
            {
                moveTo(newX, getY());
            }
        }
        
        // --- Step 3e: After movement, if on the same tile as the Player, attack.
        if (getWorld()->isPlayerHere(this)) {
            getWorld()->attackPlayer();
            return;
        }
    }
}

void Fireball::beAttacked() {
    // Step 1: Set the Fireball to a dead state.
    kill();
    
    getWorld()->GameWorld::playSound(SOUND_ENEMY_DIE);
    getWorld()->GameWorld::increaseScore(100);
    
    if (rand() % 3 == 0) {
        getWorld()->addActor(new Garlic(getWorld(), getX(), getY()));
    }
}

// Koopa

Koopa::Koopa(StudentWorld* world, int startX, int startY) : Actor(world, IID_KOOPA, startX, startY) {
    if (randInt(1, 2) == 1)
        setDirection(left);
    else
        setDirection(right);
    
    m_freezeCooldown = 0;
    m_tickCount = 0;
}

void Koopa::doSomething() {
    if (!isAlive())
        return;
    
    // Step 2: If the Koopa is on the same square as the Player and its freeze cooldown is 0,
    // freeze the Player and set the cooldown to 50.
    if (getWorld()->isPlayerHere(this) && m_freezeCooldown == 0) {
        getWorld()->freezePlayer();   // Freeze the Player (assumes freeze() handles the 50-tick duration).
        m_freezeCooldown = 50;                 // Set freeze cooldown timer.
        return;
    }
    
    // Step 3: If the Koopa’s freeze cooldown timer is greater than 0, decrement it.
    if (m_freezeCooldown > 0)
        m_freezeCooldown--;
    
    // Increment the tick counter (each call to doSomething() counts as one tick).
    m_tickCount++;
    
    // Step 4: Once every 10 ticks, attempt horizontal movement.
    if (m_tickCount % 10 == 0)
    {
        int newX = (getDirection() == left ? getX() - 1 : getX() + 1);
        
        // Step 4a: Check if the square in front is solid (i.e., a floor) OR if there is no support
        // (neither a floor nor a ladder) beneath the target square.
        if (getWorld()->checkFloor(newX, getY()) ||
            (!getWorld()->checkFloor(newX, getY() - 1) && !getWorld()->checkLadder(newX, getY() - 1)))
        {
            // Reverse direction if movement is blocked or unsupported.
            setDirection(getDirection() == left ? right : left);
        }
        else
        {
            // Otherwise, move horizontally.
            moveTo(newX, getY());
        }
        
        // Step 4b: After moving horizontally, check again for collision with the Player.
        if (getWorld()->isPlayerHere(this) && m_freezeCooldown == 0) {
            getWorld()->freezePlayer();
            m_freezeCooldown = 50;
            return;
        }
    }
}

void Koopa::beAttacked() {
    kill();
    getWorld()->GameWorld::playSound(SOUND_ENEMY_DIE);
    getWorld()->GameWorld::increaseScore(100);
    
    if (randInt(1, 3) == 1) // 1 in 3 chance of dropping an extra life
        getWorld()->addActor(new ExtraLife(getWorld(), getX(), getY()));
}

// Barrel

Barrel::Barrel(StudentWorld* world, int startX, int startY, int direction) : Actor(world, IID_BARREL, startX, startY) {
    GraphObject::setDirection(direction); // set direction
    m_tickCount = 0;
}

void Barrel::doSomething() {
    if (!isAlive())
        return;
    
    // Step 2: If the Barrel is on the same square as the Player, attack and return.
    if (getWorld()->isPlayerHere(this)) {
        getWorld()->attackPlayer();
        return;
    }
    
    // Step 3: If there is a destructive entity (e.g., a Bonfire) in the same square,
    // set the Barrel to dead and return.
    if (getWorld()->checkDestructiveEntity(getX(), getY()))
    {
        kill();
        return;
    }
    
    // Step 4: Check for falling.
    // If there is no solid floor directly beneath the Barrel, move one square down.
    if (!getWorld()->checkFloor(getX(), getY() - 1))
    {
        moveTo(getX(), getY() - 1);
        
        // NEW: Once the Barrel lands on a floor (i.e., there is now a floor beneath it),
        // reverse its horizontal direction.
        if (getWorld()->checkFloor(getX(), getY() - 1))
        {
            setDirection(getDirection() == left ? right : left);
        }
        return;  // End the tick after falling.
    }
    
    // Step 5: Once every 10 ticks, attempt horizontal movement.
    m_tickCount++;  // Increment the tick counter.
    if (m_tickCount % 10 == 0)
    {
        int newX = (getDirection() == left ? getX() - 1 : getX() + 1);
        
        // Step 5a: If the square in front is solid (a floor), reverse direction.
        if (getWorld()->checkFloor(newX, getY()))
        {
            setDirection(getDirection() == left ? right : left);
        }
        else
        {
            // Otherwise, move horizontally.
            moveTo(newX, getY());
        }
        
        // Step 5b: After horizontal movement, if the Barrel is on the same square as the Player, attack.
        if (getWorld()->isPlayerHere(this)) {
            getWorld()->attackPlayer();
            return;
        }
    }
}

void Barrel::beAttacked() {
    kill();
    
    // Step 2: Play the enemy death sound.
    getWorld()->playSound(SOUND_ENEMY_DIE);
    
    // Step 3: Award the Player 100 points for defeating the Barrel.
    getWorld()->increaseScore(100);
}

// Bonfire

Bonfire::Bonfire(StudentWorld* world, int startX, int startY) : Actor(world, IID_BONFIRE, startX, startY) {
   
}

void Bonfire::doSomething() {
    increaseAnimationNumber();
    if (getWorld()->isPlayerHere(this))
        getWorld()->attackPlayer();
}

// Extra Life

ExtraLife::ExtraLife(StudentWorld* world, int startX, int startY) : Actor(world, IID_EXTRA_LIFE_GOODIE, startX, startY) {
    // other implementations
}

void ExtraLife::doSomething() {
    if (!isAlive())
        return;
    
    if (getWorld()->isPlayerHere(this)) {
        getWorld()->GameWorld::increaseScore(50); // increase score by 50 points
        kill();
        getWorld()->GameWorld::playSound(SOUND_GOT_GOODIE);
        getWorld()->GameWorld::incLives();
    }
}

// Garlic

Garlic::Garlic(StudentWorld* world, int startX, int startY) : Actor(world, IID_GARLIC_GOODIE, startX, startY) {
    // other implementations
}

void Garlic::doSomething() {
    if (!isAlive())
        return;
    
    if (getWorld()->isPlayerHere(this)) {
        getWorld()->GameWorld::increaseScore(25); // increase score by 25 points
        kill();
        getWorld()->GameWorld::playSound(SOUND_GOT_GOODIE);
        getWorld()->addPlayerBurps();
    }
}

// Burp

Burp::Burp(StudentWorld* world, int startX, int startY, int direction) : Actor(world, IID_BURP, startX, startY) {
    // other implementations
    m_lifetime = 5;
    setDirection(direction);
}

void Burp::doSomething() {
    if (!isAlive())
            return;
        
        // Step 2: Decrement the remaining lifetime.
        m_lifetime--;
        if (m_lifetime <= 0)
        {
            kill();  // Lifetime has expired; mark the Burp as dead.
            return;     // Do nothing more this tick.
        }
        
        // Step 3: Attack any actor on the same square that can be blasted.
        // Assume getWorld()->getActorsAt(x, y) returns a vector of pointers to all actors at (x,y).
    vector<Actor*> actors = getWorld()->returnActors();
        for (Actor* actor : actors)
        {
            //fireball koopa barrel are attackable
            if (actor->isAttackable() && actor->getX() == getX() && actor->getY() == getY())
                actor->beAttacked();
        }
}

// Player

Player::Player(StudentWorld* playerWorld, int startX, int startY) : Actor(playerWorld, IID_PLAYER, startX, startY) {
    jumpTick = -1; // initialize the jump tick to -1, meaning jump NOT initiated
    burpCount = 0;
}

void Player::doSomething()
{
    if (!isAlive()) {
        return; // player has died, return immediately
    }
    
    //execute jump (if jumpTick is not 0, then we are currently jumping)
    if (jumpTick != -1) {
        executeJump();
        return;
    }

    cout << m_freezeTimer << endl;
    // frozen check
    if (m_freezeTimer > 0)
    {
        m_freezeTimer--;
        return;
    }
    // cout << "player is not frozen" << endl;
    
    fall(); //force the player to fall
    
    int ch;
    // cout << "ex" << endl;
    if (getWorld()->getKey(ch))
    {
        // user hit a key this tick!
        //cout << ch << endl;
        switch (ch)
        {
            case KEY_PRESS_LEFT: // move left
            cout << "left" << endl;
                if (getDirection() != 180) // change orientation (face left)
                    setDirection(180);
                else if (!getWorld()->checkFloor(getX()-1, getY()))
                    moveTo(getX()-1, getY()); // move left
                break;
                
            case KEY_PRESS_RIGHT: // move right
            cout << "right" << endl;
                if (getDirection() != 0) // change orientation (face right)
                    setDirection(0);
                else if (!getWorld()->checkFloor(getX()+1, getY()))
                    moveTo(getX()+1, getY()); // move right
                break;
                
            case KEY_PRESS_UP: // move up
            cout << "up" << endl;
                if (getWorld()->checkLadder(getX(), getY())) // move up
                    moveTo(getX(), getY()+1);
                break;
                
            case KEY_PRESS_DOWN: // move down
            cout << "down" << endl;
                if (getWorld()->checkLadder(getX(), getY()-1))
                    moveTo(getX(), getY()-1);
                break;
                
            case KEY_PRESS_SPACE: // jump
            cout << "space" << endl;
                // make sure you're only jumping if you're actually standing on something, otherwise abort
                if (getWorld()->checkLadder(getX(), getY()-1) || getWorld()->checkFloor(getX(), getY()-1)) {
                    jumpTick = 0;
                    getWorld()->GameWorld::playSound(SOUND_JUMP);
                    executeJump();
                }
                return;
                
            case KEY_PRESS_TAB:
            cout << "tab" << endl;
                if (burpCount <= 0)
                    break;
                int burpX = getX();
                int burpY = getY();
                if (getDirection() == left)
                    burpX--;  // Spawn one square to the left.
                else if (getDirection() == right)
                    burpX++;  // Spawn one square to the right.
                getWorld()->addActor(new Burp(getWorld(), burpX, burpY, getDirection()));
                burpCount--;
                getWorld()->GameWorld::playSound(SOUND_BURP);
                break;
        }
    }
}

// force the player to fall until reaching something that will stop the fall
// if the player is already on top of something, then nothing will happen
void Player::fall() {
    // continuously loop until the player lands on top of something
    if (!getWorld()->checkFloor(getX(), getY() - 1) // check if player is on top of a floor
        && !getWorld()->checkLadder(getX(), getY()) // if player is currently ON a ladder they hang onto it
        && !getWorld()->checkLadder(getX(), getY()-1)) // if a player is on TOP of a ladder they will sit on top
        moveTo(getX(), getY()-1);
}

int Player::numBurps() const { return burpCount; }
void Player::addBurps() { burpCount+=5; }
void Player::clearBurps() { burpCount = 0; } // clear the player burps upon death

void Player::beAttacked() { kill(); }

void Player::beFrozen() {
    // set frozen
    m_freezeTimer = 50;
}
//theres a bug that causes this to lag a little bit
void Player::executeJump() {
    switch (jumpTick) {
        case 0:
            if (!getWorld()->checkFloor(getX(), getY()+1)) {
                moveTo(getX(), getY()+1);
                jumpTick++;
            } else {
                jumpTick = -1;
            }
            if (getWorld()->checkLadder(getX(), getY())) {
                jumpTick = -1;
            }
            break;
        case 1: // moving horizontally
        case 2: // moving horizontally
        case 3: // moving horizontally
            if (getDirection() == 0) { // facing right
                if (!getWorld()->checkFloor(getX()+1, getY())) {
                    moveTo(getX() + 1, getY());
                    jumpTick++;
                }
                else
                    jumpTick = -1;
            } else {
                if (!getWorld()->checkFloor(getX()-1, getY())) {
                    moveTo(getX()-1, getY());
                    jumpTick++;
                }
                else
                    jumpTick = -1;
            }
            if (getWorld()->checkLadder(getX(), getY()))
                jumpTick = -1; // stop jump if currently on a ladder
            break;
        case 4: //faling
            if (!getWorld()->checkLadder(getX(), getY()-1)
                && !getWorld()->checkFloor(getX(), getY()-1)) {
                fall(); // continue executing fall until we hit ground
                return; // force jump animation to continue until we hit ground (by not allowing jumpTick to equal -1, which terminates jump
            }
            jumpTick = -1; // stop (end) jump
    }
}
