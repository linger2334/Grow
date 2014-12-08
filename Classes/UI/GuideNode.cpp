//
//  GuideNode.cpp
//  Grow-LevelEdit
//
//  Created by 林纲 王 on 14-12-6.
//
//

#include "GuideNode.h"
#include "CocosGUI.h"
#include "ScreenGuide.h"

using namespace ui;

void GuideNode::onNextStep()
{
    GuideManager::getInstance()->goNextStep();
}

void GuideNode::onExit()
{
    ClippingNode::onExit();
    GuideManager::getInstance()->unRegister(this);
}

///////////////
bool StrokeDirt::init()
{
    if(ClippingNode::init()){
        setInstanceName("StrokeDirt");
        
        DrawNode* stencil = DrawNode::create();
        Vec2 rect[4];
        rect[0] = Vec2(100,100);
        rect[1] = Vec2(200,100);
        rect[2] = Vec2(200,200);
        rect[3] = Vec2(100,200);
        Color4F red(1,0,0,1);
        stencil->drawPolygon(rect, 4, red, 1, Color4F::WHITE);
        stencil->setPosition(Vec2(VisibleSize.width/2,VisibleSize.height/2));
        this->setStencil(stencil);
        this->setInverted(true);
        //针对模板
        this->setAlphaThreshold(1);
        
        Layout* graylayer = Layout::create();
        graylayer->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        graylayer->setBackGroundColor(Color3B(0, 0, 0));
        graylayer->setOpacity(180);
        graylayer->setContentSize(VisibleSize);
        graylayer->setTouchEnabled(true);
        graylayer->setCascadeOpacityEnabled(true);
        addChild(graylayer);
        
        ImageView* baseboard = ImageView::create("UI_StrokeDirt.png");
        baseboard->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2));
        baseboard->setCascadeOpacityEnabled(true);
        addChild(baseboard);
        
        Button* quit = Button::create("UI_close.png");
        quit->setPosition(Vec2(baseboard->getBoundingBox().size.width*0.9,baseboard->getBoundingBox().size.height*0.95));
        quit->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventType){
            if(eventType == Widget::TouchEventType::ENDED){
                Button* button = dynamic_cast<Button*>(sender);
                button->removeFromParent();
                FadeOut* disappear = FadeOut::create(1.5);
                RemoveSelf* remove = RemoveSelf::create();
                this->runAction(Sequence::createWithTwoActions(disappear, remove));
            }
        });
        quit->setCascadeOpacityEnabled(true);
        baseboard->addChild(quit);
        
        setOpacity(0);
        setCascadeOpacityEnabled(true);
        return true;
    }
    
    return false;
}

void StrokeDirt::guideProcess(GuideInfo &guidePhase)
{
    if (guidePhase.subSeq == 1) {
        ScreenGuide::getRunningLayer()->addChild(this);
        FadeIn* appear = FadeIn::create(1.5);
        CallFunc* goNext = CallFunc::create([&](){
            this->onNextStep();
        });
        runAction(Sequence::create(appear,goNext,NULL));
    }else if(guidePhase.subSeq == 2){
        onNextStep();
        log("complete 2nd phase");
    }
    
}


////////////////////////////
