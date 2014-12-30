#include "GameLayerPlant_Level_1.h"
#include "Plant.h"
#include "PlantHelper.h"
#include "GameRuntime.h"
#include "GamePhysicalWorld.h"
#include "GameRunningInfo.h"

bool  GameLayerPlant_Level_1::initGameInfo()
{
    GameLayerPlant::initGameInfo();

    int id = addOnePlant();
    createHeadB2Body(id);
    
    return true;
}
