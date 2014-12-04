#include "GameLayerPlant.h"
#include "Plant.h"
#include "GameHeaders.h"
#include "GameManager.h"
#include "GameLayerEffects.h"
// std::string  GameLayerPlant::_sRunningGamePlantName = typeid(GameLayerPlant).name();
std::string  GameLayerPlant::_sRunningGamePlantName ; //= typeid(GameLayerPlant).name();
bool GameLayerPlant::init()
{
   // assert (_sRunningGamePlantName == typeid(GameLayerPlant).name()) ;
    GameLayerBase::init();
    
    _sRunningGamePlantName = getType().name();
    return true;
}

bool    GameLayerPlant::releaseGameInfo()
{
     removeHeadBodys();
    for (auto& i: _plants) {
        i._plant->removeFromParent();
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
    }
    
}
void GameLayerPlant::update(float dt)
{
    if(!GameRunningInfo::getInstance()->isPlantNeedGrow())return;
    int count = this->getPlantCount();
    for(int i = 0; i < count;i++)
    {
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
                growLen = growByIndex(growLen,i);
            else
            {
                growLen = reGrowByIndex( growLen,i);
            }
        }
        float nowTopHeight = node->getTopCpHeight();
        float step = nowTopHeight - topHeight;
        if(fabs(step) > 0)
        {
            isGrow = true;
        }
        if(isGrow)
        {
            updateHeadB2Body(i);
            if (i==0) {
                CocosDenshion::SimpleAudioEngine::getInstance()->resumeEffect(_musicId);
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
        }
        else
        {
            if (i==0) {
                CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_musicId);
                _musicId =CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("plant_grow_music_1.mp3",true);
                CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(_musicId);
                auto effectLayer =   GameLayerPlantCrashEffect::getRunningLayer();
                if (effectLayer) {
                    effectLayer->showCrashBorderLight();
                }
              
            }
        }
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
    float height = -9999;
    for (auto& i:_plants) {
        float y = i._plant->getHeadPositionInWorld().y;
        if (height<-9000) {
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
float GameLayerPlant::growByIndex(float length,int index)
{
       return  -1;
}
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
int GameLayerPlant::addOnePlant()
{
    int id = _plants.size();
    auto info1 = GameRunningConfig::getInstance();
    auto& plantConfigs = info1->getPlantConfigs();
    assert(id < plantConfigs.size());

    auto runtime = GameRuntime::getInstance();
    auto& first =  plantConfigs[id];
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
         plant->_updateCPRadiusCall =std::bind(&PlantHelper::PlantRadiusHelper::getRadiusByHeight,&first._radiusList,std::placeholders::_1);
    }
 
    addChild(plant);
    
    GameLayerPlant::PlantInfo info;
    
    info._plant = plant;
    info._growSpeed = tgrowSpeed;
    info._reGrowSpeed = treGrowSpeed;
    info._state = first._state;
    _plants.push_back(info);
  
    return id;
}

///////
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
    auto node = getPlantNodeByIndex(index);
    Vec2 cp = node->getHeadPositionInWorld();
    b2Vec2 newpt(cp.x/PTM_RATIO,cp.y/PTM_RATIO);
    float angle = node->getHeadAnalge();
 
    info._headBody->SetTransform(newpt, -CC_DEGREES_TO_RADIANS( angle));
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
    bodyDef.userData = info._headItem;
   // bodyDef.linearDamping = 0.3;
    bodyDef.angle = 0;
    //bodyDef.linearVelocity = b2Vec2()
    b2Body* body = GamePhysicalWorld::getInstance()->getBox2dWorld()->CreateBody(&bodyDef);
    b2FixtureDef fixtureDef;
    b2PolygonShape shape;
    float len =10.0f/PTM_RATIO;
    b2Vec2 vec[3] ={
        b2Vec2(0,5.0f/PTM_RATIO),
        b2Vec2(-len,-len),
        b2Vec2(len ,-len)
    };
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

    info._headBody = body;
}
int  GameLayerPlant::getPlantIndexByPlantHeadBodyType(unsigned char type)
{
    return  99 - type;
}