#ifndef __Grow_beta_test02__GameLayerPlant_level_3__
#define __Grow_beta_test02__GameLayerPlant_level_3__
#include "GameLayerPlant.h"
#include "PlantHelper.h"
//#include "PlantLeaf.h"
#include "GamePhysicalWorld.h"
#include "PlantLeafs.h"
using namespace     PlantHelper;
class GameLayerPlant_Level_3 : public GameLayerPlant
{
public:
    virtual ~GameLayerPlant_Level_3()
    {
    }
    CREATE_FUNC(GameLayerPlant_Level_3);
    virtual bool    init();
    virtual bool    initGameInfo();
    virtual void    update(float dt);
    virtual float   growByIndex(float length,int index);
    virtual float   reGrowByIndex(float length,int index);
    
    void onPlantHeightChange(float height,int index);
    PlantGrowContext getPlantGrowContextAngle(PlantNode* _plant);
    
    virtual  void moveDown(float yLen);

    PlantLeafsHelper_1 _leafs;
    PlantLeafsHelper_1 _leafsExt;
};

#endif /* defined(__Grow_beta_test02__GameLayerPlant_level_3__) */
