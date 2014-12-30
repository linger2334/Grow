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
#include "DoubleDragon.h"
#include "Serpent.h"
#include "GearButton.h"
#include "GearGate.h"
#include "GearReversal.h"
#include "Barrier.h"
#include "Decoration.h"
#include "Sprouts.h"
#include "TriggerNode.h"
#include "StatisticsData.h"
#include "GameLayerPlant.h"
#include "GameLayerLight.h"

Spawn* createRemoveAction();
int    getPlantIndex(ItemModel* plantItem);
void doBadCrash(ItemModel* plantHead);
void changeParentToLayerItem(ItemModel* item);
#define AddLightNumberGoodsBlue  3

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
    }else if (type==DoubDragon_Anti || type== DoubDragon_Clockwise){
        CollideWithDoubDragon(item, plantHead);
    }else if (type==Serpent_){
        CollideWithSerpent(item, plantHead);
    }else if (type==Gear_Button){
        CollideWithGearButton(item, plantHead);
    }else if (type==Gear_Gate){
        CollideWithGearGate(item, plantHead);
    }else if (type==Gear_Reversal){
        CollideWithGearReversal(item, plantHead);
    }else if (type==Barrier_){
        CollideWithBarrier(item, plantHead);
    }else if (type==Decoration_Flower || type==Decoration_FlowerInv){
        CollideWithDecorationFlower(item, plantHead);
    }else if (type==Trigger){
        CollideWithTriggerNode(item,plantHead);
    }else{
        CCASSERT(nullptr, "unknown type!");
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
//    auto scaleTo = ScaleTo::create(0.5, 0);
    auto call = CallFuncN::create([=](Node* node)
                                  {
                                      int type =dynamic_cast<Flame*>(node)->getType();
                                      int count =0;
                                      switch (type) {
                                          case Flame_White:
                                              count = 1;
                                              break;
                                          case Flame_Blue:
                                              count = 3;
                                              break;
                                          case Flame_Orange:
                                              count = 7;
                                              break;
                                          case Flame_Violet:
                                              count = 5;
                                              break;
                                              
                                          default:
                                              break;
                                      }
                                      changeParentToLayerItem(flame);
                                      flame->setZOrder(99999);
                                      flame->updateFlameCollectedCount();
                                      int index = GameLayerPlant::getRunningLayer()->getPlantIndexByPlantHeadBodyType(plantHead->getType());
                                      GameLayerLight::getRunningLayer()->addLightsUseBezier(index, node, count);
                                      
                                  });
    //  Sequence* seq = Sequence::create(scaleTo,call,nullptr);
    flame->runAction(call);
}

///蝉
void PhysicsHandler::CollideWithCicada(ItemModel *item, ItemModel *plantHead)
{
    class Cicada* cicada = dynamic_cast<class Cicada*>(item);
    _world->DestroyBody(cicada->getBody());
    cicada->setBody(nullptr);
    
    cicada->stopAllActions();
    // ActionInterval* disappear = FadeTo::create(1, 0);
    ActionInstant* remove = RemoveSelf::create();
    //正常状态，吸光
    if (cicada->getStatus() == ItemStatus::NormalStatus) {
        StatisticsData::getRunningLayer()->failures++;
        /////other effect
        doBadCrash(plantHead);
    }else{
        //反相状态，加光
        GameRunningManager::getInstance()->addLightByFlame(cicada->getParent()->convertToWorldSpace(cicada->getPosition()), 1);
        
    }
    
    cicada->runAction(Sequence::create(createRemoveAction(),remove, NULL));
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
    if (doubDragon->getCollisionSign() == 1) {
        GameRunningManager::getInstance()->addLightByFlame(doubDragon->getParent()->convertToWorldSpace(doubDragon->getPosition()), 3,index);
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
    
//    ActionInterval* disappear = FadeOut::create(1);
//    ActionInstant* remove = RemoveSelf::create();
//    
//    int index = GameLayerPlant::getRunningLayer()->getPlantIndexByPlantHeadBodyType(plantHead->getType());
//    //正常状态
//    if (serpent->getStatus() == ItemStatus::NormalStatus) {
//        StatisticsData::getRunningLayer()->failures++;
//        ///other effect
//        GameRunningManager::getInstance()->addLightByFlame(serpent->getParent()->convertToWorldSpace(serpent->getPosition()), 10,index);
//        
//    }else{
//        //反相状态
//        
//    }
//    serpent->runAction(Sequence::create(createRemoveAction(),remove, NULL));
    
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
    
    //other effect
    auto parent = gearGate->_leftEye->getParent();
    Vec2 ptLeft = parent->convertToWorldSpace(gearGate->_leftEye->getPosition());
    auto parent1 = gearGate->_rightEye->getParent();
    Vec2 ptRight = parent1->convertToWorldSpace(gearGate->_rightEye->getPosition());
    
    int size = GameLayerLight::getRunningLayer()->getLightCountByPlantIndex(0);
    if (size>0)
    {
        int subSize;
        if (size != 1)subSize =  size/2;
        else subSize = size;
        float delayTime = 0;
        if (subSize > 0) {
            delayTime = 2;
            for (int i = 0 ; i< subSize; i++) {
                if(i%2 ==0)
                {
                    GameLayerLight::getRunningLayer()->removeOneLightUseBezier(0, ptLeft);
                }
                else{
                    GameLayerLight::getRunningLayer()->removeOneLightUseBezier(0, ptRight);
                }
            }
        }
    }
    
    int index = getPlantIndex(plantHead);
    GameRunningManager::getInstance()->setPlantWaiting(index, true);
    gearGate->addWaitOpenGatePlant(index);
    
    auto callfunc = [](GearGate* node)
    {
        node->openGate();
    };
    
    Sequence* seqa = Sequence::create(DelayTime::create(2),CallFunc::create(std::bind(callfunc,gearGate)), NULL);
    gearGate->runAction(seqa);
    
}

/////机关反相道具
void PhysicsHandler::CollideWithGearReversal(ItemModel *item, ItemModel *plantHead)
{
    GearReversal* gearReversal = dynamic_cast<GearReversal*>(item);
    _world->DestroyBody(gearReversal->getBody());
    gearReversal->setBody(nullptr);
    LayerItem::getRunningLayer()->invertStatusInCurrentScreen();
    //
    
}

////伞（障碍物）
void PhysicsHandler::CollideWithBarrier(ItemModel *item, ItemModel *plantHead)
{
    Barrier* barrier = dynamic_cast<Barrier*>(item);
    _world->DestroyBody(barrier->getBody());
    barrier->setBody(nullptr);
    
    //正常状态，吸光
    if(barrier->getStatus() == ItemStatus::NormalStatus){
        StatisticsData::getRunningLayer()->failures++;
        doBadCrash(plantHead);
        //other effect
        
    }else{
        //反相状态，加光
        GameRunningManager::getInstance()->addLightByFlame(barrier->getParent()->convertToWorldSpace(barrier->getPosition()), 3);
    }
    ActionInstant* remove = RemoveSelf::create();
    barrier->runAction(Sequence::create(createRemoveAction(),remove, NULL));
}

////////花的解锁点
void PhysicsHandler::CollideWithDecorationFlower(ItemModel *item, ItemModel *plantHead)
{
    Decoration* flower = dynamic_cast<Decoration*>(item);
    _world->DestroyBody(flower->getBody());
    flower->setBody(nullptr);
    //
    StatisticsData::getRunningLayer()->checkNewFlowerUnlock(flower->getFlowerID());
    
    Animation* bloomAnimation = Animation::create();
    for (int i = 1; i<12; i++) {
        bloomAnimation->addSpriteFrameWithFile(StringUtils::format("bloom_%d.png",i));
    }
    bloomAnimation->setDelayPerUnit(1.0/12);
    Animate* bloom = Animate::create(bloomAnimation);
    flower->runAction(bloom);
    
}

/////动画触发器
void PhysicsHandler::CollideWithTriggerNode(ItemModel *item, ItemModel *plantHead)
{
    TriggerNode* trigger = dynamic_cast<TriggerNode*>(item);
    _world->DestroyBody(trigger->getBody());
    trigger->setBody(nullptr);
    //
    for (int ID : trigger->getBindIDs()) {
        ItemModel* item = dynamic_cast<ItemModel*>(LayerItem::getRunningLayer()->getChildByTag(ID));
        if (item) {
            switch (item->_type) {
                case Cicada:
                    item->setTriggerSwitchState(true);
                    break;
                case Sprouts_Dextro:
                    ((Sprouts*)item)->runSproutingAnimate();
                    break;
                case Sprouts_Levo:
                    ((Sprouts*)item)->runSproutingAnimate();
                    break;
                case Sprouts_Slope:
                    ((Sprouts*)item)->runSproutingAnimate();
                    break;
                default:
                    break;
            }
            
            if (item->_type>=Rock_Circle && item->_type<=Rock_Trape) {
                item->scheduleUpdate();
            }
            
        }
        
    }
    
}




bool PhysicsHandler::ReportFixture(b2Fixture *fixture)
{
    b2Body* itemBody = fixture->GetBody();
    ItemModel* item = (ItemModel*)(itemBody->GetUserData());
    if (item->canSwitchStatus()) {
        switch (item->_type) {
            case Cicada:
                ((class::Cicada*)item)->switchItemStatus();
                break;
            case Serpent_:
                ((Serpent*)item)->switchItemStatus();
                break;
            case Barrier_:
                ((Barrier*)item)->switchItemStatus();
                break;
                
            default:
                break;
        }
    }
    
    return true;
}

Spawn* createRemoveAction()
{
    ScaleTo* sacleTo = ScaleTo::create(1.5,1.2);
    FadeTo* fadeTo = FadeTo::create(1.5,0);
    return Spawn::create(sacleTo,fadeTo, NULL);
}
int   getPlantIndex(ItemModel* plantItem)
{
    return GameLayerPlant::getRunningLayer()->getPlantIndexByPlantHeadBodyType(plantItem->getType());
}
void doBadCrash(ItemModel* plantHead)
{
    int index = getPlantIndex(plantHead);
    GameRunningManager::getInstance()->reGrowPlantByIndex(index);
    GameRunningManager::getInstance()->removeSubLight(index);
}
void changeParentToLayerItem(ItemModel* item)
{
    item->retain();
    item->removeFromParent();
    LayerItem::getRunningLayer()->addChild(item);
    item->release();
}

