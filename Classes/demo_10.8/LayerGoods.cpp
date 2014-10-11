#include "LayerGoods.h"
#include "GameManager.h"
#include "LayerPlant.h"
#include "LayerLight.h"
 bool LayerGoods1::init()
{
    GameLayerRollY::init();
/////////modify by wlg，inorder to normalize naming rule
    _goodsBatch = SpriteBatchNode::create("Flame_White.png");
//////////////////////////////////////////////////////////
    addChild(_goodsBatch);
    
    return true;
}
void LayerGoods1::update(float dt)
{
    auto plant = GameManager::getInstance()->getLayerPlant()->_plant;
    Vec2 plantHeadTopVec = plant->convertToWorldSpace( plant->_headCur._cp._point);
    Rect rectPlantHead(plantHeadTopVec.x-10,plantHeadTopVec.y-10,20,20);
    
    auto& list = _goodsBatch->getChildren();
    
    for (auto i:list) {
        Rect nrect = i->boundingBox();
        nrect = Rect(nrect.origin.x+10,nrect.origin.y+20,nrect.size.width-20,
                     nrect.size.height-40);
        if(nrect.intersectsRect(rectPlantHead))
        {
            if ((int)i->getUserData()!=100)
            {
            i->stopAllActions();
                FadeTo* fadeout = FadeTo::create(1,0);
                ScaleTo* scaBy =ScaleTo::create(1, 0);
  
                Spawn* spawn = Spawn::create(fadeout,scaBy, NULL);
            CallFuncN* call = CallFuncN::create([](Node* node)
                                               {
                                                   node->removeFromParent();
                                               });
                
                Sequence* seq = Sequence::create( spawn,call,nullptr);
            i->runAction(seq);
            i->setUserData((void*)100);
                _layerLight = GameManager::getInstance()->getLayerLight();
                if(_layerLight->_lightListFirst.size()<30)
                {
                    _layerLight->addOneLight();
                }
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

        DelayTime* delayTime =DelayTime::create(rand()%20+3);

        FadeTo*  toMin = FadeTo::create(rand()%4+1,90);
        FadeTo*  toMax = FadeTo::create(rand()%4+1,255);
        Sequence* seq = Sequence::create(delayTime,toMin,toMax,NULL);
        sp->runAction(RepeatForever::create( seq));
        sp->setPosition(Vec2(rand()%(int)(wsize.width-200)+100,wsize.height+20));
        _goodsBatch->addChild(sp);
    }
}


