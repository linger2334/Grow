#ifndef __Grow_Demo_test__GamePhysicalWorld__
#define __Grow_Demo_test__GamePhysicalWorld__
#include "common.h"
#include "Singleton.hpp"
#include "GLES-Render.h"
#include "PhysicsHandler.h"
class ItemModel;
//物理世界单例类  测试碰撞等操作
class GamePhysicalWorld :public Singleton<GamePhysicalWorld>
{
public:
    GamePhysicalWorld();
    ~GamePhysicalWorld();
    void  initBox2dWorld();
    void  DestoryBox2dWorld();
    
    virtual void update(float dt);
    float rayCastTest(Vec2 start,Vec2 end,std::function<bool(ItemModel*)> testCall);
    float rayCastTestByType(Vec2 start,Vec2 end,unsigned char ctype);
    bool isInStone(Vec2);
    bool isInExcavatePolygon(Vec2 point);
    
    bool isInMapDirtOrOutMap(Vec2 point);
    
    b2Body* createLineBodyByList(const std::vector<Vec2>& line,void* userdata);
    static  bool testPointInBody(b2Body* body, Vec2);
    
    CC_SYNTHESIZE(b2World*,_b2World,Box2dWorld);
    GLESDebugDraw* _debugDraw;
    PhysicsHandler*   _physicsHandler;
};

class RayCastCallback : public b2RayCastCallback
{
public:
    RayCastCallback()
    {
        _crash = false;
        _crashLen = -1;
    }
    
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
    float _crashLen;
    Vec2 _orign;
    bool _crash;
    std::function<bool(ItemModel*)> _testCall;
};

#endif /* defined(__Grow_Demo_test__GamePhysicalWorld__) */
