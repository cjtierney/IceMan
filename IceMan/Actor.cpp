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

// *************************************
// ************** ICEMAN ***************
// *************************************

void Iceman::init()
{
	setVisible(true);
}

void Iceman::tick()
{	
	int key;
	if (getWorld()->getKey(key) == true)
	{
		switch (key)
		{
			case KEY_PRESS_UP:
				setDirection(up);
				if(getY() < VIEW_HEIGHT - 4)
					moveTo(getX(), getY() + 1);
				break;
			case KEY_PRESS_DOWN:
				setDirection(down);
				if (getY() > 0)
					moveTo(getX(), getY() - 1);
				break;
			case KEY_PRESS_RIGHT:
				setDirection(right);
				if(getX() < VIEW_WIDTH - 4)
					moveTo(getX() + 1, getY());
				break;
			case KEY_PRESS_LEFT:
				setDirection(left);
				if (getX() > 0)
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
				break;
			default:
				break;
		}
		// Mine Ice
		getWorld()->deleteIce(getX(), getX() + 3, getY(), getY() + 3);
	}
}

void Iceman::annoyed()
{

}

void Iceman::addWater()
{
	water_++;
}

void Iceman::addSonar()
{
	sonar_++;
}

void Iceman::addGold()
{
	gold_++;
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