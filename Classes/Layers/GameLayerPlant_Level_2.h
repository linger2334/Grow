
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
    virtual ~GameLayerPlant_Level_2()
    {
    }
    CREATE_FUNC(GameLayerPlant_Level_2);
    virtual bool    init();
    virtual bool    initGameInfo();
    virtual void    update(float dt);
    virtual float   growByIndex(float length,int index);
    virtual float   reGrowByIndex(float length,int index);
    
    void onPlantHeightChange(float height,int index);
    PlantGrowContext getPlantGrowContextAngle(PlantNode* _plant);
    
    virtual  void moveDown(float yLen);
    //virtual void    update(float dt);
    PlantNode*      _plant;
    int             _plantState;
    float           _growSpeed;
    PlantHelper::PlantRadiusHelper _radius;
    b2Body*         _headBody;
    PlantLeafsHelper_1 _leafs;
    SpriteBatchNode*   _leafBatch;
    // PlantLeafHelper _leafHelper;
};


#endif /* defined(__Grow_Beta_01__GameLayerPlant_Level_2__) */
