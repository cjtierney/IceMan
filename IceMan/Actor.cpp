#include "Actor.h"

#include <thread>

#include "StudentWorld.h"
using namespace std;

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
// ************** PERSON ***************
// *************************************

int Person::getHP() {
    return hp_;
}

void Person::subtractHP(int dmg) {
    hp_ -= dmg;
}

// *************************************
// ************ PROTESTORS *************
// *************************************

void Protester::init() {
    setVisible(true);
}

void Protester::tick() {
    int moveInterval = getMoveInterval();
    moveIntervalCounter_ = ++moveIntervalCounter_ % moveInterval;
    if (!isAlive()) return;
    if (moveIntervalCounter_ != 0) return;
    if (leaveTheOilField_) {
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

void Protester::annoyed(int dmg) {
    if (dmg == 0) { // Bribe
        leaveTheOilField_ = true;
    }
        
    else // Take damage
        subtractHP(dmg);
    //...
}


int Protester::getMoveInterval() {
    int level = getWorld()->getLevel();
    return std::max(0, 3 - level / 4);
}

// *************************************
// ************** ICEMAN ***************
// *************************************

void Iceman::init() {
    setVisible(true);
}

void Iceman::tick() {
    if (getHP() <= 0)
    {
        setAlive(false);
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        return;
    }
    else {
        int key;
        if (getWorld()->getKey(key) == true) {
            int targX = getX();
            int targY = getY();
            switch (key) {
                case KEY_PRESS_UP:
                case 'w':
                case 'W':
                case '8':
                    targY++;
                    if (getDirection() != up)
                        setDirection(up);
                    else if (targY <= VIEW_HEIGHT - 4 && !getWorld()->isActorNearby(IID_BOULDER, targX, targY, 0, 3))
                        moveTo(targX, targY);
                    break;
                case KEY_PRESS_DOWN:
                case 's':
                case 'S':
                case '2':
                    targY--;
                    if (getDirection() != down)
                        setDirection(down);
                    else if (targY >= 0 && !getWorld()->isActorNearby(IID_BOULDER, targX, targY, 0, 3))
                        moveTo(targX, targY);
                    break;
                case KEY_PRESS_RIGHT:
                case 'd':
                case 'D':
                case '6':
                    targX++;
                    if (getDirection() != right)
                        setDirection(right);
                    else if (targX <= VIEW_WIDTH - 4 && !getWorld()->isActorNearby(IID_BOULDER, targX, targY, 0, 3))
                        moveTo(targX, targY);
                    break;
                case KEY_PRESS_LEFT:
                case 'a':
                case 'A':
                case '4':
                    targX--;
                    if (getDirection() != left)
                        setDirection(left);
                    else if (targX >= 0 && !getWorld()->isActorNearby(IID_BOULDER, targX, targY, 0, 3))
                        moveTo(targX, targY);
                    break;
                case KEY_PRESS_SPACE:
                    if (water_ > 0) squirtWater();
                    break;
                case KEY_PRESS_TAB:
                    if (gold_ > 0) dropGold();
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
                    getX(), getY(), getX() + 3, getY() + 3, true))  // try to dig 4x4 area of ice
                getWorld()->playSound(SOUND_DIG);
        }
    }
}

void Iceman::annoyed(int dmg) {
    subtractHP(dmg);
    if (getHP() <= 0) {
        setAlive(false);
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
    }
}

void Iceman::squirtWater() {
    int dir = getDirection();
    int x = getX();
    int y = getY();
    switch (dir) {
        case up:
            y += 4;
            break;
        case down:
            y -= 4;
            break;
        case right:
            x += 4;
            break;
        case left:
            x -= 4;
            break;
    }

    if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT
        || getWorld()->getIceHolder()->isIceAt(x, y, x + 3, y + 3, true)
        || getWorld()->isActorNearby(IID_BOULDER, x, y, 0, 3))
    {
        cout << "fail: " << x << y << endl;
    }
    else
        getWorld()->createWaterSquirt(x, y);

    water_--;
    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
}

void Iceman::dropGold()
{
    gold_--;
    getWorld()->dropGold(getX(), getY());
}

void Iceman::useSonar() {
    sonar_--;
    getWorld()->revealGoodies(getX(), getY(), 12);
    getWorld()->playSound(SOUND_SONAR);
}

void Iceman::abortLevel() {
    subtractHP(getHP());
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

// *************************************
// *********** WATER SQUIRT ************
// *************************************

void WaterSquirt::init() {
    setVisible(true);
}

void WaterSquirt::tick() {
    if (travelDist_ == 0) {
        setAlive(false);
        return;
    }
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
            getWorld()->getIceHolder()->isIceAt(targetX, targetY, targetX + 3, targetY + 3, true)
            || getWorld()->isActorNearby(IID_BOULDER, x, y, 0, 3)) {
            setAlive(false);
            return;
        }
        if (getWorld()->damageNearbyProtesters(x, y, 3, 2)) {
            setAlive(false);
            return;
        }
        moveTo(targetX, targetY);

        travelDist_--;
    }
}

// *************************************
// ************** TERRAIN **************
// *************************************

void Terrain::init() {
    setVisible(true);
}

// *************************************
// **************** ICE ****************
// *************************************

void Ice::tick() {}

// *************************************
// ************** BOULDER **************
// *************************************

void Boulder::tick() {
    if (isAlive() == false)
        return;
    switch (state_) {
        case stable:
            if (!getWorld()->getIceHolder()->isIceAt(getX(), getY() - 1, getX() + 3, getY() - 1, true))
                state_ = waiting;
            break;
        case waiting:
            if (cooldown_ > 0)
                cooldown_--;
            else
            {
                state_ = falling;
                getWorld()->playSound(SOUND_FALLING_ROCK);
            }
            break;
        case falling:
            int targX = getX();
            int targY = getY() - 1;
            if (targY < 0 || getWorld()->getIceHolder()->isIceAt(targX, targY, targX + 3, targY, true)
                || getWorld()->isActorNearby(IID_BOULDER, targX, targY + 1, 0.1, 3)) {
                setAlive(false);
                return;
            }
            else
            {
                getWorld()->damageNearbyProtesters(targX, targY, 3, 100);

                Iceman* iceman = getWorld()->getIceman();
                if (hypot(targX - iceman->getX(), targY - iceman->getY()) <= 3)
                    iceman->subtractHP(100);
            }
            moveTo(targX, targY);
            break;
    }
}

// *************************************
// *********** COLLECTABLE *************
// *************************************

void Collectable::tick() {
    if (isAlive()) {
        if (isPickupable()) {
            if (!isVisible() && getWorld()->icemanWithinDist(getX(), getY(), 4)) {
                setVisible(true);
                return;
            }
            else if (getWorld()->icemanWithinDist(getX(), getY(), 3)) {
                activate();
            }

            // Remove collectable if lifetime runs out
            if (lifetime_ > 0) {
                lifetime_--;
            }
            else if (lifetime_ == 0) {
                setAlive(false);
            }
        }
        else
        {
            if (getWorld()->damageNearbyProtesters(getX(), getY(), 3, 0)) // 0 dmg to bribe
                activate();
        }
    }
}

void Collectable::setLifetime(int time) {
    lifetime_ = time;
}

bool Collectable::isPickupable() {
    return pickupable_;
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
// ************ GOLD NUGGET ************
// *************************************

void GoldNugget::init() {
    if (isPickupable()) {
        setVisible(false);
        setLifetime(-1);
    }
    else {
        setVisible(true);
        setLifetime(100);
    }
}

void GoldNugget::activate() {
    setAlive(false);
    if (isPickupable()) {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
        getWorld()->getIceman()->addGold();
    }
    else {
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        getWorld()->increaseScore(25);
    }
}

// *************************************
// ************ WATER POOL *************
// *************************************

void WaterPool::init()
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

void SonarKit::init()
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