//
//  ScreenStart.cpp
//  Grow-UI
//
//  Created by wlg on 11/13/14.
//
//

#include "ScreenStart.h"
#include "ScreenSelectLevel.h"
#include "CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "ScreenEnergy.h"

using namespace cocos2d::ui;

extern GLProgramState*  _glprogramstate = nullptr;

Scene* ScreenStart::createScene()
{
    Scene* scene = Scene::create();
    scene->addChild(ScreenStart::create());
    return scene;
}

bool ScreenStart::init()
{
    //灰度设定
    auto glprogram = GLProgram::createWithFilenames("ccShader_PositionTextureColor_noMVP.vert", "ccShader_PositionTextureColor_noMVP.frag");
    glprogram->retain();
    _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _glprogramstate->retain();
    
    if (Layer::init()) {
        
        initBeginnerInfo();
        //背景图片
        Sprite* background = Sprite::create("UI_StartBackground.jpg");
        background->setPosition(VisibleSize.width/2,VisibleSize.height/2);
        background->setScale(VisibleSize.height/background->getContentSize().height);
        this->addChild(background);

        //适配分辨率
        float logoScale;
        //如果是5或6 将logo变下 背景变大
        if(VisibleSize.height>1024&&VisibleSize.height<2048){
            logoScale = 0.9;
        }
        else{
            logoScale = 1;
        }
        
        //创建logo
        Sprite* logo = Sprite::create("UI_logo.png");
        logo->setScale(logoScale);
        logo->setPosition(VisibleSize.width/2, VisibleSize.height/2+VisibleSize.height/8);
        logo->runAction(ScaleBy::create(20, 1.1));
        this->addChild(logo);
        
        //add startbutton
        Button* startbutton = Button::create("UI_StartButton.png");
        startbutton->setPosition(Vec2(VisibleSize.width/2,VisibleSize.height/2.4));
        startbutton->setPressedActionEnabled(false);
        startbutton->setScale9Enabled(true);
        
        ActionInterval* rotate = RotateBy::create(5,180);
        startbutton->runAction(RepeatForever::create(rotate));
        startbutton->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventtype)
                                      {
                                          Button* btn = static_cast<Button*>(sender);
                                          btn->setTouchEnabled(false);
                                          Director::getInstance()->replaceScene(TransitionFade::create(2, ScreenSelectLevel::create()));
                                      });
        this->addChild(startbutton);
        
        //添加主界面的about按钮
        Button* aboutButton = Button::create("UI_about.png");
        aboutButton->setPosition(Vec2(30.0/768*VisibleSize.width+aboutButton->getBoundingBox().size.width/2, VisibleSize.height-30.0/1024*VisibleSize.height - aboutButton->getBoundingBox().size.height/2));
        aboutButton->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventtype){
            //
            if (eventtype == Widget::TouchEventType::ENDED) {
                this->getParent()->addChild(ScreenEnergy::create());
            }
            
        });
        addChild(aboutButton);
        
        //粒子效果
        ParticleSystemQuad* particleEffect = ParticleSystemQuad::create();
        particleEffect->setEmitterMode(cocos2d::ParticleSystem::Mode::GRAVITY);
        particleEffect->setPositionType(cocos2d::ParticleSystem::PositionType::RELATIVE);
        particleEffect->setSourcePosition(Vec2(VisibleSize.width/2,0.5*VisibleSize.height-450));
        particleEffect->setPosVar(Vec2(150,250));
        particleEffect->setTexture(Director::getInstance()->getTextureCache()->addImage("UI_particle_texture.png"));
        particleEffect->setGravity(Vec2(0,5));
        particleEffect->setSpeed(0);
        particleEffect->setSpeedVar(15);
        particleEffect->setTangentialAccel(0);
        particleEffect->setRadialAccel(0);
        particleEffect->setTotalParticles(200);
        particleEffect->setDuration(-1);
        particleEffect->setLife(3*VisibleSize.height/DefiniteSize.height);
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
        addChild(particleEffect);
        
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("UI_backgroundmusic.mp3");
        //开始播放背景音乐，true表示循环
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("UI_backgroundmusic.mp3",true);
        //设置音量0.0-1.0
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
        
        //预加载选关界面
        TextureCache* textureCache = Director::getInstance()->getTextureCache();
        textureCache->addImage("UI_level1.png");
        textureCache->addImage("UI_level2.png");
        textureCache->addImage("UI_level3.png");
        textureCache->addImage("UI_level4.png");
        textureCache->addImage("UI_Spring.png");
        textureCache->addImage("UI_Summer.png");
        textureCache->addImage("UI_Autumn.png");
        textureCache->addImage("UI_Winter.png");
        textureCache->addImage("UI_LampLight.png");
        
        return true;
    }
    
    return false;
}

void ScreenStart::initBeginnerInfo()
{
    UserDefault* userDefault = UserDefault::getInstance();
    if (userDefault->getBoolForKey("isBeginner", true)) {
        userDefault->setIntegerForKey("collectedWhite", 0);
        userDefault->setIntegerForKey("collectedBlue", 0);
        userDefault->setIntegerForKey("collectedViolet", 0);
        userDefault->setIntegerForKey("collectedOrange", 0);
        userDefault->setIntegerForKey("growHeight", 0);
        userDefault->setIntegerForKey("seedlingIndex", 0);
        userDefault->setIntegerForKey("accumulativeTime_Natural", 0);
        
    }
    
}