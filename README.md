CURRENT ACTOR INHERITENCE TREE:
              
                                                  Actor
                                                    |
              ----------------------------------------------------------------------------------
              |               |                              |                                 |
           Terrain          Iceman                      Collectable                        Protestor
              |                                              |                                 |
        -------------                    ----------------------------------------      HardcoreProtestor
        |           |                    |            |            |            |
       Ice        Boulder           WaterRefill   OilBarrel    GoldNugget    SonarKit



===== ACTOR =====

=== Actor ===
init() -> called when object is created (should be placed in constructor)
tick() -> called every game tick
annoyed() -> called when actor becomes annoyed


=== Collectable ===
activate() -> called when item is picked up by Iceman
icemanWithinDist(int numUnits) -> used to calculate distance from item to the Iceman





===== STUDENTWORLD =====

== init() ==
createIce() -> creates the 64x64 grid of ice
populateOilField() -> adds the oil barrels, gold nuggets, and boulders to the ice field

== move() ==
updateGameStats() -> grabs all of the current game data (level, # of items, etc..) and writes it to the display at the top of the game screen
removeDeadActors() -> checks if each actor's "alive_" flag is set to false. If so, it deallocates that actor
