#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}



StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    levelFinished = false;
}

StudentWorld::~StudentWorld() {
    //destructor implementation
    cleanUp();
}

void StudentWorld::addPlayerBurps() {
    player->addBurps();
}

void StudentWorld::addActor(Actor* actor) {
    actorContainer.push_back(actor);
}

//It must create the Player actor and
//place it into the level at the correct starting location (see the Level class section
//of this document for details). It must also create all of the Floor objects and add
//them into the level as specified in the current Level’s data file.
int StudentWorld::init()
{
    Level lev(assetPath());
    if (this->getLevel() > 99)
        return GWSTATUS_PLAYER_WON;
    std::ostringstream levelStream;
    levelStream << std::setw(2) << std::setfill('0') << this->getLevel();
    std::string levelStr = levelStream.str();

    Level::LoadResult result = lev.loadLevel("level" + levelStr + ".txt");
    if (result == Level::load_fail_file_not_found)
        return GWSTATUS_PLAYER_WON;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success)
    {
        int x = 0;
        int y = 0;
        for (x = 0; x < 20; x++) {
            for (y = 0; y < 20; y++) {
                Level::MazeEntry me = lev.getContentsOf(x, y); // x=5, y=10
                switch (me)
                {
                    case Level::floor:
                        actorContainer.push_back(new Floor(this, x, y));
                        break;
                    case Level::ladder:
                        actorContainer.push_back(new Ladder(this, x, y));
                        break;
                    case Level::left_kong:
                        actorContainer.push_back(new Kong(this, x, y, false));
                        break;
                    case Level::right_kong:
                        actorContainer.push_back(new Kong(this, x, y, true));
                        break;
                    case Level::fireball:
                        actorContainer.push_back(new Fireball(this, x, y));
                        break;
                    case Level::koopa:
                        actorContainer.push_back(new Koopa(this, x, y));
                        break;
                    case Level::bonfire:
                        actorContainer.push_back(new Bonfire(this, x, y));
                        break;
                    case Level::extra_life:
                        actorContainer.push_back(new ExtraLife(this, x, y));
                        break;
                    case Level::garlic:
                        actorContainer.push_back(new Garlic(this, x, y));
                        break;
                    case Level::player:
                        player = new Player(this, x, y);
                        break;
                    case Level::empty:
                        break;
                }
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

//During each tick, it
//must ask your Player and Floors to do something. Your move() method need
//not check to see if Player has died or not; you may assume at this point that the
//Player cannot die. Your move() method does not have to deal with any actors
//other than the Player and Floors for Part #1.
int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q
    
    setGameStatText("Game will end when you type q");
    
    //update screen
    setDisplayText();
    
    player->doSomething(); // ask Player to do something
    for (Actor* actor : actorContainer)
        if (actor->isAlive()) // make sure the actor is still alive
            actor->doSomething(); // ask all actors to do something
    
    // remove dead actors
    for (size_t i = actorContainer.size(); i > 0; --i) {
        size_t index = i - 1;
        if (!actorContainer[index]->isAlive()) {
            
            delete actorContainer[index];
            actorContainer.erase(actorContainer.begin() + index);
        }
    }
    
    if (!player->isAlive()) { // if player died, return code GWSTATUS_PLAYER_DIED
        decLives();
        player->clearBurps();
        
        return GWSTATUS_PLAYER_DIED;
    }
    
    if (levelFinished) { // Note* the Kong logic already increments score
        levelFinished = false;
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

std::string generate_stats(int score, int level, int livesLeft, unsigned int burps) {
    std::stringstream ss;

    ss << "Score: " << std::setw(7) << std::setfill('0') << score
       << " Level: " << std::setw(2) << std::setfill('0') << level
       << " Lives: " << std::setw(2) << std::setfill('0') << livesLeft
       << " Burps: " << std::setw(2) << std::setfill('0') << burps;

    return ss.str();
}

void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    int livesLeft = getLives();
    unsigned int burps = getCurrentPlayerBurps();
    // Next, create a string from your statistics, of the form:
    // Score: 0000100 Level: 03 Lives: 03 Burps: 08
    
    string s = generate_stats(score, level, livesLeft, burps);
    
    // Finally, update the display text at the top of the screen with your
    // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

int StudentWorld::getCurrentPlayerBurps() const { return player->numBurps(); }
bool StudentWorld::isPlayerHere(Actor* a) const { return (player->getX() == a->getX() && player->getY() == a->getY()); }
void StudentWorld::attackPlayer() { player->beAttacked(); }
void StudentWorld::setLevelFinished() { levelFinished = true; }
void StudentWorld::freezePlayer() {
    player->beFrozen();
}
vector<Actor*> StudentWorld::returnActors() {
    return actorContainer;
}

int StudentWorld::distanceToPlayerSquared(Actor* actor) const {
    int playerX = player->getX();
    int playerY = player->getY();
    int dx = playerX - actor->GraphObject::getX();
    int dy = playerY - actor->GraphObject::getY();
    
    return (int)std::sqrt(dx * dx + dy * dy);
}

//Implement a cleanUp() method that frees any dynamically allocated data that
//was allocated during calls to the init() method or the move() method (e.g., it
//should delete all your allocated Floors and the Player). Note: Your
//StudentWorld class must have ]both a destructor and the cleanUp() method
//even though they likely do the same thing (in which case the destructor could just
//call cleanUp())
void StudentWorld::cleanUp()
{
    if (player != nullptr) {
        delete player; // clean up player object
        player = nullptr;
    }
    
    while (!actorContainer.empty()){
        Actor* tempActor = actorContainer.back();
        actorContainer.pop_back();
        delete tempActor;
    }
}

// returns TRUE if a floor exists at (x, y), i.e. you can NOT move the player to that position
// returns FALSE if a floor does NOT exist at (x, y), i.e. you CAN move the player to that position
bool StudentWorld::checkFloor(int x, int y) {
    for (Actor* actor : actorContainer) {
        if (actor->isFloor() && actor->getX() == x && actor->getY() == y)
            return true;
    }
    return false;
}

// returns true if there is a ladder there
bool StudentWorld::checkLadder(int x, int y) {
    for (Actor* actor : actorContainer) {
        if (actor->isLadder() && actor->getX() == x && actor->getY() == y)
            return true;
    }
    return false;
}

// returns true if there is something capable of destroying (a barrel)
bool StudentWorld::checkDestructiveEntity(int x, int y) {
    for (Actor* actor : actorContainer) {
        if (actor->isDestructiveEntity() && actor->getX() == x && actor->getY() == y)
            return true;
    }
    return false;
}

