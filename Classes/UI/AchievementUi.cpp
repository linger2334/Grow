//
//  AchievementUi.cpp
//  GrowUp
//
//  Created by wlg on 8/28/14.
//
//
#include "AchievementUi.h"
#include "FlowerDescription.h"
#include "ScreenSelectLevel.h"
#include "StatisticsData.h"
using namespace ui;

#define SCROLL_DEACCEL_RATE  0.95f  
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.15f
#define INSET_RATIO          0.05f   //默认0.2
#define MOVE_INCH            7.0f/160.0f
#define BOUNCE_BACK_FACTOR   0.05f  //默认0.35

#define POSITION_RATIO 4.0 - 7.0/18.0 - 2.0/9.0
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
#define STONE_MIDDLE_OFFSET -0.02*VisibleSize.width
#define ITEM_OFFSET_L Vec2(0,400.0/2048*VisibleSize.height-40.0/1024.0*VisibleSize.height+ getIphonefactor())
#define ITEM_OFFSET_L_Y_ADD -0.015*VisibleSize.height
#define ITEM_OFFSET_M Vec2(0,320.0/2048*VisibleSize.height + getIphonefactor())
#define ITEM_OFFSET_M_Y_ADD  -0.015*VisibleSize.height
#define ITEM_OFFSET_R Vec2(0,400.0/2048*VisibleSize.height-40.0/1024.0*VisibleSize.height + getIphonefactor())
#define ITEM_OFFSET_R_Y_ADD -0.015*VisibleSize.height
#define CONTENT_INIT_OFFSET 0.0 
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
    isEndScrolling = false;
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
        this->isEndScrolling = true;
        this->unschedule(schedule_selector(MyScrollView::deaccelerateScrolling));
        this->relocateContainer(true);
    }
    
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

Scene* AchievementUi::createScene()
{
    Scene* scene = Scene::create();
    AchievementUi* layer = AchievementUi::create();
    scene->addChild(layer);
    return scene;
}

bool AchievementUi::init()
{
    Layer::init();
    
    m_container = Layer::create();
    addFlowers();
    addScrollview();
    addMenuUI();
    setBottomItemDisappear();
    
    scheduleUpdate();
    return true;
}
//灰度
bool gray::init()
{
    auto glprogram = GLProgram::createWithFilenames("ccShader_PositionTexture.vert", "ccShader_PositionTexture.frag");http://www.cocoachina.com/ask/app/webroot/upload/201405/20140527104152_44436.jpg
    auto _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _glprogramstate->retain();
    //setGLProgramState(_glprogramstate);
    
    return true;
};


void AchievementUi::addFlowers()
{
    Menu* menu_flower = Menu::create();
    menu_flower->setPosition(Vec2::ZERO);
    
    Sprite* left = Sprite::create("UI_AchiStone_L.png");
    Sprite* left_mid = Sprite::create("UI_AchiStone_LM.png");
    Sprite* mid = Sprite::create("UI_AchiStone_M.png");
    Sprite* right_mid = Sprite::create("UI_AchiStone_RM.png");
    Sprite* right = Sprite::create("UI_AchiStone_R.png");
    
    left->setPosition(Vec2(XL1,ceil(FLOWER_COUNT/3.0)*2.0/9.0*VisibleSize.height + 1.0/18*VisibleSize.height));
    left_mid->setPosition(Vec2(XL2,ceil(FLOWER_COUNT/3.0)*2.0/9.0*VisibleSize.height + 1.0/18*VisibleSize.height));
    mid->setPosition(Vec2(XL3,ceil(FLOWER_COUNT/3.0)*2.0/9.0*VisibleSize.height + 1.0/18*VisibleSize.height));
    right_mid->setPosition(Vec2(XL4,ceil(FLOWER_COUNT/3.0)*2.0/9.0*VisibleSize.height + 1.0/18*VisibleSize.height));
    right->setPosition(Vec2(XL5,ceil(FLOWER_COUNT/3.0)*2.0/9.0*VisibleSize.height + 1.0/18*VisibleSize.height));
    
    m_container->addChild(left);
    m_container->addChild(left_mid);
    m_container->addChild(mid);
    m_container->addChild(right_mid);
    m_container->addChild(right);
    
    for (int i = 0; i<ceil(FLOWER_COUNT/3.0); i++) {
        Sprite* spriteLeft = Sprite::create("UI_AchiStone_L.png");
        Sprite* spriteRight = Sprite::create("UI_AchiStone_R.png");
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
    
    _flowersInfo = FileUtils::getInstance()->getValueVectorFromFile("flowersinfo.plist");
    __Array* flowerUnlockInfo;
    if (StatisticsData::isHasRunningLayer()) {
        flowerUnlockInfo = StatisticsData::getRunningLayer()->getFlowerUnlockInfo();
    }else{
        std::string path = FileUtils::getInstance()->getWritablePath() +"StatisticsData.plist";
        if (FileUtils::getInstance()->isFileExist(path)) {
            __Dictionary* dict = __Dictionary::createWithContentsOfFile(path.c_str());
            flowerUnlockInfo = static_cast<__Array*>(dict->objectForKey("flowerUnlock"));
        }else{
            flowerUnlockInfo = __Array::createWithCapacity(FLOWER_COUNT);
            for (int i =0; i<FLOWER_COUNT; i++) {
                flowerUnlockInfo->addObject(__Bool::create(false));
            }
        }
        
    }
    
    Sprite* stone;
    MenuItemImage* item;
    std::string flowImageName;
    for (int i = 0; i<FLOWER_COUNT+1; i++) {
        flowImageName = _flowersInfo.at(i).asValueMap().at("imagename").asString();
        bool isUnlock = false;
        if (i<FLOWER_COUNT) {
            isUnlock = static_cast<__String*>(flowerUnlockInfo->getObjectAtIndex(i))->boolValue();
        }
        if(isUnlock){
            item = MenuItemImage::create(flowImageName,flowImageName,CC_CALLBACK_1(AchievementUi::flowerClicked, this));
            item->setScale(0.5);
        }else{
            item = MenuItemImage::create("UI_AchiSeedling.png","UI_AchiSeedling.png",CC_CALLBACK_1(AchievementUi::flowerClicked, this));
            item->setName("seedling");
            //添加浮动
            Sprite* criclefloat = Sprite::create("UI_AchiFloat.png");
            criclefloat->setPosition(0.95*item->getBoundingBox().size.width,0.75*item->getBoundingBox().size.height);
            criclefloat->runAction(RepeatForever::create(Sequence::create(MoveBy::create(5,Vec2(0,25)),MoveBy::create(5, Vec2(0,-25)), NULL)));
            item->addChild(criclefloat);
//    
//        }else{
//            item = MenuItemImage::create("UI_AchiLock.png","UI_AchiLock.png",CC_CALLBACK_1(AchievementUi::flowerClicked, this));
//            item->setEnabled(false);
        }
        
        extern GLProgramState*  _glprogramstate;
        item->getNormalImage()->setGLProgramState(_glprogramstate);
        item->getSelectedImage()->setGLProgramState(_glprogramstate);
        
        //第一列花
        if (i%3 == 0) {
            stone = Sprite::create("UI_AchiStone_LM.png");
            
            //上排，x较大
            if ((i/3)%2 == 0) {
                stone->setPosition(Vec2(XR2,(POSITION_RATIO*(i/3))*VisibleSize.height));
            }
            else//下排
            {
                stone->setPosition(Vec2(XL2,(POSITION_RATIO*(i/3))*VisibleSize.height));
            }
            item->setPosition(stone->getPosition()+ITEM_OFFSET_L);
            if (!isUnlock) {
                item->setPositionY(item->getPositionY()+ITEM_OFFSET_L_Y_ADD);
            }else{
                adjustFlowerPos(item, i);
            }

        }
        //第二列花
        else if((i-1)%3==0){
            stone = Sprite::create("UI_AchiStone_M.png");
            //上排
            if (((i-1)/3)%2==0) {
                stone->setPosition(Vec2(XR3,(POSITION_RATIO*((i-1)/3))*VisibleSize.height+STONE_MIDDLE_OFFSET));
            }
            else//下排
            {
                stone->setPosition(Vec2(XL3,(POSITION_RATIO*((i-1)/3))*VisibleSize.height+STONE_MIDDLE_OFFSET));
            }
            item->setPosition(stone->getPosition()+ITEM_OFFSET_M);
            if (!isUnlock) {
                item->setPositionY(item->getPositionY()+ITEM_OFFSET_M_Y_ADD);
            }else{
                adjustFlowerPos(item, i);
            }

        }
        //第三列花
        else{
            stone = Sprite::create("UI_AchiStone_RM.png");
            //上排
            if (((i-2)/3)%2==0) {
                stone->setPosition(Vec2(XR4,(POSITION_RATIO*((i-2)/3))*VisibleSize.height));
            }
            else//下排
            {
                stone->setPosition(Vec2(XL4,(POSITION_RATIO*((i-2)/3))*VisibleSize.height));
            }
            item->setPosition(stone->getPosition()+ITEM_OFFSET_R);
            if (!isUnlock) {
                item->setPositionY(item->getPositionY()+ITEM_OFFSET_R_Y_ADD);
            }else{
                adjustFlowerPos(item, i);
            }
        }
        
        if (i == FLOWER_COUNT) {
            item->setVisible(false);
        }
        
        menu_flower->addChild(item);
        item->setTag(i);
        m_container->addChild(stone);
    }

    m_container->addChild(menu_flower,0,"menu_flower");
}

void AchievementUi::addScrollview()
{
    _scrollview = Myscrollview::create(Size(VisibleSize.width,VisibleSize.height),m_container);
    this->addChild(_scrollview);
    _scrollview->setDirection(GrowUp::MyScrollView::Direction::VERTICAL);
    _scrollview->setContentSize(Size(VisibleSize.width, VisibleSize.height*(2*ceil(FLOWER_COUNT/3.0)+1)/9));
    _scrollview->setContentOffset(Vec2(0,(4-ceil(FLOWER_COUNT/3.0))*2.0/9.0*VisibleSize.height + CONTENT_INIT_OFFSET));
    _scrollview->setBounceable(true);
    _scrollview->setDelegate(this);
    _scrollview->setCallBackFunc(this, callfunc_selector(AchievementUi::setBottomItemDisappear));
    _scrollview->isEndScrolling = true;
}

void AchievementUi::addMenuUI()
{
    Button* back = Button::create("UI_SelectLevelBack.png");
    back->setScale9Enabled(true);
    back->setPressedActionEnabled(false);
    back->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventType){
        if (eventType == Widget::TouchEventType::ENDED) {
            Scene* scene = ScreenSelectLevel::create();
            Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
        }
    });
    back->setPosition(Vec2(0.1*VisibleSize.width,0.0625*VisibleSize.height));
    addChild(back);
}

void AchievementUi::flowerClicked(cocos2d::Ref *sender)
{
    MenuItemImage* flower = static_cast<MenuItemImage*>(sender);
    if (flower->getName() == "seedling") {
//        popupSeedlingPage(flower->getTag());
    }else{
        auto scene =  FlowerDescription::createScene(flower->getTag());
        Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
    }
}

void AchievementUi::setBottomItemDisappear()
{
    Menu* menu = static_cast<Menu*>(m_container->getChildByName("menu_flower"));
    for(int i =0;i<FLOWER_COUNT;i++)
    {
        MenuItem* item = static_cast<MenuItem*>(menu->getChildren().at(i));
        Point pt = item->getPosition()+_scrollview->getContentOffset();
        if (pt.y < item->getBoundingBox().size.height/4&&pt.y>-VisibleSize.height*2.0/9.0) {
            item->setVisible(false);
        }
    }
}

void AchievementUi::popupSeedlingPage(int index)
{
    _seedlingPage = Layout::create();
    _seedlingPage->setContentSize(VisibleSize);
    _seedlingPage->setPosition(Vec2(0, 0));
    _seedlingPage->setTouchEnabled(true);
    addChild(_seedlingPage);
    
    ImageView* disk = ImageView::create("UI_SeedlingDisk.png");
    disk->setPosition(Vec2(_seedlingPage->getContentSize().width/2,_seedlingPage->getContentSize().height/2));
    _seedlingPage->addChild(disk);
    
    Size diskSize = disk->getBoundingBox().size;
    ImageView* outring = ImageView::create("UI_OutRing.png");
    outring->setPosition(Vec2(diskSize.width/2,diskSize.height/2));
    ActionInterval* rotate = RotateBy::create(40, 180);
    outring->runAction(RepeatForever::create(rotate));
    disk->addChild(outring);
    
    ImageView* flameblue = ImageView::create("UI_FlameBlue.png");
    flameblue->setPosition(Vec2(diskSize.width/2 - 100.0/440*diskSize.width,0.55*diskSize.height));
    disk->addChild(flameblue);
    ImageView* flamewhite = ImageView::create("UI_FlameWhite.png");
    flamewhite->setPosition(Vec2(diskSize.width/2 + 100.0/440*diskSize.width,0.55*diskSize.height));
    disk->addChild(flamewhite);
    ImageView* plant = ImageView::create("UI_SeedlingPlant.png");
    plant->setPosition(Vec2(diskSize.width/2,diskSize.height/2 - 100.0/440*diskSize.height));
    disk->addChild(plant);
    
    UserDefault* userDefault = UserDefault::getInstance();
    std::string collectedWhite = userDefault->getStringForKey("collectedWhite", "0");
    std::string collectedBlue = userDefault->getStringForKey("collectedBlue", "0");
    std::string collectedOrange = userDefault->getStringForKey("collectedOrange", "0");
    std::string collectedViolet = userDefault->getStringForKey("collectedViolet","0");
    int growHeight = userDefault->getIntegerForKey("growHeight", 0);
    
    ValueMap condition = _flowersInfo.at(index).asValueMap().at("condition").asValueMap();
    std::string neededWhite = condition.at("Flame_White").asString();
    std::string neededBlue = condition.at("Flame_Blue").asString();
    std::string neededOrange = condition.at("Flame_Orange").asString();
    std::string neededViolet = condition.at("Flame_Violet").asString();
    std::string neededHeight = StringUtils::format("%d",condition.at("Height").asInt() - growHeight);
    
    Text* flameblueLabel = Text::create(collectedBlue+"/"+neededBlue, "Arial", 24);
    flameblueLabel->setColor(Color3B::BLACK);
    flameblueLabel->setPosition(Vec2(flameblue->getPositionX(),flameblue->getPositionY()-flameblue->getBoundingBox().size.height));
    disk->addChild(flameblueLabel);
                                
    Text* flamewhiteLabel = Text::create(collectedWhite+"/"+neededWhite,"Arial",24);
    flamewhiteLabel->setColor(Color3B::BLACK);
    flamewhiteLabel->setPosition(Vec2(flamewhite->getPositionX(),flamewhite->getPositionY()-flamewhite->getBoundingBox().size.height));
    disk->addChild(flamewhiteLabel);
    
    Text* plantLabel = Text::create(neededHeight, "Arial", 24);
    plantLabel->setColor(Color3B::BLACK);
    plantLabel->setPosition(Vec2(plant->getPositionX(),plant->getPositionY()-plant->getBoundingBox().size.height));
    disk->addChild(plantLabel);
    
    Button* close = Button::create("UI_close.png");
    close->setPressedActionEnabled(false);
    close->setScale9Enabled(true);
    close->setPosition(Vec2(0.82*diskSize.width,0.91*diskSize.height));
    close->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventType){
        switch (eventType) {
            case Widget::TouchEventType::ENDED:
                _seedlingPage->removeFromParent();
                break;
                
            default:
                break;
        }
    });
    disk->addChild(close);
}

void AchievementUi::update(float dt)
{
    if (_scrollview->isEndScrolling && _scrollview->getContentOffset().y >= (4-ceil(FLOWER_COUNT/3.0))*2.0/9.0*VisibleSize.height + CONTENT_INIT_OFFSET && _scrollview->getContentOffset().y <=(4-ceil(FLOWER_COUNT/3.0))*2.0/9.0*VisibleSize.height + CONTENT_INIT_OFFSET + 0.06*VisibleSize.height) {
        setBottomItemDisappear();
    }else{
        Menu* menu = static_cast<Menu*>(m_container->getChildByName("menu_flower"));
        for(int i =0;i<FLOWER_COUNT;i++)
        {
            MenuItem* item = static_cast<MenuItem*>(menu->getChildren().at(i));
            item->setVisible(true);
        }
    }
}

void AchievementUi::adjustFlowerPos(cocos2d::MenuItemImage *flower, int index)
{
    float width = flower->getBoundingBox().size.width;
    float height = flower->getBoundingBox().size.height;
    Vec2 previousPos = flower->getPosition();
    switch (index) {
        case 2:
            flower->setPosition(previousPos + Vec2(-0.2*width,0.1*height));
            break;
        case 5:
            flower->setPosition(previousPos + Vec2(0, 0.1*height));
            break;
        case 6:
            flower->setPosition(previousPos + Vec2(0, 0.05*height));
            break;
        case 8:
            flower->setPosition(previousPos + Vec2(0, 0.1*height));
            break;
        case 10:
            flower->setPosition(previousPos + Vec2(0,0.05*height));
            break;
        case 11:
            flower->setPosition(previousPos + Vec2(0, 0.1*height));
            break;
        case 12:
            flower->setPosition(previousPos + Vec2(0,0.05*height));
            break;
        case 13:
            flower->setPosition(previousPos + Vec2(0,0.07*height));
            break;
        case 14:
            flower->setPosition(previousPos + Vec2(0, 0.05*height));
            break;
        case 15:
            flower->setPosition(previousPos + Vec2(0,0.03*height));
            break;
        default:
            break;
    }
}
