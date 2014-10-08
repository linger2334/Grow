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
#include "GLES-Render.h"

class PhysicsHandler : public Ref,public b2ContactListener
{
public:
    PhysicsHandler();
    ~PhysicsHandler();
    
    CREATE_FUNC(PhysicsHandler);
    bool init();
    
    b2World* getWorld();
    bool isPointContact(cocos2d::Point ptInGl);
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact,const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact,const b2ContactImpulse* impulse);
    
    void update(float dt);

protected:
    b2World* world;
    GLESDebugDraw* _debugDraw;
    
    std::vector<b2Body*> _itembodies;
    typedef std::pair<b2Fixture*,b2Fixture*> MyContact;
    std::set<MyContact> _contacts;
};

#endif /* defined(__Grow__PhysicsHandler__) */
