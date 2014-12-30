#ifndef __Grow_Demo_test__GameLayerPlant_Level_1__
#define __Grow_Demo_test__GameLayerPlant_Level_1__
#include "GameLayerPlant.h"
#include "PlantHelper.h"
//#include "PlantLeaf.h"
#include "GamePhysicalWorld.h"
#include "PlantLeafs.h"
using namespace     PlantHelper;
class GameLayerPlant_Level_1 : public GameLayerPlant
{
public:

    CREATE_FUNC(GameLayerPlant_Level_1);
    virtual bool    initGameInfo();
};

#endif /* defined(__Grow_Demo_test__GameLayerPlant_Level_1__) */
