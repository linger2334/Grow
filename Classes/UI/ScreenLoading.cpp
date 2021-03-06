//
//  ScreenLoading.cpp
//  Grow-UI
//
//  Created by 林纲 王 on 14-11-16.
//
//

#include "ScreenLoading.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"
using namespace ui;

ScreenLoading* ScreenLoading::createScene(int levelNum)
{
    ScreenLoading* ret = new ScreenLoading();
    if(ret&&ret->init(levelNum)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ScreenLoading::init(int levelNum)
{
    if(Scene::init()){
        ImageView* loadingLogo = ImageView::create("UI_LoadingLogo.png");
        loadingLogo->setPosition(Vec2(VisibleSize.width/2,VisibleSize.height/1.5));
        addChild(loadingLogo);
        
        Text* loadingLabel = Text::create("LOADING","Arial",34);
        loadingLabel->setPosition(Vec2(VisibleSize.width/2,VisibleSize.height/5));
        addChild(loadingLabel);
        
        ImageView* loadingBG = ImageView::create("UI_LoadingBG.png");
        loadingBG->setPosition(Vec2(VisibleSize.width/2,VisibleSize.height/4));
        addChild(loadingBG);
        
        _progressBar = ProgressTimer::create(Sprite::create("UI_LoadingBar.png"));
        _progressBar->setType(cocos2d::ProgressTimer::Type::BAR);
        _progressBar->setMidpoint(Vec2(0,0));
        _progressBar->setBarChangeRate(Vec2(1,0));
        _progressBar->setPosition(VisibleSize.width/2, VisibleSize.height/4);
        addChild(_progressBar);

        _levelNum = levelNum;
        
        return true;
    }
    
    return false;
}

void ScreenLoading::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    int levelNum = _levelNum;
    ActionInterval* progressTo = ProgressTo::create(1, 100);
    CallFunc* callBack = CallFunc::create([=](){
        auto manager =  GameManager::getInstance();
        manager->navigationToGameScene();
        //        int id = manager->getPauseGameSceneLevel();
        //        if (id != levelNum) {
        //            if(manager->isHasRunningGame())
        //            {
        //                manager->releaseGameScene();
        //            }
        //            manager->navigationToGameScene(levelNum);
        //        }
        //        if (id == levelNum) {
        //            manager->navigationToPasueGameScene();
        //        }
    });
    _progressBar->runAction(Sequence::create(progressTo,callBack, NULL));
}

void ScreenLoading::loadingCallback(cocos2d::Texture2D *texture)
{

}


