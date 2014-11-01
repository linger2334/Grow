//
//  AchievementUi.cpp
//  GrowUp
//
//  Created by wlg on 8/28/14.
//
//

#include "AchievementUi.h"
#include "GameManager.h"
#include "ScenePageView.h"

#define SCROLL_DEACCEL_RATE  0.95f  
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.15f
#define INSET_RATIO          0.05f   //默认0.2
#define MOVE_INCH            7.0f/160.0f
#define BOUNCE_BACK_FACTOR   0.05f  //默认0.35

//#include "DialogLayout.h"

#define POSITION_RATIO 4.0 - 4.0/9.0 - 2.0/9.0
#define XL1 60.0/1536.0*VisibleSize.width
#define XL2 385.0/1536.0*VisibleSize.width
#define XL3 765.0/1536.0*VisibleSize.width
#define XL4 1130.0/1536.0*VisibleSize.width
#define XL5 1470.0/1536.0*VisibleSize.width
#define XR1 120.0/1536.0*VisibleSize.width
#define XR2 445.0/1536.0*VisibleSize.width
#define XR3 825.0/1536.0*VisibleSize.width
#define XR4 1190.0/1536.0*VisibleSize.width
#define XR5 1530.0/1536.0*VisibleSize.width
#define ITEM_OFFSET_L Vec2(0,400.0/2048*VisibleSize.height)
#define ITEM_OFFSET_M Vec2(0,320.0/2048*VisibleSize.height)
#define ITEM_OFFSET_R Vec2(0,400.0/2048*VisibleSize.height)
#define CONTENT_INIT_OFFSET 0.0 //40.0/2048.0*VisibleSize.height
#define SPEED_INERTIA_START 0.002000

Myscrollview* Myscrollview::create(Size size, Node* container/* = nullptr*/)
{
    Myscrollview* pRet = new Myscrollview();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

Myscrollview* Myscrollview::create()
{
    Myscrollview* pRet = new Myscrollview();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void Myscrollview::deaccelerateScrolling(float dt)
{
    if (_dragging)
    {
        this->unschedule(schedule_selector(MyScrollView::deaccelerateScrolling));
        return;
    }
    
    float newX, newY;
    Vec2 maxInset, minInset;
    
    _container->setPosition(_container->getPosition() + _scrollDistance);
    
    if (_bounceable)
    {
        maxInset = _maxInset;
        minInset = _minInset;
    }
    else
    {
        maxInset = this->maxContainerOffset();
        minInset = this->minContainerOffset();
    }
    
    newX = _container->getPosition().x;
    newY = _container->getPosition().y;
    
    _scrollDistance = _scrollDistance * SCROLL_DEACCEL_RATE;
    this->setContentOffset(Vec2(newX,newY));
    
    if ((fabsf(_scrollDistance.x) <= SCROLL_DEACCEL_DIST &&
         fabsf(_scrollDistance.y) <= SCROLL_DEACCEL_DIST) ||
        newY >= maxInset.y || newY <= minInset.y ||
        newX >= maxInset.x || newX <= minInset.x)
    {
        this->unschedule(schedule_selector(MyScrollView::deaccelerateScrolling));
        this->relocateContainer(true);
    }
    
//    //越界时
//    if ((fabsf(_scrollDistance.x) <= SCROLL_DEACCEL_DIST &&fabsf(_scrollDistance.y) <= SCROLL_DEACCEL_DIST)||newY >= maxInset.y || newY <= minInset.y ||newX >= maxInset.x || newX <= minInset.x)
//    {
//        this->unschedule(schedule_selector(MyScrollView::deaccelerateScrolling));
//        auto relocate = CallFunc::create([&](){
//            this->relocateContainer(true);
//        });
//        auto disappear = CallFunc::create([&](){
//            (this->m_callbackListener->*m_callback)();
//        });
//        this->runAction(Sequence::create(relocate,disappear,NULL));
//        return;
//    }
//    //速度极慢时
//    if ((fabsf(_scrollDistance.x) <= SCROLL_DEACCEL_DIST &&fabsf(_scrollDistance.y) <= SCROLL_DEACCEL_DIST))
//    {
//        this->unschedule(schedule_selector(MyScrollView::deaccelerateScrolling));
//        Vec2 newOffset = this->getContentOffset();
////        float Dt = 0.2;
//        float unit = (2.0/9.0)* VisibleSize.height;
//        float yoffset = this->getContentOffset().y - CONTENT_INIT_OFFSET;
//        float upIdx = ceilf(yoffset/unit);
//        float downIdx = floorf(yoffset/unit);
//        //中间位置
//        if (yoffset<((upIdx+downIdx)/2.0)*unit) {
//            newOffset = Point(0,downIdx*unit + CONTENT_INIT_OFFSET);
//        }
//        else{
//            newOffset = Point(0,upIdx*unit + CONTENT_INIT_OFFSET);
//        }
//        //越界检查
//        if(newOffset.y< -13.0*2.0/9.0*VisibleSize.height+ CONTENT_INIT_OFFSET) newOffset.y =-13.0*2.0/9.0*VisibleSize.height+ CONTENT_INIT_OFFSET;
//        if (newOffset.y > CONTENT_INIT_OFFSET+ CONTENT_INIT_OFFSET) newOffset.y =CONTENT_INIT_OFFSET+ CONTENT_INIT_OFFSET;
//        
//        MoveTo* scroll = MoveTo::create(fabsf(newOffset.y - this->getContentOffset().y)/VisibleSize.height*20, newOffset);
//        EaseSineOut * sineout = EaseSineOut::create(scroll);
//        auto expire = CallFunc::create([&](){
//            (this->m_callbackListener->*m_callback)();
//        });
//        _container->runAction(Sequence::create(sineout, expire, nullptr));
//    }

}
                                      
void Myscrollview::setCallBackFunc(Node* target, SEL_CallFunc callfun)
{
    m_callbackListener = target;
    m_callback = callfun;
}
                                       

AchievementUi::AchievementUi()
{
    
}

AchievementUi::~AchievementUi()
{
    
}
void AchievementUi::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
//    auto glview = Director::getInstance()->getOpenGLView();
//    glview->setDesignResolutionSize(1536,2048, ResolutionPolicy::FIXED_WIDTH);
    
}
void AchievementUi::onEnter()
{
    auto glview = Director::getInstance()->getOpenGLView();
    glview->setDesignResolutionSize(1536,2048, ResolutionPolicy::FIXED_WIDTH);
    Layer::onEnter();
}

void AchievementUi::onExit()
{
    Layer::onExit();
    auto glview = Director::getInstance()->getOpenGLView();
    glview->setDesignResolutionSize(768,1024, ResolutionPolicy::FIXED_WIDTH);
}

Scene* AchievementUi::createScene()
{
    Scene* scene = Scene::create();
    AchievementUi* layer = AchievementUi::create();
    scene->addChild(layer);
    return scene;
}

bool AchievementUi::init()
{
    auto glview = Director::getInstance()->getOpenGLView();
    glview->setDesignResolutionSize(1536,2048, ResolutionPolicy::FIXED_WIDTH);
    Layer::init();
    
    m_container = Layer::create();
    addFlowers();
    addScrollview();
    addMenuUI();
    setBottomItemDisappear();
    addListener();
    glview->setDesignResolutionSize(768,1024, ResolutionPolicy::FIXED_WIDTH);
    return true;
}

void AchievementUi::addFlowers()
{
    Menu* menu_flower = Menu::create();
    menu_flower->setPosition(Vec2::ZERO);
    
    Sprite* left = Sprite::create("stone01.png");
    Sprite* left_mid = Sprite::create("stone02.png");
    Sprite* mid = Sprite::create("stone03.png");
    Sprite* right_mid = Sprite::create("stone04.png");
    Sprite* right = Sprite::create("stone05.png");
    
    left->setPosition(Vec2(XL1,17.0*2.0/9.0*VisibleSize.height));
    left_mid->setPosition(Vec2(XL2,17.0*2.0/9.0*VisibleSize.height));
    mid->setPosition(Vec2(XL3,17.0*2.0/9.0*VisibleSize.height));
    right_mid->setPosition(Vec2(XL4,17.0*2.0/9.0*VisibleSize.height));
    right->setPosition(Vec2(XL5,17.0*2.0/9.0*VisibleSize.height));
    
    m_container->addChild(left);
    m_container->addChild(left_mid);
    m_container->addChild(mid);
    m_container->addChild(right_mid);
    m_container->addChild(right);
    
    for (int i = 0; i<17; i++) {
        Sprite* spriteLeft = Sprite::create("stone01.png");
        Sprite* spriteRight = Sprite::create("stone05.png");
        if (i%2==0) {
            spriteLeft->setPosition(Vec2(XR1,(POSITION_RATIO*i)*VisibleSize.height));
            spriteRight->setPosition(Vec2(XR5,(POSITION_RATIO*i)*VisibleSize.height));
        }
        else{
            spriteLeft->setPosition(Vec2(XL1,(POSITION_RATIO*i)*VisibleSize.height));
            spriteRight->setPosition(Vec2(XL5,(POSITION_RATIO*i)*VisibleSize.height));
        }
        m_container->addChild(spriteLeft);
        m_container->addChild(spriteRight);
    }
    
    for (int i = 1; i<52; i++) {
        
        //第一列花
        if ((i-1)%3 == 0) {
            Sprite* flower_left = Sprite::create("stone02.png");
            MenuItemImage* item = MenuItemImage::create("lock.png","lock.png",CC_CALLBACK_1(AchievementUi::flowerClicked, this));
            if (i%10 == (int)CCRANDOM_0_1()*10) {
                item = MenuItemImage::create("flower_01.png","flower_01.png",CC_CALLBACK_1(AchievementUi::flowerClicked, this));
                Sprite* descr = Sprite::create("title_01.png");
                descr->setPosition(Vec2(-VisibleSize.width/40,VisibleSize.height/15));
                item->addChild(descr);
            }
            //上排，x较大
            if (((i-1)/3)%2 == 0) {
                flower_left->setPosition(Vec2(XR2,(POSITION_RATIO*((i-1)/3))*VisibleSize.height));
            }
            else//下排
            {
                flower_left->setPosition(Vec2(XL2,(POSITION_RATIO*((i-1)/3))*VisibleSize.height));
            }
            item->setPosition(flower_left->getPosition()+ITEM_OFFSET_L);
            menu_flower->addChild(item);
            m_container->addChild(flower_left);
        }
        //第二列花
        else if((i-2)%3==0){
            Sprite* flower_middle = Sprite::create("stone03.png");
            MenuItemImage* item = MenuItemImage::create("lock.png","lock.png",CC_CALLBACK_1(AchievementUi::flowerClicked, this));
            if (i%10 == (int)CCRANDOM_0_1()*10) {
                item = MenuItemImage::create("flower_02.png","flower_02.png",CC_CALLBACK_1(AchievementUi::flowerClicked, this));
                Sprite* descr = Sprite::create("title_01.png");
                descr->setPosition(Vec2(-VisibleSize.width/40,VisibleSize.height/15));
                item->addChild(descr);
            }
            //上排
            if (((i-2)/3)%2==0) {
                flower_middle->setPosition(Vec2(XR3,(POSITION_RATIO*((i-2)/3))*VisibleSize.height));
            }
            else//下排
            {
                flower_middle->setPosition(Vec2(XL3,(POSITION_RATIO*((i-2)/3))*VisibleSize.height));
            }
            item->setPosition(flower_middle->getPosition()+ITEM_OFFSET_M);
            menu_flower->addChild(item);
            m_container->addChild(flower_middle);
        }
        //第三列花
        else{
            Sprite* flower_right = Sprite::create("stone04.png");
            MenuItemImage* item = MenuItemImage::create("lock.png","lock.png",CC_CALLBACK_1(AchievementUi::flowerClicked, this));
            if (i%10 == (int)CCRANDOM_0_1()*10) {
                item = MenuItemImage::create("flower_01.png","flower_01.png",CC_CALLBACK_1(AchievementUi::flowerClicked, this));
                Sprite* descr = Sprite::create("title_01.png");
                descr->setPosition(Vec2(-VisibleSize.width/40,VisibleSize.height/15));
                item->addChild(descr);
            }
            //上排
            if (((i-3)/3)%2==0) {
                flower_right->setPosition(Vec2(XR4,(POSITION_RATIO*((i-3)/3))*VisibleSize.height));
            }
            else//下排
            {
                flower_right->setPosition(Vec2(XL4,(POSITION_RATIO*((i-3)/3))*VisibleSize.height));
            }
            item->setPosition(flower_right->getPosition()+ITEM_OFFSET_R);
            
            //最后一个特别处理
            if (i==51) {
                item = MenuItemImage::create("UI_FreePowerup_btn_close_normal.png","UI_FreePowerup_btn_close_preesed.png",CC_CALLBACK_1(AchievementUi::flowerClicked, this));
                item->setPosition(flower_right->getPosition()+Vec2(0,350.0/2048*VisibleSize.height));
                item->setScale(0.7);
            }
            
            menu_flower->addChild(item);
            m_container->addChild(flower_right);
        }
    }
    
    m_container->addChild(menu_flower,0,"menu_flower");
}

void AchievementUi::addScrollview()
{
    _scrollview = Myscrollview::create(Size(VisibleSize.width,VisibleSize.height),m_container);
    this->addChild(_scrollview);
    _scrollview->setDirection(GrowUp::MyScrollView::Direction::VERTICAL);
    _scrollview->setContentSize(Size(VisibleSize.width, VisibleSize.height*(2*17+1)/9));
    _scrollview->setContentOffset(Vec2(0,-13.0*2.0/9.0*VisibleSize.height + CONTENT_INIT_OFFSET));
    _scrollview->setBounceable(true);
    _scrollview->setDelegate(this);
    _scrollview->setCallBackFunc(this, callfunc_selector(AchievementUi::setBottomItemDisappear));

}

void AchievementUi::addMenuUI()
{
    auto backSp  = Sprite::create("back.png");
    auto backSp1  = Sprite::create("back.png");
  
    auto visibleSize = Director::getInstance()->getWinSize();
    backSp1->setColor(Color3B(255,255,0));
    auto  back1 = MenuItemSprite::create(backSp, backSp1);
    back1->setAnchorPoint(Vec2::ZERO);
    back1->setPosition(Vec2(visibleSize.width*0.04,60));

    
    auto  backCall = [&](Ref*)
    {
        Director::getInstance()->replaceScene(GameManager::getInstance()->getScenePageView());
    };
    back1->setCallback(backCall);
    
    Menu * menu1 = Menu::create();
    menu1->addChild(back1);
    
    menu1->setPosition(Vec2::ZERO);
    
    addChild(menu1);
//    Menu* menu_Ui = Menu::create();
//    menu_Ui->setPosition(Vec2::ZERO);
//    MenuItemImage* back = MenuItemImage::create("flowerpop-06.png", "flowerpop-06.png", CC_CALLBACK_1(AchievementUi::back, this));
//    back->setPosition(Vec2(VisibleSize.width/10,0.95*VisibleSize.height));
//    menu_Ui->addChild(back);
//    this->addChild(menu_Ui);
}

void AchievementUi::back(Ref *sender)
{
    MenuItemImage* back = static_cast<MenuItemImage*>(sender);
    back->runAction(Sequence::create(ScaleBy::create(0.1, 1.3), ScaleBy::create(0.1, 1.3)->cocos2d::ScaleBy::reverse(),NULL));
}

void AchievementUi::flowerClicked(cocos2d::Ref *sender)
{
    MenuItemImage* flower = static_cast<MenuItemImage*>(sender);
    MoveBy::ActionInterval* move = MoveBy::create(0.1, Vec2(0,-VisibleSize.height/100));
    flower->runAction(Sequence::create(move,move->reverse(),NULL));
    
}

void AchievementUi::addListener()
{
    EventListenerTouchOneByOne* mylistener = EventListenerTouchOneByOne::create();
    
    mylistener->onTouchBegan = [&](Touch* touch,Event* event){
        
        return true;
    };
    
    mylistener->onTouchMoved =[&](Touch* touch,Event* event){
        //滑动时所有的花都应该显示出来
        for(int i =0;i<51;i++)
        {
            Menu* menu = static_cast<Menu*>(m_container->getChildByName("menu_flower"));
            MenuItem* item = static_cast<MenuItem*>(menu->getChildren().at(i));
            item->setVisible(true);
        }
    };
    
    mylistener->onTouchCancelled = [&](Touch* touch,Event* event){

    };
    
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(mylistener, this);
}

void AchievementUi::setBottomItemDisappear()
{
    for(int i =0;i<51;i++)
    {
        Menu* menu = static_cast<Menu*>(m_container->getChildByName("menu_flower"));
        MenuItem* item = static_cast<MenuItem*>(menu->getChildren().at(i));
        Point pt = item->getPosition()+_scrollview->getContentOffset();
        if (pt.y < item->getBoundingBox().size.height/2&&pt.y>-VisibleSize.height*2.0/9.0) {
            item->setVisible(false);
        }
    }
    
}


void AchievementUi::scrollViewDidScroll(GrowUp::MyScrollView* view)
{
    
    log("%f,%f",view->getContentOffset().x,view->getContentOffset().y);
}

