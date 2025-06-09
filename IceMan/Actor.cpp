#include "Actor.h"

#include <thread>

#include "StudentWorld.h"
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and
// StudentWorld.cpp

// *************************************
// ************** ACTOR ***************
// *************************************

StudentWorld* Actor::getWorld() {
    return world_;
}

bool Actor::isAlive() const {
    return alive_;
}

void Actor::setAlive(bool state) {
    alive_ = state;
}

// *************************************
// ************ PROTESTORS *************
// *************************************

void Protester::init() {
    setVisible(true);
}

void Protester::tick() {
    int moveInterval = getMoveInterval(getWorld()->getLevel());
    moveIntervalCounter = ++moveIntervalCounter % moveInterval;
    if (!isAlive()) return;
    if (moveIntervalCounter != 0) return;
    if (leaveTheOilField) {
        if (getX() == 60 && getY() == 0) {
            setAlive(false);
            return;
        } else {
            WorldExitPath::direction exitDir =
                getWorld()->ExitPathGenerator->getNextDirection(getX(), getY());
            switch (exitDir) {
                case WorldExitPath::direction::left:
                    if (getDirection() != left) setDirection(left);
                    else moveTo(getX() - 1, getY());
                    return;
                case WorldExitPath::direction::right:
                    if (getDirection() != left) setDirection(right);
                    else moveTo(getX() + 1, getY());
                    return;
                case WorldExitPath::direction::up:
                    if (getDirection() != left) setDirection(up);
                    else moveTo(getX(), getY() + 1);
                    return;
                case WorldExitPath::direction::down:
                    if (getDirection() != left) setDirection(down);
                    else moveTo(getX(), getY() - 1);
                    return;
            }
        }
    } else {
    }
}

void Protester::annoyed() {}

// *************************************
// ************** ICEMAN ***************
// *************************************

void Iceman::init() {
    setVisible(true);
}

void Iceman::tick() {
    if (hp_ == 0)
        return;
    else {
        int key;
        if (getWorld()->getKey(key) == true) {
            switch (key) {
                case KEY_PRESS_UP:
                case 'w':
                case 'W':
                case '8':
                    if (getDirection() != up)
                        setDirection(up);
                    else if (getY() < VIEW_HEIGHT - 4)
                        moveTo(getX(), getY() + 1);
                    break;
                case KEY_PRESS_DOWN:
                case 's':
                case 'S':
                case '2':
                    if (getDirection() != down)
                        setDirection(down);
                    else if (getY() > 0)
                        moveTo(getX(), getY() - 1);
                    break;
                case KEY_PRESS_RIGHT:
                case 'd':
                case 'D':
                case '6':
                    if (getDirection() != right)
                        setDirection(right);
                    else if (getX() < VIEW_WIDTH - 4)
                        moveTo(getX() + 1, getY());
                    break;
                case KEY_PRESS_LEFT:
                case 'a':
                case 'A':
                case '4':
                    if (getDirection() != left)
                        setDirection(left);
                    else if (getX() > 0)
                        moveTo(getX() - 1, getY());
                    break;
                case KEY_PRESS_SPACE:
                    if (water_ > 0) squirtWater();
                    break;
                case KEY_PRESS_TAB:
                    break;
                case 'Z':
                case 'z':
                    if (sonar_ > 0) useSonar();
                    break;
                case KEY_PRESS_ESCAPE:
                    abortLevel();
                    break;
                default:
                    break;
            }
            // Mine Ice
            if (getWorld()->getIceHolder()->deleteIce(
                    getX(), getX() + 3, getY(),
                    getY() + 3, true))  // try to dig 4x4 area of ice
                getWorld()->playSound(SOUND_DIG);
        }
    }
}

void Iceman::annoyed() {}

void Iceman::squirtWater() {
    getWorld()->createWaterSquirt();
    water_--;
    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
}

void Iceman::useSonar() {
    sonar_--;
    getWorld()->revealGoodies(getX(), getY(), 12);
    getWorld()->playSound(SOUND_SONAR);
}

void Iceman::abortLevel() {
    hp_ = 0;
    setAlive(false);
}

void Iceman::addWater() {
    water_ += 5;
}

void Iceman::addSonar() {
    sonar_++;
}

void Iceman::addGold() {
    gold_++;
}

int Iceman::getWater() {
    return water_;
}

int Iceman::getGold() {
    return gold_;
}

int Iceman::getSonar() {
    return sonar_;
}

int Iceman::getHealth() {
    return hp_;
}

// *************************************
// *********** WATER SQUIRT ************
// *************************************

void WaterSquirt::init() {
    setVisible(true);
}

void WaterSquirt::tick() {
    // Check if within radius of 3 of protestors
    // ...

    if (travelDist_ == 0)
        setAlive(false);
    else {
        Direction dir = getDirection();
        int x = getX();
        int y = getY();
        int targetX = x;
        int targetY = y;

        switch (dir) {
            case up:
                targetY++;
                break;
            case down:
                targetY--;
                break;
            case left:
                targetX--;
                break;
            case right:
                targetX++;
                break;
            default:
                break;
        }
        if (targetX > VIEW_WIDTH - 4 || targetX < 0 ||
            targetY > VIEW_HEIGHT - 4 || targetY < 0 ||
            getWorld()->getIceHolder()->isIceAt(targetX, targetY, targetX + 3,
                                targetY + 3, true))  // Check for Ice
            // Check for Boulders
            // ...
            setAlive(false);
        else
            moveTo(targetX, targetY);

        travelDist_--;
    }
}

// *************************************
// **************** ICE ****************
// *************************************

void Ice::init() {
    setVisible(true);
}

void Ice::tick() {}

void Ice::annoyed() {}

// *************************************
// *********** COLLECTABLE *************
// *************************************

void Collectable::tick() {
    if (isAlive()) {
        if (!isVisible() && icemanWithinDist(4)) {
            setVisible(true);
            return;
        } else if (icemanWithinDist(3)) {
            activate();
        }

        // Remove collectable if lifetime runs out
        if (lifetime_ > 0) {
            lifetime_--;
        } else if (lifetime_ == 0) {
            setAlive(false);
        }
    }
}

bool Collectable::icemanWithinDist(int numUnits) {
    Actor* iceman = getWorld()->getIceman();

    int icemanX = iceman->getX();
    int icemanY = iceman->getY();

    if (abs(getX() - icemanX) <= numUnits && abs(getY() - icemanY) <= numUnits)
        return true;

    return false;
}

void Collectable::setLifetime(int time) {
    lifetime_ = time;
}

// *************************************
// **************** OIL ****************
// *************************************

void OilBarrel::init() {
    setVisible(false);
    setLifetime(-1);
}

void OilBarrel::activate() {
    setAlive(false);
    getWorld()->playSound(SOUND_FOUND_OIL);
    getWorld()->increaseScore(1000);
}

// *************************************
// ************ WATER POOL *************
// *************************************

void WaterPool::init()  // Need to create common class (same code)
{
    setVisible(true);
    int current_level_num = getWorld()->getLevel();
    setLifetime(max(100, 300 - 10 * current_level_num));
}

void WaterPool::activate() {
    setAlive(false);
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(100);
    getWorld()->getIceman()->addWater();
}

// *************************************
// ************* SONAR KIT *************
// *************************************

void SonarKit::init()  // Need to create common class (same code)
{
    setVisible(true);
    int current_level_num = getWorld()->getLevel();
    setLifetime(max(100, 300 - 10 * current_level_num));
}

void SonarKit::activate() {
    setAlive(false);
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(75);
    getWorld()->getIceman()->addSonar();
}