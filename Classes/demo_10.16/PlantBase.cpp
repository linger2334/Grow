#include "PlantBase.h"
#include "GameManager.h"
bool  GameLayerRollY::init()
{
    Director * director = Director::getInstance();
    setContentSize(director->getWinSize());
    this->setAnchorPoint(Vec2(0.5,0));

    return true;
}

void GameLayerRollY::_layoutGameLayer(Node* gameLayer)
{
    gameLayer->setPosition(Vec2(Director::getInstance()->getWinSize().width*0.5f,0));
}