#ifndef __Grow_beta_test02__GameLayerPlant_level_3__
#define __Grow_beta_test02__GameLayerPlant_level_3__
#include "GameLayerPlant.h"
#include "PlantHelper.h"
//#include "PlantLeaf.h"
#include "GamePhysicalWorld.h"
#include "PlantLeafs.h"
using namespace     PlantHelper;
class ItemModel;
class GameLayerPlant_Level_3 : public GameLayerPlant
{
public:
    CREATE_FUNC(GameLayerPlant_Level_3);

    virtual bool    initGameInfo();
};

#endif /* defined(__Grow_beta_test02__GameLayerPlant_level_3__) */
