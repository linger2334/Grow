//
//  TriggerNode.cpp
//  Grow-LevelEdit
//
//  Created by wlg on 12/18/14.
//
//

#include "TriggerNode.h"
#include "GameManager.h"
#include "b2Separator.h"

TriggerNode::TriggerNode()
{
    
}

TriggerNode::~TriggerNode()
{
    
}

TriggerNode* TriggerNode::create(TriggerInfo& triggerInfo)
{
    TriggerNode* ret = new TriggerNode();
    if (ret && ret->init(triggerInfo)) {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TriggerNode::init(TriggerInfo& triggerInfo)
{
    if (Sprite::init()) {
        _type = Trigger;
        setTag(triggerInfo.tag);
        isConvex = triggerInfo.isConvex;
        setPosition(triggerInfo.position.x*DefiniteSize.width,triggerInfo.position.y*DefiniteSize.height);
        _bindIDs = triggerInfo.bindIDs;
        
        createBody(triggerInfo.vertexes);
        return true;
    }

    return false;
}

void TriggerNode::createBody(std::vector<Vec2>& vertexes)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = cc_to_b2Vec(getPositionX()+(VisibleSize.width-DefiniteSize.width)/2, getPositionY());
    bodyDef.userData = this;
    _body = GameManager::getInstance()->getBox2dWorld()->CreateBody(&bodyDef);
    
    b2FixtureDef fixtureDef;
    fixtureDef.filter.groupIndex = -1;
    fixtureDef.filter.categoryBits = 2;
    fixtureDef.filter.maskBits = 1;
    
    b2PolygonShape shape;
    int vertexCount = vertexes.size();
    CCASSERT(vertexCount>=3&&vertexCount<=b2_maxPolygonVertices, "vertex count is too little or too much!");
    b2Vec2 points[b2_maxPolygonVertices];
    if (isConvex) {
        for (int i = 0; i<vertexCount; i++) {
            points[i] = cc_to_b2Vec(vertexes[i].x, vertexes[i].y);
        }
        shape.Set(points, vertexCount);
        
        fixtureDef.shape = &shape;
        _body->CreateFixture(&fixtureDef);
    }else{
        //
        std::vector<b2Vec2> b2vertexes;
        for (Vec2 vertex : vertexes) {
            b2Vec2 point = cc_to_b2Vec(vertex.x, vertex.y);
            b2vertexes.push_back(point);
        }
        //切割
        b2Separator* separator = new b2Separator();
        if (separator->Validate(b2vertexes)==0) {
            separator->Separate(_body, &fixtureDef, &b2vertexes, PTM_RATIO);
        }else if (separator->Validate(b2vertexes)==1){
            CCASSERT(nullptr, "there are overlapping lines!");
        }else if (separator->Validate(b2vertexes)==2){
            CCASSERT(nullptr, "the points are not in clockwise order");
        }else if (separator->Validate(b2vertexes)==3){
            CCASSERT(nullptr, "there are overlapping lines and the points are not in clockwise order");
        }
        CC_SAFE_DELETE(separator);
    }
}


