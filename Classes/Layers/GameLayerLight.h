#ifndef __Grow_Beta_01__GameLayerLight__
#define __Grow_Beta_01__GameLayerLight__
#include "common.h"
#include "GameLayerBase.h"
#include "Plant.h"
#include "PathListHelper.hpp"

class TuoYuanRotateNode;
struct  LightPathContext
{
public:
    Node* _target;
    float _nowLengthByTop;
    float _toLengthByTop;
    float _moveSpeed;
    int   _state;
};
enum LightState
{
    LightIntRawState,
    LightInitToPlantLine,
    LightOutStateMoveToPlant,
    //LightNormalChangeRing,
    LightNormal,
    LightDied,
    LightOutPlantLine,
    LightNoHasTarget,
};
class GameLayerLight : public GameLayerBase,public GameLayerHelper<GameLayerLight>
{
public:
    CREATE_FUNC(GameLayerLight);
    virtual bool init();
    virtual void update(float dt);
    virtual bool initGameInfo();
    virtual bool releaseGameInfo();
    virtual void moveDown(float yLen);
    
    void moveDownLights(float yLen);
    void  updateLightsByPlantIndex(float dt,int index);
    void  onChangePlantList(float index);
    
    void  onPlantCpListChange(float height,float index);
    void  onPlantHeightChange(float height,float index);
    float getLengthByPlantHeight(float plantHeight);
    
    void testAddOneLight();
    int getLightCountByPlantIndex(int index);
    
    std::vector< std::vector<LightPathContext> > _lightLists;
    std::vector<float> _lineLenght;
//    std::vector<LightPathContext> _lights;
  //  PathListHelper::PathList<Vec2,std::list<Vec2> > _pathList;
};

#endif /* defined(__Grow_Beta_01__GameLayerLight__) */
