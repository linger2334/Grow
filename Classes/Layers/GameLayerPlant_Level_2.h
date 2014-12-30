
#ifndef __Grow_Beta_01__GameLayerPlant_Level_2__
#define __Grow_Beta_01__GameLayerPlant_Level_2__
#include "GameLayerPlant.h"
#include "PlantHelper.h"
//#include "PlantLeaf.h"
#include "GamePhysicalWorld.h"
#include "PlantLeafs.h"
using namespace     PlantHelper;
class GameLayerPlant_Level_2 : public GameLayerPlant
{
public:
    CREATE_FUNC(GameLayerPlant_Level_2);
//     PlantLeafListBase* createOnePlantLeafList()
//    {
//        return nullptr;
//    }
    virtual bool    initGameInfo();
};


#endif /* defined(__Grow_Beta_01__GameLayerPlant_Level_2__) */
