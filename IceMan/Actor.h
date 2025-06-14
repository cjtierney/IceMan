#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
   public:
    Actor(int imageID, int startX, int startY, Direction dir, double size,
          int depth, StudentWorld* w)
        : GraphObject(imageID, startX, startY, dir, size, depth),
          world_(w),
          alive_(true) {}

    virtual void init() = 0;
    virtual void tick() = 0;
    virtual void annoyed(int dmg) = 0;

    bool isAlive() const;
    void setAlive(bool state);

    StudentWorld* getWorld();

   private:
    StudentWorld* world_;
    bool alive_;
};

class Person : public Actor {
    public:
     Person(int imageID, int startX, int startY, Direction dir, int hp, StudentWorld* w)
        : Actor(imageID, startX, startY, dir, 1.0, 0, w), hp_(hp) {}

     int getHP();
     void subtractHP(int dmg);

    private:
        int hp_;
};

class Protester : public Person {
   public:
    Protester(int startX, int startY, StudentWorld* w)
        : Person(IID_PROTESTER, startX, startY, left, 5, w),
          numSquaresToMoveInCurrentDirection_(0),
          leaveTheOilField_(false) {
        init();
    };

    void init();
    void tick();
    void annoyed(int dmg);

    int getMoveInterval();

   private:
    int numSquaresToMoveInCurrentDirection_;
    bool leaveTheOilField_;
    int moveIntervalCounter_;
};

class HardcoreProtester : public Protester {
   public:
   private:
};

class Iceman : public Person {
   public:
    Iceman(int startX, int startY, StudentWorld* w)
        : Person(IID_PLAYER, startX, startY, right, 10, w),
          water_(5),
          gold_(0),
          sonar_(1) {
        init();
    };

    void init();
    void tick();
    void annoyed(int dmg);

    void addWater();
    void addSonar();
    void addGold();

    int getWater();
    int getSonar();
    int getGold();

    void squirtWater();
    void dropGold();
    void useSonar();

    void abortLevel();

   private:
    int water_;
    int sonar_;
    int gold_;
};

class WaterSquirt : public Actor {
   public:
    WaterSquirt(int startX, int startY, Direction dir, StudentWorld* w)
        : Actor(IID_WATER_SPURT, startX, startY, dir, 1.0, 1, w),
          travelDist_(4) {
        init();
    };

    void init();
    void tick();
    void annoyed(int dmg) {};

   private:
    int travelDist_;
};

class Terrain : public Actor {
   public:
    Terrain(int imageID, int startX, int startY, Direction dir, double size,
        int depth, StudentWorld* w)
        : Actor(imageID, startX, startY, dir, size, depth, w) {
        init();
    }

    virtual void init();
    void annoyed(int dmg) {};

   private:
};

class Ice : public Terrain {
   public:
    Ice(int startX, int startY, StudentWorld* w)
        : Terrain(IID_ICE, startX, startY, right, 0.25, 3, w) {}

    void tick();

   private:
};

class Boulder : public Terrain {
   public:
    Boulder(int startX, int startY, StudentWorld* w)
        : Terrain(IID_BOULDER, startX, startY, down, 1.0, 1, w),
        state_(stable), cooldown_(30) {}

    void tick();

   private:
       enum State {stable, waiting, falling};
       State state_;
       int cooldown_;
};

class Collectable : public Actor {
   public:
    Collectable(int imageID, int startX, int startY, bool pickupable, StudentWorld* w)
        : Actor(imageID, startX, startY, right, 1.0, 2, w), lifetime_(-1), pickupable_(pickupable) {}

    void tick();
    void annoyed(int dmg) {};

    virtual void activate() = 0;

    void setLifetime(int time);

    bool isPickupable();

   private:
    int lifetime_;
    bool pickupable_;
};

class WaterPool : public Collectable {
   public:
    WaterPool(int startX, int startY, StudentWorld* w)
        : Collectable(IID_WATER_POOL, startX, startY, true, w) {
        init();
    }

    void init();

    void activate();

   private:
};

class OilBarrel : public Collectable {
   public:
    OilBarrel(int startX, int startY, StudentWorld* w)
        : Collectable(IID_BARREL, startX, startY, true, w) {
        init();
    }

    void init();

    void activate();

   private:
};

class GoldNugget : public Collectable {
   public:
    GoldNugget(int startX, int startY, bool pickupable, StudentWorld* w)
        : Collectable(IID_GOLD, startX, startY, pickupable, w) {
        init();
    }

    void init();

    void activate();

   private:

};

class SonarKit : public Collectable {
   public:
    SonarKit(int startX, int startY, StudentWorld* w)
        : Collectable(IID_SONAR, startX, startY, true, w) {
        init();
    }

    void init();

    void activate();

   private:
};

#endif  // ACTOR_H_
