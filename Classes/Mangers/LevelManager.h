#ifndef __GrowDemo__LevelManager__
#define __GrowDemo__LevelManager__
#include "common.h"
#include "ContorlPoint.h"
#include "PlantHelper.h"
class MapConfigInfo
{
public:
    MapConfigInfo(){}
    MapConfigInfo(std::string bk,std::string dirt,std::string border,std::string uiborder):_mapBackground(bk),_mapDirt(dirt),_mapBorder(border),_mapUIBorder(uiborder){}
    std::string _mapBackground;
    std::string _mapDirt;
    std::string _mapBorder;
    std::string _mapUIBorder;
};

class PlantConfigInfo
{
public:
    PlantConfigInfo(std::list<ContorlPointV2> contorlPoints,std::string body, std::list<std::string> leafs, PlantHelper::PlantRadiusHelper bodyRadiusList, PlantHelper::PlantRadiusHelper leafRadiusList,float step, float range):_plantStartContorlPointList(contorlPoints),_pathPlangBody(body),_pathPlantLeaf(leafs),
        _bodyRadiusList(bodyRadiusList),_leafsRadiusList(leafRadiusList),
        _leafHeightStep(step),_leafHeightRange(range)
    {
        
    }
    float                      _growSpeed = 50.0f;
    float                      _reGrowSpeed = 0.0f;
    float                      _textureLeft = 0.0f;
    float                      _textureRight = 1.0f;
    float                      _leafHeightStep;
    float                      _leafHeightRange;
    Vec2                       _plantNodeInitPosition = Vec2(0.0f,0.0f);
    std::list<ContorlPointV2>  _plantStartContorlPointList;
    std::string                _pathPlangBody;
    std::list<std::string>   _pathPlantLeaf;
    PlantHelper::PlantRadiusHelper _bodyRadiusList;
    PlantHelper::PlantRadiusHelper _leafsRadiusList;
};
class GameLayerPlant;
class GamePlantConfig;
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
    void createBasesLayers();
    void initLevelInfo_1();
    void initLevelInfo_2();
    void initLevelInfo_3();
    void initLevelInfo_4();
    void selectLevel(int id);
    void selectRecordLevel();
    bool selectNextSubLevel();
    int  getLevelState(){return _levelManagerState;}
    
    void saveLevelInfo();
    void initLevelInfoByRecord();
    
    void initLevelInfo();

    void selectMapFile(int groupId,int groupSubId);
    void selectConfigInfo(int id);
    void initMapFile(std::string fileName);
    static void initMapsInfo();
    GameLayerPlant*   createLayerPlantById(int id);
    GamePlantConfig&  initPlantConfig(int id,Vec2 pt,float angle,float height,float  rotateRadius,std::string plantTexture);
    GamePlantConfig&  initPlantConfig(int runningConfigId,int idConfig);
    GamePlantConfig&  initPlantConfig(int runningConfigId,int idConfig,Vec2 pt);
    GamePlantConfig&  initPlantConfig(int runningConfigId,int idConfig,ContorlPointV2 cp);
    
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
    static std::vector<std::vector<std::string> > _sMapConfigInfos;
    static std::vector<PlantConfigInfo>        _sPlantConfigInfos;
};


#endif /* defined(__GrowDemo__LevelManager__) */
