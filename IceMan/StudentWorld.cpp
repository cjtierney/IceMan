#include "StudentWorld.h"
#include <string>
#include <format>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{
	IcemanPtr = new Iceman(30, 60, this);		//CREATE ICEMAN

	createIce();

	populateOilField();

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();
	//return GWSTATUS_PLAYER_DIED;

	for (Actor* actor : ActorVec)
		actor->tick();

	IcemanPtr->tick();

	updateGameStats();

	removeDeadActors();


	// Determine game status to return
	if (getOilRemaining() == 0)
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	} else if (IcemanPtr->getHealth() <= 0) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	while (!IceVec.empty())	// DEALLOCATE ICE
	{
		delete IceVec.back();
		IceVec.pop_back();
	}

	while (!ActorVec.empty()) // DEALLOCATE ACTORS
	{
		delete ActorVec.back();
		ActorVec.pop_back();
	}

	delete IcemanPtr;	//DEALLOCATE ICEMAN
}

void StudentWorld::createIce()
{
	for (int y = 0; y < VIEW_HEIGHT; y++)
		for (int x = 0; x < VIEW_WIDTH; x++)
		{
			if (x >= 30 && x <= 33 && y >= 4 && y <= 59) // Pit in center
				IceVec.push_back(nullptr);
			else if (y >= 60)							 // Top is empty
			{
				IceVec.push_back(nullptr);
			}

			else
				IceVec.push_back(new Ice(x, y, this));
		}
}

bool StudentWorld::deleteIce(int x1, int x2, int y1, int y2)
{
	bool iceDeleted = false;
	for (int y = y1; y <= y2; y++)
		for (int x = x1; x <= x2; x++)
		{
			int icePos = (VIEW_WIDTH * y) + x;
			if(icePos < VIEW_WIDTH * VIEW_HEIGHT)
				if (IceVec[icePos] != nullptr)
				{
					delete IceVec[icePos];
					IceVec[icePos] = nullptr;
					iceDeleted = true;
				}
		}
	return iceDeleted;
}

bool StudentWorld::isIceAt(int x, int y)
{
	int icePos = (VIEW_WIDTH * y) + x;
	return IceVec[icePos] != nullptr;
}

void StudentWorld::populateOilField()
{
	srand(time(0));

	int currentLevel = getLevel();

	// CREATE OIL BARRELS
	int oilCount = min(2 + currentLevel / 2, 21);

	while (oilCount > 0)
	{
		int x = rand() % 61;
		int y = rand() % 57;
		bool validPos = true;
		for (Actor*& actor : ActorVec)
		{
			if (calculateDist(x, y, actor->getX(), actor->getY()) <= 6.0)
			{
				validPos = false;
				cout << "Invalid Pos!" << endl;
                continue;
			}
		}
		if (validPos)
		{
			ActorVec.push_back(new OilBarrel(x, y, this));
			oilCount--;
			cout << "Oil Created at: " << x << ", " << y << endl;
		}
	}
}

double StudentWorld::calculateDist(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void StudentWorld::updateGameStats()
{
	// Write Display Text
	string displayText = "";

	displayText += "Lvl: " + format("{: 2}", getLevel());
	displayText += " Lives: " + to_string(getLives());
	displayText += " Hlth: " + format("{: 2}", IcemanPtr->getHealth()) + "%";
	displayText += " Wtr: " + format("{: 2}", IcemanPtr->getWater());
	displayText += " Gld: " + format("{: 2}", IcemanPtr->getGold());
	displayText += " Oil: " + format("{: 2}", getOilRemaining());
	displayText += " Sonar: " + format("{: 2}", IcemanPtr->getSonar());
	displayText += " Scr: " + format("{: 6}", getScore());

	setGameStatText(displayText);
}

void StudentWorld::removeDeadActors()
{
    erase_if(ActorVec, [](Actor* actor) {
		bool isDead = !actor->isAlive();
        if (isDead) delete actor;
        return isDead;
	});
}

int StudentWorld::getOilRemaining()
{
    return count_if(ActorVec.begin(), ActorVec.end(), [](Actor* actor) {
		return actor->getID() == IID_BARREL;	
	});
}

Iceman* StudentWorld::getIceman()
{
	return IcemanPtr;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}