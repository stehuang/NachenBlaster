#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
//#include "Actor.h"
#include <string>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class NachenBlaster;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    // removes dead objects
    void removeDead();
    
    // completes current level
    bool completeLevel() const;
    
    // increase existing alien
    void increaseAlienCount();
    
    // increase aliens destroyed
    void destroyAlien();
    
    // introduce alien
    void addAliens();
    
    // adds a cabbage
    void addActor(Actor* a);
    
    // returns pointer to nachenblaster
    NachenBlaster* getShip() const;

    // checks if objects collides
    bool shipThere(int x, int y, double radius) const;
    Actor* alienThere(int x, int y, double radius);
    NachenBlaster* getShip(int x, int y, double radius);

    
private:
    NachenBlaster* ship;
    list <Actor*> actors;
    int aliensDestroyed;
    int currentAliens;
};

#endif // STUDENTWORLD_H_

















