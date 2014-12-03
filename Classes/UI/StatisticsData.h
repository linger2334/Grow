//
//  StatisticsData.h
//  Grow-UI
//
//  Created by 林纲 王 on 14-11-29.
//
//

#ifndef __Grow_UI__StatisticsData__
#define __Grow_UI__StatisticsData__

#include <stdio.h>
#include "Macro.h"
#include "GameLayerBase.h"

class StatisticsData : public GameLayerBase,public GameLayerHelper<StatisticsData>
{
public:
    StatisticsData();
    ~StatisticsData();
    
    CREATE_FUNC(StatisticsData)
    bool init();
    
    void initData();
    void saveData();
    void checkNewFlowerUnlock();
    void moveDown(float y);
    void update(float dt);
    
    std::string date;
    int days;
    int hours;
    int minutes;
    int totalFlame_White;
    int totalFlame_Blue;
    int totalFlame_Violet;
    int totalFlame_Orange;
    int clearances;
    int failures;
    int collectionVolume;
    
protected:
    void saveBothFiles();
    
    time_t startTime;
    int levelid;
    float moveDownDistance;
    float plantHeight;
    UserDefault* _userDefault;
    __Dictionary* _statisticsdata;
    __Dictionary* _currentLevelData;
    ValueVector _flowersInfo;
    
};

#endif /* defined(__Grow_UI__StatisticsData__) */
