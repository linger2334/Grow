#include "GameLayerLight.h"
#include "GameLayerPlant.h"
#include "ContorlPoint.h"
#include "LightNode.h"
#include <algorithm>
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

bool GameLayerLight::init()
{
    GameLayerBase::init();
    return true;
}
void GameLayerLight::update(float dt)
{
    for (int i = 0 ; i < _plantCount; i++) {
   // PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(i);
        
//        PathListHelper::PathList<ContorlPointV2, std::vector<ContorlPointV2> > plantLineList(&plant->_cpList);
//        plantLineList.PathGetLength = PathGetLength;
//        plantLineList.PathGetPoint = PathGetPoint;
//        float len = plantLineList.getTotalLength();
//        if (len!=_plantLights[i]._lightLength) {
//      
//            onChangePlantList(i);
//            _plantLights[i]._lightLength = len;
//        }
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

   // testAddOneLight();
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
    if (index !=0) {
        return;
    }
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(index);
    
    PathListHelper::PathList<ContorlPointV2, std::vector<ContorlPointV2> > plantLineList(&plant->_cpList);
    plantLineList.PathGetLength = PathGetLength;
    plantLineList.PathGetPoint = PathGetPoint;
    
//    float totalLenth = plantLineList.getTotalLength();
//    float step = totalLenth - _plantLights[index]._lightLength;
//    
//    if(fabs(step)==0)
//    {
//        return ;
//    }
//    for (auto& ip : _plantLights[index]._lightLists) {
//        auto& i = ip.second;
//        if(i._state == LightNormal||
//           i._state == LightMoveToPlantLine)
//        {
//            if(step > 0)  i._nowLengthByTop += step;
//            if (i._nowLengthByTop > totalLenth) {
//                i._state = LightOutPlantLine;
//            }
//        }
//    }
    _plantLights[index]._lightLength = plantLineList.getTotalLength();
    //onChangePlantList(index);
}
void  GameLayerLight::onPlantHeightChange(float height,int index)
{
    if(height < -400)return ;
    onChangePlantList(index);
  //  onChangePlantList(index);
}
#include "GameManager.h"
#include "GameRunningInfo.h"
void GameLayerLight::testAddOneLight()
{
    int plantCount = GameLayerPlant::getRunningLayer()->getPlantCount();
    auto config = GameRunningConfig::getInstance();
    for(int i = 0 ; i< plantCount;i++)
    {
        int count = config->getLightInitCount(i);
        for (int j  = 0; j <  count; j++) {
            addOneLightByPlantIndexRandConfig(i);
        }
    }
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
void  GameLayerLight::addOneLightByPlantIndex(Vec2 startPoint,int index)
{
    auto config = randLightConfig();
    config._toLengthByTop = getOneRandLightInitLength(index);
    config._nowLengthByTop =  config._toLengthByTop;
    config._state = LightMoveToPlantLine;
   // config._state = LightOutPlantLine;
    config._target->setPosition(startPoint);
    config._target->setContorlPosition(startPoint);
    int id = addOneLightByPlantIndex(index,config);
     checkUpdateToPosition(index);
    auto config1 =   getLightContextByPlantIndexAndId(index, id);

    auto callfunc = [](LightPathContext* conf,int plantIndex,int lightId)
    {
        conf->_state = LightNormal;
        ((LightNode*)conf->_target->getChildren().at(0))->changeLightState(LightNode::StateRandAction);
    };
    CallFunc* call = CallFunc::create(std::bind(callfunc,config1,index,id));
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(index);

    PathListHelper::PathList<ContorlPointV2, std::vector<ContorlPointV2> > plantLineList(&plant->_cpList);
    plantLineList.PathGetLength = PathGetLength;
    plantLineList.PathGetPoint = PathGetPoint;;
    int len = plantLineList.getTypePointByLength(config1->_toLengthByTop,false,nullptr);
    if (len < 0 ) {
        config._state = LightOutPlantLine;
        return;
    }
    config1->_target->setContorlPosition(plant->convertToWorldSpace( plantLineList._findType._point));
    Vec2 pt = plant->convertToWorldSpace( plantLineList._findType._point);
    

    Vec2 newPt1 = (pt - startPoint);
    MoveBy* moveby = MoveBy::create(1,newPt1);

    config1->_target->runAction(Sequence::create(moveby,call, NULL));


    config1->_nowLengthByTop = config._toLengthByTop ;
   
}
void  GameLayerLight::addOneLightByPlantIndexUseBezier(Vec2 startPoint,int index)
{
    auto config = randLightConfig();
    config._toLengthByTop = getOneRandLightInitLength(index);
    config._state = LightMoveToPlantLine;
    config._target->setPosition(startPoint);
    config._nowLengthByTop = config._toLengthByTop;
    int id = addOneLightByPlantIndex(index,config);
     checkUpdateToPosition(index);
    auto config1 =   getLightContextByPlantIndexAndId(index, id);
    
    auto callfunc = [](LightPathContext* conf,int plantIndex,int lightId)
    {
        conf->_state = LightNormal;
        ((LightNode*)conf->_target->getChildren().at(0))->changeLightState(LightNode::StateRandAction);
        conf->_target->startRotate();
    };

    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(index);
    
    PathListHelper::PathList<ContorlPointV2, std::vector<ContorlPointV2> > plantLineList(&plant->_cpList);
    plantLineList.PathGetLength = PathGetLength;
    plantLineList.PathGetPoint = PathGetPoint;;
    plantLineList.getTypePointByLength(config1->_toLengthByTop,false,nullptr);
    config1->_target->setContorlPosition(plant->convertToWorldSpace( plantLineList._findType._point));
    Vec2 pt = plant->convertToWorldSpace( plantLineList._findType._point);

    Vec2 v2 = startPoint;
    Vec2 v1 = pt;
    
    Vec2 cp1 = (v1 - v2)*0.3;
    Vec2 cp2 = (v1 - v2)*(1.0/RandomHelper::rand(3,7));
    float angle1 ,angle2;
    
    angle1 = RandomHelper::rand(25, 80);
    angle2 = -RandomHelper::rand(30, 65);
    if (RandomHelper::rand(0, 255)>125) {
        angle1 = 180 - angle1;
        angle2 = 180 - angle2;
    }
    Vec2 tcp1 = MathHelper::getRotatePosition(v2,  v2+Vec2(-250,0), angle1);
    Vec2 tcp2 = MathHelper::getRotatePosition(v1, v2 + cp2, angle2);
    
    ccBezierConfig v;
    v.controlPoint_1 = tcp1 - v2;
    v.controlPoint_2 = tcp2 - v2;
    v.endPosition = v1 -v2;
    BezierBy* bezier = BezierBy::create(2.5, v);
   
    CallFunc* call = CallFunc::create(std::bind(callfunc,config1,index,id));
    config1->_target->runAction(Sequence::create(bezier,call,nullptr));
   
}
void  GameLayerLight::addOneLightByPlantIndexRandConfig(int index)
{
    addOneLightByPlantIndex(Vec2(100,-100),index);
}

////////////////////////////////////////////////////////

void  GameLayerLight::updateLightsByPlantIndex(float dt,int index)
{
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(index);
    
    PathListHelper::PathList<ContorlPointV2, std::vector<ContorlPointV2> > plantLineList(&plant->_cpList);
    plantLineList.PathGetLength = PathGetLength;
    plantLineList.PathGetPoint = PathGetPoint;
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
            
           // i._changeRotateSpeed = plantGrowSpeed*0.5;
//            float maxSpeed1 = plantGrowSpeed;
//            if ( i._changeRotateSpeed > maxSpeed1) {
//                i._changeRotateSpeed  = maxSpeed1;
//            }
           float length = i._toLengthByTop - i._nowLengthByTop;
            {
                TuoYuanRotateNode* tuoyuan = static_cast<TuoYuanRotateNode*>(i._target);
                float stepRotate = i._changeRotateSpeed*dt;
                int toa = static_cast<long>(i._toRotateA);
                int nowa = static_cast<long>(tuoyuan->_config.a);
                if (toa != nowa) {
                    int f  = (toa - nowa) > 0 ? 1 : -1;
                    nowa += stepRotate *f;
                    if ((f>0 && nowa>toa)||
                        (f < 0 && nowa < toa)) {
                        nowa =toa;
                    }
                    tuoyuan->_config.a = nowa ;
                }
                
                int tob = static_cast<long>(i._toRotateB);
                int nowb = static_cast<long>(tuoyuan->_config.b);
                if (tob != nowb) {
                    int f  = (tob - nowb) > 0 ? 1 : -1;
                    nowb += stepRotate *f;
                    if ((f > 0 && nowb > tob)||
                        (f < 0 && nowb < tob)) {
                        nowb = tob;
                    }
                    tuoyuan->_config.b = nowb ;
                }
            }
//            continue;
            if(static_cast<long>( length ) == 0)
            {
                i._moveSpeed = 0;
              continue;
            }
            
            int sign = length > 0 ? 1 : -1;
            if (sign > 0) {
                i._moveSpeed = 0;
                // continue;
            }
            float step = dt * i._moveSpeed;
            if(step > fabs(length))step = length;
      
            float newPosition = i._nowLengthByTop + sign * step;

            int ret= plantLineList.getTypePointByLength(newPosition,false,nullptr);
            if( ret < 0 )
            {
                i._moveSpeed = 0;
                i._state = LightOutPlantLine;
                continue;
            }
           
            Vec2 pt = plant->convertToWorldSpace(plantLineList._findType._point);
           // Node* parent = i._target->getParent();
           // if (parent) pt = parent->convertToNodeSpace(pt);
           // i._target->setPosition(pt);
              i._target->setContorlPosition(pt);
   
            i._nowLengthByTop = newPosition;
            if(i._nowLengthByTop<0)
            {
                log("now");
            }
             i._moveSpeed += dt*plantGrowSpeed*0.3;
            float maxSpeed = plantGrowSpeed + fabs(length) * 0.3;
            if ( i._moveSpeed > maxSpeed) {
                i._moveSpeed  = maxSpeed;
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
            //i._target->setPosition(pt);
            i._target->setContorlPosition(pt);
            i._state = LightNormal;
            i._nowLengthByTop = maxLength;
            continue;
        }
    }
    
}
void   GameLayerLight::onChangePlantList(float index)
{
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(index);
    
    PathListHelper::PathList<ContorlPointV2, std::vector<ContorlPointV2> > plantLineList(&plant->_cpList);
    plantLineList.PathGetLength = PathGetLength;
    plantLineList.PathGetPoint = PathGetPoint;
    
    float totalLenth = plantLineList.getTotalLength();
    float step = totalLenth - _plantLights[index]._lightLength;
    
//    if(fabs(step)==0)
//    {
//        return ;
//    }
    for (auto& ip : _plantLights[index]._lightLists) {
        auto& i = ip.second;
//        if(i._state == LightNormal||
//           i._state == LightMoveToPlantLine)
//        {
            i._nowLengthByTop += step;
            if (i._nowLengthByTop > totalLenth) {
                i._state = LightOutPlantLine;
            }
       // }
    }
    _plantLights[index]._lightLength += step;

}
void GameLayerLight::moveDownLights(float yLen)
{
    int count = GameLayerPlant::getRunningLayer()->getPlantCount();
    for (int i = 0; i < count;i++) {
        for (auto& ip :_plantLights[i]._lightLists) {
             auto& i = ip.second;
          //  if(i._target&&(i._state != LightOutPlantLine))
            if(i._target)
            {
                
                i._target->setPositionY(i._target->getPositionY()-yLen);
            }
        }
    }
}
void GameLayerLight::testRandRotateLength()
{
     int count = GameLayerPlant::getRunningLayer()->getPlantCount();
    for (int i = 0; i < count;i++) {
        for (auto& ip :_plantLights[i]._lightLists) {
            auto& i = ip.second;
            i._toRotateA = rand()%30+30;
            i._toRotateB = rand()%30+20;
        }
    }
}
int  GameLayerLight::addOneLightByPlantIndex(int index,const LightPathContext& config)
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
    node->initOvalConfig(Vec2(100,-100),RandomHelper::rand(8, 19),  0,  0,RandomHelper::rand(1, 255)>125);
    node->setPosition(Vec2(100,-100));
    auto sp = LightNode::create();
    LightPathContext context;
    context._moveSpeed = 0;
    context._toLengthByTop = rand()%500+50;
    context._state = LightOutPlantLine;
    context._toRotateA = RandomHelper::rand(30, 60);
    context._toRotateB = RandomHelper::rand(25, 50);
    context._changeRotateSpeed = 60;
    node->addChild(sp);
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
        FadeTo* fadeOut = FadeTo::create(1.5,255*0.2);
        ScaleTo* scaleTo = ScaleTo::create(1.5, 10);
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
void GameLayerLight::removeLightUseUseBezier(int plantIndex,int id,Vec2 endPoint)
{
    auto light = getLightContextByPlantIndexAndId(plantIndex, id);
    auto startPoint =  light->_target->getNowTimeTargetPositionInWorld();
    light->_state = LightWillRemove;

    Vec2 v2 = startPoint;
    Vec2 v1 = endPoint;
    
    Vec2 cp1 = (v1 - v2)*0.3;
    Vec2 cp2 = (v1 - v2)*0.7;
    
    Vec2 tcp1 = MathHelper::getRotatePosition(v2, v2+cp1, 45);
    Vec2 tcp2 = MathHelper::getRotatePosition(v2, v2+cp2, 45);
    
    ccBezierConfig v;
    v.controlPoint_1 = tcp1 - v2;
    v.controlPoint_2 = tcp2 - v2;
    v.endPosition = v1 -v2;
    BezierBy* bezier = BezierBy::create(1, v);
    FadeTo* fadeout = FadeTo::create(0.5,0);
    auto callfunc = [](GameLayerLight* layerLight,int plantIndex,int id)
    {
        layerLight->removeLightByPlantIndexAndLightIdNoUseAction(plantIndex,id);
    };
    CallFunc* call = CallFunc::create(std::bind(callfunc,this,plantIndex,id));
    
    light->_target->runAction(Sequence::create(bezier,fadeout,call,nullptr));
}
void GameLayerLight::removeOneLightUseBezier(int plantIndex,Vec2 endPoint)
{
   int id =  randLightByPlant(plantIndex);
    removeLightUseUseBezier(plantIndex,id,endPoint);
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
    std::function<void(void)> func;
    auto calltem = [](GameLayerLight* layerLight,int plantId)
    {
        layerLight->removeLightTopId(plantId);
    };
    func = std::bind(calltem,this,plantIndex);
    CallFunc* callFunc = CallFunc::create(func);
  //  Repeat* repeat = Repeat::create(callFunc,count);
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
    //add by wlg
    if (size) {
        int index = rand()%size;
        int i =0;
        for (auto& ip : lisths) {
            if (i == index) {
                return ip.second._id;
            }
            i++;
        }
    }
    
    return -1;
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
        temp.toLengthByTop = i.second._toLengthByTop;
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
    auto end = list.end();
    float len = getPlantInViewLength(plantIndex);
    if(len <= 3)return ;
    float stepLen = len > 300 ? len * 0.6 : len;
    float subStepLen = stepLen /list.size();
    if(subStepLen > 80)subStepLen=80;
    while (ip != end) {
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
        Repeat* repeat = Repeat::create(Sequence::create(DelayTime::create(dtime),call, NULL) , count);
        this->runAction(repeat);
    }
    Node* parent = item->getParent();
    Vec2 startPoint = item->getPosition();
    if(parent)startPoint = parent->convertToWorldSpace(startPoint);
    auto config = randLightConfig();
    config._toLengthByTop = getOneRandLightInitLength(plantIndex);
    config._nowLengthByTop =  config._toLengthByTop;
    config._state = LightMoveToPlantLine;
    config._target->setPosition(startPoint);
    int id = addOneLightByPlantIndex(plantIndex,config);
      checkUpdateToPosition(plantIndex);
    auto config1 =   getLightContextByPlantIndexAndId(plantIndex, id);
    
    auto callfunc1 = [](LightPathContext* conf,int plantIndex,int lightId)
    {
        conf->_state = LightNormal;
        ((LightNode*)conf->_target->getChildren().at(0))->changeLightState(LightNode::StateRandAction);
         conf->_target->startRotate();
    };
    CallFunc* call1 = CallFunc::create(std::bind(callfunc1,config1,plantIndex,id));
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(plantIndex);
    
    PathListHelper::PathList<ContorlPointV2, std::vector<ContorlPointV2> > plantLineList(&plant->_cpList);
    plantLineList.PathGetLength = PathGetLength;
    plantLineList.PathGetPoint = PathGetPoint;;
    plantLineList.getTypePointByLength(config1->_toLengthByTop,false,nullptr);
    Vec2 lightPosition =plant->convertToWorldSpace( plantLineList._findType._point);

   // Vec2 pt = plant->convertToWorldSpace( plantLineList._findType._point);
    config1->_target->setContorlPosition(lightPosition);
    config1->_target->setPosition(startPoint);
    
    auto lightNode =    ((LightNode*)config1->_target->getChildren().at(0));
    lightNode->setOpacity(0);
    FadeTo* fadeTo = FadeTo::create(2, 255);
    ScaleTo* scaleTo = ScaleTo::create(2, 0);


    auto* bezier = createLightBezierAction(startPoint,lightPosition,2);
    Spawn* lightSpwan = Spawn::create(bezier->clone(),TargetedAction::create(lightNode,fadeTo), NULL);
    config1->_target->runAction(Sequence::create(lightSpwan,call1,nullptr));
    item->runAction(Sequence::create(Spawn::create(scaleTo,bezier,nullptr), RemoveSelf::create(), NULL));
  
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
    Vec2 cp2 = (v1 - v2)*(1.0/RandomHelper::rand(3,7));
    float angle1 ,angle2;
  
    angle1 = RandomHelper::rand(25, 80);
    angle2 = -RandomHelper::rand(30, 65);
    if (RandomHelper::rand(0, 255)>125) {
        angle1 = 180 - angle1;
        angle2 = 180 - angle2;
    }
    Vec2 tcp1 = MathHelper::getRotatePosition(v2,  v2+Vec2(-250,0), angle1);
    Vec2 tcp2 = MathHelper::getRotatePosition(v1, v2 + cp2, angle2);
//    Vec2 tcp1 = MathHelper::getRotatePosition(v2, v2+Vec2(-250,0), angle1);
//    Vec2 tcp2 = MathHelper::getRotatePosition(v2, v2+Vec2(-100,0), angle2);
    
    ccBezierConfig v;
    v.controlPoint_1 = tcp1 - v2;
    v.controlPoint_2 = tcp2 - v2;
    v.endPosition = v1 -v2;
    return BezierBy::create(time, v);
}
void  GameLayerLight::startLightScaleAction(int plantIndex)
{
    auto& list = _plantLights[plantIndex]._lightLists;
    std::list<LightPathContext*> ligts;
//    for(auto& i: list)
//    {
//        
//    }
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
    float ret = len * (RandomHelper::rand(1, 3)/10.0f);
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
        addOneLightByPlantIndex(plantIndex,config);

    }
}
bool GameLayerLight::isAllPlantHasLights()
{
    bool ret = true;
    int count = GameLayerPlant::getRunningLayer()->getPlantCount();
    for (int i = 0; i<count; i++) {
        if (_plantLights[i]._lightLists.size()==0) {
            return false;
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