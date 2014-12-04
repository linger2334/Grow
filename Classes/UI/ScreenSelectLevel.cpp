//
//  ScreenSelectLevel.cpp
//  Grow-UI
//
//  Created by 林纲 王 on 14-11-16.
//
//

#include "ScreenSelectLevel.h"
#include "ScreenStart.h"
#include "ScreenLoading.h"
#include "AchievementUi.h"
#include "GameManager.h"
#include "GameShaders.h"
#include "LevelManager.h"
using namespace cocos2d::ui;

void MyPageView::handleReleaseLogic(Touch *touch)
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
        float boundary = pageWidth/9.0f;//可调节pageview翻页的左右灵敏度
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

bool ScreenSelectLevel::init()
{
    if (Scene::init()) {
        addPageView();
        addPointIndicator();
        addMenuUI();
        
        return true;
    }
    
    return false;
}

void ScreenSelectLevel::addPageView()
{
    _pageView = MyPageView::create();
    _pageView->setContentSize(VisibleSize);
    addChild(_pageView);

    ActionInterval* flicker = FadeTo::create(4, 30);
    ActionInterval* flicker_resverse = FadeTo::create(4, 40);
    ActionInterval* shine = RepeatForever::create(Sequence::createWithTwoActions(flicker, flicker_resverse));
    
    const char *titleFileName[] = {"UI_Spring.png","UI_Summer.png","UI_Autumn.png","UI_Winter.png"};
    
    for (int i = 0; i<LEVEL_COUNT; i++) {
        Layout* singlePage = Layout::create();
        singlePage->setContentSize(VisibleSize);
        //level button
        std::string levelname = StringUtils::format("UI_level%d",i+1);
        Sprite* image = Sprite::create(levelname+".png");
        image->setScale(VisibleSize.height/image->getContentSize().height);
        image->setAnchorPoint(Vec2(0.5,1));
        image->setPosition(Vec2(VisibleSize.width/2,VisibleSize.height));
        image->setFlippedY(true);
        
        if (i==3) {
            
             GLProgramState*  glprogramstate = GameShaders::getInstance()->getOneGrayShaderState();
            image->setGLProgramState(glprogramstate);
        }
        RenderTexture* newTexture = RenderTexture::create(VisibleSize.width, VisibleSize.height);
        newTexture->addChild(image);
        newTexture->beginWithClear(0, 0, 0, 0);
        image->visit();
        newTexture->end();
        newTexture->getSprite()->setVisible(false);
        singlePage->addChild(newTexture,-10);
        
        SpriteFrame* spriteFrame = SpriteFrame::createWithTexture(newTexture->getSprite()->getTexture(), Rect(0, 0, VisibleSize.width, VisibleSize.height));
        SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, levelname);
        
        Button* levelButton = Button::create();
        levelButton->loadTextureNormal(levelname,TextureResType::PLIST);
        levelButton->setScale9Enabled(true);
        levelButton->setPosition(Vec2(singlePage->getBoundingBox().size.width/2,singlePage->getBoundingBox().size.height/2));
        levelButton->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventType){
            switch (eventType) {
                case Widget::TouchEventType::ENDED:
                {
                    ssize_t index = _pageView->getCurPageIndex();
                    if(index == 3)
                    {
                        break;
                    }
                    auto manager =  GameManager::getInstance();
                    int id = manager->getPauseGameSceneLevel();
                    LevelManager::getInstance()->selectLevel(index+1);
                    manager->releaseGameScene();
                    auto scene = ScreenLoading::createScene(index+1);
                    manager->navigationTo(scene);
                    break;
                }
                    default:
                    break;
            }
        });
        singlePage->addChild(levelButton);
        
        //lamplight
        ImageView* lamplight = ImageView::create("UI_LampLight.png");
        lamplight->setOpacity(40);
        lamplight->setScale(levelButton->getScale());
        lamplight->setPosition(Vec2((i<2? 0.48 : 0.51)*singlePage->getContentSize().width,0.5*singlePage->getContentSize().height));
        lamplight->runAction(shine->clone());
        singlePage->addChild(lamplight);
        
        //particle effect
        ParticleSystemQuad* particleEffect = ParticleSystemQuad::create();
        particleEffect->setEmitterMode(cocos2d::ParticleSystem::Mode::GRAVITY);
        particleEffect->setPositionType(cocos2d::ParticleSystem::PositionType::RELATIVE);
        particleEffect->setSourcePosition(Vec2(VisibleSize.width/2,0.47*VisibleSize.height));
        particleEffect->setPosVar(Vec2(150,250));
        particleEffect->setTexture(Director::getInstance()->getTextureCache()->addImage("UI_particle_texture.png"));
        particleEffect->setGravity(Vec2(0,i==3? 0:-5));
        particleEffect->setSpeed(0);
        particleEffect->setSpeedVar(i==3? 0:15);
        particleEffect->setTangentialAccel(0);
        particleEffect->setRadialAccel(0);
        particleEffect->setTotalParticles(200);
        particleEffect->setDuration(-1);
        particleEffect->setLife(6*VisibleSize.height/DefiniteSize.height);
        particleEffect->setLifeVar(0);
        particleEffect->setAngle(70);//发射角，数学中的角度
        particleEffect->setAngleVar(140);
        particleEffect->setStartSize(2);
        particleEffect->setStartSizeVar(5);
        particleEffect->setEndSize(20);
        particleEffect->setEndSizeVar(50);
        particleEffect->setStartColor(Color4F(1, 1, 1, 1));
        particleEffect->setStartColorVar(Color4F(0,0,0,1));
        particleEffect->setEndColor(Color4F(1,1,1,0));
        particleEffect->setEndColorVar(Color4F(0,0,0,0));
        particleEffect->setStartSpin(0);
        particleEffect->setStartSpinVar(60);
        particleEffect->setEndSpin(120);
        particleEffect->setEndSpinVar(240);
        particleEffect->setEmissionRate(particleEffect->getTotalParticles()/particleEffect->getLife());
        BlendFunc blendfunc = {GL_SRC_ALPHA,GL_ONE};
        particleEffect->setBlendFunc(blendfunc);
        singlePage->addChild(particleEffect);
        
        //title
        ImageView* title = ImageView::create(titleFileName[i]);
        title->setScale(levelButton->getScale());
        title->setPosition(Vec2(singlePage->getContentSize().width/2,singlePage->getContentSize().height/1.2));
        singlePage->addChild(title);
        
        _pageView->addPage(singlePage);
    }
    
    _pageView->addEventListener([&](Ref* sender,PageView::EventType eventType){
        if (eventType == PageView::EventType::TURNING) {
            _point->setPosition(_pointPos[_pageView->getCurPageIndex()]);
        }
    });
    
}

void ScreenSelectLevel::addPointIndicator()
{
    ImageView* pointIndicator = ImageView::create("UI_PointBackground.png");
    pointIndicator->setPosition(Vec2(VisibleSize.width/2,VisibleSize.height/16));
    addChild(pointIndicator);

    _pointPos.push_back(Vec2(pointIndicator->getContentSize().width/2-57,pointIndicator->getContentSize().height/2+4));
    _pointPos.push_back(Vec2(pointIndicator->getContentSize().width/2-20,pointIndicator->getContentSize().height/2-4));
    _pointPos.push_back(Vec2(pointIndicator->getContentSize().width/2+17,pointIndicator->getContentSize().height/2+3));
    _pointPos.push_back(Vec2(pointIndicator->getContentSize().width/2+56,pointIndicator->getContentSize().height/2-3));
    
    _point = ImageView::create("UI_PointSelected.png");
    _point->setPosition(_pointPos[0]);
    pointIndicator->addChild(_point,-1);
}

void ScreenSelectLevel::addMenuUI()
{
    //
    Button* back = Button::create("UI_SelectLevelBack.png");
    back->setPosition(Vec2(0.1*VisibleSize.width,VisibleSize.height/16));
    back->setScale9Enabled(true);
    back->setPressedActionEnabled(false);
    back->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventType){
        if (eventType == Widget::TouchEventType::ENDED) {
            GameManager::getInstance()->navigationTo(SceneLogo);
        }
    });
    addChild(back);
    //
    Button* flowerIcon = Button::create("UI_FlowerIcon.png");
    flowerIcon->setScale9Enabled(true);
    flowerIcon->setPressedActionEnabled(false);
    flowerIcon->setPosition(Vec2(0.9*VisibleSize.width,VisibleSize.height/16));
    flowerIcon->addTouchEventListener([&](Ref*,Widget::TouchEventType eventType){
        if (eventType == Widget::TouchEventType::ENDED) {
            GameManager::getInstance()->navigationTo(SceneFlower);
        }
    });
    addChild(flowerIcon);
}




