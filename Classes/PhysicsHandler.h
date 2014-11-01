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

class PhysicsHandler : public Ref,public b2ContactListener
{
public:
    PhysicsHandler();
    ~PhysicsHandler();
    
    static PhysicsHandler* create(b2World* world);
    bool init(b2World* world);
    
    inline b2World* getWorld() { return _world;}
    
    bool isPointContact(cocos2d::Point ptInGl);
    
    void dealCollisions();
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact,const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact,const b2ContactImpulse* impulse);
    
    void update(float dt);

protected:
    b2World* _world;
    typedef std::pair<b2Fixture*,b2Fixture*> MyContact;
    std::set<MyContact> _contacts;
};

#endif /* defined(__Grow__PhysicsHandler__) */
