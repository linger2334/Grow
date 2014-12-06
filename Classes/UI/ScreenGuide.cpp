//
//  ScreenGuide.cpp
//  Grow-UI
//
//  Created by wlg on 12/1/14.
//
//

#include "ScreenGuide.h"
#include "LevelManager.h"
#include "GameLayerPlant.h"
#include "GuideNode.h"

bool ScreenGuide::init()
{
    if(GameLayerBase::init()){
        int levelid = LevelManager::getInstance()->_levelId;
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
    if (GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(0)._growSpeed == 0) {
        addChild(StrokeDirt::create());
    }
    
}

