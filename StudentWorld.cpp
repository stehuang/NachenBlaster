#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Actor.h"
using namespace std;

// euclidian distance
double euclidian_dist(double x1, double y1, double x2, double y2){
    double r = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
    return r;
}


int min(int a, int b){
    if(a<=b)
        return a;
    else
        return b;
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
    aliensDestroyed = 0;
    currentAliens = 0;
}


StudentWorld::~StudentWorld(){
    cleanUp();
}


int StudentWorld::init()
{
    currentAliens=0;
    aliensDestroyed=0;
    playSound(0);
    ship = new NachenBlaster(IID_NACHENBLASTER, 0, 128, 0, 1, 0, 30, 50,this);
    for(int i=0; i<30; i++){
        actors.push_back(new Star(IID_STAR, randInt(0, VIEW_WIDTH-1), randInt(0, VIEW_HEIGHT-1), 0, (double)(randInt(5, 50))/100.0, 3));
    }

    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    // introducing new stars
    if(randInt(1, 15) == 1){
        actors.push_back(new Star(IID_STAR, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), 0, (double)(randInt(5, 50))/100.0, 3));
    }

    // add aliens if needed
    addAliens();
    
    
    // make ship do something
    ship->doSomething();
    
    //  making actors do something
    list<Actor*>::iterator i;
    i = actors.begin();
    for(; i != actors.end(); i++){
        if((*i)->isAlive()){
            (*i)->doSomething();
            if(!ship->isAlive()){
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }
    

    // removing dead actors
    removeDead();
    
    
    // check if player completed a level
    if(completeLevel()){
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    if(ship->isAlive()==false){     // if player dies
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    
    // display stats
    ostringstream output;
    output.setf(ios::fixed);
    setprecision(0);
    output.fill(' ');
    output << setprecision(0) << "Lives: " << setw(2) << getLives() << "  Health: " << setw(3) << ship->getHitPoints()/50.0*100 << "%" << "  Score: " << setw(6) << getScore() << " Level: " << setw(2) << getLevel() << "  Cabbages: " << setw(2) << ship->getCabbage()/30.0*100 << "%" << "  Torpedoes: " << setw(2) << ship->getTorpedo();
    setGameStatText(output.str());

    
    // continue the game
    return GWSTATUS_CONTINUE_GAME;
}

// clean up function (i.e. destructor)
void StudentWorld::cleanUp()
{
    // iterates through vec of actors and deletes dead ones
    list<Actor*>::iterator i;
    i = actors.begin();

    while(i!=actors.end()){
        Actor* object = *i;
        delete object;
        i = actors.erase(i);
    }
    if(ship != nullptr){
        delete ship;
        ship = nullptr;
    }
}

// removes dead objects
void StudentWorld::removeDead(){
    // removing dead actors
    // iterates through list of actors and deletes dead ones
    list<Actor*>::iterator i;
    i = actors.begin();
    for(; i != actors.end(); i++){
        if((*i)->isAlive()==false){
            if((*i)->isAlien()){
                currentAliens--;
            }
//            delete *i; // deletes the actor object and pointer to the actor
//            actors.erase(i);
//            --i;
            Actor* object = *i;
            delete object;
            i = actors.erase(i);
        }
    }
}


// completes a level
bool StudentWorld::completeLevel() const{
    int min = 6+(4*getLevel());
    return (aliensDestroyed >= min);
}

// increase alien count
void StudentWorld::increaseAlienCount(){
    currentAliens++;
}


// increases the # of aliens destroyed by 1
void StudentWorld::destroyAlien(){
    aliensDestroyed++;
}

// add aliens
void StudentWorld::addAliens(){
    int total = 6+(4*(getLevel()));
    int remaining = total - aliensDestroyed;
    int max = 4 + (0.5*(getLevel()));
    if(currentAliens < min(max, remaining)){
        // ADD ALIEN
        int s1 = 60;
        int s2 = 20+(getLevel())*5;
        int s3 = 5+(getLevel())*10;
        int sTotal = s1+s2+s3;
        int alien = randInt(1, sTotal);
        
        if(alien<=s1){
            // add smallgon
            int startX = VIEW_WIDTH-1;
            int startY = randInt(0, VIEW_HEIGHT-1);
            Actor* temp = new Smallgon(IID_SMALLGON, startX, startY, 0, 1.5, 1, 5*(1+(getLevel()-1)*0.1), 0, 2, 0, this);
            addActor(temp);
            return;
        }
        else if(alien > s1 && alien <= s1+s2){
            // add smoregon
            int startX = VIEW_WIDTH-1;
            int startY = randInt(0, VIEW_HEIGHT-1);
            Actor* temp = new Smoregon(IID_SMOREGON, startX, startY, 0, 1.5, 1, 5*(1+(getLevel()-1)*0.1), 0, 2, 0, this);
            addActor(temp);
            return;
        }
        else{
            // add snagglegon
            int startX = VIEW_WIDTH-1;
            int startY = randInt(0, VIEW_HEIGHT-1);
            Actor* temp = new Snagglegon(IID_SNAGGLEGON, startX, startY, 0, 1.5, 1, 10*(1+(getLevel()-1)*0.1), 0, 1.75, 2,this);
            addActor(temp);
            return;
        }
        return;
    }
}


// add actor to the list
void StudentWorld::addActor(Actor* a){
    actors.push_back(a);
    if(a->isAlien()){
        increaseAlienCount();
    }
}


// returns reference to nachenblaster
NachenBlaster* StudentWorld::getShip() const{
    return ship;
}


// checks if nachenblaster is there
bool StudentWorld::shipThere(int x, int y, double radius) const{
    if(euclidian_dist(x, y, ship->getX(), ship->getY()) < 0.75*(radius+ship->getRadius())){
        return true;
    }
    else
        return false;
}

// checks if alien is there & returns pointer to alien
Actor* StudentWorld::alienThere(int x, int y, double radius){
    list<Actor*>::iterator iter;
    if(actors.size() != 0){
    iter = actors.begin();
    while(iter!=actors.end()){
        Actor* actor = *iter;
        if(actor->isAlien() && euclidian_dist(x, y, actor->getX(), actor->getY()) < 0.75*(radius+actor->getRadius())){
            return *iter;
        }
        iter++;
    }
    }
    return nullptr;
}

// returns pointer to ship
NachenBlaster* StudentWorld::getShip(int x, int y, double radius){
    if(euclidian_dist(x, y, ship->getX(), ship->getY()) < 0.75*(radius+(ship->getRadius()))){
        return ship;
    }
    else
        return nullptr;
}



