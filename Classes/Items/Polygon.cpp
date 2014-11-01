//
//  Polygon.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-4.
//
//

#include "Polygon.h"
#include "GameManager.h"
#include "b2Separator.h"

Polygon::Polygon()
{
    
}

Polygon::~Polygon()
{
    b2World* world = GameManager::getInstance()->getBox2dWorld();
    if (world&&_body) {
        world->DestroyBody(_body);
        _body = nullptr;
    }

}

class Polygon* Polygon::create(PolygonInfo& polygonInfo)
{
    Polygon *pRet = new Polygon();
    if (pRet && pRet->init(polygonInfo))
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

bool Polygon::init(PolygonInfo& polygonInfo)
{
    if (Node::init()) {
        
        setPosition(polygonInfo.position.x*WinSize.width, polygonInfo.position.y*WinSize.height);
        
        GameManager* gameManager = GameManager::getInstance();
        _type = 10;
        polygonTag = polygonInfo.tag;
        this->isConvex = polygonInfo.isConvex;
        
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position = cc_to_b2Vec(getPositionX(), getPositionY());
        bodyDef.userData = this;
        _body = gameManager->getBox2dWorld()->CreateBody(&bodyDef);
        
        b2FixtureDef fixtureDef;
        fixtureDef.filter.groupIndex = -1;
        fixtureDef.filter.categoryBits = 0;
        fixtureDef.filter.maskBits = 0;
        
        b2PolygonShape shape;
        int vertexCount = polygonInfo.vertexes.size();
        CCASSERT(vertexCount>=3&&vertexCount<=b2_maxPolygonVertices, "vertex count is too little or too much!");
        b2Vec2 points[b2_maxPolygonVertices];
        if (isConvex) {
            for (int i = 0; i<vertexCount; i++) {
                points[i] = cc_to_b2Vec(polygonInfo.vertexes[i].x, polygonInfo.vertexes[i].y);
            }
            shape.Set(points,vertexCount);
            
            fixtureDef.shape = &shape;
            _body->CreateFixture(&fixtureDef);
        }else{
            //
            std::vector<b2Vec2> vertexes;
            for (Vec2 vertex : polygonInfo.vertexes) {
                b2Vec2 point = cc_to_b2Vec(vertex.x, vertex.y);
                vertexes.push_back(point);
            }
            //切割
            b2Separator* separator = new b2Separator();
            if (separator->Validate(vertexes)==0) {
                separator->Separate(_body, &fixtureDef, &vertexes, PTM_RATIO);
            }else if (separator->Validate(vertexes)==1){
                CCASSERT(0==1, "there are overlapping lines!");
            }else if (separator->Validate(vertexes)==2){
                CCASSERT(0==2, "the points are not in clockwise order");
            }else if (separator->Validate(vertexes)==3){
                CCASSERT(0==3, "there are overlapping lines and the points are not in clockwise order");
            }
            CC_SAFE_DELETE(separator);
        }
        
        return true;
    }
    
    return false;
}



