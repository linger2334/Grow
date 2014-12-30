//
//  Cicada.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-8.
//
//

#include "Cicada.h"
#include "GameManager.h"
#include "GB2ShapeCache-x.h"
#include "LayerItem.h"
#include "CustomAction.h"

Cicada::Cicada()
{
    
}

Cicada::~Cicada()
{

}

class Cicada* Cicada::create(Item& item)
{
    Cicada *cicada = new Cicada();
    if (cicada && cicada->init(item))
    {
        cicada->autorelease();
        return cicada;
    }
    CC_SAFE_DELETE(cicada);
    return nullptr;
}

bool Cicada::init(Item& item)
{
    bool result;
    if (ItemModel::init(item)) {
        fanningSpeed = kDefaultCicadaFanningSpeed;
        autoTurnHead = kDefaultCicadaAutoTurnHeadState;
        autoFanning = kDefaultCicadaAutoFanningState;
        bool isReversalStatus = kDefaultCicadaReversalStatus;
        if (item.features) {
            Features_Cicada* feat = (Features_Cicada*)item.features;
            fanningSpeed = feat->fanningSpeed;
            autoTurnHead = feat->autoTurnHead;
            autoFanning = feat->autoFanning;
            isReversalStatus = feat->isReversalStatus;
        }
        
        setTexture(isReversalStatus? "Cicada_Trunk_Blue.png" : "Cicada_Trunk_Red.png");
        setCascadeOpacityEnabled(true);
        
        _belly = Sprite::create(isReversalStatus? "Cicada_Belly_Blue.png" : "Cicada_Belly_Red.png");
        _belly->setPosition(getBoundingBox().size.width/2, getBoundingBox().size.height/2);
        _belly->setCascadeOpacityEnabled(true);
        addChild(_belly);
        
        _wing = Sprite::create(isReversalStatus? "Cicada_WingStill_Blue.png" : "Cicada_WingStill_Red.png");
        _wing->setPosition(Vec2(getBoundingBox().size.width/2,getBoundingBox().size.height/2));
        _wing->setCascadeOpacityEnabled(true);
        addChild(_wing);
        
        setStatus(isReversalStatus? ItemStatus::ReversalStatus : ItemStatus::NormalStatus);
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        ActionInterval* shine = FadeTo::create(kDefaultFlickeringHalfCycle ,kDefaultCicadaBellyTransparency*255);
        ActionInterval* shine_reverse = FadeTo::create(kDefaultFlickeringHalfCycle, 255);
        _belly->runAction(RepeatForever::create(Sequence::createWithTwoActions(shine, shine_reverse)));
        
        prePosition = getPosition();
        setCicadaState(Cicada_State::Motionless);
        onStateChangeToMotionless();
        
        result = true;
    }else{
        result = false;
    }

    return result;
}

void Cicada::createBody()
{
    b2World* world = GameManager::getInstance()->getBox2dWorld();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(getParent()-> convertToWorldSpace(getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(getPosition()).y/PTM_RATIO);
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.linearDamping = 0.3;
    bodyDef.userData = this;
    _body= world->CreateBody(&bodyDef);
    
    ((LayerItem*)getParent())->_fixturesCache->addFixturesToBody(_body, "Cicada");
    
    for (b2Fixture* fixture = _body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        b2Shape* shape = fixture->GetShape();
        if (shape->GetType() == b2Shape::Type::e_circle) {
            b2CircleShape* circleShape = (b2CircleShape*)shape;
            circleShape->m_radius *= getScale();
        }else{
            b2PolygonShape* polygonShape = (b2PolygonShape*)shape;
            int count = polygonShape->GetVertexCount();
            for (int i = 0; i<count; i++) {
                polygonShape->m_vertices[i] *= getScale();
            }
        }
    }
    
    b2MassData bodymassData;
    _body->GetMassData(&bodymassData);
    bodymassData.mass *= getScale();
    bodymassData.I *= getScale();
    _body->SetMassData(&bodymassData);
    //
    scheduleUpdate();
}

void Cicada::wingStir()
{
    CallFunc* expand = CallFunc::create([&](){
        std::string texturename = status==ItemStatus::NormalStatus? "Cicada_WingMotion_Red.png" : "Cicada_WingMotion_Blue.png";
        _wing->setTexture(texturename);
    });
    CallFunc* combine = CallFunc::create([&](){
        std::string texturename = status==ItemStatus::NormalStatus? "Cicada_WingStill_Red.png" : "Cicada_WingStill_Blue.png";
        _wing->setTexture(texturename);
    });
    Sequence* fan1 = Sequence::create(expand,DelayTime::create(0.05),combine, NULL);
    Sequence* fan2 = Sequence::create(fan1,DelayTime::create(0.1),fan1, NULL);
    CallFunc* callback = CallFunc::create([&](){
        this->wingStir();
    });
    
    float waitTime1 = 1+CCRANDOM_0_1();
    float waitTime2 = 2+CCRANDOM_0_1();
    Sequence* fan = Sequence::create(DelayTime::create(waitTime1),fan1,DelayTime::create(waitTime2),fan2,callback, NULL);
    fan->setTag(1);
    runAction(fan);
}

void Cicada::wingFanning()
{
    DelayTime* remain = DelayTime::create(0.07/fanningSpeed);
    CallFunc* expand = CallFunc::create([&](){
        _wing->setTexture("Cicada_WingFanning_1.png");
    });
    DelayTime* stay = DelayTime::create(0.07/fanningSpeed);
    CallFunc* combine = CallFunc::create([&](){
        _wing->setTexture("Cicada_WingFanning_2.png");
    });
    
    Sequence* fanning = Sequence::create(remain,expand,stay,combine, NULL);
    RepeatForever* fanningForever = RepeatForever::create(fanning);
    fanningForever->setTag(2);
    runAction(fanningForever);
}

void Cicada::onStateChangeToMotionless()
{
    if (_actionManager->getActionByTag(2, this)) {
        _actionManager->removeActionByTag(2, this);
        std::string texturename = status==ItemStatus::NormalStatus? "Cicada_WingStill_Red.png" : "Cicada_WingStill_Blue.png";
        _wing->setTexture(texturename);
    }
    wingStir();
}

void Cicada::onStateChangeToMoving()
{
    if (_actionManager->getActionByTag(1, this)) {
        _actionManager->removeActionByTag(1, this);
    }
    wingFanning();
}

void Cicada::switchItemStatus()
{
    std::string trunkFileName;
    std::string bellyFileName;
    std::string wingFileName;
    CallFunc* changeTexture;
    if (getCicadaState() == Cicada_State::Motionless) {
        if (status==ItemStatus::NormalStatus) {
            status = ItemStatus::ReversalStatus;
            trunkFileName = "Cicada_Trunk_Blue.png";
            bellyFileName = "Cicada_Belly_Blue.png";
            wingFileName = "Cicada_WingStill_Blue.png";
            
        }else{
            status = ItemStatus::NormalStatus;
            trunkFileName = "Cicada_Trunk_Red.png";
            bellyFileName = "Cicada_Belly_Red.png";
            wingFileName = "Cicada_WingStill_Red.png";
        }
        
        changeTexture = CallFunc::create([=](){
            this->setTexture(trunkFileName);
            this->_belly->setTexture(bellyFileName);
            this->_wing->setTexture(wingFileName);
        });
    }else{
        if (status==ItemStatus::NormalStatus) {
            status = ItemStatus::ReversalStatus;
            trunkFileName = "Cicada_Trunk_Blue.png";
            bellyFileName = "Cicada_Belly_Blue.png";
        }else{
            status = ItemStatus::NormalStatus;
            trunkFileName = "Cicada_Trunk_Red.png";
            bellyFileName = "Cicada_Belly_Red.png";
        }
        
        changeTexture = CallFunc::create([=](){
            this->setTexture(trunkFileName);
            this->_belly->setTexture(bellyFileName);
        });
    }
    
    FadeOut* fadeaway = FadeOut::create(kDefaultSwitchStatusInterval);
    FadeIn* fadeIn = FadeIn::create(kDefaultSwitchStatusInterval);
    Sequence* turnColor = Sequence::create(fadeaway,changeTexture,fadeIn, NULL);
    runAction(turnColor);
}

void Cicada::update(float dt)
{
    if (bindedTriggerID == kDefaultBindedTriggerID) {
        ItemModel::update(dt);
    }else{
        if (isTriggerSwitchOn) {
            ItemModel::update(dt);
        }
    }
    Vec2 diff = getPosition() - prePosition;
    if (diff == Vec2::ZERO) {
        if (getCicadaState() != Cicada_State::Motionless) {
            setCicadaState(Cicada_State::Motionless);
            onStateChangeToMotionless();
        }
    }else{
        if (getCicadaState() != Cicada_State::Moving) {
            setCicadaState(Cicada_State::Moving);
            if (autoFanning) {
                onStateChangeToMoving();
            }
        }
        if (autoTurnHead) {
            float dy = getPositionY()-prePosition.y;
            float dx = getPositionX()-prePosition.x;
            float rotation = CC_RADIANS_TO_DEGREES(M_PI/2 - atan2f(dy, dx));
            setRotation(rotation);
            float distance = sqrtf(dy*dy + dx*dx);
            log("%f",distance/dt);
        }
    }
    prePosition = getPosition();
    
}


