//
//  StatisticsData.cpp
//  Grow-UI
//
//  Created by 林纲 王 on 14-11-29.
//
//

#include "StatisticsData.h"
#include "CocosGUI.h"
#include "LevelManager.h"
#include "GameManager.h"
#include "GameLayerPlant.h"
#include "Plant.h"

using namespace ui;

StatisticsData::StatisticsData():_statisticsdata(nullptr)
{
    
}

StatisticsData::~StatisticsData()
{
    saveData();
    saveBothFiles();
    CC_SAFE_RELEASE_NULL(_statisticsdata);
}

bool StatisticsData::init()
{
    if(GameLayerBase::init()){
        _userDefault = UserDefault::getInstance();
        levelid = LevelManager::getInstance()->_levelId;
#ifdef GROW_LEVELEDITOR
        levelid = *(GameManager::getInstance()->_fileHandler->_filename.c_str() + strlen("levels/level ")) - ('0' - 0);
#endif
        initData();
        moveDownDistance = 0;
        plantHeight = 0;
        _flowersInfo = FileUtils::getInstance()->getValueVectorFromFile("flowersinfo.plist");
        return true;
    }
    
    return false;
}

void StatisticsData::initData()
{
    struct timeval start;
    gettimeofday(&start, nullptr);
    startTime = start.tv_sec;
    
    struct tm* tm;
    tm = localtime(&startTime);
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    std::string currentdate = StringUtils::format("%d%02d%02d",year,month,day);
    
    std::string fullPath = FileUtils::getInstance()->getWritablePath() + "StatisticsData.plist";
    if(true){
        _userDefault->setIntegerForKey("collectedWhite", 0);
        _userDefault->setIntegerForKey("collectedBlue", 0);
        _userDefault->setIntegerForKey("collectedViolet", 0);
        _userDefault->setIntegerForKey("collectedOrange", 0);
        _userDefault->setIntegerForKey("growHeight", 0);
        _userDefault->setIntegerForKey("seedlingIndex", 0);
        _userDefault->setIntegerForKey("accumulativeTime_Natural", 0);
        
        __Dictionary* root = __Dictionary::create();
        for (int i = 0; i<LEVEL_COUNT; i++) {
            __Dictionary* leveldata = __Dictionary::create();
            
            leveldata->setObject(__String::create(currentdate), "date");
            leveldata->setObject(__Integer::create(0), "clearances");
            
            leveldata->setObject(__Integer::create(1), "days");
            leveldata->setObject(__Integer::create(0), "hours");
            leveldata->setObject(__Integer::create(0), "minutes");
            leveldata->setObject(__Integer::create(0), "totalFlame_White");
            leveldata->setObject(__Integer::create(0), "totalFlame_Blue");
            leveldata->setObject(__Integer::create(0), "totalFlame_Violet");
            leveldata->setObject(__Integer::create(0), "totalFlame_Orange");
            leveldata->setObject(__Float::create(0.0), "totalDistance");
            leveldata->setObject(__Integer::create(0), "failures");
            leveldata->setObject(__Integer::create(0), "collectionVolume");
            
            leveldata->setObject(__Bool::create(true), "needGuide");
            leveldata->setObject(__Integer::create(0), "guideCourse");
            leveldata->setObject(__Integer::create(0), "guideSequence");

            root->setObject(leveldata, StringUtils::format("level%d",i+1));
        }
        root->writeToFile(fullPath.c_str());
        
        _userDefault->setBoolForKey("isBeginner", false);
        _userDefault->flush();
    }
    
    _statisticsdata =__Dictionary::createWithContentsOfFile(fullPath.c_str());
    CC_SAFE_RETAIN(_statisticsdata);
    _currentLevelData = dynamic_cast<__Dictionary*>(_statisticsdata->objectForKey(StringUtils::format("level%d",levelid)));
    date = ((__String*)_currentLevelData->objectForKey("date"))->getCString();
    days = ((__String*)_currentLevelData->objectForKey("days"))->intValue();
    if (date.compare(currentdate)) {
        date = currentdate;
        days++;
    }
    hours = ((__String*)_currentLevelData->objectForKey("hours"))->intValue();
    minutes = ((__String*)_currentLevelData->objectForKey("minutes"))->intValue();
    totalFlame_White = ((__String*)_currentLevelData->objectForKey("totalFlame_White"))->intValue();
    totalFlame_Blue = ((__String*)_currentLevelData->objectForKey("totalFlame_Blue"))->intValue();
    totalFlame_Violet = ((__String*)_currentLevelData->objectForKey("totalFlame_Violet"))->intValue();
    totalFlame_Orange = ((__String*)_currentLevelData->objectForKey("totalFlame_Orange"))->intValue();
    clearances = ((__String*)_currentLevelData->objectForKey("clearances"))->intValue();
    failures = ((__String*)_currentLevelData->objectForKey("failures"))->intValue();
    collectionVolume = ((__String*)_currentLevelData->objectForKey("collectionVolume"))->intValue();
    needGuide = ((__String*)(_currentLevelData->objectForKey("needGuide")))->boolValue();
    guideCourse = ((__String*)_currentLevelData->objectForKey("guideCourse"))->intValue();
    guideSequence = ((__String*)_currentLevelData->objectForKey("guideSequence"))->intValue();
    
}

void StatisticsData::saveData()
{
    struct timeval end;
    gettimeofday(&end, nullptr);
    time_t endTime = end.tv_sec;
    double playTime = difftime(endTime,startTime);
    _currentLevelData = dynamic_cast<__Dictionary*>(_statisticsdata->objectForKey(StringUtils::format("level%d",levelid)));
    _currentLevelData->setObject(__String::create(date), "date");
    _currentLevelData->setObject(__Integer::create(days), "days");
    _currentLevelData->setObject(__Integer::create(hours+playTime/3600), "hours");
    _currentLevelData->setObject(__Integer::create(minutes + int(playTime)%3600/60), "minutes");
    _currentLevelData->setObject(__Integer::create(totalFlame_White), "totalFlame_White");
    _currentLevelData->setObject(__Integer::create(totalFlame_Blue), "totalFlame_Blue");
    _currentLevelData->setObject(__Integer::create(totalFlame_Violet), "totalFlame_Violet");
    _currentLevelData->setObject(__Integer::create(totalFlame_Orange), "totalFlame_Orange");
    
    LevelManager* levelManager = LevelManager::getInstance();
    if(levelManager->_mapGroupSubId >= levelManager->_mapGroups[levelManager->_mapGroupId].size()){
        clearances++;
        _currentLevelData->setObject(__Integer::create(clearances), "clearances");
    }
    srand((unsigned)time(nullptr));
    __Float* totalDistance= __Float::create(PAGE_COUNTS*DefiniteSize.height*levelManager->_mapGroups[levelManager->_mapGroupId].size()*clearances + DefiniteSize.height*rand()/(RAND_MAX+1.0));
    _currentLevelData->setObject(totalDistance, "totalDistance");
    _currentLevelData->setObject(__Integer::create(failures), "failures");
    _currentLevelData->setObject(__Integer::create(collectionVolume), "collectionVolume");
    _currentLevelData->setObject(__Bool::create(needGuide), "needGuide");
    _currentLevelData->setObject(__Integer::create(guideCourse), "guideCourse");
    _currentLevelData->setObject(__Integer::create(guideSequence), "guideSequence");
    _statisticsdata->setObject(_currentLevelData, StringUtils::format("level%d",levelid));
}

void StatisticsData::saveBothFiles()
{
    std::string plistPath = FileUtils::getInstance()->getWritablePath() + "StatisticsData.plist";
    bool result = _statisticsdata->writeToFile(plistPath.c_str());
#ifdef COCOS2D_DEBUG
    if(result){
        log("save statisticsdata success!in:%s",plistPath.c_str());
        log("save userdefault success in:%s",_userDefault->getXMLFilePath().c_str());
    }else{
        log("save statisticsdata failed!");
    }
#endif
    int growHeight = _userDefault->getIntegerForKey("growHeight") + moveDownDistance + plantHeight;
    _userDefault->setIntegerForKey("growHeight", growHeight);
    _userDefault->flush();
}

void StatisticsData::checkNewFlowerUnlock()
{
    int FlameWhiteCount = _userDefault->getIntegerForKey("collectedWhite");
    int FlameBlueCount = _userDefault->getIntegerForKey("collectedBlue");
    int FlameVioltCount = _userDefault->getIntegerForKey("collectedViolet");
    int FlameOrangeCount = _userDefault->getIntegerForKey("collectedOrange");
    int growHeight = _userDefault->getIntegerForKey("growHeight") + moveDownDistance + GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(0)->getHeadPosition().y;
    int seedlingIndex = _userDefault->getIntegerForKey("seedlingIndex", 0);
    
    ValueMap flowerTerm = _flowersInfo.at(seedlingIndex).asValueMap();
    if (FlameWhiteCount>=flowerTerm.at("condition").asValueMap().at("Flame_White").asInt()&&
        FlameBlueCount>=flowerTerm.at("condition").asValueMap().at("Flame_Blue").asInt()&&
        FlameVioltCount>=flowerTerm.at("condition").asValueMap().at("Flame_Violet").asInt()&&
        FlameOrangeCount>=flowerTerm.at("condition").asValueMap().at("Flame_Orange").asInt()&&
        growHeight>=flowerTerm.at("condition").asValueMap().at("Height").asInt())
    {
        _userDefault->setIntegerForKey("collectedWhite", 0);
        _userDefault->setIntegerForKey("collectedBlue", 0);
        _userDefault->setIntegerForKey("collectedViolet", 0);
        _userDefault->setIntegerForKey("collectedOrange", 0);
        _userDefault->setIntegerForKey("growHeight", 0);
        moveDownDistance = 0;
        _userDefault->setIntegerForKey("seedlingIndex", ++seedlingIndex<FLOWER_COUNT? seedlingIndex : FLOWER_COUNT-1);
        collectionVolume++;
        
        ImageView* newFlowerUnlock = ImageView::create("UI_NewFlowerUnlock.png");
        Node* gameSceneNode = GameRunningInfo::getInstance()->getGameSceneNode();
        newFlowerUnlock->setPosition(Vec2(VisibleSize.width/2,VisibleSize.height/2));
        newFlowerUnlock->setOpacity(0);
        FadeIn* appear = FadeIn::create(1);
        FadeOut* fadeOut = FadeOut::create(1);
        CallFuncN* remove = CallFuncN::create([](Node* node){
            node->removeFromParent();
        });
        newFlowerUnlock->runAction(Sequence::create(appear,fadeOut, remove,NULL));
        gameSceneNode->addChild(newFlowerUnlock,999);
    }
    
}

void StatisticsData::moveDown(float y)
{
    moveDownDistance += y;
    plantHeight = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(0)->getHeadPosition().y;
}

void StatisticsData::update(float dt)
{
    checkNewFlowerUnlock();
    
}