#ifndef Grow_Goods_test_LayerPlantBase_h
#define Grow_Goods_test_LayerPlantBase_h
#include "common.h"
#include "PlantBase.h"
#include "Box2D/Box2D.h"
#include "ItemModel.h"
#include "TypeDef.h"
class GameManager;
class MapGrid;
class LayerPlantBase : public GameLayerRollY
{
public:
    bool init();
    virtual float getPlantTopHeightInPlant(){}
    virtual float getPlantTopHeightInView(){}
    //virtual float getPlantCount();
   // virtual Node* getPlantNodeByIndex();
    //virtual CPLineNode* getCPLineNodeByIndex();
    
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

        return -1;
    }
    float _crashLen;
    Vec2 _orign;
    bool _crash;
    char _crashType;
};
class RayCastStoneCallback : public b2RayCastCallback
{
public:
    RayCastStoneCallback()
    {
        _crash = false;
    }
    
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        auto type = static_cast<ItemModel*>(fixture->GetBody()->GetUserData());
        if (type->_type ==101) {
            _crash = true;
        }
        return -1;
    }
    
    bool _crash;
};

class RayCastMapGridCallback : public b2RayCastCallback
{
public:
    RayCastMapGridCallback()
    {
        _crash = false;
    }
    
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        auto type = static_cast<ItemModel*>(fixture->GetBody()->GetUserData());
        if (type->_type ==TypeDirtLine) {
            _crash = true;
        }
        return -1;
    }
   
    bool _crash;
};
class GrowContext
{
public:
    GrowContext():_left(false),_right(false),_top(false){}
    GrowContext(bool isCrash):_left(isCrash),_right(isCrash),_top(isCrash){}
    GrowContext(bool left,bool right,bool top):_left(left),_right(right),_top(top){}
    bool isCanGrowByDir(FaceDirection dir)
    {
        bool ret = false;
        switch(dir)
        {
            case FaceLeft:
                ret  = _left;
                break;
            case FaceRight:
                ret  = _right;
                break;
            case FaceTop:
                ret  = _top;
                break;
            default:
                assert(0);
                break;
        };
        return ret;
    }
    bool _left,_right,_top;
};
#endif
