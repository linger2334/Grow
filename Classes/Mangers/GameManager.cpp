#include "GameManager.h"
#include "GameSceneMain.h"
#include "GameRunningInfo.h"
#include "LevelManager.h"
#include "GameRunningManager.h"
#include "GameSceneWin.h"
#include "LayerItem.h"
#include "ScreenLoading.h"
#include "ScreenSelectLevel.h"
#include "ScreenStart.h"
#include "AchievementUi.h"
#include "GameSceneNavigation.h"
void  GameManager::clearRunningInfo()
{
    GameRunningInfo::getInstance()->removeAllGameLayer();
    GameRunningConfig::getInstance()->clearInfo();
    safeReleaseFileHandler();
}

void  GameManager::navigationTo(int id)
{
    Scene* scene = nullptr;
    switch (id) {
        case ScenePageView:
            scene = ScreenSelectLevel::create();
            break;
        case SceneLogo:
            scene = ScreenStart::createScene();
            break;
        case SceneLoding:
            scene = GameSceneNavigation::createScene();
            break;
        case SceneGameScene:
            scene = GameSceneMain::createScene();
            break;
        case SceneFlower:
            scene = AchievementUi::createScene();
            break;
        case SceneGameWin:
            scene = GameSceneWin::createScene();
            break;
        default:
            break;
    }
    navigationTo(scene);
}
void  GameManager::navigationToPasueGameScene()
{
    navigationTo(_gameScne);
    reStartActions();
}
void   GameManager::releaseGameScene()
{
    if (_gameScne) {
        // GameRunningManager::getInstance()->onGameSceneMainExit();
        _gameScne->release();
        _gameScne = nullptr;
    }
}
void  GameManager::navigationToGameScene()
{
    if (!_gameScne) {
        _gameScne = GameSceneMain::createScene();
        _gameScne->retain();
        _gameLevel = LevelManager::getInstance()->_levelId;
    }
    navigationTo(_gameScne);
}
void  GameManager::navigationToGameScene(int level)
{
    if (!_gameScne) {
        _gameLevel = level;
    }
    navigationToGameScene();
}
void  GameManager::createNewGameScene()
{
    releaseGameScene();
    _gameLevel = 0;
}
void  GameManager::navigationTo(Scene* scene)
{
    Director::getInstance()->replaceScene(TransitionFade::create(0.1, scene, Color3B(0,0,0)));
}
void GameManager::pauseGame()
{
    // _pauseNodes = Director::getInstance()->getActionManager()->pauseAllRunningActions();
    GameLayerUI::getRunningLayer()->pauseGame();
    GameRunningInfo::getInstance()->pauseGame();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
void GameManager::reStartGame()
{
    GameLayerUI::getRunningLayer()->reStartGame();
    GameRunningInfo::getInstance()->reStartGame();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
void GameManager::clearGameLevelInfo()
{
    releaseGameScene();
    _gameLevel =  0;
}
void  GameManager::saveActions()
{
    _pauseNodes = Director::getInstance()->getActionManager()->pauseAllRunningActions();
}
void  GameManager::reStartActions()
{
    Director::getInstance()->getActionManager()->resumeTargets(_pauseNodes);
}