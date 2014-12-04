#include "GameRuntime.h"
#include "GameManager.h"
#include "LevelFileHandler.h"

void  GameRuntime::initGameRuntimeInfo()
{
    Size size = Director::getInstance()->getWinSize();
    this->setMapGridSize(Size(size.width/8,size.height/8));
    this->setMapGridUnitPixelSize(Size(8,8));
    this->setGameLayerSize(size);
    this->setVisibleSize(size);
    this->setMapDirtMaskGridSize(Size(size.width/8,(size.height/8)+1));
    this->setTouchClearGridRadius(6);
    this->setPlantCPListUnitHeight(4);
    this->setPlantVerticesUnitHeight(1);
    this->setPlantRayCrashMinLength(4);
    
    Vec2 left(0,0);
    Vec2 right(1,0);
    if (size.width<750) {
        left = Vec2(0.3,0);
        right = Vec2(0.7,0);
    }
    this->setGameLayerUIBorderAPLeft(left);
    this->setGameLayerUIBorderAPRight(right);

}