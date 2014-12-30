#include "AppDelegate.h"
#include "GameLayerMap.h"
#include "GameSceneMain.h"
#include "GameRuntime.h"
#include "GameManager.h"
#include "LevelsMenu.h"
#include "StatisticsData.h"
USING_NS_CC;


AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Grow");
        director->setOpenGLView(glview);
    }
    if (director->getWinSize().width>768) {
        glview->setDesignResolutionSize(768, 1024, ResolutionPolicy::FIXED_HEIGHT);
    }
    
    GameRuntime::getInstance()->initGameRuntimeInfo();
    GameManager::getInstance()->initGameManagerInfo();
    GameRunningInfo::getInstance()->initGameRunningInfo();
    
    director->setDisplayStats(true);

    director->setAnimationInterval(1.0 / 60);
    auto menu = LevelsMenu::createScene();
    
    director->runWithScene(menu);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    if (StatisticsData::isHasRunningLayer()) {
        StatisticsData::getRunningLayer()->saveData();
        StatisticsData::getRunningLayer()->saveBothFiles();
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

}
