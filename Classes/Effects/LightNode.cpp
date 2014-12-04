#include "LightNode.h"

bool LightNode::init()
{
    bool ret =  Sprite::initWithFile(GamePaths::_sPathLight);
    if (!ret)return ret;
    _state = StateNormal;
    return ret;
}

void LightNode::changeLightState(int state)
{
     if (state == StateRandAction) {
         randFadeActionCall();
         randScaleActionCall();
     }
}

void LightNode::stopLightState(int state)
{
    if (state == StateRandAction) {
        stopActionByTag(ActionRandFade);
        stopActionByTag(ActionRandScale);
    }
}
void LightNode::randFadeActionCall()
{
    FadeTo* fadeTo = FadeTo::create(RandomHelper::rand(1, 5), 99);
    FadeTo* fadeTo1 = FadeTo::create(RandomHelper::rand(1, 5), 255);
    DelayTime* time = DelayTime::create(RandomHelper::rand(0, 7));
    DelayTime* timeEnd = DelayTime::create(RandomHelper::rand(0, 11));
    CallFunc* call = CallFunc::create(CC_CALLBACK_0(LightNode::randFadeActionCall,this));
    Sequence* seq = Sequence::create(fadeTo, time,fadeTo1,timeEnd,call,nullptr);
    seq->setTag(ActionRandFade);

    runAction(seq);
}
void LightNode::randScaleActionCall()
{
    bool flag = RandomHelper::rand(1,255) > 155 ? true : false;
    int scale = !flag ? 1 :  RandomHelper::rand(1, 3);
    ScaleTo* scaleTo = ScaleTo::create(RandomHelper::rand(1, 5), scale);
    // ScaleTo* scaleTo1 = ScaleTo::create(rand()%5+1, 1);
    //DelayTime* time = DelayTime::create(rand()%7);
    DelayTime* timeEnd = DelayTime::create(RandomHelper::rand(1, 7));
    CallFunc* call = CallFunc::create(CC_CALLBACK_0(LightNode::randScaleActionCall,this));
    Sequence* seq = Sequence::create(scaleTo,timeEnd,call,nullptr);
    seq->setTag(ActionRandScale);
    runAction(seq);
}
void LightNode::runRandFadeAction()
{
    randFadeActionCall();
}
void LightNode::runRandScaleAction()
{
    randScaleActionCall();
}
void LightNode::stopRandFadeAction()
{
    stopActionByTag(ActionRandFade);
}
void LightNode::stopRandScaleAction()
{
    stopActionByTag(ActionRandScale);
}