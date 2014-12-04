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
    virtual ~GameLayerPlant_Level_1()
    {
    }
    CREATE_FUNC(GameLayerPlant_Level_1);
    virtual bool    init();
    virtual bool    initGameInfo();
    virtual void    update(float dt);
    virtual float   growByIndex(float length,int index);
    virtual float   reGrowByIndex(float length,int index);
    bool  isNeadBorder(PlantNode* plant);
    bool  isNearStone(PlantNode* plant);
    void onPlantHeightChange(float height,int index);
    PlantGrowContext getPlantGrowContextAngle(PlantNode* _plant);
    PlantGrowContext getPlantGrowContextStone(PlantNode* _plant);
    float nearAngle(float lenght ,float MaxLength, float startAngle ,float endAngle);
    virtual  void moveDown(float yLen);    
    //virtual void    update(float dt);
    PlantNode*      _plant;
    int             _plantState;
    float           _growSpeed;
    PlantHelper::PlantRadiusHelper _radius;
    b2Body*         _headBody;
    PlantLeafsHelper_1 _leafs;
    SpriteBatchNode*   _leafBatch;
    std::list<FaceDirection> _growList;
   // PlantLeafHelper _leafHelper;
};

#endif /* defined(__Grow_Demo_test__GameLayerPlant_Level_1__) */
