#include "GamePhysicalWorld.h"
#include "ItemModel.h"
#include "GameManager.h"
#include "LayerItem.h"
#include "GameLayerMap.h"
GamePhysicalWorld::GamePhysicalWorld()
{
    
    initBox2dWorld();
}

GamePhysicalWorld::~GamePhysicalWorld()
{
    DestoryBox2dWorld();
    CC_SAFE_RELEASE(_physicsHandler);
}

void GamePhysicalWorld::update(float dt)
{
    _physicsHandler->update(dt);
}
 bool GamePhysicalWorld::isInStone(Vec2 pt)
{
    return _physicsHandler->isPointContact(pt);
}
bool GamePhysicalWorld::isInMapDirtOrOutMap(Vec2 point)
{
   
}
void  GamePhysicalWorld::initBox2dWorld()
{
    b2Vec2 gravity = b2Vec2(0.0,-0.0);
    auto _b2World = new b2World(gravity);
    _physicsHandler = PhysicsHandler::create(_b2World);
    CC_SAFE_RETAIN(_physicsHandler);

    _b2World->SetAllowSleeping(false);
    _b2World->SetContinuousPhysics(true);
   // _b2World->SetContactListener(nullptr);
    _b2World->SetContactListener(_physicsHandler);
    setBox2dWorld(_b2World);
    //调试绘图
    _debugDraw = new GLESDebugDraw(PTM_RATIO);
    //调试绘图
    _b2World->SetDebugDraw(_debugDraw);
    uint32 flags = 0;
    flags+=b2Draw::e_shapeBit;
    _debugDraw->SetFlags(flags);
}

bool GamePhysicalWorld::testPointInBody(b2Body* body, Vec2 point)
{
    b2Vec2 vec(point.x/PTM_RATIO,point.y/PTM_RATIO);
    
    for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture=fixture->GetNext()) {

        if(fixture->TestPoint(vec))
        {
            return true;
        }
    }
    return false;
}
bool GamePhysicalWorld::isInExcavatePolygon(Vec2 point)
{
    auto list =  LayerItem::getRunningLayer()->getPolygons();
    for (auto& ip: list) {
        b2Vec2 vec(point.x/PTM_RATIO,point.y/PTM_RATIO);
        
        for (b2Fixture* fixture = ip->getBody()->GetFixtureList(); fixture; fixture=fixture->GetNext()) {
            if(fixture->TestPoint(vec))
            {
                return true;
            }
        }
    }
    return false;
}
void  GamePhysicalWorld::DestoryBox2dWorld()
{
    if(_b2World)
    {
        delete _b2World;
        _b2World = nullptr;
    }
    
    if(_debugDraw)
    {
        delete _debugDraw;
        _debugDraw = nullptr;
    }

}

float GamePhysicalWorld::rayCastTest(Vec2 start,Vec2 end,std::function<bool(ItemModel*)> testCall)
{
    
    float ret = -1;
    RayCastCallback call;
    call._orign = start;
    call._testCall = testCall;
    _b2World->RayCast(&call, b2Vec2(start.x/PTM_RATIO,start.y/PTM_RATIO), b2Vec2(end.x/PTM_RATIO,end.y/PTM_RATIO));
    if (call._crash) {
        ret = call._crashLen;
    }
    return ret;
}
#include "ItemModel.h"
float GamePhysicalWorld::rayCastTestByType(Vec2 start,Vec2 end,unsigned char ctype)
{
    auto call = [=](ItemModel* item)->bool{
        return item->getType()== ctype;
    };
    return rayCastTest(start,end,call);
}

b2Body*  GamePhysicalWorld::createLineBodyByList(const std::vector<Vec2>& line,void* userdata)
{
    if (line.size()<2) {
        return nullptr;
    }
//    static ItemModel model;
//    model.setType(97);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(0,0);

    bodyDef.userData = userdata;
    
    b2Body* body = nullptr;
    int index =0 ;
    float count = 0;
    b2Vec2* verts = new b2Vec2[line.size()];
    memset(verts,0,sizeof(line.size()*sizeof(b2Vec2)));
    Vec2 preV = line.front();
    for (auto i : line) {
  
        auto v = b2Vec2(i.x/PTM_RATIO,i.y/PTM_RATIO);
        if(index > 0 && ((int)v.x == (int)verts[index-1].x)&&
           ((int)v.y == (int)verts[index-1].y))
        {
            count++;
            continue;
        }
        
        verts[index] =v;
        index++;
        count = 0;
    }
    
    if(index >= 2)
    {
       body = GamePhysicalWorld::getInstance()->getBox2dWorld()->CreateBody(&bodyDef);
        b2FixtureDef fixtureDef;
        

        b2ChainShape b2line;
        b2line.CreateChain(verts,index);
        
        fixtureDef.shape = &b2line;
        fixtureDef.filter.groupIndex = -1;
        body->CreateFixture(&fixtureDef);
    }
    delete [] verts;
    return body;
}
float32 RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
    auto type = static_cast<ItemModel*>(fixture->GetBody()->GetUserData());
    if(type == nullptr)
    {
        
    }
    if (_testCall(type)) {
        
        Vec2 crashPoint(point.x,point.y);
        float len  = _orign.getDistance(Vec2(crashPoint.x*PTM_RATIO,crashPoint.y*PTM_RATIO));
        if (len < _crashLen ||_crashLen < 0) {
            _crashLen = len;
        }
      
        _crash = true;
    }
    return  -1;
}

