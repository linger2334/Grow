//
//  PhysicsHandler.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-8.
//
//

#include "PhysicsHandler.h"

PhysicsHandler::PhysicsHandler()
{
    
}

PhysicsHandler::~PhysicsHandler()
{
    
}

bool PhysicsHandler::init()
{
    b2Vec2 gravity = b2Vec2(0.0,-0.0);
    world = new b2World(gravity);
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);
    world->SetContactListener(this);
    //调试绘图
    _debugDraw = new GLESDebugDraw(PTM_RATIO);
    world->SetDebugDraw(_debugDraw);
    uint32 flags = 0;
    flags+=b2Draw::e_shapeBit;
    _debugDraw->SetFlags(flags);
    

    return true;
}

b2World* PhysicsHandler::getWorld()
{
    return world;
}

void PhysicsHandler::update(float dt)
{
    int32 velocityIterations = 8;
    int32 positionIterations = 1;
    world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body* b = world->GetBodyList(); b; b=b->GetNext()) {
        Node* item = static_cast<Node*>(b->GetUserData());
        cocos2d::Point ptInGL = item->getParent()->convertToWorldSpace(item->getPosition());
        
        b2Vec2 b2Position = b2Vec2(ptInGL.x/PTM_RATIO,ptInGL.y/PTM_RATIO);
        float32 b2Angle = -CC_DEGREES_TO_RADIANS(item->getRotation());
        
//        if(item == _plantLayer->_plant)
//        {
//            b2Angle = -CC_DEGREES_TO_RADIANS(_plantLayer->_plant->_headCur.getAngle());
//            ptInGL = _plantLayer->_plant->_headCur.getPosition();
//            float headx =ptInGL.x + WinSize.width/2 +  0.04*WinSize.height*sinf(-b2Angle);
//            float heady =_plantLayer->getPlantTopHeight()+ 0.04*WinSize.height*cosf(-b2Angle);
//            b2Position = b2Vec2(headx/PTM_RATIO,heady/PTM_RATIO);
//        }
        
        b->SetTransform(b2Position, b2Angle);
    }
    
    for(b2Body* body : _itembodies) {
        if (find(_itembodies.begin(),_itembodies.end(),body)!=_itembodies.end()) {
            Sprite* item = static_cast<Sprite*>(body->GetUserData());
            if (item&&item->getParent()->convertToWorldSpace(item->getPosition()).y<-WinSize.height/2) {
                item->removeFromParent();
                _itembodies.erase(find(_itembodies.begin(),_itembodies.end(),body));
                world->DestroyBody(body);
            }
        }
        
    }
    
}

bool PhysicsHandler::isPointContact(cocos2d::Point ptInGl)
{
    b2Vec2 ptInB2 = b2Vec2(ptInGl.x/PTM_RATIO,ptInGl.y/PTM_RATIO);
    for (b2Body* body : _itembodies) {
        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture=fixture->GetNext()) {
            if(fixture->TestPoint(ptInB2))
            {
                return true;
            }
        }
    }
    return false;
}

void PhysicsHandler::BeginContact(b2Contact* contact)
{
    MyContact myContact(contact->GetFixtureA(),contact->GetFixtureB());
    _contacts.insert(myContact);
}

void PhysicsHandler::EndContact(b2Contact* contact)
{
    MyContact myContact(contact->GetFixtureA(),contact->GetFixtureB());
    _contacts.erase(myContact);
}

void PhysicsHandler::PreSolve(b2Contact* contact,const b2Manifold* oldManifold)
{
    
}

void PhysicsHandler::PostSolve(b2Contact* contact,const b2ContactImpulse* impulse)
{
    
}


