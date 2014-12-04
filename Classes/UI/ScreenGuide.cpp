//
//  ScreenGuide.cpp
//  Grow-UI
//
//  Created by wlg on 12/1/14.
//
//

#include "ScreenGuide.h"
#include "CocosGUI.h"

using namespace ui;

bool ScreenGuide::init()
{
    if(Layer::init()){
        
        Layout* graylayer = Layout::create();
        graylayer->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        graylayer->setBackGroundColor(Color3B(0, 0, 0));
        graylayer->setOpacity(180);
        graylayer->setContentSize(VisibleSize);
        graylayer->setTouchEnabled(true);
        addChild(graylayer);
        
        ImageView* baseboard = ImageView::create("UI_StrokeDirt.png");
        baseboard->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2));
        addChild(baseboard);
        
        Button* quit = Button::create("UI_close.png");
        quit->setPosition(Vec2(baseboard->getBoundingBox().size.width*0.9,baseboard->getBoundingBox().size.height*0.95));
        quit->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventType){
            if(eventType == Widget::TouchEventType::ENDED){
                Button* button = dynamic_cast<Button*>(sender);
                button->removeFromParent();
                ActionInterval* move = EaseExponentialOut::create(MoveBy::create(0.5, Vec2(0,-getContentSize().height)));
                this->runAction(Sequence::create(move,RemoveSelf::create(), NULL));
            }
        });
        baseboard->addChild(quit);
        
        return true;
    }
    
    return false;
}

void ScreenGuide::onEnter()
{
    Layer::onEnter();
    setPosition(Vec2(0,-getContentSize().height));
    ActionInterval* moveUp = EaseExponentialOut::create(MoveBy::create(0.5, Vec2(0,getContentSize().height)));
    runAction(moveUp);
    
}

void ScreenGuide::onExit()
{
    Layer::onExit();
    
}

void ScreenGuide::guideProcess(GuideInfo &guidePhase)
{
    
}

void ScreenGuide::onNextStep()
{
    
}