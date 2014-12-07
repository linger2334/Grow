//
//  ScreenGuide.cpp
//  Grow-UI
//
//  Created by wlg on 12/1/14.
//
//

#include "ScreenGuide.h"
#include "LevelManager.h"
#include "GameManager.h"
#include "GameLayerPlant.h"
#include "GuideNode.h"
#include "Plant.h"

bool ScreenGuide::init()
{
    if(GameLayerBase::init()){
        int levelid = LevelManager::getInstance()->_levelId;
#ifdef GROW_LEVELEDITOR
        levelid = *(GameManager::getInstance()->_fileHandler->_filename.c_str() + strlen("levels/level ")) - ('0' - 0);
#endif
        _guideManager = GuideManager::getInstance();
        if (levelid != _guideManager->levelid) {
            _guideManager->levelid = levelid;
            _guideManager->loadGuideXML();
        }
        return true;
    }
    
    return false;
}

void ScreenGuide::moveDown(float y)
{
    
}

void ScreenGuide::update(float dt)
{
    float plantHeight = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(0)->getHeadPosition().y;
    if (plantHeight >= 200.0) {
        unscheduleUpdate();
        addChild(StrokeDirt::create());
    }
    
}

