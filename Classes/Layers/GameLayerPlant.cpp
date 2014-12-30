#include "GameLayerPlant.h"
#include "Plant.h"
#include "GameHeaders.h"
#include "GameManager.h"
#include "GameLayerEffects.h"
#include "PlantLeafs.h"

#define GROW_MIN_ANGLE_LEFT (-110)
#define GROW_MIN_ANGLE_RIGHT 110
#define GROW_MIN_ANGLE_CRASH_STONE_LEFT (-70)
#define GROW_MIN_ANGLE_CRASH_STONE_RIGHT 70

#define GROW_MIN_ANGLE_NEAR_BORDER_LEFT (-80)
#define GROW_MIN_ANGLE_NEAR_BORDER_RIGHT (80)



// std::string  GameLayerPlant::_sRunningGamePlantName = typeid(GameLayerPlant).name();
std::string  GameLayerPlant::_sRunningGamePlantName ; //= typeid(GameLayerPlant).name();
bool GameLayerPlant::init()
{
   // assert (_sRunningGamePlantName == typeid(GameLayerPlant).name()) ;
    GameLayerBase::init();
    
    _sRunningGamePlantName = getType().name();
//    Size  size = Director::getInstance()->getWinSize();
//    _render = RenderTexture::create(size.width, size.height);
//    addChild(_render);
//    _render->setPosition(Vec2(size.width,size.height)*0.5);
    return true;
}

bool    GameLayerPlant::releaseGameInfo()
{
    removeHeadBodys();
    for (auto& i: _plants) {
        i._plant->removeFromParent();
        if(i._leafsContext)delete i._leafsContext;
    }
    _plants.clear();
    return true;
}

void  GameLayerPlant::setPlantStateByIndex(int state,int index)
{
    auto config = GameRunningConfig::getInstance();
    for(auto call : config->_plantChangeStateCallbacks)
    {
        call(state,_plants[index]._state,index);
    }
    _plants[index]._state = state;
}
int   GameLayerPlant::getPlantStateByIndex(int index)
{
    return    _plants[index]._state;
}
void  GameLayerPlant::changePlantHeightByIndex(float height, int index)
{
    auto config = GameRunningConfig::getInstance();
    for(auto call : config->_plantHeightChangeCallbacks)
    {
        call(height,index);
    }
    PlantLeafListBase* leafContext = nullptr;
    if (height < 0 && (leafContext = getPlantInfoByIndex(index)._leafsContext))
    {
        leafContext->updateLeafsHeight(height);
    }
}
void   GameLayerPlant::changePlantCPPositionYByIndex(float moveLen , int index)
{

    auto config = GameRunningConfig::getInstance();
    for(auto call : config->_plantCPChangeCallbacks)
    {
        call(moveLen,index);
    }
}
void GameLayerPlant::moveDown(float yLen)
{
    for(int i =0;i < getPlantCount();i++)
    {
        PlantNode* node = getPlantNodeByIndex(i);

        float oldY = node->getPositionY();
        float newY = oldY - yLen;
        if (newY < -200) {
            node->subContorlPoint(100);
            newY = newY +100;
            changePlantCPPositionYByIndex(-100,i);
        }
        node->setPositionY(newY);
          updateHeadB2Body(i);
        auto leafContext=  getPlantInfoByIndex(i)._leafsContext;
        if (leafContext) {
            leafContext->moveDownLeafs(yLen);
        }
    }
}
void GameLayerPlant::update(float dt)
{
    if(GameRunningInfo::getInstance()->isPause()||!GameRunningInfo::getInstance()->isPlantNeedGrow())return;
    int count = this->getPlantCount();
    bool isPlantGrow = false;
    for(int i = 0; i < count;i++)
    {
        PlantInfo& info = getPlantInfoByIndex(i);

        if(info._waitingNumber > 0)continue;
        PlantNode* node = getPlantNodeByIndex(i);
        float topHeight = node->getTopCpHeight();
        float growLen;
        float growSpeed = 0;
        if(!isReGrowByIndex(i))
        growSpeed= getGrowSpeedByIndex(i) ;
        else  growSpeed= getReGrowSpeedByIndex(i);
        if (growSpeed <= 0) {
            continue;
        }
        growLen = growSpeed *dt;
        
        bool isGrow = false;
        
        while (growLen > 0) {
            if(!isReGrowByIndex(i))
            {
                PlantNode* node = getPlantNodeByIndex(i);
                if (node->getGrowLength() > 0.003) {
                    growLen = node->grow(getPlantInfoByIndex(i)._preGrowDir, growLen);
                }
                else{ growLen = growByIndex(growLen,i);}
            }
            else{ growLen = reGrowByIndex( growLen,i); }
        }
        float nowTopHeight = node->getTopCpHeight();
        float step = nowTopHeight - topHeight;
        if(fabs(step) > 0){ isGrow = true;}
        
        if(isGrow)
        {
            isPlantGrow = true;
            updateHeadB2Body(i);
            if (i==0) {
                auto effectLayer =   GameLayerPlantCrashEffect::getRunningLayer();
                if (effectLayer) {
                    effectLayer->removeBorderLight();
                }
            }
            changePlantHeightByIndex(step, i);
            if (step > 0) {
                    PlantNode* node = getPlantNodeByIndex(i);
                    float height = node->getTopContorlPoint()._height;
                    if (height > 512) {
                    node->subTextureHeight(512);
                    changePlantHeightByIndex(-512,i);
                }
            }
            
            auto leafContext=  getPlantInfoByIndex(i)._leafsContext;
            if (leafContext) {
                leafContext->checkAddLeaf();
                leafContext->updateLeafsScale();
            }
        }
        else
        {
           // setGrowSpeedByIndex(0, i);
            if (i==0) {
                CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(_musicId);
                auto effectLayer =   GameLayerPlantCrashEffect::getRunningLayer();
                if (effectLayer) {
                    effectLayer->showCrashBorderLight();
                }
            }
        }
    }
    if (isPlantGrow) {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeEffect(_musicId);
        //updateRender();
    }
    else{
        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_musicId);
        _musicId =CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("plant_grow_music_1.mp3",true);
    }
}
float   GameLayerPlant::getGrowSpeedByIndex(int index)
{
    return _plants[index]._growSpeed;
}
void   GameLayerPlant::setGrowSpeedByIndex(float speed,int index)
{
    _plants[index]._growSpeed = speed;
}
float   GameLayerPlant::getPlantMinTopHeightInView()
{
    float height = -999999;
    for (auto& i:_plants) {
        float y = i._plant->getHeadPositionInWorld().y;
        if (height<-900000) {
            height = y;
        }
       else height = std::min(y,height);
    }
    return height;
    
}
float   GameLayerPlant::getPlantMaxTopHeightInView()
{
    float height = 0;
    for (auto& i:_plants) {
        float y = i._plant->getHeadPositionInWorld().y;
        height = std::max(y,height);
    }
    return height;

}
int     GameLayerPlant::getPlantCount()
{
    return _plants.size();
}
PlantNode* GameLayerPlant::getPlantNodeByIndex(int index)
{
    return _plants[index]._plant;
}
float GameLayerPlant::getReGrowSpeedByIndex(int index)
{
    return  _plants[index]._reGrowSpeed;
};
void  GameLayerPlant::setReGrowSpeedByIndex(float speed,int index)
{
    _plants[index]._growSpeed = speed;
}
//float GameLayerPlant::growByIndex(float length,int index)
//{
//       return  -1;
//}
float  GameLayerPlant::reGrowByIndex(float length,int index)
{
    float ret = -1;
    auto& info =  getPlantInfoByIndex(index);
    
    float needReGrowLength = length;
    if (needReGrowLength >= info._reGrowNeedLength) {
        needReGrowLength = info._reGrowNeedLength;
        info._reGrowNeedLength = 0;
        info._state = PlantStateGrowIng;
    }
    else
    {
        needReGrowLength = length;
        info._reGrowNeedLength = info._reGrowNeedLength -length;
    }
    ret = getPlantNodeByIndex(index)->reGrow(length);
    return ret;
}
bool  GameLayerPlant::isReGrowByIndex(int index)
{
   return _plants[index]._state == (int)PlantStateReGrowIng;
}

GameLayerPlant::PlantInfo& GameLayerPlant::getPlantInfoByIndex(int index)
{
    return _plants[index];
}
int  GameLayerPlant::addOnePlant(int plantConfigId,ContorlPointV2 startCp)
{
    int id = addOnePlant(plantConfigId);
    getPlantNodeByIndex(id)->resetStartContorlPoint(startCp);
    return id;
}
ContorlPointV2 GameLayerPlant::defaultStartContorlPoint()
{
    
    return ContorlPointV2();
}
int GameLayerPlant::addOnePlant(int plantConfigId)
{
    int id = _plants.size();
    auto info1 = GameRunningConfig::getInstance();
    auto& plantConfigs = info1->getPlantConfigs();
    assert(plantConfigId < plantConfigs.size());
    
    auto runtime = GameRuntime::getInstance();
    auto& first =  plantConfigs[plantConfigId];
    auto plant = PlantNode::create();
    plant->setTexture(first._texturePlantBody);
    
    plant->_textureLeft = first._textureLeft;
    plant->_textureRight = first._textureRight;
    float tgrowSpeed =  first._growSpeed;
    float treGrowSpeed = first._reGrowSpeed;
    plant->_updateCPRadiusCall = first._plantRadiusCallBack;
    plant->initPlant(first._plantStartContorlPointList,runtime->getPlantCPListUnitHeight(),runtime->getPlantVerticesUnitHeight());
    plant->setPosition(first._plantNodeInitPosition);
    
    if (first._plantRadiusCallBack) {
        plant->_updateCPRadiusCall = first._plantRadiusCallBack;
    }
    else
    {
        plant->_updateCPRadiusCall = std::bind(&PlantHelper::PlantRadiusHelper::getRadiusByHeight,&first._radiusList,std::placeholders::_1);
    }
    
    addChild(plant);
    // Render
   // plant->setVisible(false);
    ///
    GameLayerPlant::PlantInfo info;
    info._plant = plant;
    info._growSpeed = tgrowSpeed;
    info._reGrowSpeed = treGrowSpeed;
    info._state = first._state;
    _plants.push_back(info);
    _plants[id]._leafsContext = createOnePlantLeafList();
    if(_plants[id]._leafsContext)
    {
    for (auto& i :plantConfigs[id]._texturePlantLeaf ) {
        _plants[id]._leafsContext->_pathLeafs.push_back(i);
    }
    _plants[id]._leafsContext->_plantId = id;
    _plants[id]._leafsContext->_leafScaleRadius = first._leafsRadiusList;
    _plants[id]._leafsContext->_leafStepHeight = 201;
    _plants[id]._leafsContext->_leafStepRandHeight = 51;
    _plants[id]._leafsContext->_node = this;
    }
    return id;
}
int  GameLayerPlant::addOnePlant(int plantConfigId,Vec2 startCp)
{
    return 0;
}
int GameLayerPlant::addOnePlant()
{
    int id = _plants.size();
    return addOnePlant(id);
}///////
void GameLayerPlant::onEnter()
{
    GameLayerBase::onEnter();
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("plant_grow_music_1.mp3");
    
    _musicId =CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("plant_grow_music_1.mp3",true);
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(_musicId);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.04);
}
void GameLayerPlant::onExit()
{   GameLayerBase::onExit();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(_musicId);
}

int GameLayerPlant::getPlantAdormsInHeightRangeByIndex(int index,float start, float end,PlantAdormList* list)
{
    return 0;
}
void  GameLayerPlant::updateHeadB2Body(int index)
{
    auto& info =getPlantInfoByIndex(index);
    if (!info._headBody)return;
    updateHeadB2Body( info._headBody,index);
}
void  GameLayerPlant::updateHeadB2Body(b2Body* body,int index)
{    auto node = getPlantNodeByIndex(index);
    Vec2 cp = node->getHeadPositionInWorld();
    b2Vec2 newpt(cp.x/PTM_RATIO,cp.y/PTM_RATIO);
    float angle = node->getHeadAnalge();
    body->SetTransform(newpt, -CC_DEGREES_TO_RADIANS( angle));
}
 void   GameLayerPlant::removeHeadBodys()
{
    auto world =GamePhysicalWorld::getInstance()->getBox2dWorld();
    for (auto& i : _plants) {
        world->DestroyBody(i._headBody);
        if( i._headItem)delete i._headItem;
    }
}
void  GameLayerPlant::createHeadB2Body(int index)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(0,0);
    auto& info =getPlantInfoByIndex(index);
    info._headItem = new ItemModel();
    info._headItem->setType(99-index);
    info._headBody = createHeadB2Body(info._headItem,10);
}
int  GameLayerPlant::getPlantIndexByPlantHeadBodyType(unsigned char type)
{
    return  99 - type;
}
b2Body*   GameLayerPlant::createHeadB2Body(ItemModel* item,float radius)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(0,0);
    bodyDef.userData = item;
    // bodyDef.linearDamping = 0.3;
    bodyDef.angle = 0;
    //bodyDef.linearVelocity = b2Vec2()
    b2Body* body = GamePhysicalWorld::getInstance()->getBox2dWorld()->CreateBody(&bodyDef);
    if (!body) {
        return nullptr;
    }
    b2FixtureDef fixtureDef;
    b2PolygonShape shape;
    float len =radius/PTM_RATIO;
    
    b2Vec2 vec1[16];
    float step = 360.0f / 16.0f;
    for (int i = 0; i < 16; i++) {
        Vec2 pt =  MathHelper::getRotatePosition(Vec2(0,0), Vec2(0,len), i * step);
        vec1[i] = b2Vec2(pt.x,pt.y);
    }
    //b2CircleShape circleShape;
    //circleShape.m_radius = 70/PTM_RATIO;
    
    shape.Set(vec1, 16);
    fixtureDef.shape = &shape;
    fixtureDef.density = 10.0;
    fixtureDef.filter.categoryBits = 0x01;
    fixtureDef.filter.maskBits = 0x02;
    fixtureDef.restitution = 0.1;
    body->CreateFixture(&fixtureDef);
    
    return body;
}

float  GameLayerPlant::growByIndex(float length,int index)
{
    float retLen = -1;
    
    FaceDirection turnDir;
    
    bool isGrow = false;
    
    PlantNode* node = getPlantNodeByIndex(index);
    float angle = node->getHeadAnalge();
    auto growAngle = getPlantGrowContextAngle(node);
    Vec2 headTop = node->getHeadPositionInWorld();
    auto call2 = [=](ItemModel* item)
    {
        return item->isStone()||item->getType() == TypeUIBorderLine;
    };
    auto con2 = PlantHelper::getPlantRayCastContext1(headTop,80,angle - 90,10,18,call2);
    
    int tcount =con2.getCrashLengthCountByAngleRange(70, angle-15, angle+15);
    
    PlantHelper::PlantGrowContext growStone(true);
    if (tcount > 0) {
        growStone._top = false;
    }
//    if (growAngle.allIsValue(true)) {
//        int lcount =con2.getCrashLengthCountByAngleRange(70, angle-50, angle-15);
//        int rcount =con2.getCrashLengthCountByAngleRange(70, angle+15, angle+50);
//        if(lcount > 0 && rcount > 0)
//        {
//            float lmin =con2.getMinCrashLengthByAngleRange(angle-70, angle-10);
//            float rmin =con2.getMinCrashLengthByAngleRange( angle+10, angle+70);
//            growStone._left = lmin < rmin ?false:true;
//            growStone._right = rmin < lmin ?false:true;
//            
//            if (growStone.allIsValue(true)||growStone.allIsValue(false)) {
//                log("error");
//            }
//        }
//        else if(lcount > 0 || rcount > 0)
//        {
//            int count = 1;
//            if (growStone._top) {
//                count = 3;
//            }
//            if (lcount > count) {
//                growStone._left = false;
//            }
//            else if(rcount > count)
//            {
//                growStone._right  = false;
//            }
//            if (growStone.allIsValue(false)) {
//                growStone._left = lcount > rcount ? false : true;
//                growStone._right = ! growStone._left;
//            }
//        }
//    }
//    
    int lcounts =con2.getCrashLengthCountByAngleRange(44, angle-70, angle-10);
    int rcounts =con2.getCrashLengthCountByAngleRange(44, angle+10, angle+70);
    bool isTestStone = true;
//    if (!growAngle.allIsValue(true)||
//        !growStone.allIsValue(true) ||
//        (lcounts > 0 && rcounts > 0)) {
//        isTestStone = false;
//    }
    if (lcounts > 0 && rcounts > 0) isTestStone = false;
    if (!isTestStone) {
        float lmin =con2.getMinCrashLengthByAngleRange(angle-70, angle-10);
        float rmin =con2.getMinCrashLengthByAngleRange( angle+10, angle+70);
        growStone._left = lmin < rmin ?false:true;
        growStone._right = rmin < lmin ?false:true;
        if (growStone.allIsValue(true)||growStone.allIsValue(false)) {
            log("error");
        }
    }
    
    
    auto call = [=](ItemModel* item)
    {
        if(item->isDirtLine())
        {
            return true;
        }
        else if(isTestStone)
        {
            if( item->isStone()||item->getType() == TypeUIBorderLine)
                return true;
        }
        return false;
    };
    
    std::vector<FaceDirection> growDirList;
    PlantHelper::getGroeDirList(node,growDirList);
    growDirList.push_back(FaceTop);
    auto minGrow = PlantHelper::getGrowContextGrowNextUnitLengthTestMap(node);
    PlantHelper::PlantGrowContext min(true);
    min &= minGrow;
    min &= growAngle;
    if(!(growAngle&growStone).allIsValue(false))
    {
        min &= growStone;
    }
    if (tcount > 0) {
        min._top = false;
    }
    auto con1 = PlantHelper::getPlantRayCastContext1(headTop,90,angle - 90,10,18,call);
    
    PlantHelper::PlantGrowContext  growTest(true);
    int lcount1 =con1.getCrashLengthCountByAngleRange(40, angle-70, angle-10);
    int rcount1 =con1.getCrashLengthCountByAngleRange(40, angle+10, angle+70);
    int tcount1 =con1.getCrashLengthCountByAngleRange(45, angle-10, angle+10);
    if (lcount1 > 1 )  growTest._left = false;
    if (rcount1 > 1 )  growTest._right = false;
    if (tcount1 > 0 )  growTest._top   = false;
    
    float al1 = con1.getCrashLengthAveByAngleRange(angle - 70,angle-10);
    float ar1 = con1.getCrashLengthAveByAngleRange(angle + 10,angle + 70);
    
    min &= growTest;
    
    for (int i = 0 ; i < 2 ; i++)
    {
        PlantHelper::PlantGrowContext  grow(true);
        if (i<1&&min.allIsValue(true)) {
            if((fabs(angle)<90)||
               (fabs(angle)>=90&&al1<80&& ar1<80))
            {
                grow._left = (ar1 - al1) > 3 ? false : true;
                grow._right = (al1 - ar1) > 3 ? false : true;
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
        getPlantInfoByIndex(index)._preGrowDir = turnDir;
        retLen =node->grow(turnDir, length);
    }
    return retLen;
}
#include "ItemModel.h"
PlantGrowContext  GameLayerPlant::getPlantGrowContextAngle(PlantNode* plant)
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
    Vec2 vleft = MathHelper::getRotatePosition(cpo, cpo + Vec2(-150,0),0);
    Vec2 vright = MathHelper::getRotatePosition(cpo, cpo + Vec2(150,0),0);
    float lenLeft = physicalWorld->rayCastTest(cpo,vleft,rayCall,&outLeft);
    float lenRight = physicalWorld->rayCastTest(cpo,vright,rayCall,&outRight);
    
    if (lenLeft > 0 && outLeft){
        Vec2 pt =outLeft->getPosition();
        float itemy = pt.y;
        if (outLeft->getParent()) {
            itemy = outLeft->getParent()->convertToWorldSpace(pt).y;
        }
        if (cpo.y > itemy + 15 - outLeft->getContentSize().height*0.5) {
            angleLeft = -PlantHelper::getMinMaxAngleByLength(lenLeft, 15, 120, 30, 80);
        }
    }
    if (lenRight > 0 && outRight){
        Vec2 pt =outRight->getPosition();
        float itemy = pt.y;
        if (outRight->getParent()) {
            itemy = outRight->getParent()->convertToWorldSpace(pt).y;
        }
        if (cpo.y > itemy + 15 - outRight->getContentSize().height*0.5) {
            angleRight= PlantHelper::getMinMaxAngleByLength(lenRight,15,120, 30, 80);
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
    
    float len = 180;
    if (size.width < 768) {
        len = 140;
    }
    
    if (x < len)
    {
        float testAngle = -PlantHelper::getMinMaxAngleByLength(x,30,len + 30,0,110);
        if(angle < testAngle)
        {
            grow._left = false;
        }
    }
    else if(x > size.width - len)
    {
        float testAngle = PlantHelper::getMinMaxAngleByLength(size.width - x,30,len - 30,0,110);
        if( angle > testAngle)
        {
            grow._right = false;
        }
    }
    return grow;
}
bool   GameLayerPlant::saveToFile()
{
    int plantCount = getPlantCount();
    auto userInfo =  cocos2d::UserDefault::getInstance();
    userInfo->setIntegerForKey(ConfigKeys::_sKeyPlantCount,plantCount);
    for (int i = 0; i < plantCount ; i++) {
        auto plant = getPlantNodeByIndex(i);
        auto key = StringUtils::format("%s%d",ConfigKeys::_sKeyPlantDataId,i);
        cocos2d::Data data;
        plant->saveInfoToData(data);
        userInfo->setDataForKey(key.c_str(), data);
        
        auto keyLeaf = StringUtils::format("%s%d",ConfigKeys::_sKeyPlantLeafsId,i);
        cocos2d::Data dataLeaf;
        _plants[i]._leafsContext->saveInfoToData(dataLeaf);
        userInfo->setDataForKey(keyLeaf.c_str(), dataLeaf);
        
        Vec2 position = plant->getPosition();
        auto keyPosition = StringUtils::format("%s%d",ConfigKeys::_sKeyPlantNodePositionId,i);
        cocos2d::Data dataPosition;
        dataPosition.copy((unsigned char*)&position, sizeof(Vec2));
        userInfo->setDataForKey(keyPosition.c_str(),dataPosition);
    }
    
    userInfo->setIntegerForKey(ConfigKeys::_sKeyPlantInfosCount,(int)_plants.size());
    for (int i = 0 ; i<_plants.size(); i++) {
        std::string str;
        str.append((const char*)(&_plants[i]),sizeof(_plants[0]));
        auto key = StringUtils::format("%s%d",ConfigKeys::_sKeyPlantInfosId,i);
        cocos2d::Data data;
        data.copy((unsigned char*)str.data(), str.size());
        userInfo->setDataForKey(key.c_str(), data);
    }
    userInfo->flush();
    return true;
}
bool   GameLayerPlant::initBySaveConfig()
{
    auto userInfo =  cocos2d::UserDefault::getInstance();
    int plantCount = userInfo->getIntegerForKey(ConfigKeys::_sKeyPlantCount);
    for (int i = 0; i < plantCount ; i++) {
        auto plant = getPlantNodeByIndex(i);
        auto key = StringUtils::format("%s%d",ConfigKeys::_sKeyPlantDataId,i);
        cocos2d::Data data;
        data = userInfo->getDataForKey(key.c_str());
        plant->resetInfoBySaveData(data);
        auto keyLeaf = StringUtils::format("%s%d",ConfigKeys::_sKeyPlantLeafsId,i);
        cocos2d::Data dataLeaf;
        dataLeaf = userInfo->getDataForKey(keyLeaf.c_str());
        _plants[i]._leafsContext->resetInfoBySaveData(dataLeaf);
        

        auto keyPosition = StringUtils::format("%s%d",ConfigKeys::_sKeyPlantNodePositionId,i);
        cocos2d::Data dataPosition;
        dataPosition =  userInfo->getDataForKey(keyPosition.c_str());
        plant->setPosition(*((Vec2*)dataPosition.getBytes()));

    }
    
    int infoSize = userInfo->getIntegerForKey(ConfigKeys::_sKeyPlantInfosCount);
    for (int i = 0 ; i < infoSize; i++) {
        std::string str;
        auto key = StringUtils::format("%s%d",ConfigKeys::_sKeyPlantInfosId,i);
        cocos2d::Data data;
        data = userInfo->getDataForKey(key.c_str());
        str.append((const char*)data.getBytes(),data.getSize());
        
        PlantInfo* plantInfo = (PlantInfo*)str.data();
        _plants[i]._growSpeed = plantInfo->_growSpeed;
        _plants[i]._isNeedWaiting = plantInfo->_isNeedWaiting;
        _plants[i]._reGrowSpeed = plantInfo->_reGrowSpeed;
        _plants[i]._reGrowNeedLength = plantInfo->_reGrowNeedLength;
        _plants[i]._waitingNumber = plantInfo->_waitingNumber;
        _plants[i]._state   = plantInfo->_state;
        _plants[i]._preGrowDir   = plantInfo->_preGrowDir;
    }
    

    return true;
}
PlantLeafListBase* GameLayerPlant::createOnePlantLeafList()
{
    
    return new PlantLeafListBase();
}
void  GameLayerPlant::updateRender()
{
    _render->beginWithClear(0, 0, 0, 0);
    for (auto& i : _plants) {
        i._plant->setVisible(true);
        i._plant->visit();
    }
    _render->end();
    for (auto& i : _plants) {
        i._plant->setVisible(false);
    }
}