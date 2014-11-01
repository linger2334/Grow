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
#include "DoubleDragon.h"

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
    
    this->dealCollisions();
}

bool PhysicsHandler::isPointContact(cocos2d::Vec2 ptInGl)
{
    b2Vec2 ptInB2 = b2Vec2(ptInGl.x/PTM_RATIO,ptInGl.y/PTM_RATIO);
    std::list<ItemModel*> items = GameManager::getInstance()->_layerItem->getItems();
    
    for(ItemModel* item : items){
        for(b2Fixture* fixture = item->getBody()->GetFixtureList();fixture;fixture = fixture->GetNext()){
            if(fixture->TestPoint(ptInB2)) return true;
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
    ItemModel* item;
    ItemModel* plantHead;
    for (it = _contacts.begin(); it!= _contacts.end(); it++) {
        item = (ItemModel*)(it->first->GetBody()->GetUserData());
        plantHead = (ItemModel*)it->second->GetBody()->GetUserData();
        //
        bool exchange = false;
        if (!item->isNeedCallBackType()) {
            plantHead = item;
            item = (ItemModel*)it->second->GetBody()->GetUserData();
            exchange = true;
        }
        //
        if (item->_type == DoubDragon_Anti || item->_type == DoubDragon_Clockwise) {
            if (!exchange) {
                if(it->first->GetDensity() == 1.0){
                    ((DoubleDragon*)item)->setCollisionSign(1);
                }else{
                    ((DoubleDragon*)item)->setCollisionSign(-1);
                }
            }else{
                if (it->second->GetDensity() == 1.0) {
                    ((DoubleDragon*)item)->setCollisionSign(1);
                }else{
                    ((DoubleDragon*)item)->setCollisionSign(-1);
                }
            }
        }
        
        
        _toDealWith.insert(item);
    }
    
    for (ItemModel* itemModel : _toDealWith) {
        itemModel->_collisionCallBack(plantHead);
    }
    
}


