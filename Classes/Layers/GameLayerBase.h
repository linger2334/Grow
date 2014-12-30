#ifndef __Grow_Demo_test__GameLayerBase__
#define __Grow_Demo_test__GameLayerBase__
#include "common.h"
#include <typeinfo>
#include "GameRunningInfo.h"

class GameLayerBase : public  Node
{
public:
    virtual ~GameLayerBase()
    {
    }
    virtual bool  init();
    virtual bool  initGameInfo(){return true;}
    virtual bool  releaseGameInfo(){return true;}
    virtual void  moveDown(float y) = 0;
    virtual void  updateConfigs(){}
    virtual bool  onTouchBegan(Touch*,Event*){return false;}
    virtual void  onTouchMoved(Touch*,Event*){}
    virtual void  onTouchEnded(Touch*,Event*){}
    virtual void  onTouchCancelled(Touch*,Event*){}
    
    virtual bool  saveToFile(){return true;}
    virtual bool  initBySaveConfig(){return true;}
    
    virtual const std::type_info& getType();
    virtual const std::string getTypeName();
    bool         _isHandle;
};

template<class _Type>
class GameLayerHelper
{
public:
    static void removeRunningLayer()
    {
        GameRunningInfo::getInstance()->removeGameLayer(typeid(_Type).name());
    }
    static _Type* getRunningLayer()
    {
     return static_cast<_Type*>(GameRunningInfo::getInstance()->getGameLayer(typeid(_Type).name()));
    }
   static bool        isHasRunningLayer()
    {
        return  GameRunningInfo::getInstance()->isHasGameLayer(typeid(_Type).name());
    }
};

#endif /* defined(__Grow_Demo_test__GameLayerBase__) */
