#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


// *************************************
// ************** ACTOR ***************
// *************************************

StudentWorld* Actor::getWorld()
{
	return world_;
}

bool Actor::isAlive() const
{
	return alive_;
}

void Actor::setAlive(bool state)
{
	alive_ = state;
}

// *************************************
// ************** ICEMAN ***************
// *************************************

void Iceman::init()
{
	setVisible(true);
}

void Iceman::tick()
{	
	if (hp_ == 0)
		return;
	else
	{
		int key;
		if (getWorld()->getKey(key) == true)
		{
			switch (key)
			{
			case KEY_PRESS_UP:
				if(getDirection() != up)
					setDirection(up);
				else if (getY() < VIEW_HEIGHT - 4)
					moveTo(getX(), getY() + 1);
				break;
			case KEY_PRESS_DOWN:
				if (getDirection() != down)
					setDirection(down);
				else if (getY() > 0)
					moveTo(getX(), getY() - 1);
				break;
			case KEY_PRESS_RIGHT:
				if (getDirection() != right)
					setDirection(right);
				else if (getX() < VIEW_WIDTH - 4)
					moveTo(getX() + 1, getY());
				break;
			case KEY_PRESS_LEFT:
				if (getDirection() != left)
					setDirection(left);
				else if (getX() > 0)
					moveTo(getX() - 1, getY());
				break;
			case KEY_PRESS_SPACE:
				break;
			case KEY_PRESS_TAB:
				break;
			case 'Z':
			case 'z':
				break;
			case KEY_PRESS_ESCAPE:
				abortLevel();
				break;
			default:
				break;
			}
			// Mine Ice
			if(getWorld()->deleteIce(getX(), getX() + 3, getY(), getY() + 3)) // try to dig 4x4 area of ice
				getWorld()->playSound(SOUND_DIG);
		}
	}
}

void Iceman::annoyed()
{

}

void Iceman::squirtWater()
{
	water_--;
}

void Iceman::abortLevel()
{
	hp_ = 0;
}


void Iceman::addWater()
{
	water_ += 5;
}

void Iceman::addSonar()
{
	sonar_++;
}

void Iceman::addGold()
{
	gold_++;
}


int Iceman::getWater()
{
	return water_;
}

int Iceman::getGold()
{
	return gold_;
}

int Iceman::getSonar()
{
	return sonar_;
}

int Iceman::getHealth()
{
	return hp_;
}



// *************************************
// **************** ICE ****************
// *************************************

void Ice::init()
{
	setVisible(true);
}

void Ice::tick()
{

}

void Ice::annoyed()
{

}





// *************************************
// *********** COLLECTABLE *************
// *************************************

void Collectable::tick()
{
	if (isAlive())
	{
		if (!isVisible() && icemanWithinDist(4))
		{
			setVisible(true);
			return;
		}
		else if (icemanWithinDist(3))
		{
			activate();
		}
	}
}

bool Collectable::icemanWithinDist(int numUnits)
{
	Actor* iceman = getWorld()->getIceman();

	int icemanX = iceman->getX();
	int icemanY = iceman->getY();

	if (abs(getX() - icemanX) <= numUnits && abs(getY() - icemanY) <= numUnits)
		return true;

	return false;
}

// *************************************
// **************** OIL ****************
// *************************************

void OilBarrel::init()
{
	setVisible(false);
}

void OilBarrel::activate()
{
	setAlive(false);
	getWorld()->playSound(SOUND_FOUND_OIL);
	getWorld()->increaseScore(1000);
}