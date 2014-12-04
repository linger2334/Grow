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
#include "DoubleDragon.h"
#include "LayerItem.h"
#include "Flame.h"
#include "Cicada.h"
#include "Dragon.h"
#include "DoubleDragon.h"
#include "Serpent.h"
#include "GearButton.h"
#include "GearGate.h"
#include "Barrier.h"
#include "StatisticsData.h"
#include "GameLayerPlant.h"
#include "GameLayerLight.h"

Spawn* createRemoveAction();

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
    std::list<ItemModel*> rocks = LayerItem::getRunningLayer()->getRocks();
    for(ItemModel* rock : rocks){
        for(b2Fixture* fixture = rock->getBody()->GetFixtureList();fixture;fixture = fixture->GetNext()){
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
    std::map<ItemModel*,ItemModel*> _toDealWith;
    
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
        
        _toDealWith.insert(std::make_pair(item, plantHead));
    }
    
    for (std::map<ItemModel*,ItemModel*>::iterator it = _toDealWith.begin();it!=_toDealWith.end();it++) {
        CollisionCallBack(it->first, it->second);
    }
    
}

void PhysicsHandler::CollisionCallBack(ItemModel *item, ItemModel *plantHead)
{
    Item_Type type = static_cast<Item_Type>(item->_type);
    if (type==Flame_White || type==Flame_Blue || type==Flame_Violet || type==Flame_Orange) {
        CollideWithFlame(item,plantHead);
    }else if (type==Cicada){
        CollideWithCicada(item, plantHead);
    }else if (type==Dragon_Anti || type==Dragon_Clockwise){
        CollideWithDragon(item, plantHead);
    }else if (type==DoubDragon_Anti || type== DoubDragon_Clockwise){
        CollideWithDoubDragon(item, plantHead);
    }else if (type==Serpent_){
        CollideWithSerpent(item, plantHead);
    }else if (type==Gear_Button){
        CollideWithGearButton(item, plantHead);
    }else if (type==Gear_Gate){
        CollideWithGearGate(item, plantHead);
    }else if (type==Barrier_){
        CollideWithBarrier(item, plantHead);
    }else{
        CCASSERT(0==1, "unknown type!");
    }
    
}

////光芒
void PhysicsHandler::CollideWithFlame(ItemModel *item, ItemModel *plantHead)
{
    Flame* flame = dynamic_cast<Flame*>(item);
    _world->DestroyBody(flame->getBody());
    flame->setBody(nullptr);
    
    flame->stopAllActions();
    /////other effect
    //auto fadeout = FadeTo::create(1, 0);
    auto scaleTo = ScaleTo::create(0.5, 0);
    auto call = CallFuncN::create([=](Node* node)
                                  {
                                      int type =dynamic_cast<Flame*>(node)->getType();
                                      int count =0;
                                      switch (type) {
                                          case Flame_White:
                                              count = 1;
                                              break;
                                          case Flame_Blue:
                                              count = 5;
                                              break;
                                          case Flame_Orange:
                                              count = 10;
                                              break;
                                          case Flame_Violet:
                                              count = 7;
                                              break;
                                              
                                          default:
                                              break;
                                      }
                                      flame->updateFlameCollectedCount();
                                      int index = GameLayerPlant::getRunningLayer()->getPlantIndexByPlantHeadBodyType(plantHead->getType());
                                      GameLayerLight::getRunningLayer()->addLightsUseBezier(index, node, count);
                                  });
    Sequence* seq = Sequence::create(call,nullptr);
    flame->runAction(seq);
}

///蝉
void PhysicsHandler::CollideWithCicada(ItemModel *item, ItemModel *plantHead)
{
    class Cicada* cicada = dynamic_cast<class Cicada*>(item);
    _world->DestroyBody(cicada->getBody());
    cicada->setBody(nullptr);
    
    cicada->stopAllActions();
    ActionInterval* disappear = FadeTo::create(1, 0);
    ActionInstant* remove = RemoveSelf::create();
    //正常状态，吸光
    if (cicada->getStatus() == ItemStatus::NormalStatus) {
        StatisticsData::getRunningLayer()->failures++;
        /////other effect
        int index = GameLayerPlant::getRunningLayer()->getPlantIndexByPlantHeadBodyType(plantHead->getType());
        GameRunningManager::getInstance()->reGrowPlantByIndex(index);
        GameRunningManager::getInstance()->removeSubLight(index);
    }else{
        //反相状态，加光

    }
    
    cicada->runAction(Sequence::create(disappear,remove, NULL));
}

////单龙
void PhysicsHandler::CollideWithDragon(ItemModel *item, ItemModel *plantHead)
{
    Dragon* dragon = dynamic_cast<Dragon*>(item);
    _world->DestroyBody(dragon->getBody());
    dragon->setBody(nullptr);
    
    dragon->stopAllActions();
    //other effect
    ActionInterval* disappear = FadeOut::create(1);
    ActionInstant* remove = RemoveSelf::create();
    //正常状态，吸光
    if(dragon->getStatus() == ItemStatus::NormalStatus){
        StatisticsData::getRunningLayer()->failures++;
        
        dragon->runAction(Sequence::create(createRemoveAction(),remove,NULL));
    }else{
        //反相状态，加光
        
    }
    
    dragon->runAction(Sequence::create(disappear,remove,NULL));
}

///双龙
void PhysicsHandler::CollideWithDoubDragon(ItemModel *item, ItemModel *plantHead)
{
    DoubleDragon* doubDragon = dynamic_cast<DoubleDragon*>(item);
    _world->DestroyBody(doubDragon->getBody());
    doubDragon->setBody(nullptr);
    
    doubDragon->stopAllActions();
    ActionInterval* disappear = FadeOut::create(1);
    ActionInstant* remove = RemoveSelf::create();
    //other effect
    int index = GameLayerPlant::getRunningLayer()->getPlantIndexByPlantHeadBodyType(plantHead->getType());
    if (doubDragon->getCollisionSign()* (int)doubDragon->getStatus() == 1) {
        GameRunningManager::getInstance()->addLightByFlame(doubDragon->getParent()->convertToWorldSpace(doubDragon->getPosition()), 5,index);
    }else{
        StatisticsData::getRunningLayer()->failures++;
        
        GameRunningManager::getInstance()->reGrowPlantByIndex(index);
        GameRunningManager::getInstance()->removeSubLight(index);
    }
    
    doubDragon->runAction(Sequence::create(disappear,remove, NULL));
}

///蛇
void PhysicsHandler::CollideWithSerpent(ItemModel *item, ItemModel *plantHead)
{
    Serpent* serpent = dynamic_cast<Serpent*>(item);
    _world->DestroyBody(serpent->getBody());
    serpent->setBody(nullptr);
    
    ActionInterval* disappear = FadeOut::create(1);
    ActionInstant* remove = RemoveSelf::create();
    
    int index = GameLayerPlant::getRunningLayer()->getPlantIndexByPlantHeadBodyType(plantHead->getType());
    //正常状态
    if (serpent->getStatus() == ItemStatus::NormalStatus) {
        StatisticsData::getRunningLayer()->failures++;
        ///other effect
        GameRunningManager::getInstance()->addLightByFlame(serpent->getParent()->convertToWorldSpace(serpent->getPosition()), 10,index);
        
    }else{
        //反相状态
        
    }
    serpent->runAction(Sequence::create(createRemoveAction(),remove, NULL));
    
}

////机关按钮
void PhysicsHandler::CollideWithGearButton(ItemModel *item, ItemModel *plantHead)
{
    GearButton* gearButton = dynamic_cast<GearButton*>(item);
    if (_world) {
        _world->DestroyBody(gearButton->getBody());
        gearButton->setBody(nullptr);
    }
    
    CallFuncN* openBindGate = CallFuncN::create([&](Node* sender){
        GearButton* gearButton = dynamic_cast<GearButton*>(sender);
        if(gearButton->bindID != kDefaultGearButtonBindID){
            GearGate* bindGate = dynamic_cast<GearGate*>(((LayerItem*)(gearButton->getParent()->getUserObject()))->getChildByTag(gearButton->bindID));
            if (bindGate) {
                bindGate->openGate();
            }
        }
    });
    gearButton->runAction(Sequence::create(gearButton->_sink,openBindGate, NULL));
    //other effect
    
}

////机关门
void PhysicsHandler::CollideWithGearGate(ItemModel *item, ItemModel *plantHead)
{
    GearGate* gearGate = dynamic_cast<GearGate*>(item);
    _world->DestroyBody(gearGate->getBody());
    gearGate->setBody(nullptr);
    StatisticsData::getRunningLayer()->failures++;
    
    FadeTo* disappear = FadeTo::create(1, 0);
    TargetedAction* leftDisappear = TargetedAction::create(gearGate->_left, disappear);
    TargetedAction* rightDisappear = TargetedAction::create(gearGate->_right, disappear->clone());
    CallFuncN* remove = CallFuncN::create([&](Node* sender){
        GearGate* gearGate = dynamic_cast<GearGate*>(sender);
        gearGate->_left->removeFromParent();
        gearGate->_left = nullptr;
        gearGate->_right->removeFromParent();
        gearGate->_right = nullptr;
        gearGate->removeFromParent();
    });
    gearGate->runAction(Sequence::create(Spawn::create(leftDisappear,rightDisappear,NULL),remove, NULL));
    //other effect
    int index = GameLayerPlant::getRunningLayer()->getPlantIndexByPlantHeadBodyType(plantHead->getType());
    GameRunningManager::getInstance()->reGrowPlantByIndex(index);
    GameRunningManager::getInstance()->removeSubLight(item->getRemovePositionInWorld(),index);
    
}

////伞（障碍物）
void PhysicsHandler::CollideWithBarrier(ItemModel *item, ItemModel *plantHead)
{
    Barrier* barrier = dynamic_cast<Barrier*>(item);
    _world->DestroyBody(barrier->getBody());
    barrier->setBody(nullptr);
    
    FadeTo* disappear = FadeTo::create(1,0);
    ActionInstant* remove = RemoveSelf::create();
    
    int index = GameLayerPlant::getRunningLayer()->getPlantIndexByPlantHeadBodyType(plantHead->getType());
    //正常状态，吸光
    if(barrier->getStatus() == ItemStatus::NormalStatus){
        StatisticsData::getRunningLayer()->failures++;
        
        //other effect
        GameRunningManager::getInstance()->reGrowPlantByIndex(index);
        GameRunningManager::getInstance()->removeSubLight(item->getRemovePositionInWorld(),index);
    }else{
        //反相状态，加光
        
    }
    
    
    barrier->runAction(Sequence::create(createRemoveAction(),remove, NULL));
}

bool PhysicsHandler::ReportFixture(b2Fixture *fixture)
{
    b2Body* itemBody = fixture->GetBody();
    ItemModel* item = (ItemModel*)(itemBody->GetUserData());
    if (item->getStatus() != ItemStatus::InvertStatus) {
        item->setStatus(ItemStatus::InvertStatus);
    }
    
    return true;
}

Spawn* createRemoveAction()
{
    ScaleTo* sacleTo = ScaleTo::create(1.5,1.2);
    FadeTo* fadeTo = FadeTo::create(1.5,0);
    return Spawn::create(sacleTo,fadeTo, NULL);
}

Action* createRemoveLightsAction()
{
    
}

