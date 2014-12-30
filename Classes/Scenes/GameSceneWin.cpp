#include "GameSceneWin.h"
#include "GameManager.h"
#include "ScreenSelectLevel.h"

using namespace ui;

const char* kDefaultTextFontName = "PMingLiU-CN.ttf";
const int kDefaultTextFontSize = 30;

Scene* GameSceneWin::createScene()
{
    auto scene = Scene::create();
    auto layer = GameSceneWin::create();
    scene->addChild(layer);
    return scene;
}

#include "LevelManager.h"
bool GameSceneWin::init()
{
    if(Layer::init()){
        ValueMap levelData = FileUtils::getInstance()->getValueMapFromFile(FileUtils::getInstance()->getWritablePath()+"StatisticsData.plist");
        if(LevelManager::getInstance()->_levelId < 1 ||
           LevelManager::getInstance()->_levelId > 4)
        {
            GameManager::getInstance()->navigationTo(ScenePageView);
        }
        ValueMap currentLevelData = levelData.at(StringUtils::format("level%d",LevelManager::getInstance()->_levelId)).asValueMap();
        float defaultScale = VisibleSize.height/DefiniteSize.height;
        //图片相关
        ImageView* background = ImageView::create("UI_WinBackground.jpg");
        background->setScale(defaultScale);
        background->setAnchorPoint(Vec2(0.5, 0));
        background->setPosition(Vec2(VisibleSize.width/2,0));
        addChild(background);
        
        ImageView* hillBack = ImageView::create("UI_HillBack.png");
        hillBack->setScale(defaultScale);
        hillBack->setAnchorPoint(Vec2(0.5,0));
        hillBack->setPosition(Vec2(VisibleSize.width/2,0));
        addChild(hillBack);
        
        ImageView* plant = ImageView::create("UI_WinPlant.png");
        plant->setScale(defaultScale);
        plant->setPosition(Vec2(VisibleSize.width/2,0.16*VisibleSize.height));
        addChild(plant);
        
        ImageView* hillFront = ImageView::create("UI_HillFront.png");
        hillFront->setScale(defaultScale);
        hillFront->setAnchorPoint(Vec2(0.5,0));
        hillFront->setPosition(Vec2(VisibleSize.width/2,0));
        addChild(hillFront);

        ImageView* logo = ImageView::create("UI_WinLogo.png");
        logo->setScale(defaultScale);
        logo->setPosition(Vec2(VisibleSize.width/2,0.85*VisibleSize.height));
        addChild(logo);
        //文字相关
        std::vector<Node*> textLabels;
        _textView = ui::ScrollView::create();
        _textView->setContentSize(cocos2d::Size(DefiniteSize.height*0.4,DefiniteSize.height*0.42));
        _textView->setInnerContainerSize(Size(_textView->getContentSize().width, 3.47*_textView->getContentSize().height));
        _textView->setAnchorPoint(Vec2(0.5,1));
        _textView->setPosition(Vec2(VisibleSize.width/2,0.78*VisibleSize.height));
        _textView->setLayoutType(cocos2d::ui::Layout::Type::VERTICAL);
        
        Text* label_days = Text::create(StringUtils::format("感謝你陪伴了我%d天",currentLevelData.at("days").asInt()), kDefaultTextFontName, kDefaultTextFontSize);
        Text* label_hours = Text::create(StringUtils::format("     %4d小時",currentLevelData.at("hours").asInt()), kDefaultTextFontName, kDefaultTextFontSize);
        Text* label_minutes = Text::create(StringUtils::format("     %4d分鐘",currentLevelData.at("minutes").asInt()), kDefaultTextFontName, kDefaultTextFontSize);
        Text* label_flame = Text::create("一起遇到了不同的光", kDefaultTextFontName, kDefaultTextFontSize);
        textLabels.push_back(label_days);
        textLabels.push_back(label_hours);
        textLabels.push_back(label_minutes);
        textLabels.push_back(label_flame);
        //光芒
        Layout* flamePanel = Layout::create();
        flamePanel->setLayoutType(cocos2d::ui::Layout::Type::HORIZONTAL);
        ImageView* flameWhite = ImageView::create("Flame_White.png");
        ImageView* flameOrange = ImageView::create("Flame_Orange.png");
        ImageView* flameBlue = ImageView::create("Flame_Blue.png");
        ImageView* flameViolet = ImageView::create("Flame_Violet.png");
        flamePanel->addChild(flameWhite);
        flamePanel->addChild(flameOrange);
        flamePanel->addChild(flameBlue);
        flamePanel->addChild(flameViolet);
        flamePanel->setContentSize(cocos2d::Size(flameWhite->getBoundingBox().size.width*4,flameWhite->getBoundingBox().size.height));
        
        Layout* collectedPanel = Layout::create();
        collectedPanel->setContentSize(Size(flamePanel->getContentSize().width,flamePanel->getContentSize().height/2));
        cocos2d::Size panelSize = collectedPanel->getBoundingBox().size;
        Text* collectedWhite = Text::create(StringUtils::format("%d",currentLevelData.at("totalFlame_White").asInt()),kDefaultTextFontName, kDefaultTextFontSize);
        collectedWhite->setPosition(Vec2(1.0/8*panelSize.width,panelSize.height/2));
        Text* collectedOrange = Text::create(StringUtils::format("%d",currentLevelData.at("totalFlame_Orange").asInt()), kDefaultTextFontName, kDefaultTextFontSize);
        collectedOrange->setPosition(Vec2(3.0/8*panelSize.width,panelSize.height/2));
        Text* collectedBlue = Text::create(StringUtils::format("%d",currentLevelData.at("totalFlame_Blue").asInt()), kDefaultTextFontName, kDefaultTextFontSize);
        collectedBlue->setPosition(Vec2(5.0/8*panelSize.width, panelSize.height/2));
        Text* collectedViolet = Text::create(StringUtils::format("%d",currentLevelData.at("totalFlame_Violet").asInt()), kDefaultTextFontName, kDefaultTextFontSize);
        collectedViolet->setPosition(Vec2(7.0/8*panelSize.width,panelSize.height/2));
        collectedPanel->addChild(collectedWhite);
        collectedPanel->addChild(collectedOrange);
        collectedPanel->addChild(collectedBlue);
        collectedPanel->addChild(collectedViolet);
        textLabels.push_back(flamePanel);
        textLabels.push_back(collectedPanel);
        //
        Text* label_distance = Text::create(StringUtils::format("成長了   %f   的距離",currentLevelData.at("totalDistance").asFloat()), kDefaultTextFontName, kDefaultTextFontSize);
        Text* label_failure = Text::create(StringUtils::format("  經歷了%d次失敗",currentLevelData.at("failures").asInt()), kDefaultTextFontName, kDefaultTextFontSize);
        Text* label_unlock = Text::create(StringUtils::format("  結出%d朵花",currentLevelData.at("collectionVolume").asInt()), kDefaultTextFontName, kDefaultTextFontSize);
        Text* label_stars = Text::create("陪伴我一直到看到這片星海", kDefaultTextFontName, kDefaultTextFontSize);
        textLabels.push_back(label_distance);
        textLabels.push_back(label_failure);
        textLabels.push_back(label_unlock);
        textLabels.push_back(label_stars);
        //
        Text* label_comment1 = Text::create("或許，", kDefaultTextFontName, 1.5*kDefaultTextFontSize);
        Text* label_comment2 = Text::create("生活或有悲傷，", kDefaultTextFontName, 1.5*kDefaultTextFontSize);
        Text* label_comment3 = Text::create("卻仍不忘前行，", kDefaultTextFontName, 1.5*kDefaultTextFontSize);
        Text* label_comment4 = Text::create("祝平安，如願。", kDefaultTextFontName, 1.5*kDefaultTextFontSize);
        textLabels.push_back(label_comment1);
        textLabels.push_back(label_comment2);
        textLabels.push_back(label_comment3);
        textLabels.push_back(label_comment4);
        
        //最后的按钮
        Button* CH = Button::create("UI_StartButton.png");
        CH->setPressedActionEnabled(false);
        CH->setScale9Enabled(true);
        ActionInterval* rotate = RotateBy::create(5, 180);
        CH->runAction(RepeatForever::create(rotate));
        CH->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventType){
            if (eventType == Widget::TouchEventType::ENDED) {
                Scene* scene = ScreenSelectLevel::create();
                GameManager::getInstance()->navigationTo(scene);
            }
        });
        
        //
        LinearLayoutParameter* defaultLayoutParameter = LinearLayoutParameter::create();
        defaultLayoutParameter->setGravity(cocos2d::ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
        defaultLayoutParameter->setMargin(cocos2d::ui::Margin(0,10,0,10));
        log("%f,%f",label_comment1->getContentSize().width,label_comment1->getContentSize().height);
        for (int i = 0; i<42; i++) {
            Layout* placeHolder = Layout::create();
            placeHolder->setContentSize(cocos2d::Size(150,10));
            _textView->addChild(placeHolder);
        }

        for (int i = 0; i<textLabels.size(); i++) {
            Widget* label = dynamic_cast<Widget*>(textLabels.at(i));
            label->setLayoutParameter(defaultLayoutParameter);
            _textView->addChild(label);
        }
        
        LinearLayoutParameter* CHLayoutParameter = static_cast<LinearLayoutParameter*>(defaultLayoutParameter->clone());
        CHLayoutParameter->setMargin(cocos2d::ui::Margin(0,50,0,50));
        CH->setLayoutParameter(CHLayoutParameter);
        _textView->addChild(CH);
        
        startPosition = _textView->getInnerContainer()->getPosition();
        _textView->setTouchEnabled(false);
        addChild(_textView);
        addAnimation();
        return true;
    }

    return false;
}

void GameSceneWin::addAnimation()
{
    Layout* contanier = _textView->getInnerContainer();
    ActionInterval* moveUp = MoveTo::create(-contanier->getPositionY()/50.0, Vec2(0,0));
    contanier->runAction(moveUp);
    
    EventListenerTouchOneByOne* myListener = EventListenerTouchOneByOne::create();
    myListener->onTouchBegan = [=](Touch* touch,Event* event){
        if(_textView->getBoundingBox().containsPoint(touch->getLocation())){
            contanier->stopAllActions();
            return true;
        }
        return false;
    };
    myListener->onTouchMoved = [=](Touch* touch,Event* event){
        float destY = contanier->getPositionY() + touch->getDelta().y;
        if (destY >= startPosition.y && destY <= 0) {
            contanier->setPositionY(destY);
        }
        
    };
    myListener->onTouchEnded = [=](Touch* touch,Event* event){
        contanier->runAction(MoveTo::create(-contanier->getPositionY()/50.0, Vec2(0,0)));
    };
    myListener->onTouchCancelled = [=](Touch* touch,Event* event){
        contanier->runAction(MoveTo::create(-contanier->getPositionY()/50.0, Vec2(0,0)));};
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, _textView);
    
}

void GameSceneWin::addMenuUI()
{
    Button* CH = Button::create("UI_StartButton.png");
    CH->setScale(VisibleSize.height/DefiniteSize.height);
    CH->setPosition(Vec2(VisibleSize.width/2,0.4*VisibleSize.height));
    CH->setPressedActionEnabled(false);
    CH->setScale9Enabled(true);
    ActionInterval* rotate = RotateBy::create(5, 180);
    CH->runAction(RepeatForever::create(rotate));
    CH->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventType){
        if (eventType == Widget::TouchEventType::ENDED) {
            Scene* scene = ScreenSelectLevel::create();
            GameManager::getInstance()->navigationTo(scene);
        }
        
    });
    
    addChild(CH);
}
