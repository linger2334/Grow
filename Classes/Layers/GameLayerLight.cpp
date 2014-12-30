#include "GameLayerLight.h"
#include "GameLayerPlant.h"
#include "ContorlPoint.h"
#include "LightNode.h"
#include <algorithm>

#define REMOVE_LIGHT_NORMAL_TIME 2
#define REMOVE_LIGHT_NORMAL_SCALE 10


static float PathGetLength(const ContorlPointV2& p1,const ContorlPointV2& p2)
{
    return p1._point.getDistance(p2._point);
}

static ContorlPointV2 PathGetPoint(float lenght, const ContorlPointV2& start,const ContorlPointV2& end)
{
    Vec2 tstart = start._point;
    Vec2 tend   = end._point;
    float len = fabs( tstart.getDistance(tend) );
    float d = lenght / len;
    float xlen = (tend.x-tstart.x)*d;
    float ylen = (tend.y-tstart.y)*d;
    
    ContorlPointV2 ret = start;
    ret._point =  Vec2(tstart.x+xlen,tstart.y+ylen);
    return ret;
}
PlantPathList GameLayerLight::getOnePlantPathList(int plantIndex)
{
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(plantIndex);
    return {&plant->_cpList , PathGetLength,PathGetPoint};
}
bool GameLayerLight::init()
{
    GameLayerBase::init();
    return true;
}
void GameLayerLight::update(float dt)
{
    _plantCount = GameLayerPlant::getRunningLayer()->getPlantCount();
    for (int i = 0 ; i < _plantCount; i++) {
        
        checkLightUpdateAction(i,4,LightActionFadeInAndOut);
        checkLightUpdateAction(i,2,LightActionScaleToBigAndSmall);
        updateLightsByPlantIndex(dt,i);
        for (auto& ip : _plantLights[i]._lightLists)
        {
            if (ip.second._state == LightMoveToPlantLine) {
                continue;
            }
            if(ip.second._state != LightNormal) continue;
            ip.second._target->update(dt);
        }
        
    }

}
bool GameLayerLight::initGameInfo()
{
    GameLayerBase::initGameInfo();

    _plantLights.resize(2);
    auto call = std::bind(&GameLayerLight::onPlantHeightChange,this,std::placeholders::_1,std::placeholders::_2);
    GameRunningConfig::getInstance()->_plantHeightChangeCallbacks.push_back(call);
    
    auto call1 = std::bind(&GameLayerLight::onPlantCpListChange,this,std::placeholders::_1,std::placeholders::_2);
    GameRunningConfig::getInstance()->_plantCPChangeCallbacks.push_back(call1);
    _plantCount = GameLayerPlant::getRunningLayer()->getPlantCount();
    return  true;
}
bool GameLayerLight::releaseGameInfo()
{
    return true;
}
void GameLayerLight::moveDown(float yLen)
{
    moveDownLights(yLen);
    auto ip = _lightReleaseList.begin();
    while (ip!=_lightReleaseList.end()) {
        float newY = (*ip)->getPositionY()-yLen;
        if (newY < -100) {
            (*ip)->removeFromParent();
            ip = _lightReleaseList.erase(ip);
            continue;
        }
        (*ip)->setPositionY(newY);
        ip++;
    }
}
float GameLayerLight::getLengthByPlantHeight(float plantHeight)
{
    return 0;
}
void  GameLayerLight::onPlantCpListChange(float height,int index)
{

    PlantPathList  plantLineList = getOnePlantPathList(index);

    _plantLights[index]._lightLength = plantLineList.getTotalLength();
}
void  GameLayerLight::onPlantHeightChange(float height,int index)
{
    if(height < -400)return ;
    onChangePlantList(index);
}
void GameLayerLight::initLightsBySubMap()
{
    int plantCount = GameLayerPlant::getRunningLayer()->getPlantCount();
    auto config = GameRunningConfig::getInstance();
    for(int i = 0 ; i< plantCount;i++)
    {
        int count = config->getLightInitCount(i);
        addLightsBySubMapInit(i, count);
    }
}
Action* GameLayerLight::createInitAction(Vec2 startPoint,const LightPathContext& context)
{
    assert(0);
    return nullptr;
}
LightPathContext* GameLayerLight::getLightContextByPlantIndexAndId(int plantIndex,int id)
{
    auto& list = _plantLights[plantIndex]._lightLists;
    if (list.size()==0)return nullptr;
    auto ip = list.find(id);
    if (ip!= list.end()) {
        return &(ip->second);
    }
    return nullptr;
}
//////////////////////////////////////////////////////

void  onLightRunningInitActionEnd(LightPathContext* conf,int plantIndex,int lightId)
{
    conf->_state = LightNormal;
    ((LightNode*)conf->_target->getChildren().at(0))->changeLightState(LightNode::StateRandAction);
    conf->_target->startRotate();
}
CallFunc* GameLayerLight::createLightRunningInitActionEndCall(LightPathContext* conf,int plantIndex,int lightId)
{
    auto callfunc = [](LightPathContext* conf,int plantIndex,int lightId)
    {
        conf->_state = LightNormal;
        ((LightNode*)conf->_target->getChildren().at(0))->changeLightState(LightNode::StateRandAction);
        conf->_target->startRotate();
    };
    return CallFunc::create(std::bind(callfunc,conf,plantIndex,lightId));
   
}
void  GameLayerLight::addOneLightByPlantIndex(Vec2 startPoint,int index)
{
    auto config1 =  randCreateLightPahtContext(index,startPoint);
    int id = config1->_id;

    CallFunc* call = createLightRunningInitActionEndCall(config1,index,id);
    
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(index);
    PlantPathList  plantLineList = getOnePlantPathList(index);

    
    int len = plantLineList.getTypePointByLength(config1->_toLengthByTop,false,nullptr);
    if (len < 0 ) {
        config1->_state = LightOutPlantLine;
        return;
    }
    config1->_target->setContorlPosition(plant->convertToWorldSpace( plantLineList._findType._point));
    Vec2 pt = plant->convertToWorldSpace( plantLineList._findType._point);
    
    Vec2 newPt1 = (pt - startPoint);
    MoveBy* moveby = MoveBy::create(1,newPt1);

    config1->_target->runAction(Sequence::create(moveby,call, NULL));
    config1->_nowLengthByTop = config1->_toLengthByTop ;
}
LightPathContext* GameLayerLight::randCreateLightPahtContext(int plantIndex,Vec2 startPoint)
{
    auto config = randLightConfig();
    config._toLengthByTop = getOneRandLightInitLength(plantIndex);
    config._nowLengthByTop = config._toLengthByTop;
    config._state = LightMoveToPlantLine;
    startPoint = UIHelper::getPositionToSameParentByWorld(config._target, startPoint);
    config._target->setPosition(startPoint);
    int id = _addOneLightByPlantIndex(plantIndex,config);
    
    return getLightContextByPlantIndexAndId(plantIndex, id);
}
void  GameLayerLight::addOneLightByPlantIndexUseBezier(Vec2 startPoint,int index)
{

    startPoint = UIHelper::getNodePositionByWorld(this, startPoint);
    auto config1 =  randCreateLightPahtContext(index,startPoint);
    int id = config1->_id;
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(index);
    
    PlantPathList  plantLineList = getOnePlantPathList(index);
    
    plantLineList.getTypePointByLength(config1->_toLengthByTop,false,nullptr);
    config1->_target->setContorlPosition(plant->convertToWorldSpace( plantLineList._findType._point));
    Vec2 lightPosition = plant->convertToWorldSpace( plantLineList._findType._point);
    lightPosition = UIHelper::getPositionToSameParentByWorld( config1->_target, lightPosition);
    config1->_target->setContorlPosition(lightPosition);
    config1->_target->setPosition(startPoint);
    Vec2 endPoint = config1->_target->getNowTimeTargetPositionInWorld();
    auto* bezier = createLightBezierAction(startPoint,endPoint,2);
    //auto* bezier = MoveBy::create(2, endPoint - startPoint);
    CallFunc* call = createLightRunningInitActionEndCall(config1,index,id);
    ;
    config1->_target->runAction(Sequence::create(bezier,call,nullptr));
   
}


////////////////////////////////////////////////////////

void  GameLayerLight::updateLightsByPlantIndex(float dt,int index)
{
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(index);
    
    PlantPathList  plantLineList = getOnePlantPathList(index);
    
    float maxLength = plantLineList.getTotalLength();
    float plantGrowSpeed = GameLayerPlant::getRunningLayer()->getGrowSpeedByIndex(index);
    
    //log("Size %d",_lightLists[index].size());
    for (auto& ip :  _plantLights[index]._lightLists) {
        auto& i = ip.second;
        if(!i._target)
        {
            i._state = LightNoHasTarget;
            continue;
        }
        if (i._state == LightNormal) {
           float length = i._toLengthByTop - i._nowLengthByTop;

            bool isNeedAddMoveSpeed = false;
            float newPosition;
            int sign = length > 0 ? 1 : -1;
            if (static_cast<long>( length ) == 0 || sign > 0) {
                i._moveSpeed = 0;
                newPosition= i._nowLengthByTop;
            }
            else
            {
                float step = dt * i._moveSpeed;
                if(step > fabs(length))step = length;
                newPosition= i._nowLengthByTop + sign * step;
                isNeedAddMoveSpeed = true;
            }

            int ret= plantLineList.getTypePointByLength(newPosition,false,nullptr);
            if( ret < 0 )
            {
                i._moveSpeed = 0;
                i._state = LightOutPlantLine;
                continue;
            }
           
            Vec2 pt = plant->convertToWorldSpace(plantLineList._findType._point);
            Node* parent = i._target->getParent();
            if (parent) pt = parent->convertToNodeSpace(pt);
            i._target->setContorlPosition(pt);
   
            i._nowLengthByTop = newPosition;
            if (isNeedAddMoveSpeed) {
                i._moveSpeed += dt*plantGrowSpeed*0.3;
                float maxSpeed = fabs(length);
                if ( i._moveSpeed > maxSpeed) {
                    i._moveSpeed  = maxSpeed;
                }
            }
            continue ;
        }
        else if(i._state == LightOutPlantLine)
        {
            float len = plantLineList.getTotalLength();
            if(i._toLengthByTop > len)continue;
            
            int ret= plantLineList.getTypePointByLength(i._toLengthByTop,false,nullptr);
            if( ret < 0 )continue;
            plantLineList.getTypePointByLength(0, true, nullptr);
            Vec2 pt = plant->convertToWorldSpace(plantLineList._findType._point);
            Node* parent = i._target->getParent();
            if (parent) pt = parent->convertToNodeSpace(pt);
            i._target->setContorlPosition(pt);
            i._state = LightNormal;
            i._nowLengthByTop = maxLength;
            i._target->setTime(0.75);
            continue;
        }
    }
    
}
void   GameLayerLight::onChangePlantList(float index)
{
    PlantPathList  plantLineList = getOnePlantPathList(index);
    
    float totalLenth = plantLineList.getTotalLength();
    float step = totalLenth - _plantLights[index]._lightLength;

    for (auto& ip : _plantLights[index]._lightLists) {
        auto& i = ip.second;

            i._nowLengthByTop += step;
            if (i._nowLengthByTop > totalLenth) {
                i._state = LightOutPlantLine;
            }
    }
    _plantLights[index]._lightLength += step;

}
void GameLayerLight::moveDownLights(float yLen)
{
    int count = GameLayerPlant::getRunningLayer()->getPlantCount();
    for (int i = 0; i < count;i++) {
        for (auto& ip :_plantLights[i]._lightLists) {
             auto& i = ip.second;
            if(i._target)
            {
                i._target->setPositionY(i._target->getPositionY()-yLen);
                Vec2 cp = i._target->getContorlPosition();
                i._target->setContorlPosition(Vec2(cp.x,cp.y-yLen));
            }
        }
    }
}
void GameLayerLight::testRandRotateLength()
{
//     int count = GameLayerPlant::getRunningLayer()->getPlantCount();
//    for (int i = 0; i < count;i++) {
//        for (auto& ip :_plantLights[i]._lightLists) {
//            auto& i = ip.second;
//            i._toRotateA = rand()%30+30;
//            i._toRotateB = rand()%30+20;
//        }
//    }
}
int  GameLayerLight::_addOneLightByPlantIndex(int index,const LightPathContext& config)
{
    int i = 0;
    int findNewId = 0;
    while (true) {
        auto ip = _plantLights[index]._lightLists.find(i);
        if (ip == _plantLights[index]._lightLists.end()) {
            findNewId = i;
            break;
        }
        i++;
    }
    auto& configMap = _plantLights[index]._lightLists[findNewId];
    configMap = config;
    configMap._id = findNewId;
    configMap._plantIndex = index;
    configMap._target->setTag(getLightTag(index, findNewId));
    return findNewId;
}
LightPathContext GameLayerLight::randLightConfig()
{
    TuoYuanRotateNode* node = TuoYuanRotateNode::create();
    node->initOvalConfig(Vec2(100,-100),RandomHelper::rand(8, 19),  RandomHelper::rand(30, 60),  RandomHelper::rand(25, 50),RandomHelper::rand(0,255)<125);
    node->setPosition(Vec2(100,-100));
    node->_timeNow =  node->randTime();
    LightPathContext context;
    context._moveSpeed = 0;
    context._toLengthByTop = rand()%400+50;
    context._state = LightOutPlantLine;
//    context._toRotateA = RandomHelper::rand(30, 60);
//    context._toRotateB = RandomHelper::rand(25, 50);
//    context._changeRotateSpeed = 60;
    node->addChild(createOnLightChildNode());
    context._target = node;
    addChild(node);
    return  context;
}
void GameLayerLight::removeLight(int plantIndex,int id)
{
    auto context = getLightContextByPlantIndexAndId(plantIndex,id);
    if(!context)return ;
    context->_state = LightWillRemove;
    
    if (context->_target&&context->_target->getChildrenCount()>0)
    {
        FadeTo* fadeOut = FadeTo::create(REMOVE_LIGHT_NORMAL_TIME,0);
        ScaleTo* scaleTo = ScaleTo::create(REMOVE_LIGHT_NORMAL_TIME, REMOVE_LIGHT_NORMAL_SCALE);
        auto call = [](GameLayerLight* layerLight,int planti,int lightid)
        {
            auto context = layerLight->getLightContextByPlantIndexAndId(planti,lightid);
             layerLight->_lightReleaseList.push_back(context->_target);
            layerLight->removeLightToPlantList(planti,lightid);
        };
      
        context->_target->stopAllActions();
        auto node = context->_target->getChildren().at(0);
        node->stopAllActions();
        node->runAction(Spawn::create(fadeOut,scaleTo,CallFunc::create(std::bind(call,this,plantIndex,id)),nullptr));
    }
    else{
        auto node = context->_target->getChildren().at(0);
        node->stopAllActions();
         _lightReleaseList.push_back(context->_target);
        removeLightToPlantList(plantIndex,id);
    }
    
}
void GameLayerLight::removeLightUseUseBezier(int plantIndex,int id,Vec2 endPoint,float bezierTime ,float fadeTime)
{
    auto light = getLightContextByPlantIndexAndId(plantIndex, id);
    
    //修正坐标位置 贝塞尔 起点 终点位置 都应该是 光父节点的相对位置
    endPoint = UIHelper::getPositionToSameParentByWorld(light->_target, endPoint);
    auto startPoint = light->_target->getPosition();;
    light->_state = LightWillRemove;
    
    FadeTo* fadein = FadeTo::create(bezierTime, 255);
    ScaleTo* scaleto = ScaleTo::create(bezierTime, 1.0f);
    BezierBy* bezier = createLightBezierAction(startPoint, endPoint, bezierTime);
    FadeTo* fadeout = FadeTo::create(fadeTime,0);
    auto callfunc = [](GameLayerLight* layerLight,int plantIndex,int id)
    {
        layerLight->removeLightByPlantIndexAndLightIdNoUseAction(plantIndex,id);
    };
    CallFunc* call = CallFunc::create(std::bind(callfunc,this,plantIndex,id));
    light->_target->stopRotate();
    light->_target->runAction(Sequence::create(Spawn::create(bezier,fadein,scaleto,NULL),fadeout,call,nullptr));
}
void GameLayerLight::removeLightUseUseBezier(int plantIndex,int id,Vec2 endPoint)
{
    removeLightUseUseBezier(plantIndex,id,endPoint,1.0f,0.5f);
}
void GameLayerLight::removeOneLightUseBezier(int plantIndex,Vec2 endPoint)
{
   int id =  randLightByPlantNotWillRemove(plantIndex);
    if(id<0)return ;
    removeLightUseUseBezier(plantIndex,id,endPoint);
}
void GameLayerLight::removeOneLightUseBezier(int plantIndex,Vec2 endPoint,float bezierTime ,float fadeTime)
{
    int id =  randLightByPlant(plantIndex);
    removeLightUseUseBezier(plantIndex,id,endPoint,bezierTime,fadeTime);
}
int  GameLayerLight::getMinLengthLightId( int plantIndex)
{
    std::vector<tempCheckToPositionContext> list ;
    
    auto& lightList = _plantLights[plantIndex]._lightLists;
    for (auto& i : lightList) {
        if(i.second._state != LightNormal)continue;
        tempCheckToPositionContext temp ;
        temp.id = i.second._id;
        temp.toLengthByTop = i.second._nowLengthByTop;
        list.push_back(temp);
    }
    if (list.size() ==  0) {
        return -1;
    }
    auto call = []( const tempCheckToPositionContext& a,
                   const tempCheckToPositionContext& b)->bool
    {
        return a.toLengthByTop < b.toLengthByTop;
    };
    std::sort(list.begin(), list.end(),call);
    return  list.front().id;
}
void GameLayerLight::removeLightRandId(int plantIndex)
{
    int id = randLightByPlant(plantIndex);
    if (id <0)return ;
    removeLight(plantIndex, id);
    checkUpdateToPosition(plantIndex);
}

void GameLayerLight::removeLightTopId(int plantIndex)
{
    int id = getMinLengthLightId(plantIndex);
    if (id <0)return ;
    removeLight(plantIndex, id);
    checkUpdateToPosition(plantIndex);
}
void  GameLayerLight::removeLights(int plantIndex,int count)
{
   // removeLightTopId(plantIndex);
    std::function<void(void)> func;
    auto calltem = [](GameLayerLight* layerLight,int plantId)
    {
        layerLight->removeLightTopId(plantId);
    };
    func = std::bind(calltem,this,plantIndex);
    CallFunc* callFunc = CallFunc::create(func);
    this->runAction(callFunc);
}
int   GameLayerLight::getLightCountByPlantIndex(int index)
{
    return _plantLights[index]._lightLists.size();
}
int GameLayerLight::randLightByPlant(int plantIndex)
{
    auto& lisths =  _plantLights[plantIndex]._lightLists;
    int size = lisths.size();
    if(size == 0 )return -1;
    int index = rand()%size;
    int i =0;
    for (auto& ip : lisths) {
        if (i == index) {
            return ip.second._id;
        }
        i++;
    }
    return -1;
}
int GameLayerLight::randLightByPlantNotWillRemove(int plantIndex)
{
    //auto& lisths =  _plantLights[plantIndex]._lightLists;
    std::vector<int> lightList;
    lightList.reserve(_plantLights[plantIndex]._lightLists.size());
    for (auto& i:_plantLights[plantIndex]._lightLists) {
        if (i.second._state==LightWillRemove)continue;
        lightList.push_back(i.second._id);
    }
    int id = -1;
    int size = lightList.size();
    if (size > 0) {
        id = lightList[RandomHelper::rand(0, size-1)];
    }
    return id;
}
void GameLayerLight::removeLightToPlantList(int plantIndex,int id)
{
    auto& lisths =  _plantLights[plantIndex]._lightLists;
    auto ip =lisths.find(id);
    if (ip != lisths.end()) {
        lisths.erase(ip);
    }
    return ;
}
void GameLayerLight::removeLightByPlantIndexAndLightIdNoUseAction(int plantIndex,int id)
{
    auto& lisths =  _plantLights[plantIndex]._lightLists;
    auto ip =lisths.find(id);
    if (ip != lisths.end()) {
        ip->second._target->removeFromParent();
        lisths.erase(ip);
    }
    return ;
}
int GameLayerLight::getLightTag(int plantIndex,int id)
{
    return plantIndex * MAX_LIGHT_COUNT + id;
}
LightPathContext&  GameLayerLight::getOneInsertLightConfig(int plantIndex)
{
    auto& lightlist = _plantLights[plantIndex]._lightLists;
    int size =lightlist.size();
    
    int index = size < 4 ? size : (4 -1);

    std::map<int,LightPathContext>  map = std::move(lightlist);
    lightlist.clear();
    for (int i = 0 ; i < index ; i++) {
        lightlist[i]= map[i];
    }
    
    auto config = randLightConfig();
    config._id = index;
    config._plantIndex = plantIndex;
    config._target->setTag(getLightTag(plantIndex, index));
    lightlist[index] = config;
    for (int i = index+1; i <= map.size() ; i++) {
        lightlist[i] = map[i-1];
       // lightlist[i]._id = i ;
        lightlist[i]._moveSpeed = 0;
     }
    updateLightToPosition(plantIndex);
    return lightlist[index];
}
void GameLayerLight::updateLightToPosition(int plantindex)
{
    auto& lightlist = _plantLights[plantindex]._lightLists;
    for (auto& ip : lightlist) {
        auto& i = ip.second;
        float to = i._nowLengthByTop ;
        if (fabs(to - i._id*50)>30) {
            i._nowLengthByTop = i._id*50 - 30 + rand()%55;
        }
    }
}

void GameLayerLight::checkUpdateToPosition(int plantIndex)
{
    std::vector<tempCheckToPositionContext> list ;

    auto& lightList = _plantLights[plantIndex]._lightLists;
    for (auto& i : lightList) {
        int state = i.second._state;
        if(state ==  LightNormal||
           state == LightMoveToPlantLine||
           state == LightOutPlantLine)
        {
            tempCheckToPositionContext temp ;
            temp.id = i.second._id;
            temp.toLengthByTop = i.second._nowLengthByTop;
            list.push_back(temp);
        }
    }
    if (list.size()==0) {
        return ;
    }
    auto call = []( const tempCheckToPositionContext& a,
                   const tempCheckToPositionContext& b)->bool
    {
        return a.toLengthByTop < b.toLengthByTop;
    };
    std::sort(list.begin(), list.end(),call);
    
   // float length = 70;
    int i = 1;
    auto ip = list.begin();
    //auto end = list.end();
    float len = getPlantInViewLength(plantIndex);
    if(len <= 3)return ;
    float stepLen = len > 300 ? len * 0.6 : len;
    float subStepLen = stepLen /list.size();
    if(subStepLen > 80)subStepLen=80;
    while (ip != list.end()) {
        float ilength = ip->toLengthByTop;
        float needLength = i*subStepLen ;
        if (fabs(ilength - needLength) > 20)
        {
            ip->toLengthByTop = rand()%(int)(subStepLen*0.3) + i*subStepLen -subStepLen*0.15;
            lightList[ip->id]._toLengthByTop = ip->toLengthByTop;
           // lightList[ip->id]._moveSpeed = 0;
        }
        i++;
        ip++;
    }
    
}
 void GameLayerLight::removeLightsRandIdUseSacel(int plantIndex,int count)
{
    auto callfunc = [](GameLayerLight* layerLight,int index)
    {
        layerLight->removeLightRandId(index);
    };
    CallFunc* call = CallFunc::create(std::bind(callfunc, this,plantIndex));
    float dtime = 1.0f/count;
    if (dtime > 0.1) dtime = 0.1;
    Repeat* repeat = Repeat::create(Sequence::create(call,DelayTime::create(dtime), NULL) , count);
    this->runAction(repeat);
}
#include "ItemModel.h"
void GameLayerLight::addLightsUseBezier(int plantIndex,Node* item,int count)
{
    if (count > 1) {
        auto callfunc = [](GameLayerLight* layerLight,Node* node,int index)
        {
            Node* parent = node->getParent();
            Vec2 pt = node->getPosition();
            if(parent)pt = parent->convertToWorldSpace(pt);
            layerLight->addOneLightByPlantIndexUseBezier(pt, index);
        };
        CallFunc* call = CallFunc::create(std::bind(callfunc, this,item,plantIndex));
        float dtime = 1.0f/count;
        if (dtime > 0.1) dtime = 0.1;
        Repeat* repeat = Repeat::create(Sequence::create(DelayTime::create(dtime),call, NULL) , count-1);
        this->runAction(repeat);
    }
    Node* parent = item->getParent();
    Vec2 startPoint = item->getPosition();
    if(parent)startPoint = parent->convertToWorldSpace(startPoint);
    
    auto config1 =  randCreateLightPahtContext(plantIndex,startPoint);

    int id = config1->_id;
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(plantIndex);
    
    PlantPathList  plantLineList = getOnePlantPathList(plantIndex);
    
    plantLineList.getTypePointByLength(config1->_toLengthByTop,false,nullptr);
    config1->_target->setContorlPosition(plant->convertToWorldSpace( plantLineList._findType._point));
    Vec2 lightPosition = plant->convertToWorldSpace( plantLineList._findType._point);
    lightPosition = UIHelper::getPositionToSameParentByWorld( config1->_target, lightPosition);
    config1->_target->setContorlPosition(lightPosition);
    
    startPoint = UIHelper::getNodePositionByWorld(config1->_target->getParent(), startPoint);
    config1->_target->setPosition(startPoint);
    Vec2 endPoint = config1->_target->getNowTimeTargetPosition();
    auto* bezier = createLightBezierAction(startPoint,endPoint,2);
   // auto* bezier = MoveBy::create(2, endPoint - startPoint);
    CallFunc* call = createLightRunningInitActionEndCall(config1,plantIndex,id);
    ;
   // config1->_target->runAction(Sequence::create(bezier,call,nullptr));
    

    auto lightNode =    ((LightNode*)config1->_target->getChildren().at(0));
    lightNode->setOpacity(0);
    FadeTo* fadeTo = FadeTo::create(2, 255);
    ScaleTo* scaleTo = ScaleTo::create(2, 0);

    Spawn* lightSpwan = Spawn::create(bezier->clone(),TargetedAction::create(lightNode,fadeTo), NULL);
    config1->_target->runAction(Sequence::create(lightSpwan,call,nullptr));
    item->runAction(Sequence::create(Spawn::create(scaleTo,bezier,nullptr), RemoveSelf::create(), NULL));
   checkUpdateToPosition(plantIndex);
}
void GameLayerLight::addLightsUseBezier(int plantIndex,Vec2 point,int count,float waitTime)
{
    auto callFunc = [=]()
    {
        GameLayerLight::getRunningLayer()->addOneLightByPlantIndexUseBezier(point, plantIndex);
    };
    auto repeat = Repeat::create(Sequence::create(CallFunc::create(callFunc),DelayTime::create(waitTime), NULL),count);
    this->runAction(repeat);
}
BezierBy* GameLayerLight::createLightBezierAction(Vec2 start,Vec2 end,float time)
{

    Vec2 v2 = start;
    Vec2 v1 = end;
    
    Vec2 cp1 = (v1 - v2)*0.3;
    Vec2 cp2 = (v1 - v2)*(1.0/RandomHelper::rand(3,6));
    float angle1 ,angle2;
  
    angle1 = RandomHelper::rand(25, 80);
    angle2 = -RandomHelper::rand(30, 65);
    if (RandomHelper::rand(0, 255)>125) {
        angle1 = 180 - angle1;
        angle2 = fabs(angle2);
    }
    Vec2 tcp1 = MathHelper::getRotatePosition(v2,  v2+Vec2(-250,0), angle1);
    Vec2 tcp2 = MathHelper::getRotatePosition(v1, v1 + cp2, angle2);

    ccBezierConfig v;
    v.controlPoint_1 = tcp1 - v2;
    v.controlPoint_2 = tcp2 - v2;
    v.endPosition = v1 -v2;
    return BezierBy::create(time, v);
}
void  GameLayerLight::startLightScaleAction(int plantIndex)
{

}
float GameLayerLight::getPlantInViewLength(int plantIndex)
{
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(plantIndex);
    auto ip = plant->_cpList.begin();
    auto end = plant->_cpList.end();
    
    while (ip!=end) {
        if (plant->convertToWorldSpace(ip->_point).y >= 0) {
            break;
        }
        ip++;
    }
    float len = 0;
    ContorlPointV2* precp = nullptr;
  
    if (ip == end) return  len;
    precp = &(*ip);
    ip++;
    while (ip != end) {
        len += fabs(PathGetLength(*precp, *ip));
        precp = &(*ip);
        ip++;
    }
    return len;
}
float GameLayerLight::getOneRandLightInitLength(int plantIndex)
{
    float len = getPlantInViewLength(plantIndex);
    if(len<100) len = 100;
    if (len > 300) {
        len = 300;
    }
    float ret = len * (RandomHelper::randFloat(2, 10)/10.0f);
    return ret;
}
void GameLayerLight::addLightsBySubMapInit(int plantIndex,int count)
{
    float step = 100 * 1.0f /count;
    for ( int i = 0; i < count; i++) {
        auto config = randLightConfig();
        config._toLengthByTop = (i+1)*step ;
        config._nowLengthByTop =  config._toLengthByTop;
        config._state = LightOutPlantLine;
        config._target->setPosition(Vec2(100,-300));
        config._target->setContorlPosition(Vec2(100,-300));
        ((LightNode*)(config._target->getChildren().at(0)))->changeLightState(LightNode::StateRandAction);
        config._target->startRotate();
        _addOneLightByPlantIndex(plantIndex,config);

    }
}
bool GameLayerLight::isAllPlantHasLights()
{
    bool ret = false;
    int count = GameLayerPlant::getRunningLayer()->getPlantCount();
    for (int i = 0; i<count; i++) {
        if (((count == 1) && (_plantLights[i]._lightLists.size() > 1)) ||
            ((count != 1) &&_plantLights[i]._lightLists.size() != 0) ) {
            return true;
        }
    }
    return ret;
}
void GameLayerLight::runChangeLightsAction(int loopCount,float waitTime)
{
    auto callFunc = []()
    {
        float count = GameLayerPlant::getRunningLayer()->getPlantCount();
        for (int i = 0; i<count; i++) {
            GameLayerLight::getRunningLayer()->checkUpdateToPosition(i);
        }
    };
    auto repeat = Repeat::create(Sequence::create(DelayTime::create(waitTime),CallFunc::create(callFunc), NULL),loopCount);
    this->runAction(repeat);
}


bool GameLayerLight::LightIsFadeOutAndIn(const LightPathContext& context )
{
    return context._target->getActionByTag(LightActionFadeInAndOut)!=nullptr;
}
void GameLayerLight::LightFadeOutAndIn(const LightPathContext& context )
{
}
void GameLayerLight::LightStartRotate(const LightPathContext& context )
{
    context._target->startRotate();
}
void GameLayerLight::LightStopRotate(const LightPathContext& context )
{
     context._target->stopRotate();
}


void  GameLayerLight::LightChildRunAction(const LightPathContext& context,Action* action)
{
    getLightChildNode(context)->runAction(action);
}

Node*  GameLayerLight::getLightChildNode(const LightPathContext& context)
{
    return context._target->getChildren().at(0);
}
Sprite* GameLayerLight::createOnLightChildNode()
{
    auto sp = Sprite::create(GamePaths::_sPathLight);
    //sp->setColor(Color3B(RandomHelper::rand(0, 255),RandomHelper::rand(0, 255),RandomHelper::rand(0, 255)));
    return sp;//Sprite::create(GamePaths::_sPathLight);
}
int GameLayerLight::getLightRunActionCount(int plantIndex,LightAction act)
{
    int count = 0;
    for (auto& ip : _plantLights[plantIndex]._lightLists ) {
        if(getLightChildNode(ip.second)->getActionByTag(act)!=nullptr)
        {
            count++;
        }
    }
    return count;
}
void GameLayerLight::checkLightUpdateAction(int plantIndex,int count,LightAction act)
{
    int count1 = 0;
    std::vector<int> ids;
    float lightsSize = _plantLights[plantIndex]._lightLists.size();
    ids.reserve(lightsSize);
    for (auto& ip : _plantLights[plantIndex]._lightLists ) {
        if(getLightChildNode(ip.second)->getActionByTag(act)!=nullptr)
        {
            count1++;
        }
        else  ids.push_back(ip.first);
    }
    int flag =  (float)lightsSize  / (float)count;
    if (flag == 0 ) {
        return ;
    }
    if (count1 < flag ) {
        auto action = createOneLightAction(act);
        Sequence* seq = Sequence::create(DelayTime::create(RandomHelper::randFloat(0,1.5)),action, NULL);
        seq->setTag(act);
        int randIndex = ids[RandomHelper::rand(0, ids.size()-1)];
        if (_plantLights[plantIndex]._lightLists[randIndex]._state != LightWillRemove) {
            LightChildRunAction(_plantLights[plantIndex]._lightLists[randIndex], seq);
        }
    }
}

Action* GameLayerLight::createOneLightAction(LightAction act)
{
    Action* action = nullptr;
    switch(act)
    {
        case LightActionFadeInAndOut:
        {
            FadeTo* fadeTo = FadeTo::create(RandomHelper::randFloat(3.0f, 6.0f), 99);
            FadeTo* fadeTo1 = FadeTo::create(RandomHelper::randFloat(3.0f, 6.0f), 255);
             auto delay = DelayTime::create(RandomHelper::randFloat(0.3,2.5));
            Sequence* seq =Sequence::create(fadeTo,delay,fadeTo1,nullptr);
            action = seq;
        }
            break;
        case LightActionScaleToBigAndSmall:
        {
            ScaleTo* scaleTo = ScaleTo::create(RandomHelper::randFloat(3.0, 6.0),1);
            ScaleTo* scaleTo1 = ScaleTo::create(RandomHelper::randFloat(3.0, 6.0),RandomHelper::randFloat(1.8, 3));
            auto delay = DelayTime::create(RandomHelper::randFloat(1.0,8));
            Sequence* seq =Sequence::create(scaleTo1,delay,scaleTo,nullptr);
    
            action = seq;
        }
            break;

        default:break;
    }
    return action;
}

#define OUTMAP(T,D) *((T*)(&buf[index])) = D;\
index += sizeof(D)
bool  GameLayerLight::saveToFile()
{
//    std::map<int,LightPathContext> _lightLists;
//    float                          _lightLength;
//
//    auto userInfo =  cocos2d::UserDefault::getInstance();
//
//    int count = sizeof(float) + sizeof(Vec2) ;
//    unsigned char * buf = (unsigned char*)malloc(sizeof(unsigned char) * count);
//    int index = 0;
//    //OUTMAP(float,_mapGrid._mapGridStarty);
//    //OUTMAP(Vec2,_maskAlphaSprite->getPosition());
//    //memcpy((unsigned char*)(&buf[index]),_mapGrid._gridData,mapInfoSize);
//    cocos2d::Data data;
//    data.fastSet(buf, count);
//    userInfo->setDataForKey(ConfigKeys::_sKeyMapData,data);
    return true;
}
bool  GameLayerLight::initBySaveConfig()
{
        return true;
}