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
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();
	//return GWSTATUS_PLAYER_DIED;

	IcemanPtr->tick();

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	while (!IceVec.empty())	// DEALLOCATE ICE
	{
		delete IceVec.back();
		IceVec.pop_back();
	}

	delete IcemanPtr;	//DEALLOCATE ICEMAN
}

void StudentWorld::createIce()
{
	// **Maybe remove constants later?
	for (int y = 0; y < VIEW_HEIGHT - 4; y++)
		for (int x = 0; x < VIEW_WIDTH; x++)	// top 4 layers are not ice
		{
			if (x >= 30 && x <= 33 && y >= 4 && y <= 59)
				IceVec.push_back(nullptr);
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
			if(icePos < VIEW_WIDTH * (VIEW_HEIGHT - 4))
				if (IceVec[icePos] != nullptr)
				{
					delete IceVec[icePos];
					IceVec[icePos] = nullptr;
					playSound(SOUND_DIG);
					iceDeleted = true;
				}
		}
	return iceDeleted;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}