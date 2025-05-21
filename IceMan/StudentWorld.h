#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{}

	int init() override;
	
	int move() override;

	void cleanUp() override;


	// creates the initial 64 by 64 ice grid with a pit
	void createIce();

	// Delete ice in a _ by _ area, returns true if ice deleted
	bool deleteIce(int x1, int x2, int y1, int y2);

	// Destructor
	~StudentWorld();

private:
	std::vector<Actor*> ActorVec;	// Includes all Actors (besides ice and iceman)
	std::vector<Ice*> IceVec;		// Includes all Ice objects
	Iceman* IcemanPtr;				// Includes the Iceman player
};

#endif // STUDENTWORLD_H_
