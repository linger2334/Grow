#include "GameLayerPlant_level_3.h"
#include "Plant.h"
#include "PlantHelper.h"
#include "GameRuntime.h"
#include "GamePhysicalWorld.h"
#include "GameRunningInfo.h"
#define GROW_MIN_ANGLE_LEFT (-110)
#define GROW_MIN_ANGLE_RIGHT 110
#define GROW_MIN_ANGLE_CRASH_STONE_LEFT (-70)
#define GROW_MIN_ANGLE_CRASH_STONE_RIGHT 70

#define GROW_MIN_ANGLE_NEAR_BORDER_LEFT (-80)
#define GROW_MIN_ANGLE_NEAR_BORDER_RIGHT (80)
#define LEAF_RAND_LENGTH 51
#define LEAF_LENGHT      201



bool  GameLayerPlant_Level_3::initGameInfo()
{
    GameLayerPlant::initGameInfo();
  
    int id = addOnePlant();
      createHeadB2Body(id);

    int id1 = addOnePlant();
      createHeadB2Body(id1);

    return true;
}
