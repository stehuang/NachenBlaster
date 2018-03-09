#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <list>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//// ACTOR CLASS////
////////////////////

// deconstructor
Actor::~Actor(){

}

// testing whether object is alive
bool Actor::isAlive(){
    return getX()>=0 && alive==true;
}

// default that everything isn't an alien (true for alien class though)
bool Actor::isAlien() const{
    return false;
}


// return alive status
bool Actor::getAlive() const{
    if(alive == true)
        return true;
    else
        return false;
}


// set actor to dead
void Actor::die(){
    alive = false;
}



//// ALIEN CLASS////
////////////////////

// return pointer to student world
StudentWorld* Alien::getStudentWorld(){
    return m_studentWorld;
}

// return health points
double Alien::getHitPoints() const{
    return m_hitPoints;
}

// return flight plan number
int Alien::getFlightPlan() const{
    return m_flightPlan;
}

// change flight plan number
void Alien::updateFlightPlan(int x){
    m_flightPlan=x;
}

// for alien to move
void Alien::move(){
    // left
    if(m_travelDirection==1){
        moveTo(getX()-m_travelSpeed,getY());
    }
    // up & left
    if(m_travelDirection==2){
        moveTo(getX()-m_travelSpeed, getY()+m_travelSpeed);
    }
    // down & left
    if(m_travelDirection==3){
        moveTo(getX()-m_travelSpeed,getY()-m_travelSpeed);
    }
    // smoregon only: speed up
    if(m_travelDirection==4){
        moveTo(getX()-m_travelSpeed, getY());
    }
}

// do something every tick
void Alien::doSomething(){
    // return if it's dead
    if(!isAlive())
        return;
    // update flight plan if necessarily
    setFlightPlan();
    // check for collision with player
    checkCollide();
    // fire turnip if alien can
    if(fireTurnip())
        return;
    // fire torpedo if alien can
    if(fireTorpedo())
        return;
    // if it's a smoregon, then check for possibility to speed up
    if(isSmoregon())
        smoregonSpeedUp();
    // ask alien to move
    move();
    
    m_flightPlan--;
}


// testing for colliding
void Alien::checkCollide(){
    // checks in studentworld whether it has collided with the player
    Actor* actor = getStudentWorld()->getShip(getX(), getY(), getRadius());
    if(actor != nullptr){
        // make pointer to ship and let it suffer damage
        NachenBlaster* ship = static_cast<NachenBlaster*> (actor);
        if(isSnagglegon()){
            ship->sufferDamage(15);
        }
        else{
            ship->sufferDamage(5);
        }
        // alien dies
        sufferDamage(getHitPoints());
    }
}


// testing if alive
bool Alien::isAlive(){
    // if alien dies by nachenblaster-related cause (getX()>0 because it didn't die through flying off screen)
    if(m_hitPoints<=0){
        die();
        m_studentWorld->destroyAlien(); // add another alien killed to the count
        // increase score according to alien type
        if(isSnagglegon()){
            m_studentWorld->increaseScore(1000);
        }
        else{
            getStudentWorld()->increaseScore(250);
        }
        // graphics, sound, and drop goodie if applicable
        Actor* temp = new Explosion(IID_EXPLOSION, getX(), getY(), 0, 0.5, 1);
        getStudentWorld()->addActor(temp);
        getStudentWorld()->playSound(SOUND_DEATH);
        dropGoodie();
    }
    // bool statement
    return (getX()>=0 && getAlive()==true);
}


// if it's an alien
bool Alien::isAlien() const{
    return true;
}


// decrease health points
void Alien::sufferDamage(int damage){
    m_hitPoints = m_hitPoints-damage;
}


// flight plan
void Alien::setFlightPlan(){
    // 1 = LEFT
    // 2 = UP & LEFT
    // 3 = DOWN & LEFT
    // 4 = smoregon only- LEFT & 5 PIXELS/TICK

    if(getY()>=VIEW_HEIGHT || getY() <= 0 || getFlightPlan() == 0){
        if(getY()>=VIEW_HEIGHT-1){
            changeDirection(3); // due left & down
        }
        if(getY()<=0){
            changeDirection(2); // due left & up
        }
        if(getFlightPlan()==0){
            int dir = randInt(1, 3);
            if(dir==1)
                changeDirection(1); // due left
            if(dir==2)
                changeDirection(2); // due left & up
            else
                changeDirection(3); // due left & down
        }
        updateFlightPlan(randInt(1, 32));
    }
    else
        return;
}

// change the direction in which its flying
void Alien::changeDirection(int x){
    m_travelDirection = x;
}

// firing turnip
bool Alien::fireTurnip(){
    // check if alien is nearby
    NachenBlaster* ship = m_studentWorld->getShip();
    int yDiff = ship->getY() - getY();
    if(ship->getX() < getX() && yDiff >=-4 && yDiff<=4){
        if(randInt(1, ((20/m_studentWorld->getLevel())+5)) == 1){
            // add turnip to the student world
            Actor* temp = new Turnip(IID_TURNIP, getX()-14, getY(), 0, 0.5, 1, m_studentWorld);
            m_studentWorld->addActor(temp);
            m_studentWorld->playSound(SOUND_ALIEN_SHOOT);
            return true;
        }
    }
    return false;
}

// firing torpedo- default false
bool Alien::fireTorpedo(){
    return false;
}

// check if its a smoregon- default false
bool Alien::isSmoregon() const{
    return false;
}

// check if its a snagglegon- default false
bool Alien::isSnagglegon() const{
    return false;
}

// default false
void Alien::smoregonSpeedUp(){
    return;
}


/////// SMALLGON //////
///////////////////////
// default false because smallgons dont drop stuff
void Smallgon::dropGoodie(){
    return;
}



/////// SMOREGON //////
///////////////////////
// check for possibility to speed up
void Smoregon::smoregonSpeedUp(){
    // check if ship is nearby
    StudentWorld* world = getStudentWorld();
    NachenBlaster* ship = world->getShip();
    int yDiff = ship->getY() - getY();
    if(ship->getX() < getX() && yDiff >=-4 && yDiff<=4){
        if(randInt(1, ((20/world->getLevel())+5)) == 1){
            // change traveling direction and fight plan
            changeDirection(4);
            Alien::checkCollide();
            updateFlightPlan(VIEW_WIDTH);
        }
    }
}

// it's a smoregon
bool Smoregon::isSmoregon() const{
    return true;
}

// drop goodie when it dies
void Smoregon::dropGoodie(){
    // calculate chance and use it to determine what goodie to drop
    if(randInt(1, 3)==1){
        int chance = randInt(1, 2);
        if(chance==1){
            // add repair goodie to student world
            Actor* temp = new repairGoodie(IID_REPAIR_GOODIE, getX(), getY(), 0, 0.5, 1, getStudentWorld());
            getStudentWorld()->addActor(temp);
            return;
        }
        else{
            // add torpedo goodie to student world
            Actor* temp = new torpedoGoodie(IID_TORPEDO_GOODIE, getX(), getY(), 0, 0.5, 1, getStudentWorld());
            getStudentWorld()->addActor(temp);
            return;
        }
    }
}



/////// SNAGGLEGON //////
/////////////////////////

// true its a snagglegon
bool Snagglegon::isSnagglegon() const{
    return true;
}

// set flight plan for snagglegon
void Snagglegon::setFlightPlan(){
    // 1 = LEFT
    // 2 = UP & LEFT
    // 3 = DOWN & LEFT
    if(getY()>=VIEW_HEIGHT || getY() <= 0){
        if(getY()>=VIEW_HEIGHT-1){
            changeDirection(3);
        }
        if(getY()<=0){
            changeDirection(2);
        }
    }
}

// false because it doesnt do this
bool Snagglegon::fireTurnip(){
    return false;
}

// check if it should fire a torpedo
bool Snagglegon::fireTorpedo(){
    // check if ship is nearby
    StudentWorld* world = getStudentWorld();
    NachenBlaster* ship = world->getShip();
    int yDiff = ship->getY() - getY();
    if(ship->getX() < getX() && yDiff >=-4 && yDiff<=4){
        if(randInt(1, ((15/world->getLevel())+10)) == 1){
            // add torpedo to student world
            Actor* temp = new FlatulenceTorpedo(IID_TORPEDO, getX()-14, getY(), 180, 0.5, 1, getStudentWorld());
            world->addActor(temp);
            world->playSound(SOUND_TORPEDO);
            return true;
        }
    }
    return false;
}

// calling the drop extra life goodie function
void Snagglegon::dropGoodie(){
    if(randInt(1, (15/getStudentWorld()->getLevel())+10)==1){
        // add extra life goodie to game
        Actor* temp = new extraLifeGoodie(IID_LIFE_GOODIE, getX(), getY(), 0, 0.5, 1, getStudentWorld());
        getStudentWorld()->addActor(temp);
        return;
    }
}



//// NACHENBLASTER////
//////////////////////

// colliding doesn't apply to this object because it doesnt need to check
void NachenBlaster::checkCollide(){
    return;
}

// do something for nachenblaster ship
void NachenBlaster::doSomething(){
    if(isAlive()){
        addCabbage();
        int key;
        if(m_studentWorld->getKey(key)){
            switch(key){
                    // move up
                case KEY_PRESS_UP:
                    if(getY()>=VIEW_HEIGHT)
                        break;
                    else{
                        moveTo(getX(), getY()+6);
                    }
                    break;
                    // move down
                case KEY_PRESS_DOWN:
                    if(getY()<0)
                        break;
                    else{
                        moveTo(getX(), getY()-6);
                    }
                    break;
                    // move left
                case KEY_PRESS_LEFT:
                    if(getX()<0)
                        break;
                    else{
                        moveTo(getX()-6, getY());
                    }
                    break;
                    // move right
                case KEY_PRESS_RIGHT:
                    if(getX()>=VIEW_WIDTH)
                        break;
                    else{
                        moveTo(getX()+6, getY());
                    }
                    break;
                    // shoot cabbage
                case KEY_PRESS_SPACE:
                    shootCabbage();
                    m_studentWorld->playSound(SOUND_PLAYER_SHOOT);
                    break;
                    // shoot torpedo
                case KEY_PRESS_TAB:
                    shootTorpedo();
                    m_studentWorld->playSound(SOUND_TORPEDO);
            }
        }
    }
    else
        return;
}

// testing whether ship is alive
bool NachenBlaster::isAlive(){
    return m_hitPoints > 0;
}


// shoot cabbage
void NachenBlaster::shootCabbage(){
    // only shoot if there are more than 5 cabbage points
    if(m_cabbage>=5){
        m_cabbage = m_cabbage-5;
        Cabbage* temp = new Cabbage(IID_CABBAGE, getX()+12, getY(), 0, 0.5, 1, m_studentWorld);
        m_studentWorld->addActor(temp);
    }
}

// shoot torpedo
void NachenBlaster::shootTorpedo(){
    // return if there aren't any torpedos
    if(m_torpedo <= 0)
        return;
    // add torpedo to the student world
    Actor* temp = new FlatulenceTorpedo(IID_TORPEDO, getX()+12, getY(), 0, 0.5, 1, m_studentWorld);
    m_studentWorld->addActor(temp);
    // minus torpedo from inventory
    m_torpedo--;
}


// add one cabbage point per tick
void NachenBlaster::addCabbage(){
    if(m_cabbage<30)
        m_cabbage++;
}

// returns health points
double NachenBlaster::getHitPoints() const{
    return m_hitPoints;
}

// returns cabbage points
int NachenBlaster::getCabbage() const{
    return m_cabbage;
}

// torpedo inventory
int NachenBlaster::getTorpedo() const{
    return m_torpedo;
}

// decrease health points
void NachenBlaster::sufferDamage(int x){
    m_hitPoints = m_hitPoints-x;
}

// increase health points
void NachenBlaster::increaseHitPoints(int x){
    m_hitPoints= m_hitPoints+x;
    if(m_hitPoints > 50)
        m_hitPoints = 50;
}

// increase torpedoes
void NachenBlaster::increaseTorpedo(int x){
    m_torpedo = m_torpedo+x;
}

//// STAR CLASS ////
////////////////////

// check if its alive
bool Star::isAlive(){
    return getX()>-1;
}

// do something for stars
void Star::doSomething(){
    moveTo(getX()-1, getY());
}

// stars dont collide
void Star::checkCollide(){
    return;
}


//// EXPLOSION CLASS ////
/////////////////////////

// do something for explosions
void Explosion::doSomething(){
    if(!isAlive())
        return;
    // die after 4 ticks
    if(m_tick>=4){
        die();
    }
    setSize(1.5*getSize());
    m_tick++;
}

// check if alive
bool Explosion::isAlive(){
    return getX()>-1 && getAlive()==true;
}

// explosions dont collide
void Explosion::checkCollide(){
    return;
}


//// PROJECTILES////
//////////////////////

// return pointer to student world
StudentWorld* Projectile::getStudentWorld(){
    return m_studentWorld;
}


//// CABBAGE CLASS ////
///////////////////////

// do something for cabbage
void Cabbage::doSomething(){
    if(isAlive()==true){
        checkCollide();
        moveTo(getX()+8, getY());
        setDirection(getDirection()+20);
        checkCollide();
    }
}

// if is alive
bool Cabbage::isAlive(){
    return (getX()<=VIEW_WIDTH && getAlive()==true);
}


// if it collides (with alien)
void Cabbage::checkCollide(){
    Actor* actor = getStudentWorld()->alienThere(getX(), getY(), getRadius());
    if(actor != nullptr){
        // if it does, then cause damage and die
        die();
        Alien* alien = static_cast<Alien*> (actor);
        alien->sufferDamage(2);
        if(alien->getHitPoints() > 0)
            getStudentWorld()->playSound(SOUND_BLAST);
    }
}


///// TURNIP CLASS ////
///////////////////////

// do something for turnips
void Turnip::doSomething(){
    if(isAlive()){
        checkCollide();
        moveTo(getX()-6, getY());
        setDirection(getDirection()+20);
    }
}

// if is alive
bool Turnip::isAlive(){
    return (getX() > 0 && getAlive()==true);
}


// if it collides (with nachenblaster)
void Turnip::checkCollide(){
    Actor* actor = getStudentWorld()->getShip(getX(), getY(), getRadius());
    if(actor != nullptr){
        // if it does, cause damage
        NachenBlaster* ship = static_cast<NachenBlaster*> (actor);
        ship->sufferDamage(2);
        die();
    }
    else
        return;
}



///// TORPEDO CLASS ///
///////////////////////

// check if its alive
bool FlatulenceTorpedo::isAlive(){
    // if shot by alien
    if(getDirection()==180)
        return getX() > 0 && Actor::isAlive();
    //if shot by nachenblaster
    else if (getDirection()==0)
        return getX()<=VIEW_WIDTH && Actor::isAlive();
    else
        return false;
}

// do something for torpedos
void FlatulenceTorpedo::doSomething(){
    if(getAlive()==true && isAlive()){
        checkCollide();
        // direction is 180, meaning it was shot by a snagglegon
        if(getDirection()==180)
            moveTo(getX()-8, getY());
        // direction = 0 means it was shot by the nachenblaster
        else if(getDirection()==0)
            moveTo(getX()+8, getY());
    }

}

// check if it collided with the ship/aliens
void FlatulenceTorpedo::checkCollide(){
    // collide with ship
    if(getDirection()==180){
        Actor* actor = getStudentWorld()->getShip(getX(), getY(), getRadius());
        if(actor != nullptr){
            // cause damage
            NachenBlaster* ship = static_cast<NachenBlaster*> (actor);
            ship->sufferDamage(8);
            die();
        }
    }
    // collide with alien
    if(getDirection()==0){
        Actor* actor = getStudentWorld()->alienThere(getX(), getY(), getRadius());
        if(actor != nullptr){
            // cause damage
            Alien* alien = static_cast<Alien*> (actor);
            alien->sufferDamage(8);
            die();
        }
    }
}



////// GOODIE CLASS /////
/////////////////////////

// check if its alive
bool Goodie::isAlive(){
    return (Actor::isAlive() && getX() >= 0 && getY() >= 0);
}

// asks the goodie to do something
void Goodie::doSomething(){
    if(isAlive()==false)
        return;
    else{
        checkCollide();
        moveTo(getX()-0.75, getY()-0.75);
        }
}

// check if it collides with ship
void Goodie::checkCollide(){
    if(getStudentWorld()->shipThere(getX(), getY(), getRadius())){
        getStudentWorld()->increaseScore(100);
        die();
        getStudentWorld()->playSound(SOUND_GOODIE);
        informWorld();
    }
}

// returns pointer to student world
StudentWorld* Goodie::getStudentWorld(){
    return m_studentWorld;
}


////// EXTRA LIFE CLASS /////
/////////////////////////////

// adds another life
void extraLifeGoodie::informWorld(){
    getStudentWorld()->incLives();
}


////// REPAIR GOODIE CLASS /////
////////////////////////////////

// increases health points
void repairGoodie::informWorld(){
    getStudentWorld()->getShip()->increaseHitPoints(10);
}


////// TORPEDO GOODIE CLASS /////
/////////////////////////////////

// increase torpedo inventory by 5
void torpedoGoodie::informWorld(){
    getStudentWorld()->getShip()->increaseTorpedo(5);
}















