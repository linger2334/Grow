#ifndef __Grow_Demo_test__GameLayerPlantBase__
#define __Grow_Demo_test__GameLayerPlantBase__
#include "common.h"
#include "GameLayerBase.h"
#include "ItemModel.h"
class PlantNode;

class PlantAdormContext
{
public:
    Node*  _adorm;
    bool   _isUnderPlant;
};
typedef std::vector<PlantAdormContext> PlantAdormList;
class GameLayerPlant : public GameLayerBase
{
public:
    static GameLayerPlant* getRunningLayer()
    {
        return static_cast<GameLayerPlant*>(GameRunningInfo::getInstance()->getGameLayer(GameLayerPlant::_sRunningGamePlantName));
    }
    static bool        isHasRunningLayer()
    {
        return  GameRunningInfo::getInstance()->isHasGameLayer(GameLayerPlant::_sRunningGamePlantName);
    }
    enum
    {
        PlantStateStopGrow,
        PlantStateGrowIng,
        PlantStateReGrowIng
    };
    class PlantInfo
    {
    public:
        PlantInfo():_growSpeed(0.0f),_reGrowSpeed(0.0f),_reGrowNeedLength(0.0f),_state(0),_plant(nullptr),_headBody(nullptr),_headItem(nullptr){}
       
        float _growSpeed;
        float _reGrowSpeed;
        float _reGrowNeedLength;
        int  _state;
        ItemModel* _headItem;
        b2Body*   _headBody;
        PlantNode* _plant;
    };
    virtual ~GameLayerPlant()
    {
//        removeHeadBodys();
    }
    virtual bool    init();
//    virtual const   std::type_info& getType();
    virtual bool    releaseGameInfo();
    virtual void    moveDown(float yLen);
    virtual void    update(float);
    virtual void    onEnter();
    virtual void    onExit();
    virtual float   getPlantMaxTopHeightInView();
    virtual float   getPlantMinTopHeightInView();
    virtual int     getPlantCount() ;
    
    virtual PlantNode* getPlantNodeByIndex(int index) ;
    
    virtual float   getGrowSpeedByIndex(int index);
    virtual void    setGrowSpeedByIndex(float speed,int index);
    
    virtual float   getReGrowSpeedByIndex(int index);
    virtual void    setReGrowSpeedByIndex(float speed,int index);
    
    virtual float   growByIndex(float length,int index);
    virtual float   reGrowByIndex(float length,int index);
    
    virtual bool    isReGrowByIndex(int index);
    
    virtual int     getPlantStateByIndex(int index);
    virtual void    setPlantStateByIndex(int state,int index);

    virtual PlantInfo& getPlantInfoByIndex(int index);
    virtual int     addOnePlant();
    virtual void    changePlantHeightByIndex(float height , int index);
    virtual void    changePlantCPPositionYByIndex(float moveLen , int index);
    
    virtual int     getPlantAdormsInHeightRangeByIndex(int index,float start, float end,PlantAdormList* list = nullptr);
    virtual void    createHeadB2Body(int index);
    virtual void    updateHeadB2Body(int index);
    
    virtual void    removeHeadBodys();
    std::vector<PlantInfo> _plants;
    
    static  std::string _sRunningGamePlantName;
    int                 _musicId;
};



#endif /* defined(__Grow_Demo_test__GameLayerPlantBase__) */
