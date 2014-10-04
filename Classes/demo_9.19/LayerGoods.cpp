#include "LayerGoods.h"
#include "GameManager.h"
#include "LayerPlant.h"

 bool LayerGoods1::init()
{
    GameLayerRollY::init();
    
    _goodsBatch = SpriteBatchNode::create("light_big.png");
    addChild(_goodsBatch);
    
    return true;
}
void LayerGoods1::update(float dt)
{
    auto plant = GameManager::getInstance()->getLayerPlant()->_plant;
    Vec2 plantHeadTopVec = plant->convertToWorldSpace( plant->_headCur._cp._point);
    Rect rectPlantHead(plantHeadTopVec.x-15,plantHeadTopVec.y,30,40);
    
    auto& list = _goodsBatch->getChildren();
    
    for (auto i:list) {
        Rect nrect = i->boundingBox();
        nrect = Rect(nrect.origin.x+15,nrect.origin.y+15,nrect.size.width-30,
                     nrect.size.height-30);
        if(nrect.intersectsRect(rectPlantHead))
        {
            if ((int)i->getUserData()!=100)
            {
            i->stopAllActions();
                ScaleBy* scaBy =ScaleBy::create(3, -1.3);
                Point pt =i->getPosition();
                MoveBy* move = MoveTo::create(3, Vec2(-pt.x,-pt.y));
                Spawn* spawn = Spawn::create(move,scaBy, NULL);
           // FadeOut* fadout = FadeOut::create(3);
            CallFuncN* call = CallFuncN::create([](Node* node)
                                               {
                                                   node->removeFromParent();
                                               });
                Sequence* seq = Sequence::create(DelayTime::create(1), spawn,call,nullptr);
            i->runAction(seq);
            i->setUserData((void*)100);
            }
        }
    }
    
}
void LayerGoods1::moveDown(float yLen)
{
    auto& list = _goodsBatch->getChildren();
    
    float topHeight = 0;
    for (auto i:list) {
        i->setPositionY(i->getPositionY()-yLen);
        float y =i->getPositionY();
        if (y>topHeight) {
            topHeight = y;
        }
    }
    Size wsize =Director::getInstance()->getWinSize();
    if (topHeight<wsize.height*0.7) {
        Sprite* sp = Sprite::createWithTexture(_goodsBatch->getTexture());
        sp->setScale(3.0);
        sp->setColor(Color3B::YELLOW);
        sp->setAnchorPoint(Vec2::ZERO);
        DelayTime* delayTime =DelayTime::create(rand()%20+3);
        FadeOut* out = FadeOut::create(rand()%4+1);
        FadeIn* in = FadeIn::create(rand()%4+1);
        Sequence* seq = Sequence::create(delayTime,out,in,NULL);
        sp->runAction(RepeatForever::create( seq));
        sp->setPosition(Vec2(rand()%(int)(wsize.width-200)+100,wsize.height+20));
        _goodsBatch->addChild(sp);
    }
}


