//
//  FlowerDescription.cpp
//  Grow-UI
//
//  Created by wlg on 11/14/14.
//
//

#include "FlowerDescription.h"
#include "AchievementUi.h"
#include "ScreenSelectLevel.h"
#include "ui/CocosGUI.h"
using namespace ui;

Scene* FlowerDescription::createScene(int index)
{
    Scene* scene = Scene::create();
    scene->addChild(FlowerDescription::create(index));
    return scene;
}

FlowerDescription* FlowerDescription::create(int index)
{
    FlowerDescription* ret = new FlowerDescription();
    if(ret&&ret->init(index))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FlowerDescription::init(int index)
{
    if (Layer::init()) {
        _pageView = MyPageView::create();
        _pageView->setContentSize(Size(VisibleSize.width,VisibleSize.height));
        _pageView->setBackGroundImage("UI_FlowDescBackgdround.png");
        _pageView->setBackGroundImageScale9Enabled(true);
        
        ImageView* lamplight = ImageView::create("UI_LampLight.png");
        lamplight->setPosition(Vec2(VisibleSize.width/2,VisibleSize.height/2 + 350/1024.0*VisibleSize.height));
        lamplight->setOpacity(40);
        lamplight->setScale9Enabled(true);
        ActionInterval* flicker = FadeTo::create(5, 30);
        ActionInterval* flicker_resverse = FadeTo::create(5, 40);
        lamplight->runAction(RepeatForever::create(Sequence::create(flicker,flicker_resverse, NULL)));
        _pageView->addChild(lamplight);
        
        createDescriptionPage(index);
        addChild(_pageView);
        
        Button* back = Button::create("UI_FlowerIcon.png");
        back->setPressedActionEnabled(false);
        back->setScale9Enabled(true);
        back->setPosition(Vec2(VisibleSize.width/12,VisibleSize.height/1.07));
        back->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventtype){
            switch (eventtype) {
                case Widget::TouchEventType::ENDED:
                {
                    Scene* scene = AchievementUi::createScene();
                    Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
                }
                break;
                    
                default:
                    break;
            }
        });
        
        addChild(back);
        
        return true;
    }
    
    return false;
}

void FlowerDescription::createDescriptionPage(int index)
{
    ValueVector flowersInfo = FileUtils::getInstance()->getValueVectorFromFile("flowersinfo.plist");
    int seedlingIndex = UserDefault::getInstance()->getIntegerForKey("seedlingIndex");
    
    for (int i = 0;i<seedlingIndex;i++) {
        Layout* layout = Layout::create();
        layout->setContentSize(_pageView->getContentSize());
        
        ImageView* stone = ImageView::create("UI_FlowDescStone.png");
        stone->setPosition(Vec2(layout->getContentSize().width/2,layout->getContentSize().height/3.1));
        layout->addChild(stone);
        
        Button* button = Button::create("UI_FlowDescButton.png");
        button->setPressedActionEnabled(false);
        button->setScale9Enabled(true);
        button->setPosition(Vec2(stone->getBoundingBox().size.width/2,stone->getBoundingBox().size.height*0.85));
        stone->addChild(button);
        
        ValueMap flowerInfo = flowersInfo.at(i).asValueMap();
        ImageView* flower = ImageView::create(flowerInfo.at("imagename").asString());
        flower->setPosition(Vec2(stone->getBoundingBox().size.width/2,stone->getBoundingBox().size.height + flower->getBoundingBox().size.height/2.2));
        stone->addChild(flower);
        
        Text* unlockdate = Text::create(flowerInfo.at("unlockdate").asString(), "Arial", 24);
        unlockdate->setPosition(Vec2(stone->getPositionX(),stone->getPositionY() + 0.12*VisibleSize.height));
        layout->addChild(unlockdate);
        
        Text* flowername = Text::create(flowerInfo.at("flowername").asString(), "Arial", 48);
        flowername->setPosition(Vec2(stone->getPositionX(), stone->getPositionY() + 45.0/1024*VisibleSize.height));
        layout->addChild(flowername);
        
        Text* description = Text::create(flowerInfo.at("description").asString(), "Helvetica-light", 24);
        description->setPosition(Vec2(stone->getPositionX(),stone->getPositionY()-25.0/1024*VisibleSize.height));
        layout->addChild(description);
        
        ImageView* arrow = ImageView::create("UI_FlowDescArrow.png");
        arrow->setPosition(Vec2(stone->getPositionX(),stone->getPositionY()-240.0/1024*VisibleSize.height));
        layout->addChild(arrow);
        
        Text* Id = Text::create(flowerInfo.at("id").asString(), "Arial", 24);
        Id->setPosition(Vec2(stone->getPositionX(),stone->getPositionY()-240.0/1024*VisibleSize.height));
        layout->addChild(Id);
        
        _pageView->addPage(layout);
    }
    
    _pageView->scrollToPage(index);
}