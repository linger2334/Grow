#include "ScenePageView.h"
#include "GameLogo.h"
#include "GameManager.h"
void Page::handleReleaseLogic(Touch *touch)
{
    if (this->getPageCount() <= 0)
    {
        return;
    }
    Widget* curPage = dynamic_cast<Widget*>(this->getPages().at(_curPageIdx));
    if (curPage)
    {
        Vec2 curPagePos = curPage->getPosition();
        ssize_t pageCount = this->getPageCount();
        float curPageLocation = curPagePos.x;
        float pageWidth = getContentSize().width;
        float boundary = pageWidth/9.0f;
        if (curPageLocation <= -boundary)
        {
            if (_curPageIdx >= pageCount-1)
            {
                scrollPages(-curPageLocation);
            }
            else
            {
                scrollToPage(_curPageIdx+1);
            }
        }
        else if (curPageLocation >= boundary)
        {
            if (_curPageIdx <= 0)
            {
                scrollPages(-curPageLocation);
            }
            else
            {
                scrollToPage(_curPageIdx-1);
            }
        }
        else
        {
            scrollToPage(_curPageIdx);
        }
    }
}


bool ScenePageView::init()
{
    if (!Scene::init())
    {
        return false;
    }
    //下面的4个点

    Sprite* spri[4];
    spri[0] = Sprite::create("pointselected.png");
    spri[0]->setScale(0.5, 0.5);
    addChild(spri[0],88888888);
    spri[0]->setPosition(winSize.height/3,60);
    spri[1] = Sprite::create("pointnormal.png");
    spri[1]->setScale(0.5, 0.5);
    addChild(spri[1],88888888);
    spri[1]->setPosition(winSize.height/2.8,60);
    spri[2] = Sprite::create("pointnormal.png");
    spri[2]->setScale(0.5, 0.5);
    addChild(spri[2],88888888);
    spri[2]->setPosition(winSize.height/2.6,60);
    spri[3]= Sprite::create("pointnormal.png");
    spri[3]->setScale(0.5, 0.5);
    addChild(spri[3],88888888);
    spri[3]->setPosition(winSize.height/2.4,60);
    
    for (int i=1; i<=3;i++) {
        spri[i]->setTexture("pointnormal.png");
    }
    

    _pageView = Page::create();
    _pageView->setContentSize(Size(visible.width,visible.height));
    _pageView->removeAllPages();

    //粒子效果
    auto lizi = ParticleSystemQuad::create("lizi.plist");
    
    lizi->setPosition(Vec2(0,0));
    lizi->setLife(6);
    lizi->setAnchorPoint(Vec2(0,0));
    lizi->setColor(Color3B(255,0,0));
    lizi->setSourcePosition(Vec2(winSize.width/2,visible.height/2.1));
    lizi->setPositionType(ParticleSystem::PositionType::RELATIVE);
    
    auto lizi1 = ParticleSystemQuad::create("lizi.plist");
    
    lizi1->setPosition(Vec2(0,0));
    lizi1->setLife(6);
    lizi1->setAnchorPoint(Vec2(0,0));
    lizi1->setColor(Color3B(255,0,0));
    lizi1->setSourcePosition(Vec2(winSize.width/2,visible.height/2));
    lizi1->setPositionType(ParticleSystem::PositionType::RELATIVE);
    
    auto lizi2 = ParticleSystemQuad::create("lizi.plist");
    
    lizi2->setPosition(Vec2(0,0));
    lizi2->setLife(6);
    lizi2->setAnchorPoint(Vec2(0,0));
    lizi2->setColor(Color3B(255,0,0));
    lizi2->setSourcePosition(Vec2(winSize.width/2,visible.height/2));
    lizi2->setPositionType(ParticleSystem::PositionType::RELATIVE);
    
    auto lizi3 = ParticleSystemQuad::create("lizi.plist");
    
    lizi3->setPosition(Vec2(0,0));
    lizi3->setLife(6);
    lizi3->setAnchorPoint(Vec2(0,0));
    lizi3->setColor(Color3B(255,0,0));
    lizi3->setSourcePosition(Vec2(winSize.width/2,visible.height/2));
    lizi3->setPositionType(ParticleSystem::PositionType::RELATIVE);
    
    auto func = [=](Ref* ref)
    {
        auto node =  static_cast<Node*>(ref);
        bool flag = (bool)node->getUserData();
        auto o = node->getOpacity();
        if(flag)
        {
            if(o > 90)o -= 3;
            else flag =false;
        }
        else
        {
            if(o < 255)o += 3;
            else flag =true;
        }
        node->setUserData((void*)flag);
        node->setOpacity(o);
    };
    //4个关卡
    for (int i = 0; i<4; ++i)
    {
        Layout* layout = Layout::create();

        layout->setContentSize(Size(visible.width,visible.height));

        auto imageView = Button::create(StringUtils::format("00%d.jpg",(i+1)));
        imageView->setScale9Enabled(true); //取消点击后放大
        //imageView->setContentSize(Size(visible.width,visible.height));
        imageView->setScale(0.5,0.5);
        imageView->setPosition(Vec2(layout->getContentSize().width/2 , layout->getContentSize().height/2 ));
        layout->addChild(imageView);
        
        //imageView->setColor(Color3B(i*50,0,0));
        
        imageView->addTouchEventListener(CC_CALLBACK_2(ScenePageView::onButtonClicked, this));
   
        if(i==0)
        {
            //标题
            auto spring = Sprite::create("spring.png");
            spring->setScale(0.5, 0.5);
            spring->setPosition(winSize.width/2,winSize.height/1.2);
            layout->addChild(spring,88888888);
            
            //灯光
            auto sun = Sprite::create("sun.png");
            
            sun->setUserData((void*)true);
            

            sun->setAnchorPoint(Vec2(0,1));
            
            sun->setScale(0.5, 0.5);
            sun->cocos2d::Node::setPosition(winSize.width/2.8,winSize.height/1.4);
            layout->addChild(sun);
            CallFuncN* call = CallFuncN::create(func);
            Sequence* seq = Sequence::create(DelayTime::create(0.1),call,nullptr);
            sun->setUserData((void*)true);
            sun->runAction(RepeatForever::create(seq));
            //加入植物
            auto plant1 = Sprite::create("plant1.png");
            plant1->setScale(0.5, 0.5);
            plant1->setPosition(winSize.width/2,winSize.height/3);
            
            layout->addChild(plant1);
            layout->addChild(lizi);
            
        }
        else if(i==1)
        {
            auto summer = Sprite::create("summer.png");
            summer->setScale(0.5, 0.5);
            summer->setPosition(winSize.width/2,winSize.height/1.2);
            layout->addChild(summer,88888888);
            //灯光
            auto sun = Sprite::create("sun.png");
            
            sun->setUserData((void*)true);
            
            sun->setAnchorPoint(Vec2(0,1));
            
            sun->setScale(0.5, 0.5);
            sun->cocos2d::Node::setPosition(winSize.width/2.8,winSize.height/1.4);
            layout->addChild(sun);
            CallFuncN* call = CallFuncN::create(func);
            Sequence* seq = Sequence::create(DelayTime::create(0.1),call,nullptr);
            sun->setUserData((void*)true);
            sun->runAction(RepeatForever::create(seq));
            auto plant2 = Sprite::create("plant2.png");
            plant2->setScale(0.5, 0.5);
            plant2->setPosition(winSize.width/2,winSize.height/3);
            layout->addChild(plant2,88888888);
            layout->addChild(lizi1);

        }
        else if(i==2)
        {
            auto autumn = Sprite::create("autumn.png");
            autumn->setScale(0.5, 0.5);
            autumn->setPosition(winSize.width/2,winSize.height/1.2);
            layout->addChild(autumn,88888888);
            //灯光
            auto sun = Sprite::create("sun.png");
            
            sun->setUserData((void*)true);
            sun->setAnchorPoint(Vec2(0,1));
            
            sun->setScale(0.5, 0.5);
            sun->cocos2d::Node::setPosition(winSize.width/2.8,winSize.height/1.4);
            layout->addChild(sun);
            CallFuncN* call = CallFuncN::create(func);
            Sequence* seq = Sequence::create(DelayTime::create(0.1),call,nullptr);
            sun->setUserData((void*)true);
            sun->runAction(RepeatForever::create(seq));
            
            auto plant3 = Sprite::create("plant3.png");
            plant3->setScale(0.5, 0.5);
            plant3->setPosition(winSize.width/2,winSize.height/3);
            layout->addChild(plant3,88888888);
            layout->addChild(lizi2);
        }
        else if(i==3)
        {
            auto winter = Sprite::create("winter.png");
            winter->setScale(0.5, 0.5);
            winter->setPosition(winSize.width/2,winSize.height/1.2);
            layout->addChild(winter,88888888);
            
            auto plant4 = Sprite::create("plant4.png");
            plant4->setScale(0.5, 0.5);
            plant4->setPosition(winSize.width/2,winSize.height/3);
            layout->addChild(plant4,88888888);
            layout->addChild(lizi3);
            
        }

        _pageView->insertPage(layout, i);
       
    }

    _pageView->addEventListener([=](Ref*,PageView::EventType type)
                                {
                                    static int oldIndex =0;
                                    for (int i=0; i<4; i++) {
                                        spri[i]->setTexture("pointnormal.png");
                                    }
                                    int i =_pageView->getCurPageIndex();
                                    spri[i]->setTexture("pointselected.png");
                                });
    _pageView->setPosition(Vec2(0, 0));
    addChild(_pageView,0);
    auto backSp  = Sprite::create("back.png");
    auto backSp1  = Sprite::create("back.png");
    backSp1->setColor(Color3B(255,255,0));
    auto  back = MenuItemSprite::create(backSp, backSp1);
    back->setAnchorPoint(Vec2::ZERO);
    back->setPosition(Vec2(visible.width*0.04,30));
    back->setScale(0.5);
    
    auto  backCall = [&](Ref*)
    {
        GameManager::getInstance()->navigationToLogo();
    };
    back->setCallback(backCall);
    
    auto flowerSp  = Sprite::create("flower.png");
    auto flowerSp1  = Sprite::create("flower.png");
    flowerSp1->setColor(Color3B(255,255,0));
    auto  flower = MenuItemSprite::create(flowerSp, flowerSp1);
    flower->setAnchorPoint(Vec2(1,0));
    flower->setPosition(Vec2(visible.width*0.96,30));
    flower->setScale(0.5);
    
    auto  flowerCall = [&](Ref*)
    {
        GameManager::getInstance()->navigationToFlowerScene();
    };
    flower->setCallback(flowerCall);
    Menu * menu1 = Menu::create();
    menu1->addChild(back);
    menu1->addChild(flower);
    menu1->setPosition(Vec2::ZERO);
    
    addChild(menu1);
    
    
    return true;
}

void ScenePageView::onButtonClicked(Ref* pSender, Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED :{
            
          //  GameManager::getInstance()->navigationToGameScene();
//            ssize_t index = _pageView->getCurPageIndex();
//            
//            log("click index %zd",index);
//            auto scene = SceneProcess::create(index);//跳转到SceneProcess界面
//            Director::getInstance()->replaceScene(scene);
            
        }
            break;
        default:
            break;
    }
    
}
