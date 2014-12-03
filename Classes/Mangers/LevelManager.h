#ifndef __GrowDemo__LevelManager__
#define __GrowDemo__LevelManager__
#include "common.h"

class GameLayerPlant;
class LevelManager : public Singleton<LevelManager>
{
public:
    enum
    {
        NoSelectLevel,
        FirstInit,
        ReloadInit,
        ChangeNextSubMapInit,
    };
    LevelManager(): Singleton<LevelManager>()
    {
        initMapsInfo();
        _levelId = 0;
        _mapGroupId = 0;
        _mapGroupSubId = 0;
        _levelManagerState = NoSelectLevel ;
    }
    static bool isHasRecord()
    {
        return false;
    }
    static void clearRecord()
    {

    }
    void releaseLevelInfo();

    void createBasesLayers();
    void initLevelInfo_1();
    void initLevelInfo_2();
    void initLevelInfo_3();
    void initLevelEditorMenu();
    void selectLevel(int id);
    void selectRecordLevel();
    void selectSubLevel(int id);
    
    int getLevelState(){return _levelManagerState;}
    void selectNextMap();
    void initGameScene();
    void initGameSceneByRecord();
    
    void initPlantAndMapConfig();
    
    
    void initGameSceneByLevel(int id);
    void initGameSceneByLevelAndSubId(int id,int subId);
    
    virtual
    void selectMapFile(int groupId,int groupSubId);
    void initMapFile(std::string fileName);
    static void initMapsInfo();
    GameLayerPlant* createLayerPlantById(int id);
    int _levelId;
    int _mapGroupId;
    int _mapGroupSubId;
    int _levelManagerState;
    int _initState;
    Vec2 _subInitPlantPoints[2];
    CC_SYNTHESIZE(int, _selectedMapItemGroupId, SelectedMapItemGroupId);
    
    static std::map<int,std::string> _mapFiles;
    static std::map<int, std::vector<int> > _mapGroups;
    static bool _sIsInitMapInfo;
};


#endif /* defined(__GrowDemo__LevelManager__) */
