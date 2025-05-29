#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, int depth, StudentWorld* w)
		: GraphObject(imageID, startX, startY, dir, size, depth), world_(w), alive_(true)
	{
		//init();
	}

	virtual void init() = 0;
	virtual void tick() = 0;
	virtual void annoyed() = 0;

	bool isAlive();
	void setAlive(bool state);

	StudentWorld* getWorld();

private:
	StudentWorld* world_;
	bool alive_;
};




class Protestor : public Actor
{
public:

private:

};

class HardcoreProtestor : public Protestor
{
public:

private:

};




class Iceman : public Actor
{
public:
	Iceman(int startX, int startY, StudentWorld* w)
		: Actor(IID_PLAYER, startX, startY, right, 1.0, 0, w), hp_(100), water_(10), gold_(2), sonar_(1)
	{
		init();
	};

	virtual void init();
	virtual void tick();
	virtual void annoyed();

	void addWater();

	void addSonar();

	void addGold();

	void squirtWater();

	void abortLevel();

	int getWater();

	int getSonar();
	
	int getGold();

	int getHealth();

private:
	int hp_;
	int water_;
	int sonar_;
	int gold_;
};



class WaterSquirt : public Actor
{
public:

private:

};



class Terrain : public Actor
{
public:
	Terrain(int imageID, int startX, int startY, Direction dir, double size, int depth, StudentWorld* w)
		: Actor(imageID, startX, startY, dir, size, depth, w) {};

	//virtual void init();
	//virtual void tick() = 0;
	//virtual void annoyed() = 0;

private:

};

class Ice : public Terrain
{
public:
	Ice(int startX, int startY, StudentWorld* w)
		: Terrain(IID_ICE, startX, startY, right, 0.25, 3, w)
	{
		init();
	}

	void init();
	void tick();
	void annoyed();

private:

};

class Boulder : public Terrain
{
public:

private:

};




class Collectable : public Actor
{
public:
	Collectable(int imageID, int startX, int startY, StudentWorld* w)
		: Actor(imageID, startX, startY, right, 1.0, 2, w) {};

	void tick();
	void annoyed() {};

	virtual void activate() = 0;

	bool icemanWithinDist(int numUnits);

	


private:
};

class WaterRefill : public Collectable
{
public:

private:

};

class OilBarrel : public Collectable
{
public:
	OilBarrel(int startX, int startY, StudentWorld* w)
		: Collectable(IID_BARREL, startX, startY, w)
	{
		init();
	}

	void init();

	void activate();

private:

};

class GoldNugget : public Collectable
{
public:

private:

};

class SonarKit : public Collectable
{
public:

private:

};

#endif // ACTOR_H_
