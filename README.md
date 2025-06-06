CURRENT ACTOR INHERITENCE TREE:
              
                                                           Actor
                                                             |
              -----------------------------------------------------------------------------------------------------
              |               |                              |                                 |                  |
           Terrain          Iceman                      Collectable                        Protestor         Water Squirt
              |                                              |                                 |
        -------------                    ----------------------------------------      HardcoreProtestor
        |           |                    |            |            |            |
       Ice        Boulder           WaterRefill   OilBarrel    GoldNugget    SonarKit



---------- ACTOR ----------

--- Actor ---

init() -> called when object is created (should be placed in constructor)

tick() -> called every game tick

annoyed() -> called when actor becomes annoyed

world_ -> pointer to the active StudentWorld (For if you need to manipulate the world, such as deleting ice)

alive_ -> flag for whether actor is alive. Default set to true. Actor gets deleted when flag is false


--- Iceman ---

init() -> sets Iceman to be visible

tick() -> checks for user input and act accordingly

annoyed() ->

addWater() -> adds +5 to water_

addSonar() -> adds +1 to sonar_

addGold() -> adds +1 to gold_

getWater() -> returns water_

getSonar() -> returns sonar_

getGold() -> returns gold_

getHealth() -> returns hp_

abortLevel() -> sets hp_ to 0 and sets alive_ to false

squirtWater() -> creates a water squirt at current position/direction

useSonar() -> reveals oil barrels and gold nuggets in radius of 12


hp_ -> percent of hp left. Default value is 100

water_ -> number of water squirts held. Default is 10

sonar_ -> number of sonar kits held. Default is 1

gold_ -> number of gold nuggets held. Default is 2


--- Water Squirt ---

init() -> sets water squirt to be visible

tick() -> checks if water squirt collides with protestor, ice, boulder, or travels full distance of 4 squares


--- Ice ---

init() -> sets ice to be visible

tick() -> nothing

annoyed() -> nothing



--- Collectable ---

tick() -> sets collectable to be visible when iceman within 4 units. Activates when iceman within 3 units

activate() -> called when item is picked up by Iceman (behavior varies based on child class)

icemanWithinDist(int numUnits) -> used to calculate distance from item to the Iceman



--- OilBarrel ---

init() -> sets oil to be invisible

activate() -> sets alive_ flag to false, plays sound effect, increases score by 1000


--- Water Pool ---

init() -> sets water pool to be visible, caluclates/sets lifetime

activate() -> sets alive_ flag to false, plays sound effect, increases score by 100, increase iceman's water_ count by 5


--- Sonar Kit ---

init() -> sets sonar kit to be visible, caluclates/sets lifetime

activate() -> sets alive_ flag to false, plays sound effect, increases score by 75, increase iceman's sonar_ count by 1



---------- STUDENTWORLD ----------

init() -> creates iceman, creates ice, populates oil field with barrels (later will add gold and boulders)

move() -> calls the tick() function for every Actor, takes user input for Iceman action, updates the stats display, removes dead actors, checks if all oil barrels have been collected, checks for if iceman has died

createIce() -> creates the 64x64 grid of ice

deleteIce(int x1, int x2, int y1, int y2) -> deletes ice from (x1, y1) to (x2, y2) in a square shape

isIceAt(int x, int y) -> returns true/false whether there is currently ice at the given position (x, y)

isIceAt(int x1, int y1, int x2, int y2) -> returns true if any amount of ice exists within given range, false if no ice at all

populateOilField() -> adds the oil barrels, gold nuggets, and boulders to the ice field

updateGameStats() -> grabs all of the current game data (level, # of items, etc..) and writes it to the display at the top of the game screen

removeDeadActors() -> checks if each actor's "alive_" flag is set to false. If so, it deallocates that actor

addGoodies() -> Has a chance to add sonar kits and water pools to the oil field

revealGoodies(int xc, int yc, int r) -> sets oil barrels and gold nuggets to be visible within a radius of r centered at (xc, yc)

getOilRemaining() -> iterates through the Actor Vector (ActorVec) and counts how many actors use the ID for Oil Barrel

createWaterSquirt() -> creates a new Water Squirt object based on the Iceman's position and direction

getIceman() -> returns the Iceman Pointer (For if you need to check the status of the Iceman, such as health)



