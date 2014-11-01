#ifndef Grow_Goods_test_LayerPlantBase_h
#define Grow_Goods_test_LayerPlantBase_h
#include "common.h"
#include "PlantBase.h"
#include "Box2D/Box2D.h"
#include "ItemModel.h"
class GameManager;
class MapGrid;
class LayerPlantBase : public GameLayerRollY
{
public:
    bool init();
    virtual float getPlantTopHeightInPlant(){}
    virtual float getPlantTopHeightInView(){}
    virtual void  setGrowSpeed(float  speed)
    {
        if(speed<0)return;
        _growSpeed =speed;
    }
    virtual float getGrowSpeed()
    {
        return _growSpeed;
    }
    float     _growSpeed;
    virtual bool isMapGridCrash(Vec2 pt,bool outRetCrash = true);
    virtual bool isStoneCrash(Vec2 pt);
    std::function<int(cocos2d::Vec2)>  _mapCrashTestCallBack;
    GameManager*            _gameManager;
    MapGrid*                _mapGrid;
};
class RayCastCallback : public b2RayCastCallback
{
public:
    RayCastCallback()
    {
        _crash = false;
        _crashLen = 1024;
        _crashType = 0;
    }
    
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        auto type = static_cast<ItemModel*>(fixture->GetBody()->GetUserData());
        if (type->isMustCrashType()) {
            
            Vec2 crashPoint(point.x,point.y);
            float len  = _orign.getDistance(Vec2(crashPoint.x*PTM_RATIO,crashPoint.y*PTM_RATIO));
            if (len < _crashLen) {
                _crashLen = len;
            }

            _crash = true;
      
        }
        else
        {
            int n = 9;
        }
        return -1;
    }
    float _crashLen;
    Vec2 _orign;
    bool _crash;
    char _crashType;
};

#endif
