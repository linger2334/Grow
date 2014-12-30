//
//  PhysicsHandler.h
//  Grow
//
//  Created by 林纲 王 on 14-10-8.
//
//

#ifndef __Grow__PhysicsHandler__
#define __Grow__PhysicsHandler__

#include <iostream>
#include "Box2d/Box2d.h"
#include "Macro.h"
#include "ItemModel.h"

class PhysicsHandler : public Ref,public b2ContactListener,public b2QueryCallback
{
public:
    PhysicsHandler();
    ~PhysicsHandler();
    
    static PhysicsHandler* create(b2World* world);
    bool init(b2World* world);
    
    inline b2World* getWorld() { return _world;}
    
    bool isPointContact(cocos2d::Point ptInGl);
    
    void dealCollisions();
    void CollisionCallBack(ItemModel* item,ItemModel* plantHead);
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact,const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact,const b2ContactImpulse* impulse);
    
    virtual bool ReportFixture(b2Fixture* fixture);
    
    void CollideWithFlame(ItemModel* item,ItemModel* plantHead);
    void CollideWithCicada(ItemModel* item,ItemModel* plantHead);
    void CollideWithDragon(ItemModel* item,ItemModel* plantHead);
    void CollideWithDoubDragon(ItemModel* item,ItemModel* plantHead);
    void CollideWithSerpent(ItemModel* item,ItemModel* plantHead);
    void CollideWithGearButton(ItemModel* item,ItemModel* plantHead);
    void CollideWithGearGate(ItemModel* item,ItemModel* plantHead);
    void CollideWithGearReversal(ItemModel* item,ItemModel* plantHead);
    void CollideWithBarrier(ItemModel* item,ItemModel* plantHead);
    void CollideWithDecorationFlower(ItemModel* item,ItemModel* plantHead);
    void CollideWithTriggerNode(ItemModel* item,ItemModel* plantHead);
    
    void update(float dt);

protected:
    b2World* _world;
    typedef std::pair<b2Fixture*,b2Fixture*> MyContact;
    std::set<MyContact> _contacts;
};

#endif /* defined(__Grow__PhysicsHandler__) */
