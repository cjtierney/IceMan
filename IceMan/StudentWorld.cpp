#include "StudentWorld.h"

#include <cassert>
#include <format>
#include <queue>
#include <string>
#include <unordered_set>
using namespace std;

GameWorld* createStudentWorld(string assetDir) {
    return new StudentWorld(assetDir);
}

int StudentWorld::init() {
    srand(time(0));

    IceVec = new IceHolder();
    IceVec->createIce(this);
    ExitPathGenerator = new WorldExitPath(IceVec);
    IcemanPtr = new Iceman(30, 60, this);  // CREATE ICEMAN

    populateOilField();

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {

    ExitPathGenerator->calulateDirections();

    for (Actor* actor : ActorVec) actor->tick();

    IcemanPtr->tick();

    updateGameStats();

    removeDeadActors();

    addGoodies();

    // Determine game status to return
    if (getOilRemaining() == 0) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    } else if (IcemanPtr->isAlive() == false) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    getIceHolder()->clear(); // DEALLOCATE ICE

    while (!ActorVec.empty())  // DEALLOCATE ACTORS
    {
        delete ActorVec.back();
        ActorVec.pop_back();
    }

    delete IcemanPtr;  // DEALLOCATE ICEMAN
}

void StudentWorld::populateOilField() {
    int currentLevel = getLevel();

    // CREATE OIL BARRELS
    int oilCount = min(2 + currentLevel / 2, 21);

    while (oilCount > 0) {
        int x = rand() % 61;    // x = 0-60
        int y = rand() % 57;    // y = 0-56
        if (x >= 27 && x <= 33 && y >= 1 && y <= 59) // don't generate in pit
            continue;
        bool validPos = true;
        for (Actor*& actor : ActorVec) {
            if (hypot(x - actor->getX(), y - actor->getY()) <= 6.0) {
                validPos = false;
                break;  // If one fails, no need to check the rest
            }
        }
        if (validPos) {
            ActorVec.push_back(new OilBarrel(x, y, this));
            oilCount--;
        }
    }

    // CREATE GOLD NUGGETS
    int goldCount = max(5 - currentLevel / 2, 2);

    while (goldCount > 0) {
        int x = rand() % 61;    // x = 0-60
        int y = rand() % 57;    // y = 0-56
        if (x >= 27 && x <= 33 && y >= 1 && y <= 59) // don't generate in pit
            continue;
        bool validPos = true;
        for (Actor*& actor : ActorVec) {
            if (hypot(x - actor->getX(), y - actor->getY()) <= 6.0) {
                validPos = false;
                break;  // If one fails, no need to check the rest
            }
        }
        if (validPos) {
            ActorVec.push_back(new GoldNugget(x, y, true, this));
            goldCount--;
        }
    }

    // CREATE BOULDERS
    int boulderCount = min(currentLevel / 2 + 2, 9);

    while (boulderCount > 0) {
        int x = rand() % 61;         // x = 0-60
        int y = (rand() % 37) + 20;  // y = 20-56
        if (x >= 27 && x <= 33 && y >= 1 && y <= 59) // don't generate in pit
            continue;
        bool validPos = true;
        for (Actor*& actor : ActorVec) {
            if (hypot(x - actor->getX(), y - actor->getY()) <= 6.0) {
                validPos = false;
                break;  // If one fails, no need to check the rest
            }
        }
        if (validPos) {
            getIceHolder()->deleteIce(x, y, x + 3, y + 3, false);
            ActorVec.push_back(new Boulder(x, y, this));
            boulderCount--;
        }
    }
}

void StudentWorld::addGoodies() {
    int goodieChance = getLevel() * 30 + 290;
    int goodieSpawn = rand() % goodieChance;
    if (goodieSpawn == 0) {
        if (rand() % 5 == 0)  // Create Sonar Kit
        {
            ActorVec.push_back(new SonarKit(0, 60, this));
        } else  // Create Water Pool
        {
            bool validPos = false;
            while (!validPos) {
                int x = rand() % 61;
                int y = rand() % 57;
                if (!getIceHolder()->isIceAt(x, y, x + 3, y + 3, true)) {
                    ActorVec.push_back(new WaterPool(x, y, this));
                    validPos = true;
                }
            }
        }
    }
}

void StudentWorld::revealGoodies(int xc, int yc, int r) {
    for (Actor*& actor : ActorVec) {
        int id = actor->getID();
        if (id == IID_BARREL ||
            id == IID_GOLD)  // Only reveal oil barrels and gold nuggets
        {
            int x = actor->getX();
            int y = actor->getY();
            if (hypot(xc - x, yc - y) <= r) actor->setVisible(true);
        }
    }
}

void StudentWorld::createWaterSquirt(int x, int y) {
    ActorVec.push_back(new WaterSquirt(x, y, IcemanPtr->getDirection(), this));
}

void StudentWorld::dropGold(int x, int y) {
    ActorVec.push_back(new GoldNugget(x, y, false, this));
}

void StudentWorld::updateGameStats() {
    // Write Display Text
    string displayText = "";

    displayText += "Lvl: " + format("{:2}", getLevel());
    displayText += " Lives: " + to_string(getLives());
    displayText +=
        " Hlth: " + format("{:3}", IcemanPtr->getHP() * 10) + "%";
    displayText += " Wtr: " + format("{:2}", IcemanPtr->getWater());
    displayText += " Gld: " + format("{:2}", IcemanPtr->getGold());
    displayText += " Oil: " + format("{:2}", getOilRemaining());
    displayText += " Sonar: " + format("{:2}", IcemanPtr->getSonar());
    displayText += " Scr: " + format("{:06}", getScore());

    setGameStatText(displayText);
}

void StudentWorld::removeDeadActors() {
    erase_if(ActorVec, [](Actor* actor) {
        bool isDead = !actor->isAlive();
        if (isDead) delete actor;
        return isDead;
    });
}

int StudentWorld::getOilRemaining() {
    return count_if(ActorVec.begin(), ActorVec.end(),
                    [](Actor* actor) { return actor->getID() == IID_BARREL; });
}

bool StudentWorld::isActorNearby(int actorID, int xc, int yc, double rMin, double rMax) {
    for (Actor*& actor : ActorVec) {
        int id = actor->getID();
        if (id == actorID)
        {
            int x = actor->getX();
            int y = actor->getY();
            double r = hypot(xc - x, yc - y);
            if (r >= rMin && r <= rMax)
                return true;
        }
    }
    return false;
}

bool StudentWorld::icemanWithinDist(int xc, int yc, int r) {
    if (hypot(xc - IcemanPtr->getX(), yc - IcemanPtr->getY()) <= r)
        return true;
    return false;
}

bool StudentWorld::damageNearbyProtesters(int xc, int yc, int r, int dmg)
{
    int res = false;
    for (Actor*& actor : ActorVec) {
        int id = actor->getID();
        if (id == IID_PROTESTER || id == IID_HARD_CORE_PROTESTER)
        {
            int x = actor->getX();
            int y = actor->getY();
            if (hypot(xc - x, yc - y) <= r) {
                actor->annoyed(dmg);
                res = true;
            }
        }
    }
    return res;
}

IceHolder* StudentWorld::getIceHolder() {
    return IceVec;
}

Iceman* StudentWorld::getIceman() {
    return IcemanPtr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

void IceHolder::createIce(StudentWorld* world) {
    for (int y = 0; y < VIEW_HEIGHT; y++)
        for (int x = 0; x < VIEW_WIDTH; x++) {
            if (x >= 30 && x <= 33 && y >= 4 && y <= 59)  // Pit in center
                IceVec.push_back(nullptr);
            else if (y >= 60)  // Top is empty
            {
                IceVec.push_back(nullptr);
            }

            else
                IceVec.push_back(new Ice(x, y, world));
        }
}

bool IceHolder::deleteIce(int x1, int y1, int x2, int y2, bool shouldLock) {
    if (shouldLock) lock();
    bool iceDeleted = false;
    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
            int icePos = (VIEW_WIDTH * y) + x;
            if (icePos < VIEW_WIDTH * VIEW_HEIGHT)
                if (IceVec[icePos] != nullptr) {
                    delete IceVec[icePos];
                    IceVec[icePos] = nullptr;
                    iceDeleted = true;
                }
        }
    }
    if (shouldLock) unlock();
    return iceDeleted;
}

bool IceHolder::isIceAt(int x, int y, bool shouldLock) {
    if (shouldLock) lock();
    assert(x < VIEW_WIDTH);
    assert(y < VIEW_HEIGHT);
    assert(x > -1);
    assert(y > -1);
    int icePos = (VIEW_WIDTH * y) + x;
    bool res = IceVec[icePos] != nullptr;
    if (shouldLock) unlock();
    return res;
}

bool IceHolder::isIceAt(int x1, int y1, int x2, int y2, bool shouldLock) {
    if (shouldLock) lock();
    assert(x2 >= x1);
    assert(y2 >= y1);
    assert(x1 > -1);
    assert(y1 > -1);
    assert(x2 < VIEW_WIDTH);
    assert(y2 < VIEW_HEIGHT);
    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
            if (IceVec[(VIEW_WIDTH * y) + x] != nullptr) {
                if (shouldLock) unlock();
                return true;
            }
        }
    }
    if (shouldLock) unlock();
    return false;
}

void IceHolder::clear() {
    while (!IceVec.empty())  // DEALLOCATE ICE
    {
        delete IceVec.back();
        IceVec.pop_back();
    }
}

WorldExitPath::direction WorldExitPath::getNextDirection(int x, int y) {
    lock->lock();
    direction res;
    int index = VIEW_WIDTH * y + x;
    if (index >= directions.size()) {
        res = direction::unresolved;
    } else {
        res = directions.at(VIEW_WIDTH * y + x);
    }
    lock->unlock();
    return res;
}

struct position {
    int x;
    int y;

    bool operator==(const position& o) const {
        return x == o.x && y == o.y;
    }
};

vector<tuple<position, WorldExitPath::direction>> getAdjacent(position p) {
    vector<tuple<position, WorldExitPath::direction>> res(4);
    if (p.x > 0)
        res.push_back(make_tuple(position(p.x - 1, p.y), WorldExitPath::direction::left));
    if (p.y > 0)
        res.push_back(make_tuple(position(p.x, p.y - 1), WorldExitPath::direction::up));
    if (p.x < VIEW_WIDTH - 4)
        res.push_back(make_tuple(position(p.x + 1, p.y), WorldExitPath::direction::right));
    if (p.y < VIEW_HEIGHT - 4)
        res.push_back(make_tuple(position(p.x, p.y + 1), WorldExitPath::direction::down));
    return res;
}

void WorldExitPath::calulateDirections() {
    vector<direction> newDirections(VIEW_WIDTH * VIEW_HEIGHT, direction::unresolved);
    newDirections[VIEW_WIDTH * 60 + 60] = direction::is_exit;

    queue<position> positionQueue;
    for (tuple<position, direction> pos : getAdjacent(position(60, 60)))
        positionQueue.push(get<0>(pos));

    ice->lock();
outer_loop:
    while (!positionQueue.empty()) {
        position pos = positionQueue.front();
        positionQueue.pop();
        if (ice->isIceAt(pos.x, pos.y, pos.x + 3,
                                           pos.y + 3, false)) {
            newDirections[VIEW_WIDTH * pos.y + pos.x] = direction::none;
            continue;
        }
        vector<tuple<position, direction>> adjacent = getAdjacent(pos);
        for (tuple<position, direction> adj : adjacent) {
            position adjPos = get<0>(adj);
            int adjIndex = VIEW_WIDTH * adjPos.y + adjPos.x;
            if (newDirections[adjIndex] == direction::unresolved) {
                newDirections[adjIndex] = direction::resolving;
                positionQueue.push(adjPos);
            }
        }
        for (tuple<position, direction> testPosDir : adjacent) {
            position testPos = get<0>(testPosDir);
            direction testDir =
                newDirections[VIEW_WIDTH * testPos.y + testPos.x];
            if (testDir != direction::none && testDir != direction::unresolved && testDir != direction::resolving) {
                newDirections[VIEW_WIDTH * pos.y + pos.x] = get<1>(testPosDir);
                // C++ doesn't have named loops and I don't want to have to deal
                // with function scopes
                goto outer_loop;
            }
        }
    }
    ice->unlock();
    lock->lock();
    directions = std::move(newDirections);
    lock->unlock();
}
