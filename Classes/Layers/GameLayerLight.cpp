#include "GameLayerLight.h"
#include "GameLayerPlant.h"
#include "ContorlPoint.h"
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
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(0);
    
    PathListHelper::PathList<ContorlPointV2, std::vector<ContorlPointV2> > plantLineList(&plant->_cpList);
    plantLineList.PathGetLength = PathGetLength;
    plantLineList.PathGetPoint = PathGetPoint;
    float len = plantLineList.getTotalLength();
    if (len!=_lineLenght[0]) {
        onChangePlantList(0);
        _lineLenght[0] = len;
    }
    
    float totalLenth = plantLineList.getTotalLength();
    updateLightsByPlantIndex(dt,0);
}
bool GameLayerLight::initGameInfo()
{
    GameLayerBase::initGameInfo();
    //GameRunningConfig::_plantHeightChangeCallbacks
    PlantNode* node = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(0);
   // _pathList._list = &node->_cpList;
    _lineLenght.resize(2);
    _lightLists.resize(2);
    _lineLenght[0] = 0;
    testAddOneLight();
    auto call = std::bind(&GameLayerLight::onPlantHeightChange,this,std::placeholders::_1,std::placeholders::_2);
    GameRunningConfig::getInstance()->_plantHeightChangeCallbacks.push_back(call);
    
    auto call1 = std::bind(&GameLayerLight::onPlantCpListChange,this,std::placeholders::_1,std::placeholders::_2);
    GameRunningConfig::getInstance()->_plantCPChangeCallbacks.push_back(call1);
    return  true;
}
bool GameLayerLight::releaseGameInfo()
{
    return true;
}
void GameLayerLight::moveDown(float yLen)
{
    moveDownLights(yLen);
}
float GameLayerLight::getLengthByPlantHeight(float plantHeight)
{
    return 0;
}
void  GameLayerLight::onPlantCpListChange(float height,float index)
{
    //onChangePlantList(index);
}
void  GameLayerLight::onPlantHeightChange(float height,float index)
{
  //  onChangePlantList(index);
}

void GameLayerLight::testAddOneLight()
{
    Sprite* sp = Sprite::create( "light_2.png");
    sp->setColor(Color3B::RED);
    LightPathContext context;
    context._moveSpeed = 30;
    context._toLengthByTop = 200;
    context._state = LightOutPlantLine;
    context._target = sp;
    addChild(sp);
    _lightLists[0].push_back(context);
}
void  GameLayerLight::updateLightsByPlantIndex(float dt,int index)
{
    PlantNode* plant = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(index);
    
    PathListHelper::PathList<ContorlPointV2, std::vector<ContorlPointV2> > plantLineList(&plant->_cpList);
    plantLineList.PathGetLength = PathGetLength;
    plantLineList.PathGetPoint = PathGetPoint;
    float maxLength = plantLineList.getTotalLength();
    for (auto& i : _lightLists[index]) {
        if(!i._target)
        {
            i._state = LightNoHasTarget;
            continue;
        }
        
        if (i._state == LightNormal) {
            
            float length = i._toLengthByTop - i._nowLengthByTop;
            if(static_cast<long>( fabs(length) ) == 0)continue;
            
            int sign = length > 0 ? 1 : -1;
        
            float step = dt * i._moveSpeed;
            if(step > fabs(length))step = length;
          
            
            float newPosition = i._nowLengthByTop + sign * step;
            
            int ret= plantLineList.getTypePointByLength(newPosition,false,nullptr);
            if( ret < 0 )
            {
                i._state = LightOutPlantLine;
                continue;
            }
           
            Vec2 pt = plant->convertToWorldSpace(plantLineList._findType._point);
            Node* parent = i._target->getParent();
            if (parent) pt = parent->convertToNodeSpace(pt);
            i._target->setPosition(pt);
            i._nowLengthByTop = newPosition;
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
            i._target->setPosition(pt);
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
    float step = totalLenth - _lineLenght[index];
    for (auto& i : _lightLists[index]) {
        if(i._state == LightNormal)
        {
            if(step > 0)  i._nowLengthByTop += step;
            if (i._nowLengthByTop > totalLenth) {
                i._state = LightOutPlantLine;
            }
        }
    }
}
void GameLayerLight::moveDownLights(float yLen)
{
    for (auto& ilist : _lightLists) {
        for (auto& i :ilist) {
            if(i._state == LightNormal&&i._target)
            {
                i._target->setPositionY(i._target->getPositionY()-yLen);
            }
        }
    }
}
