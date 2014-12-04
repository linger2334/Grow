#ifndef __Grow_Beta_01__GameLayerLight__
#define __Grow_Beta_01__GameLayerLight__
#include "common.h"
#include "GameLayerBase.h"
#include "Plant.h"
#include "PathListHelper.hpp"
#include "TuoYuanRotateNode.h"
#define  MAX_LIGHT_COUNT 1000
class TuoYuanRotateNode;
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
    LightMoveToPlantLine,
    LightWillRemove,
};
enum
{
    LightActionNormal,
    LightActionScaleToNormal,
    LightActionScaleToBig
};
struct  LightPathContext
{
public:
    LightPathContext():_plantIndex(0),_id(0),_target(nullptr),
        _moveSpeed(0),_toRotateA(0),_toRotateB(0),_changeRotateSpeed(0),
    _isRemoving(false),_state(LightIntRawState),_isSacling(false),_lightAction(LightActionNormal){}
    int _plantIndex;
    int _id;
    TuoYuanRotateNode* _target;
    float _nowLengthByTop;
    float _toLengthByTop;
    float _moveSpeed;
    int   _state;
    float _toRotateA;
    float _toRotateB;
    float _changeRotateSpeed;
    bool  _isRemoving;
    bool  _isSacling;
    int   _lightAction;
   // float _heightInPlant;
};

struct PlantLightContext
{
public:
    PlantLightContext():_lightLength(0.0f),_isRemoveingLight(false){}
    std::map<int,LightPathContext> _lightLists;
    float                          _lightLength;
    ContorlPointV2                 _topCP;
    float                          _heightInPlant;
    bool                           _isRemoveingLight;
};
class ItemModel;
class tempCheckToPositionContext
{
public:
    int id;
    float toLengthByTop;
    bool operator < (const tempCheckToPositionContext& t)
    {
        return toLengthByTop < t.toLengthByTop;
    }
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
    
    BezierBy* createLightBezierAction(Vec2 start,Vec2 end,float time);
    
    void  moveDownLights(float yLen);
    void  updateLightsByPlantIndex(float dt,int index);
    void  onChangePlantList(float index);
    
    void  onPlantCpListChange(float height,int index);
    void  onPlantHeightChange(float height,int index);
    float getLengthByPlantHeight(float plantHeight);
    
    void  testAddOneLight();
    void  addOneLightByPlantIndex(Vec2 startPoint,int index);
    void  addOneLightByPlantIndexUseBezier(Vec2 startPoint,int index);
    void  addOneLightByPlantIndex(int index);
    void  addOneLightByPlantIndexRandConfig(int index);
    int   addOneLightByPlantIndex(int index,const LightPathContext& config);
    void  addLightsBySubMapInit(int plantIndex,int count);
    void addLightsUseBezier(int plantIndex,Node* item,int count);
    void addLightsUseBezier(int plantIndex,Vec2 point,int count,float waitTime);
    int   getLightCountByPlantIndex(int index);
    
    void checkUpdateToPosition(int plantIndex);
    
    Action* createInitAction(Vec2 startPoint,const LightPathContext& context );
    void onRemoveLightCall(Node* node,void* data);
    void removeLight(int plantIndex,int id);
    void removeLightUseUseBezier(int plantIndex,int id,Vec2 endPoint);
    void removeLightRandId(int plantIndex);
    void removeLightTopId(int plantIndex);
    void removeLights(int plantIndex,int count);
    void removeOneLightUseBezier(int plantIndex,Vec2 endPoint);
    
    int  getMinLengthLightId( int plantIndex);
    
    LightPathContext* getLightContextByPlantIndexAndId(int plantIndex,int id);
    void removeLightToPlantList(int plantIndex,int id);

    void removeLightByPlantIndexAndLightId(int plantIndex,int id);
    
    void removeLightByPlantIndexAndLightIdNoUseAction(int plantIndex,int id);
    
    void removeLightRandIdByPlantIndex(int plantIndex);
    

    
    int randLightByPlant(int plantIndex);
    
    int getLingtPlantIndexByTag(int tag)
    {
        return tag/MAX_LIGHT_COUNT;
    }
    int getLightIdByTag(int tag)
    {
          return tag%MAX_LIGHT_COUNT;
    }
    int getLightTag(int plantIndex,int id);
    

    void removeLightsRandIdUseSacel(int plantIndex,int count);
    
    LightPathContext&  getOneInsertLightConfig(int plantIndex);
    void updateLightToPosition(int plantindex);
    
    LightPathContext randLightConfig();
    
    void getLightConfigById(int id);
    void testRandRotateLength();
    
    void startLightScaleAction(int plantIndex);
    
    float getPlantInViewLength(int plantIndex);
    
    float getOneRandLightInitLength(int plantIndex);
    

    float getLenghtByPlantHeightToTop(int plantIndex,float height);
    

    void initLightsBySubMap();
    
    bool isAllPlantHasLights();
    
    void runChangeLightsAction(int loopCount,float waitTime);
    
    std::vector<PlantLightContext> _plantLights;
    
    std::list<Node*> _lightReleaseList;
    int     _plantCount;
};

#endif /* defined(__Grow_Beta_01__GameLayerLight__) */
