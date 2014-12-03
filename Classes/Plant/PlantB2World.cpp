#include "PlantB2World.h"
#include "GameLayerPlant.h"
PlantNode* PlantHeadBody::getPlantNode()
{
    return GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(_plantIndex);
}