#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <mutex>
#include <string>
#include <vector>

#include "Actor.h"
#include "GameConstants.h"
#include "GameWorld.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class WorldExitPath;

class StudentWorld : public GameWorld {
   public:
    StudentWorld(std::string assetDir)
        : GameWorld(assetDir), IcemanPtr(nullptr), ExitPathGenerator(nullptr) {}

    int init() override;

    int move() override;

    void cleanUp() override;

    Iceman* getIceman();

    // creates the initial 64 by 64 ice grid with a pit
    void createIce();

    // Delete ice in a _ by _ area, returns true if ice deleted
    bool deleteIce(int x1, int x2, int y1, int y2);

    // Checks if there is ice at a given position
    bool isIceAt(int x, int y);
    bool isIceAt(int x1, int y1, int x2, int y2);

    // Add collectables to oil field (barrels, gold, boulders)
    void populateOilField();

    // Updates the values in the stats display
    void updateGameStats();

    // Removes any actors marked as dead
    void removeDeadActors();

    // Attempt to add sonar kits and water pools to the oil field
    void addGoodies();

    // Sets goodies as visible that are within given radius from center (xc,yc)
    void revealGoodies(int xc, int yc, int r);

    // Returns the number of oil barrels left
    int getOilRemaining();

    // Shoots a water squirt from the Iceman's position/direction
    void createWaterSquirt();

    // Destructor
    ~StudentWorld();

    WorldExitPath* ExitPathGenerator;

   private:
    std::vector<Actor*> ActorVec;  // Includes all Actors (besides ice and iceman)
    std::vector<Ice*> IceVec;  // Includes all Ice objects
    Iceman* IcemanPtr;         // Includes the Iceman player
};

class WorldExitPath {
   public:
    WorldExitPath() : directions(std::vector<direction>()), lock(std::make_unique<std::mutex>()) {}

    enum direction { up, down, left, right, is_exit, none, unresolved, resolving };
    direction getNextDirection(int x, int y);
    void calulateDirections(StudentWorld* world);

   private:
    std::vector<direction> directions;
    std::unique_ptr<std::mutex> lock;
};

#endif  // STUDENTWORLD_H_
