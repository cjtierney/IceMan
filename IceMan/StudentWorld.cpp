#include "StudentWorld.h"
#include <string>
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
	}
	else if (IcemanPtr->getHealth() <= 0)
		return GWSTATUS_PLAYER_DIED;
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
	if (IceVec[icePos] != nullptr)
		return true;
	return false;
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
		for (int i = 0; i < ActorVec.size(); i++)
		{
			if (calculateDist(x, y, ActorVec[i]->getX(), ActorVec[i]->getY()) <= 6.0)
			{
				validPos = false;
				cout << "Invalid Pos!" << endl;
				break;
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
	int level = getLevel();
	int lives = getLives();
	int health = IcemanPtr->getHealth();
	int water = IcemanPtr->getWater();
	int gold = IcemanPtr->getGold();
	int oil = getOilRemaining();
	int sonar = IcemanPtr->getSonar();
	int score = getScore();


	// Write Display Text
	string displayText = "";

	if (level <= 9)
		displayText += ("Lvl:  " + to_string(level));
	else
		displayText += ("Lvl: " + to_string(level));

	displayText += (" Lives: " + to_string(lives));

	if (health <= 9)
		displayText += (" Hlth:   " + to_string(health));
	else if (health <= 99)
		displayText += (" Hlth:  " + to_string(health));
	else
		displayText += (" Hlth: " + to_string(health));

	if (water <= 9)
		displayText += ("% Wtr:  " + to_string(water));
	else
		displayText += ("% Wtr: " + to_string(water));

	if (gold <= 9)
		displayText += (" Gld:  " + to_string(gold));
	else
		displayText += (" Gld: " + to_string(gold));

	if (oil <= 9)
		displayText += (" Oil:  " + to_string(oil));
	else
		displayText += (" Oil: " + to_string(oil));

	if (sonar <= 9)
		displayText += (" Sonar:  " + to_string(sonar));
	else
		displayText += (" Sonar: " + to_string(sonar));

	if (score <= 9)
		displayText += (" Scr:      " + to_string(score));
	else if (score <= 99)
		displayText += (" Scr:     " + to_string(score));
	else if (score <= 999)
		displayText += (" Scr:    " + to_string(score));
	else if (score <= 9999)
		displayText += (" Scr:   " + to_string(score));
	else if (score <= 99999)
		displayText += (" Scr:  " + to_string(score));
	else
		displayText += (" Scr: " + to_string(score));

	setGameStatText(displayText);
}

void StudentWorld::removeDeadActors()
{
	for (int i = 0; i < ActorVec.size(); i++)
	{
		if (ActorVec[i]->isAlive() == false)
		{
			delete ActorVec[i];
			ActorVec.erase(ActorVec.begin() + i);
		}
	}
}

int StudentWorld::getOilRemaining()
{
	int oilCount = 0;

	for (Actor*& actor : ActorVec)
		if (actor->getID() == IID_BARREL)
			oilCount++;
	return oilCount;
}

Iceman* StudentWorld::getIceman()
{
	return IcemanPtr;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}