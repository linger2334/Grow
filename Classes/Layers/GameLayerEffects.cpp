#include "GameLayerEffects.h"
#include "GameLayerPlant.h"
#include "GameLayerMap.h"
#include "GameLayerMapBorder.h"
#include "Plant.h"
bool GameLayerPlantCrashEffect::init()
{
    GameLayerBase::init();
    
    return true;
}
bool GameLayerPlantCrashEffect::initGameInfo()
{
    GameLayerBase::initGameInfo();
    _islightRuningAction = false;
    this->_crashBorderBatch = SpriteBatchNode::create("border_outlight.png");
    addChild(_crashBorderBatch);
    BlendFunc func = {GL_ONE,GL_ONE_MINUS_SRC_ALPHA};
    this->_crashBorderBatch->setBlendFunc(func);
    _gridWidth=GameRuntime::getInstance()->getMapGridSize().width;
    
    auto call = std::bind(&GameLayerPlantCrashEffect::onChangeCellCall,this,std::placeholders::_1);
    GameRunningConfig::getInstance()->_changeMapCellCalls.push_back(call);
    
    int count = GameLayerPlant::getRunningLayer()->getPlantCount();
    _crashBorderBatchs.resize(count);
    _islightRuningActions.resize(count);

    for(int i = 0; i < count; i++)
    {
        _crashBorderBatchs[i] =  SpriteBatchNode::create("border_outlight.png");
       // addChild(_crashBorderBatch);
        BlendFunc func = {GL_ONE,GL_ONE_MINUS_SRC_ALPHA};
        _crashBorderBatchs[i] ->setBlendFunc(func);
        addChild(_crashBorderBatchs[i]);

        _islightRuningActions[i] = true;
    }
    return true;
}
void GameLayerPlantCrashEffect::onChangeCellCall(const GridCell& cell)
{
    
    removeLightBorderByCell(cell._y*_gridWidth+cell._x);
}
void GameLayerPlantCrashEffect::moveDown(float yLen)
{
   
}
bool GameLayerPlantCrashEffect::releaseGameInfo()
{
    GameLayerBase::releaseGameInfo();
    return true;
}
void GameLayerPlantCrashEffect::showCrashBorderLight(int plantIndex)
{
    if (_islightRuningActions[plantIndex]) {
        return ;
    }
    
    auto pt = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(plantIndex)->getHeadPositionInWorld();
    auto border = GameLayerMapBorder::getRunningLayer();
    auto mapGrid = &GameLayerMap::getRunningLayer()->_mapGrid;
    auto cell = mapGrid->getMapGridCellByPosition(pt);
    int x, y,width,height;
    int rid = 23;
    x = cell._x-rid;
    y = cell._y-rid;
    width = x + 2*rid;
    height = y + 2*rid;
    if (x<0)x=0;
    if (y<0)y=0;
    if (width > mapGrid->_gridWidth)width = mapGrid->_gridWidth;
    if (height > mapGrid->_gridHeight)height = mapGrid->_gridHeight;
    auto end = border->_borderMap.end();
    std::list<GridCell> list;
    for (; y < height; y++) {
        for (int tx = x;tx < width ;tx++) {
            auto gridCell =GridCell(tx,y);
            // log("map %d,%d",gridCell._x,gridCell._y);
            auto ip = border->_borderMap.find(gridCell);
            if (ip!=end) {
                list.push_back(gridCell);
                
            }
        }
    }
    
    for (auto& i : list) {
        Sprite* sp = Sprite::createWithTexture(_crashBorderBatchs[plantIndex]->getTexture());
        
        auto pt = mapGrid->getPositionByMapGridCell(i);
        pt += Vec2(4,4);
        sp->setPosition(pt);
        sp->setScale(0.15);
        sp->setOpacity(0);
        sp->setUserData((void*)0);
        sp->setTag(i._y*mapGrid->_gridWidth+i._x);
        FadeTo* fadeto = FadeTo::create(2,60);
        sp->runAction(fadeto);
        _crashBorderBatchs[plantIndex]->addChild(sp);
    }
    _islightRuningActions[plantIndex] = true;
}
void GameLayerPlantCrashEffect::removeBorderLight(int plantIndex)
{
    auto& list = _crashBorderBatchs[plantIndex]->getChildren();
    for (auto& i: list) {
        if ((long)i->getUserData()==100) continue;
        FadeTo* fadeto = FadeTo::create(0.3,0);
        
        CallFuncN* call = CallFuncN::create([](Node* node)
                                            {
                                                node->removeFromParent();
                                            });
        i->setUserData((void*)100);
        Sequence* seq = Sequence::create(fadeto, call,nullptr);
        i->runAction(seq);
    }
     _islightRuningActions[plantIndex] = false;
}
//void  GameLayerPlantCrashEffect::showCrashBorderLight()
//{
//
//    if (_islightRuningAction) {
//        return ;
//    }
// 
//    auto pt = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(0)->getHeadPositionInWorld();
//    auto border = GameLayerMapBorder::getRunningLayer();
//    auto mapGrid = &GameLayerMap::getRunningLayer()->_mapGrid;
//    auto cell = mapGrid->getMapGridCellByPosition(pt);
//    int x, y,width,height;
//    int rid = 23;
//    x = cell._x-rid;
//    y = cell._y-rid;
//    width = x + 2*rid;
//    height = y + 2*rid;
//    if (x<0)x=0;
//    if (y<0)y=0;
//    if (width > mapGrid->_gridWidth)width = mapGrid->_gridWidth;
//    if (height > mapGrid->_gridHeight)height = mapGrid->_gridHeight;
//    auto end = border->_borderMap.end();
//    std::list<GridCell> list;
//    for (; y < height; y++) {
//        for (int tx = x;tx < width ;tx++) {
//            auto gridCell =GridCell(tx,y);
//            // log("map %d,%d",gridCell._x,gridCell._y);
//            auto ip = border->_borderMap.find(gridCell);
//            if (ip!=end) {
//                list.push_back(gridCell);
//                
//            }
//        }
//    }
//    
//    for (auto& i : list) {
//        Sprite* sp = Sprite::createWithTexture(_crashBorderBatch->getTexture());
//        
//        auto pt = mapGrid->getPositionByMapGridCell(i);
//        pt += Vec2(4,4);
//        sp->setPosition(pt);
//        sp->setScale(0.15);
//        sp->setOpacity(0);
//        sp->setUserData((void*)0);
//        sp->setTag(i._y*mapGrid->_gridWidth+i._x);
//        FadeTo* fadeto = FadeTo::create(2,60);
//        sp->runAction(fadeto);
//        _crashBorderBatch->addChild(sp);
//    }
//    _islightRuningAction = true;
//    
//}
//void GameLayerPlantCrashEffect::removeBorderLight()
//{
//    
//    auto& list = _crashBorderBatch->getChildren();
//    for (auto& i: list) {
//        if ((long)i->getUserData()==100) continue;
//        FadeTo* fadeto = FadeTo::create(0.3,0);
//        
//        CallFuncN* call = CallFuncN::create([](Node* node)
//                                            {
//                                                node->removeFromParent();
//                                            });
//        i->setUserData((void*)100);
//        Sequence* seq = Sequence::create(fadeto, call,nullptr);
//        i->runAction(seq);
//    }
//    _islightRuningAction = false;
//}
void GameLayerPlantCrashEffect::removeLightBorderByCell(int tag)
{
    if(_crashBorderBatch)return;
    auto node = _crashBorderBatch->getChildByTag(tag);
    if (node) {
        node->removeFromParent();
    }
   // else  _islightRuningAction = false;
}




bool GameLayerShowLandmark::initGameInfo()
{
    GameLayerBase::initGameInfo();
    autoAddLandmark();
    return true;
}

void GameLayerShowLandmark::moveDown(float yLen)
{
    auto ip = _landmarks.begin();
    while (ip != _landmarks.end()) {
        float y = ip->second->getPositionY()-yLen;
        if (y < -100) {
            ip->second->removeFromParent();
            ip = _landmarks.erase(ip);
            continue;
        }
        ip->second->setPositionY(y);
        ip++;
    }

    autoAddLandmark();
}
#include "GameRunningInfo.h"
#include "LevelManager.h"
#define HEIGHT_STEP 2000
void GameLayerShowLandmark::autoAddLandmark()
{
    auto runningInfo = GameRunningInfo::getInstance();
    float height = runningInfo->_gamePassHeight;
    height += LevelManager::getInstance()->_mapGroupSubId * 10240;
    int n = (int)height /HEIGHT_STEP;
    int n2 =((int)height) % HEIGHT_STEP;

    Size  wSize = GameRuntime::getInstance()->getVisibleSize();
    float maxHeight = wSize.height;
    int i=n;
    int t = 0;
    float addLen = 0;
    do{
        addLen = HEIGHT_STEP*t-n2;
        Vec2 pt(60,addLen);
        addLandmark(i*HEIGHT_STEP,pt);
        i++;
        t++;
    }while (addLen <= maxHeight);
}
void GameLayerShowLandmark::addLandmark(int n,Vec2 pt)
{
    auto ip = _landmarks.find(n);
    if (ip !=_landmarks.end())return;
    
    Label* label =  Label::createWithSystemFont(StringUtils::format("%d",n/10),  "Arial",28);
    addChild(label);
    label->setColor(Color3B(125,125,125));
    label->setPosition(pt);
    label->setOpacity(125);
    _landmarks[n] = label;
}
