#include "GameLayerPlant_Level_1.h"
#include "Plant.h"
#include "PlantHelper.h"
#include "GameRuntime.h"
#include "GamePhysicalWorld.h"
#include "GameRunningInfo.h"
#define GROW_MIN_ANGLE_LEFT (-110)
#define GROW_MIN_ANGLE_RIGHT 110
#define GROW_MIN_ANGLE_CRASH_STONE_LEFT (-70)
#define GROW_MIN_ANGLE_CRASH_STONE_RIGHT 70

#define GROW_MIN_ANGLE_NEAR_BORDER_LEFT (-80)
#define GROW_MIN_ANGLE_NEAR_BORDER_RIGHT (80)
#define LEAF_RAND_LENGTH 51
#define LEAF_LENGHT      201


bool    GameLayerPlant_Level_1::init()
{
    GameLayerPlant::init();
    return true;
}
void  GameLayerPlant_Level_1::moveDown(float yLen)
{
    GameLayerPlant::moveDown(yLen);
    _leafs.moveDownLeafs(yLen);
   // _leafHelper.moveDownLeafs(yLen);
}
void  GameLayerPlant_Level_1::update(float dt)
{
    GameLayerPlant::update(dt);
    _leafs.checkAddLeaf();
    _leafs.updateLeafsScale();

}
void GameLayerPlant_Level_1::onPlantHeightChange(float height,int index)
{
    if(index >0)return;
    if (height < 0 ) {
        _leafs.updateLeafsHeight(height);
    }
}
bool  GameLayerPlant_Level_1::initGameInfo()
{
    GameLayerPlant::initGameInfo();
    std::vector<GamePlantConfig>& config = GameRunningConfig::getInstance()->_plantConfigs;

    auto* list = &(config[0]._radiusList);
   // list->_defaultRadius =80;
    list->addOneContext(0, 0);
    list->addOneContext(60, 25);
    list->addOneContext(400, 80);
    
    
    int id = addOnePlant();
    createHeadB2Body(id);
    auto call = std::bind(&GameLayerPlant_Level_1::onPlantHeightChange,this,std::placeholders::_1,std::placeholders::_2);
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
    
    for (int i = 0; i<100; i++) {
        _growList.push_back(FaceTop);
    }
    for (int i = 0; i<180; i++) {
        _growList.push_back(FaceLeft);
    }
    for (int i = 0; i<330; i++) {
        _growList.push_back(FaceRight);
    }
    return true;
}
#include "ItemModel.h"
 PlantGrowContext  GameLayerPlant_Level_1::getPlantGrowContextAngle(PlantNode* plant)
{
    PlantGrowContext grow(true);
    float angle = plant->getHeadAnalge();
    float angleLeft = GROW_MIN_ANGLE_LEFT;
    float angleRight = GROW_MIN_ANGLE_RIGHT;
    Vec2 cpo = plant->getHeadPositionInWorld();
    auto physicalWorld = GamePhysicalWorld::getInstance();
    auto rayCall = [](ItemModel* item)->bool
    {
        return item->isStone();
    };
    ItemModel* outLeft = nullptr;
    ItemModel* outRight = nullptr;
    Vec2 vleft = MathHelper::getRotatePosition(cpo, cpo + Vec2(-250,0),0);
    Vec2 vright = MathHelper::getRotatePosition(cpo, cpo + Vec2(250,0),0);
    float lenLeft = physicalWorld->rayCastTest(cpo,vleft,rayCall,&outLeft);
    float lenRight = physicalWorld->rayCastTest(cpo,vright,rayCall,&outRight);
    
   
    if (lenLeft > 0 && outLeft){
        Vec2 pt =outLeft->getPosition();
        float itemy = pt.y;
        if (outLeft->getParent()) {
            itemy = outLeft->getParent()->convertToWorldSpace(pt).y;
        }
        if (cpo.y > itemy + 30 - outLeft->getContentSize().height*0.5) {
            if (lenLeft > 80) {
                angleLeft = -80;
            }
            angleLeft = -nearAngle(lenLeft, 90,20,70);
        }
    }
    if (lenRight > 0 && outRight){
        Vec2 pt =outRight->getPosition();
        float itemy = pt.y;
        if (outRight->getParent()) {
            itemy = outRight->getParent()->convertToWorldSpace(pt).y;
        }
   
        if (cpo.y > itemy + 30 - outRight->getContentSize().height*0.5) {
            if (lenRight > 80) {
                angleRight = 80;
            }
              angleRight = nearAngle(lenRight, 90,20,70);
        }
       
    }
    
    Size size= GameRuntime::getInstance()->getVisibleSize();
    auto vhead = plant->getHeadPositionInWorld();

    if( angle < angleLeft )
    {
        grow._left = false;
    }
    if( angle > angleRight)
    {
        grow._right = false;
    }
    
    float x = vhead.x;
  
    float len =180;
    if (size.width<768) {
        len = 140;
    }

    if (x < len)
    {
        float testAngle = GROW_MIN_ANGLE_NEAR_BORDER_LEFT * ( x/len);
        if (testAngle  < GROW_MIN_ANGLE_NEAR_BORDER_LEFT ) testAngle =GROW_MIN_ANGLE_NEAR_BORDER_LEFT;
        if(angle < testAngle)
            grow._left = false;
    }
    else if(x > size.width - len)
    {
        float testAngle = GROW_MIN_ANGLE_NEAR_BORDER_RIGHT * ( (size.width - x)/len);
        if (testAngle >GROW_MIN_ANGLE_NEAR_BORDER_RIGHT ) testAngle =GROW_MIN_ANGLE_NEAR_BORDER_RIGHT;
        if( angle > testAngle)
            grow._right = false;
    }
    return grow;

 }
float GameLayerPlant_Level_1::nearAngle(float lenght , float maxLength,float startAngle ,float endAngle)
{
    float testAngle = endAngle * lenght / maxLength;
    if (testAngle < startAngle) {
        testAngle = startAngle;
    }
    return testAngle;
}
PlantGrowContext GameLayerPlant_Level_1::getPlantGrowContextStone(PlantNode* plant)
{
    PlantGrowContext grow(true);
   
    float angle = plant->getHeadAnalge();
    Vec2 cpo = plant->getHeadPositionInWorld();
    auto physicalWorld = GamePhysicalWorld::getInstance();
//    if (fabs(angle) < 30) {
//        angle = 0;
//    }
    auto rayCall = [](ItemModel* item)->bool
    {
        return item->isStone();
    };
  
   auto ptTop = plant->getNextContorlPointInWorld(60, FaceTop)._point;
    float crashTop = physicalWorld->rayCastTest(cpo, ptTop, rayCall);
    
    if (crashTop > 0 ) {
        auto con = PlantHelper::getPlantRayCastContext(plant,80,0, 55,1,rayCall);
        int countLeft =  con.getLeftCrashCount(60);
        int countRight =  con.getRightCrashCount(60);
        
        if (angle <-45 && countLeft > 0) {
            grow._left =false;
        }
        else if(angle > 45 && countRight > 0)
        {
            grow._right =false;
        }
    }

//    if (countLeft > 0 && countLeft >= countRight) {
//        if(angle < -30)
//        grow._left =false;
//    }
//    else if (countRight > 0 && countRight > countLeft) {
//            if(angle > 30)
//            grow._right =false;
//        }
    return grow;
}
bool  GameLayerPlant_Level_1::isNearStone(PlantNode* plant)
{
    
}
#define STEP_ANGLE 30
float  GameLayerPlant_Level_1::growByIndex(float length,int index)
{
    float retLen = -1;
    
    FaceDirection turnDir;

    bool isGrow = false;
    
    PlantNode* node = getPlantNodeByIndex(index);
    float angle = node->getHeadAnalge();
    
    Vec2 headTop = node->getHeadPositionInWorld();
    auto call2 = [=](ItemModel* item)
    {
        return item->isStone();
    };
    auto con2 = PlantHelper::getPlantRayCastContext1(headTop,150,angle - 90,10,18,call2);
    
    int lcount =con2.getCrashLengthCountByAngleRange(60, -90, angle);
    int rcount =con2.getCrashLengthCountByAngleRange(60, angle, 90);
    
    float lave =con2.getCrashLengthAveByAngleRange(-90, angle);
    float rave =con2.getCrashLengthAveByAngleRange( angle, 90);
    
    PlantHelper::PlantGrowContext growStone(true);
    if (lcount > 0 && rcount ==0 && lave < 60) {
        growStone._left = false;
    }
    else if(lcount > 0 && rcount > 0)
    {
        growStone._left = lave <= rave ? false : true;
        growStone._right =  !growStone._left;
    }
    else  if (rcount > 0 && lcount ==0 && rave < 60) {
        growStone._right = false;
    }
    else if(rcount > 0 && lcount > 0)
    {
        growStone._right = rave < lave ? false : true;
        growStone._left =  !growStone._left;
    }
    
    auto growAngle = getPlantGrowContextAngle(node);
    float al1s = con2.getCrashLengthAveByAngleRange(angle - 80,angle);
    float ar1s = con2.getCrashLengthAveByAngleRange(angle,angle + 80);
    bool isNearStone = al1s < 44 && ar1s < 44;
    bool isAndGrowStone =!(growAngle&growStone).allIsValue(false);
    bool isTestStone = (!isNearStone) && (isAndGrowStone);

    auto call = [=](ItemModel* item)
    {
        if(item->isDirtLine())
        {
            return true;
        }
        else if(isTestStone &&item->isStone()){
            return true;
        }
        return false;
      // return item->isDirtLine()||(isTestStone && item->isStone());
    };
    
    
    
    std::vector<FaceDirection> growDirList;
    PlantHelper::getGroeDirList(node,growDirList);
    
    auto minGrow = PlantHelper::getGrowContextGrowNextUnitLengthTestMap(node);

   
    PlantHelper::PlantGrowContext min(true);
    min &= minGrow;
    min &= growAngle;
    if (isAndGrowStone) {
        min &= growStone;
    }
//    min._left = minGrow._left && growAngle._left;
//    min._right = minGrow._right  && growAngle._right;
//    min._top = minGrow._top && growAngle._top;
    
    auto con1 = PlantHelper::getPlantRayCastContext1(headTop,80,angle - 90,10,18,call);

    PlantHelper::PlantGrowContext  growTest(true);
    float al1 = con1.getCrashLengthAveByAngleRange(angle - 80,angle);
    float ar1 = con1.getCrashLengthAveByAngleRange(angle,angle + 80);
    if (al1 < 30 )  growTest._left = false;
    if (ar1 < 30 )  growTest._right = false;
    min &= growTest;

    for (int i = 0 ; i < 2 ; i++)
    {
        PlantHelper::PlantGrowContext  grow(true);
        if (i<1&&min.allIsValue(true)) {
        if((fabs(angle)<90)||
            (fabs(angle)>=90&&al1<80&& ar1<80))
            {
                grow._left = (ar1 - al1) > 0 ? false : true;
                grow._right = (al1 - ar1) > 0 ? false : true;
            }
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
        if (isGrow )break;
    }
    if (isGrow)
    {
        retLen =node->grow(turnDir, length);
    }
    return retLen;
}
float   GameLayerPlant_Level_1::reGrowByIndex(float length,int index)
{
    float len =GameLayerPlant::reGrowByIndex(length, index);
    return len;
}
bool  GameLayerPlant_Level_1::isNeadBorder(PlantNode* plant)
{
    bool ret = false;
    
    float x = plant->getHeadUnitPositionInWorld().x;
    Size size = GameRuntime::getInstance()->getVisibleSize();
    if (x < size.width*(1.0f/5.0f)||
        x > size.width*(4.0f/5.0f)) {
        ret = true;
    }
    return ret;
}
