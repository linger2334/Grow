//
//  PhysicsHandler.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-8.
//
//

#include "PhysicsHandler.h"
#include "GameManager.h"
#include "TypeBase.h"
#include "ItemModel.h"
#include "SceneGame.h"

PhysicsHandler::PhysicsHandler():_world(nullptr)
{
    
}

PhysicsHandler::~PhysicsHandler()
{

}

PhysicsHandler* PhysicsHandler::create(b2World* world)
{
    PhysicsHandler *pRet = new PhysicsHandler();
    if (pRet && pRet->init(world))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool PhysicsHandler::init(b2World* world)
{
    _world = world;
    return true;
}

void PhysicsHandler::update(float dt)
{
    int32 velocityIterations = 1;
    int32 positionIterations = 2;
    _world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body* b : _itembodies) {
        Node* item = static_cast<Node*>(b->GetUserData());
        cocos2d::Point ptInGL = item->getParent()->convertToWorldSpace(item->getPosition());
        
        b2Vec2 b2Position = b2Vec2(ptInGL.x/PTM_RATIO,ptInGL.y/PTM_RATIO);
        float32 b2Angle = -CC_DEGREES_TO_RADIANS(item->getRotation());
        
        if(item == GameManager::getInstance()->_sceneGame->_plantLayer->_plant)
        {
            b2Angle = -CC_DEGREES_TO_RADIANS(GameManager::getInstance()->_sceneGame->_plantLayer->_plant->_headCur.getAngle());
            ptInGL = GameManager::getInstance()->_sceneGame->_plantLayer->_plant->_headCur.getPosition();
            float headx =ptInGL.x + WinSize.width/2 +  0.02*WinSize.height*sinf(-b2Angle);
            float heady =GameManager::getInstance()->_sceneGame->_plantLayer->getPlantTopHeight()+ 0.02*WinSize.height*cosf(-b2Angle);
            b2Position = b2Vec2(headx/PTM_RATIO,heady/PTM_RATIO);
        }
        
        b->SetTransform(b2Position, b2Angle);
    }
    
    this->dealCollisions();
    
    for(b2Body* body : _itembodies) {
        if (find(_itembodies.begin(),_itembodies.end(),body)!=_itembodies.end()) {
            Sprite* item = static_cast<Sprite*>(body->GetUserData());
            if (item&&item->getParent()->convertToWorldSpace(item->getPosition()).y<-WinSize.height/2) {
                item->removeFromParent();
                _itembodies.erase(find(_itembodies.begin(),_itembodies.end(),body));
                _world->DestroyBody(body);
            }
        }
        
    }
}

bool PhysicsHandler::isPointContact(cocos2d::Vec2 ptInGl)
{
    b2Vec2 ptInB2 = b2Vec2(ptInGl.x/PTM_RATIO,ptInGl.y/PTM_RATIO);
    for (b2Body* body : _itembodies) {
        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture=fixture->GetNext()) {
            if(fixture->TestPoint(ptInB2))
            {
//                ItemModel* item = static_cast<ItemModel*>(fixture->GetBody()->GetUserData());
//                if (item->isStone()) {
//                    return true;
//                }
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

void PhysicsHandler::dealCollisions()
{
    std::set<ItemModel*> _toDealWith;
    
    std::set<MyContact>::iterator it;
    for (it = _contacts.begin(); it!= _contacts.end(); it++) {
        ItemModel* concrete = (ItemModel*)(it->first->GetBody()->GetUserData());
        concrete = concrete->isNeedCallBackType() ? concrete : (ItemModel*)(it->second->GetBody()->GetUserData());
//          Node* item = static_cast<Node*>(it->first->GetBody()->GetUserData());
//        if(item == GameManager::getInstance()->_sceneGame->_plantLayer->_plant)
//        {
//            item = static_cast<Node*>(it->second->GetBody()->GetUserData());
//        }
        _toDealWith.insert(concrete);
    }
    
    for (ItemModel* itemModel : _toDealWith) {
        itemModel->_collisionCallBack();
    }
    
}


