#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_

class StudentWorld;
class Actor:public GraphObject{
public:
    Actor(int imageID, int startX, int startY, int startDirection, double size,int depth):GraphObject(imageID, startX, startY, startDirection, size, depth), alive(true){}
    virtual ~Actor();
    virtual void doSomething()=0;
    virtual bool isAlive()=0;
    virtual bool isAlien() const;
    virtual void checkCollide()=0;
    bool getAlive() const;
    void die();
private:
    bool alive;
};


class Alien:public Actor{
public:
    Alien(int imageID, int startX, int startY, int startDirection, double size,int depth, double hitPoints, int flightPlan, double travelSpeed, int direction, StudentWorld* world):Actor(imageID, startX, startY, startDirection, size, depth){
        m_hitPoints = hitPoints;
        m_flightPlan = flightPlan;
        m_travelSpeed = travelSpeed;
        m_travelDirection = direction;
        m_studentWorld = world;
    }
    StudentWorld* getStudentWorld();
    double getHitPoints() const;
    int getFlightPlan() const;
    void updateFlightPlan(int x);
    void move();
    void doSomething();
    void checkCollide();
    bool isAlive();
    virtual void setFlightPlan();
    void changeDirection(int x);
    virtual bool isAlien() const;
    void sufferDamage(int damage);
    virtual bool fireTurnip();
    virtual bool fireTorpedo();
    virtual bool isSmoregon() const;
    virtual bool isSnagglegon() const;
    virtual void smoregonSpeedUp();
    virtual void dropGoodie()=0;
private:
    double m_hitPoints;
    int m_flightPlan;
    double m_travelSpeed;
    /*
      1 = LEFT
      2 = UP & LEFT
      3 = DOWN & LEFT
      4 = smoregon only- LEFT & 5 PIXELS/TICK
     */
    int m_travelDirection;
    StudentWorld* m_studentWorld;
};

class Smallgon:public Alien{
public:
    Smallgon(int imageID, int startX, int startY, int startDirection, double size,int depth, int hitPoints, int flightPlan, double travelSpeed, int direction, StudentWorld*  world):Alien(IID_SMALLGON, startX, startY, 0, 1.5, 1, hitPoints, 0, 2.0, direction, world){}
    virtual void dropGoodie();

private:
    
};

class Smoregon:public Alien{
public:
    Smoregon(int imageID, int startX, int startY, int startDirection, double size,int depth, int hitPoints, int flightPlan, double travelSpeed, int direction, StudentWorld*  world):Alien(IID_SMOREGON, startX, startY, 0, 1.5, 1, hitPoints, 0, 2.0, direction, world){}
    virtual void smoregonSpeedUp();
    virtual bool isSmoregon() const;
    virtual void dropGoodie();
private:
    
};


class Snagglegon:public Alien{
public:
    Snagglegon(int imageID, int startX, int startY, int startDirection, double size,int depth, int hitPoints, int flightPlan, double travelSpeed, int direction, StudentWorld*  world):Alien(IID_SNAGGLEGON, startX, startY, 0, 1.5, 1, hitPoints, 0, 1.75, 2, world){}
    virtual bool isSnagglegon() const;
    virtual void setFlightPlan();
    virtual bool fireTurnip();
    virtual bool fireTorpedo();
    virtual void dropGoodie();
private:
    
};


class NachenBlaster:public Actor{
public:
    NachenBlaster(int imageID, int startX, int startY, int startDirection, double size,int depth, int cabbage, double hitPoints, StudentWorld* world):Actor(IID_NACHENBLASTER, 0, 128, 0, 1, 0), m_cabbage(30), m_hitPoints(50),m_studentWorld(world) {
        m_torpedo = 0;
    }
    virtual void checkCollide();
    virtual void doSomething();
    virtual bool isAlive();
    void shootCabbage();
    void shootTorpedo();
    void addCabbage();
    double getHitPoints() const;
    int getCabbage() const;
    int getTorpedo() const;
    void sufferDamage(int x);
    void increaseHitPoints(int x);
    void increaseTorpedo(int x);
    
private:
    int m_cabbage;
    double m_hitPoints;
    int m_torpedo;
    StudentWorld* m_studentWorld;
};


class Star:public Actor{
public:
    Star(int imageID, int startX, int startY, int startDirection, double size, int depth):Actor(IID_STAR, startX, startY, 0, size, 3){}
    virtual bool isAlive();
    virtual void doSomething();
    void checkCollide();
private:
};


class Explosion:public Actor{
public:
    Explosion(int imageID, int startX, int startY, int startDirection, double size, int depth):Actor(IID_EXPLOSION, startX, startY, 0, 1, 0), m_tick(0){}
    virtual void doSomething();
    virtual bool isAlive();
    void checkCollide();
private:
    int m_tick;
};


class Projectile:public Actor{
public:
    Projectile(int imageID, int startX, int startY, int startDirection, double size,int depth, StudentWorld* world):Actor(imageID, startX, startY, startDirection, size, depth), m_studentWorld(world){}
    virtual void doSomething()=0;
    virtual void checkCollide()=0;
    StudentWorld* getStudentWorld();
private:
    StudentWorld* m_studentWorld;
};


class Cabbage:public Projectile{
public:
    Cabbage(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* world):Projectile(IID_CABBAGE, startX, startY, 0, 0.5, 1, world){}
    virtual bool isAlive();
    virtual void doSomething();
    void checkCollide();
private:
};


class Turnip:public Projectile{
public:
    Turnip(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* world):Projectile(IID_TURNIP, startX, startY, 0, 0.5, 1, world){}
    virtual bool isAlive();
    virtual void doSomething();
    void checkCollide();
private:
};


class FlatulenceTorpedo:public Projectile{
public:
    FlatulenceTorpedo(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* world):Projectile(IID_TORPEDO, startX, startY, startDirection, 0.5, 1, world){}
    virtual bool isAlive();
    virtual void doSomething();
    void checkCollide();

private:

};


class Goodie:public Actor{
public:
    Goodie(int imageID, int startX, int startY, int startDirection, double size,int depth, StudentWorld* world):Actor(imageID, startX, startY, startDirection, size, depth), m_studentWorld(world){}
    bool isAlive();
    virtual void doSomething();
    virtual void checkCollide();
    virtual void informWorld()=0;
    StudentWorld* getStudentWorld();
private:
    StudentWorld* m_studentWorld;
};


class extraLifeGoodie:public Goodie{
public:
    extraLifeGoodie(int imageID, int startX, int startY, int startDirection, double size,int depth, StudentWorld* world):Goodie(IID_LIFE_GOODIE, startX, startY, 0, 0.5, 1, world){}
    virtual void informWorld();
private:

};


class repairGoodie:public Goodie{
public:
    repairGoodie(int imageID, int startX, int startY, int startDirection, double size,int depth, StudentWorld* world):Goodie(IID_REPAIR_GOODIE, startX, startY, 0, 0.5, 1, world){}
    virtual void informWorld();
private:
    
};

class torpedoGoodie:public Goodie{
public:
    torpedoGoodie(int imageID, int startX, int startY, int startDirection, double size,int depth, StudentWorld* world):Goodie(IID_TORPEDO_GOODIE, startX, startY, 0, 0.5, 1, world){}
    virtual void informWorld();
private:
    
};














