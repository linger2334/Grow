#include "PlantBase.h"
#include "GameManager.h"
bool  GameLayerRollY::init()
{
    Director * director = Director::getInstance();
    setContentSize(director->getWinSize());
    this->setAnchorPoint(Vec2(0.5,0));

    return true;
}
