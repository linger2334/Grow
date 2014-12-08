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
        _statisticsData = StatisticsData::getRunningLayer();
        bool needGuide = _statisticsData->needGuide;
        guideCourse = _statisticsData->guideCourse;

        levelid= LevelManager::getInstance()->_levelId;
#ifdef GROW_LEVELEDITOR
        levelid = *(GameManager::getInstance()->_fileHandler->_filename.c_str() + strlen("levels/level ")) - ('0' - 0);
#endif
        _guideManager = GuideManager::getInstance();
        if (needGuide) {
            if (levelid != _guideManager->levelid) {
                _guideManager->uninstall();
                _guideManager->levelid = levelid;
                _guideManager->loadGuideXML();
                int sequenceNum = StatisticsData::getRunningLayer()->guideSequence;
                _guideManager->start(_guideManager->getStepIndexBySequence(sequenceNum));
            }
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
    if (levelid == 1) {
        switch (static_cast<GuideCourse1>(guideCourse)) {
            case GuideCourse1::StrokeDirt:
                if (plantHeight >= 200.0) {
                    if (_guideManager->isSetUp()) {
                        _guideManager->Register(StrokeDirt::create());
                        guideCourse++;
                        _statisticsData->guideCourse = guideCourse;
                    }
                }
                break;
            case GuideCourse1::FlameBlue:
                
                
                break;
            case GuideCourse1::FlameWhite:
                
                
                break;
            default:
                break;
        }
    }else if (levelid == 2){
        switch (static_cast<GuideCourse2>(guideCourse)) {
            case GuideCourse2::EncounterCicada:
                
                break;
                
            default:
                break;
        }
    }else if (levelid == 3){
        switch (static_cast<GuideCourse3>(guideCourse)) {
            case GuideCourse3::EncounterSerpent:
                
                break;
                
            default:
                break;
        }
    }else if (levelid == 4){
        switch (static_cast<GuideCourse4>(guideCourse)) {
            case GuideCourse4::EncounterGear:
                
                break;
                
            default:
                break;
        }
    }
    
    
}

