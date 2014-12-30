#ifndef __Grow_Beta_01__GameLayerLight__
#define __Grow_Beta_01__GameLayerLight__
#include "common.h"
#include "GameLayerBase.h"
#include "Plant.h"
#include "PathListHelper.hpp"
#include "TuoYuanRotateNode.h"
#define  MAX_LIGHT_COUNT 1000

typedef  PathListHelper::PathList<ContorlPointV2, std::vector<ContorlPointV2> > PlantPathList;

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

struct  LightPathContext
{
public:
    LightPathContext():_plantIndex(0),_id(0),_target(nullptr),
        _moveSpeed(0),_state(LightIntRawState),_isSacling(false){}
    
    //,_toRotateA(0),_toRotateB(0),_changeRotateSpeed(0),_isRemoving(false)
    //bool  _isRemoving = false;
    
    bool  _isSacling = false;
    bool  _isFading = false;
    int   _id  = 0;
    int   _state;
    int   _plantIndex = 0;
  
    TuoYuanRotateNode* _target;
    float _nowLengthByTop;
    float _toLengthByTop;
    float _moveSpeed;

    //float _changeRotateSpeed;
    //float _toRotateA;
    //float _toRotateB;
    //int   _lightAction;
    //float _heightInPlant;
};
enum LightAction
{
    LightActionFadeInAndOut = 1000,
    LightActionFadeIn,
    LightActionFadeOut,
    LightActionScaleToBigAndSmall,
    LightActionScaleToBig,
    LightActionScaleToSmall,
};
struct PlantLightContext
{
public:
    PlantLightContext():_lightLength(0.0f){}
    //,_isRemoveingLight(false)
    std::map<int,LightPathContext> _lightLists;
    float                          _lightLength;
    //ContorlPointV2               _topCP;
    //float                        _heightInPlant;
    //bool                         _isRemoveingLight;
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

    CallFunc* createLightRunningInitActionEndCall(LightPathContext* conf,int plantIndex,int lightId);
    void  onLightRunningInitActionEnd(LightPathContext* conf,int plantIndex,int lightId);
    
    void  addOneLightByPlantIndex(Vec2 startPoint,int index);
    
    int   _addOneLightByPlantIndex(int index,const LightPathContext& config);
    
    void  addLightsBySubMapInit(int plantIndex,int count);
    void  addOneLightByPlantIndexUseBezier(Vec2 startPoint,int index);
    void  addLightsUseBezier(int plantIndex,Node* item,int count);
    void  addLightsUseBezier(int plantIndex,Vec2 point,int count,float waitTime);
    int   getLightCountByPlantIndex(int index);
    
    void checkUpdateToPosition(int plantIndex);
    
    Action* createInitAction(Vec2 startPoint,const LightPathContext& context );
    void onRemoveLightCall(Node* node,void* data);
    
    void removeLight(int plantIndex,int id);
    void removeLightUseUseBezier(int plantIndex,int id,Vec2 endPoint);
    void removeLightUseUseBezier(int plantIndex,int id,Vec2 endPoint,float bezierTime ,float fadeTime);
    void removeLightRandId(int plantIndex);
    void removeLightTopId(int plantIndex);
    void removeLights(int plantIndex,int count);
    void removeOneLightUseBezier(int plantIndex,Vec2 endPoint);
    void removeOneLightUseBezier(int plantIndex,Vec2 endPoint,float bezierTime ,float fadeTime );
    int  getMinLengthLightId( int plantIndex);
    
    LightPathContext* getLightContextByPlantIndexAndId(int plantIndex,int id);
    void removeLightToPlantList(int plantIndex,int id);

    void removeLightByPlantIndexAndLightId(int plantIndex,int id);
    
    void removeLightByPlantIndexAndLightIdNoUseAction(int plantIndex,int id);
    
    void removeLightRandIdByPlantIndex(int plantIndex);
    
    int randLightByPlant(int plantIndex);
    int randLightByPlantNotWillRemove(int plantIndex);
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
    
    bool LightIsRotete(const LightPathContext& context );
    
    bool LightIsFadeOutAndIn(const LightPathContext& context );
    void LightFadeOutAndIn(const LightPathContext& context );
    

    void LightStartRotate(const LightPathContext& context );
    void LightStopRotate(const LightPathContext& context );
    
    void LightChildRunAction(const LightPathContext& context,Action* action);
    
    int getLightRunActionCount(int plantIndex,LightAction act);
    

    Sprite* createOnLightChildNode();
    
    Node* getLightChildNode(const LightPathContext& context);
   
    ScaleTo*  LightActionScaleTo(float time,float scale);
    
    Action* createOneLightAction(LightAction act);
    
    void checkLightUpdateAction(int plantIndex,int count,LightAction act);
    
    void runChangeLightsAction(int loopCount,float waitTime);
    
    PlantPathList getOnePlantPathList(int plantIndex);
    LightPathContext* randCreateLightPahtContext(int plantIndex,Vec2 startPoint);
    
    bool  saveToFile();
    bool  initBySaveConfig();
    
    std::vector<PlantLightContext> _plantLights;
    
    std::list<Node*> _lightReleaseList;
    int     _plantCount;
};

#endif /* defined(__Grow_Beta_01__GameLayerLight__) */
