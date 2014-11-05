#include "LayerLight.h"
#include "GameManager.h"
#include "LayerPlant.h"
#include <time.h>
bool  LayerLight::init()
{
    GameLayerRollY::init();

    _lightBatch = SpriteBatchNode::create("light_2.png");
    addChild(_lightBatch);
    GameManager::getInstance()->_layerLight = this;
    return true;
}
void LayerLight::addOneLight(Vec2 ptlight)
{
    //return ;
    srand(time(nullptr));
    float height = rand()%20+40 ;
    if (!_lightListFirst.empty()) {
        height   = (*_lightListFirst.rbegin())->_heightTop;
        height +=rand()%20+20 ;
    }
    LightNode2* nodelinght =  LightNode2::create();
    nodelinght->_cpList = &GameManager::getInstance()->getCPListRefByIndex(0);
    nodelinght->_plantNode = ((LayerPlant_1*)(GameManager::getInstance()->getLayerPlant()))->_plant;
    nodelinght->initByTopHeight(height);
    nodelinght->_isAutoMoving = true;
    
    nodelinght->setPosition(ptlight);
    //nodelinght->addChild(createOneSpriteLight());
    //nodelinght->_time = rand()%10/10.0f;
    _lightListFirst.push_back(nodelinght);
    addChild(nodelinght);
}
void  LayerLight::addOneLight()
{
       // return ;
    srand(time(nullptr));
    float height = rand()%20+40 ;
    if (!_lightListFirst.empty()) {
        height   = (*_lightListFirst.rbegin())->_heightTop;
        height +=rand()%20+20 ;
    }
    LightNode2* nodelinght =  LightNode2::create();
    nodelinght->_cpList = &GameManager::getInstance()->getCPListRefByIndex(0);
    nodelinght->_plantNode = ((LayerPlant_1*)(GameManager::getInstance()->getLayerPlant()))->_plant;
    nodelinght->initByTopHeight(height);
    nodelinght->_isAutoMoving = true;
    
    //nodelinght->addChild(createOneSpriteLight());
    //nodelinght->_time = rand()%10/10.0f;
    _lightListFirst.push_back(nodelinght);
    addChild(nodelinght);
}
void  LayerLight::subLightHeight(float height)
{
    for (auto i: _lightListFirst) {
        i->_heightInPlant -= height;
        
    }
}
void LayerLight::update(float dt)
{
    for (auto i:_lightListFirst) {
        i->update(dt);
    }
}
void LayerLight::moveDown(float yLen)
{
    for (auto i: _lightListFirst) {
        i->setPositionY(i->getPositionY()-yLen);
        ((LightNode2*)i)->_contorlPosion.y-=yLen;
        ((LightNode2*)i)->_initEndPosition.y-=yLen;
       
    
    }
}

LightNode2*  LayerLight::createOneLightNode(float height)
{
    LightNode2* nodelinght =  LightNode2::create();
    nodelinght->_cpList = &GameManager::getInstance()->getCPListRefByIndex(0);
    nodelinght->_plantNode = ((LayerPlant_1*)(GameManager::getInstance()->getLayerPlant()))->_plant;
    nodelinght->initByTopHeight(height*40);
    
    nodelinght->_isAutoMoving = true;;
    //nodelinght->addChild(createOneSpriteLight());
    //nodelinght->_time = rand()%10/10.0f;
    return nodelinght;
}
Node*      LayerLight::createOneSpriteLight()
{
    
    Node* light = Node::create();
    light->setAnchorPoint(Vec2(0.5,0.5));
    
    static Color3B colors[] = {Color3B(255,255,255),Color3B(22,97,121),Color3B(164,183,209)};
    Color3B color = Color3B(255,255,255);//Color3B(rand()%255,rand()%255,rand()%255);//colors[rand()%(sizeof(colors)/sizeof(Color3B))];
    Node* node = Node::create();
    node->setAnchorPoint(Vec2(0.5,0.5));
    Sprite* sp1 = Sprite::create("light_2.png");
    sp1->setColor(color);
    sp1->setOpacity(90);
    Sprite* sp2 = Sprite::create("light_2.png");
    //sp1->setScale(0.8);
    sp2->setColor(color);
    // sp2->setScale(0.8);
    
    CallFuncN* call = CallFuncN::create(CC_CALLBACK_1(LayerLight::lightCallback, this));
    sp2->runAction(call);
    
    node->addChild(sp1);
    node->addChild(sp2);
    light->addChild(node);
    //light->setPosition(Vec2(VisibleSize.width*0.5,-200));
    return light;
    
}
void LayerLight::removeOneLight(Vec2 ptlight)
{
   
    if(_lightListFirst.empty())return;
    
   auto light = _lightListFirst.back();
    //
    light->_state = LightNode2::LightNode2::StateStop;
    Vec2 v2 =light->getPosition() ;
    Vec2 v1 =  ptlight;
    
    
    Vec2 cp1 = (v1 - v2)*0.3;
    Vec2 cp2 = (v1 - v2)*0.7;
    
    Vec2 tcp1 = MathHelper::getRotatePosition(v2, v2+cp1, 45);
    Vec2 tcp2 = MathHelper::getRotatePosition(v2, v2+cp2, 45);
    
    ccBezierConfig v;
    v.controlPoint_1 = tcp1 - v2;
    v.controlPoint_2 = tcp2 - v2;
    v.endPosition = v1 -v2;
    
    BezierBy* bezier = BezierBy::create(2.5, v);
    FadeOut* fade= FadeOut::create(0.6);
    CallFuncN* call = CallFuncN::create([=](Node* node)
                                        {
                                            _lightListFirst.remove((LightNode2*)node);
                                            node->removeFromParent();
                                        });
    
    Sequence* seq = Sequence::create(bezier,fade,call,nullptr);
    light->stopAllActions();
    
    light->runAction(seq);
    
}
void LayerLight::lightCallback(Node* node1)
{
    srand(time(nullptr));
    node1->stopAllActions();
    DelayTime* delayTime =DelayTime::create(rand()%20+3);
    FadeIn* in = FadeIn::create(rand()%4+1);
    FadeOut* out = FadeOut::create(rand()%4+1);
    CallFuncN* call = CallFuncN::create(CC_CALLBACK_1(LayerLight::lightCallback, this));
    Sequence* seq = Sequence::create(delayTime,out,in, call,NULL);
    node1->runAction(seq);
}