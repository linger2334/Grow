#include "GameLayerPlant_level_3.h"
#include "Plant.h"
#include "PlantHelper.h"
#include "GameRuntime.h"
#include "GamePhysicalWorld.h"
#include "GameRunningInfo.h"
#define GROW_MIN_ANGLE_LEFT (-130)
#define GROW_MIN_ANGLE_RIGHT 130
#define GROW_MIN_ANGLE_CRASH_STONE_LEFT (-45)
#define GROW_MIN_ANGLE_CRASH_STONE_RIGHT 45
#define GROW_MIN_ANGLE_NEAR_BORDER_LEFT (-60)
#define GROW_MIN_ANGLE_NEAR_BORDER_RIGHT (60)
#define LEAF_RAND_LENGTH 211
#define LEAF_LENGHT      200

bool    GameLayerPlant_Level_3::init()
{
    GameLayerPlant::init();
    return true;
}
void  GameLayerPlant_Level_3::moveDown(float yLen)
{
    GameLayerPlant::moveDown(yLen);
    _leafs.moveDownLeafs(yLen);
    _leafsExt.moveDownLeafs(yLen);
    // _leafHelper.moveDownLeafs(yLen);
}
void  GameLayerPlant_Level_3::update(float dt)
{
    GameLayerPlant::update(dt);
    _leafs.checkAddLeaf();
    _leafs.updateLeafsScale();
    _leafsExt.checkAddLeaf();
    _leafsExt.updateLeafsScale();
    
}
void GameLayerPlant_Level_3::onPlantHeightChange(float height,int index)
{
    
    if (index ==0 && height < 0 ) {
        _leafs.updateLeafsHeight(height);
    }
    else if (index == 1 && height < 0 ) {
        _leafsExt.updateLeafsHeight(height);
    }
}
bool  GameLayerPlant_Level_3::initGameInfo()
{
    GameLayerPlant::initGameInfo();
    std::vector<GamePlantConfig>& config = GameRunningConfig::getInstance()->_plantConfigs;
    int id = addOnePlant();
    auto* list = &(config[0]._radiusList);
    list->addOneContext(0, 0);
    list->addOneContext(60, 25);
    list->addOneContext(400, 80);
    
    int id1 = addOnePlant();
    auto* list1 = &(config[1]._radiusList);
    list1->addOneContext(0, 0);
    list1->addOneContext(60, 25);
    list1->addOneContext(400, 80);


    auto call = std::bind(&GameLayerPlant_Level_3::onPlantHeightChange,this,std::placeholders::_1,std::placeholders::_2);
    GameRunningConfig::getInstance()->_plantHeightChangeCallbacks.push_back(call);

    _leafs._leafScaleRadius.addOneContext(0, 0);
    _leafs._leafScaleRadius.addOneContext(100, 0.1);
    _leafs._leafScaleRadius.addOneContext(300,0.2);
    _leafs._leafScaleRadius.addOneContext(600,0.3);
    _leafs._leafScaleRadius._defaultRadius = 0;
    _leafs._plantId = 0;
    _leafs._pathLeafs.push_back("plant1_leaf_2.png");
    _leafs._pathLeafs.push_back("plant1_leaf_3.png");
    _leafs._pathLeafs.push_back("plant1_leaf_4.png");
    _leafs._node = this;
    _leafs._leafStepHeight = LEAF_LENGHT;
    _leafs._leafStepRandHeight = LEAF_RAND_LENGTH;
    
    _leafsExt._leafScaleRadius.addOneContext(0, 0);
    _leafsExt._leafScaleRadius.addOneContext(100, 0.1);
    _leafsExt._leafScaleRadius.addOneContext(300,0.2);
    _leafsExt._leafScaleRadius.addOneContext(600,0.3);
    _leafsExt._leafScaleRadius._defaultRadius = 0;
    _leafsExt._plantId = 1;
    _leafsExt._pathLeafs.push_back("plant1_leaf_2.png");
    _leafsExt._pathLeafs.push_back("plant1_leaf_3.png");
    _leafsExt._pathLeafs.push_back("plant1_leaf_4.png");
    _leafsExt._node = this;
    _leafsExt._leafStepHeight = LEAF_LENGHT;
    _leafsExt._leafStepRandHeight = LEAF_RAND_LENGTH;
    return true;
}
#include "ItemModel.h"
PlantGrowContext  GameLayerPlant_Level_3::getPlantGrowContextAngle(PlantNode* plant)
{
    PlantGrowContext grow(true);
    float angle = plant->getHeadAnalge();
    Vec2 cpo = plant->getHeadPositionInWorld();
    auto physicalWorld = GamePhysicalWorld::getInstance();
    auto rayCall = [](ItemModel* item)->bool
    {
        return item->isStone();
    };
    float lenLeft = physicalWorld->rayCastTest(cpo,cpo + Vec2(-100,0),rayCall);
    float lenRight = physicalWorld->rayCastTest(cpo,cpo + Vec2(100,0),rayCall);
    bool isHeadInStone = physicalWorld->isInStone(cpo);
    
    float angleLeft = GROW_MIN_ANGLE_LEFT;
    float angleRight = GROW_MIN_ANGLE_RIGHT;
    
    if (!isHeadInStone) {
        if (lenLeft > 0) {
            angleLeft = GROW_MIN_ANGLE_CRASH_STONE_LEFT;
        }
        if (lenRight > 0)
        {
            angleRight =  GROW_MIN_ANGLE_CRASH_STONE_RIGHT;
        }
    }
    
    if( angle < angleLeft)
    {
        grow._left = false;
    }
    if( angle > angleRight)
    {
        grow._right = false;
    }
    
    float  x = plant->getHeadPositionInWorld().x;
    Size size= GameRuntime::getInstance()->getVisibleSize();
    float len =140;
    if (size.width<768) {
        len = 100;
    }
    if (x < len)
    {
        if(angle <GROW_MIN_ANGLE_NEAR_BORDER_LEFT)
            grow._left = false;
    }
    else if(x > size.width - len)
    {
        if( angle >GROW_MIN_ANGLE_NEAR_BORDER_RIGHT)
            grow._right = false;
        
    }
    return grow;
}

float  GameLayerPlant_Level_3::growByIndex(float length,int index)
{
    
    float retLen = -1;
    
    FaceDirection turnDir;
    bool isGrow = false;
    
    //    std::list<unsigned char> types ={101,102,103,104,105,106,107,111};
    auto call = [](ItemModel* item)
    {
        return item->isStone()||item->isDirtLine();
    };
    PlantNode* node = getPlantNodeByIndex(index);
    
    std::vector<FaceDirection> growDirList;
    PlantHelper::getGroeDirList(node,growDirList);
    
    
    auto minGrow = PlantHelper::getGrowContextGrowNextUnitLengthTestMap(node);
    auto minGrowStone = PlantHelper::getGrowContextGrowNextUnitLengthTestStone(node);
    minGrowStone = PlantGrowContext(true);
    auto  growAngle = getPlantGrowContextAngle(node);
    PlantHelper::PlantGrowContext min;
    min._left = minGrow._left && minGrowStone._left && growAngle._left;
    min._right = minGrow._right && minGrowStone._right && growAngle._right;
    // min  = growAngle;
    float angle = node->getHeadAnalge();
    auto con = PlantHelper::getPlantRayCastContext(node,80                                                            ,angle, 10,8,call);
    
    for (int i = 0 ; i < 5; i++)
    {
        PlantHelper::PlantGrowContext  grow(true);
        
        float ml = con.getMinCrashLengthLeft();
        float mr = con.getMinCrashLenghtRight();
        float len = 80-(i+1)*10;
        if (ml > 0&& ml < len)grow._left = false;
        if (mr > 0&& mr < len)grow._right = false;
        for(auto& ip : growDirList)
        {
            if(min.isCanGrowByDir(ip)&& grow.isCanGrowByDir(ip))
            {
                isGrow = true;
                turnDir = ip;
                break ;
            }
        }
        if (isGrow )break;
    }
    if(!isGrow)//&&
        //con._leftCrashLen.front()>30&&
        // con._rightCrashLen.front()>30)
    {
        PlantHelper::PlantGrowContext  grow(true);
        float al = con.getLeftCrashLengthAve();
        float ar = con.getRightCrashLengthAve();
        if (al < 30)  grow._left = false;
        if (ar < 30)  grow._right = false;
        if(grow.allIsValue(true))
        {
            grow._left = al < ar ? false : true;
            grow._right = ar < al ? false : true;
            
        }
        for(auto& ip : growDirList)
        {
            if(min.isCanGrowByDir(ip)&& grow.isCanGrowByDir(ip))
            {
                isGrow = true;
                turnDir = ip;
                break ;
            }
        }
    }
    
    if (isGrow)
    {
        
        retLen =node->grow(turnDir, length);
        
    }
    return retLen;
}
float   GameLayerPlant_Level_3::reGrowByIndex(float length,int index)
{
    float len =GameLayerPlant::reGrowByIndex(length, index);
    return len;
}
