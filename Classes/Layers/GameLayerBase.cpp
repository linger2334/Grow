#include "GameLayerBase.h"
#include "GameRuntime.h"

bool  GameLayerBase::init()
{
    setContentSize(GameRuntime::getInstance()->getGameLayerSize());
    this->setAnchorPoint(Vec2(0.5,0));
   
    _isHandle = false;
    GameRunningInfo::getInstance()->addGameLayer(getTypeName(), this);
      UIHelper::layout(this, UIHelper::ViwePoistionType::BottomCenter);

    return true;
}
const std::string GameLayerBase::getTypeName()
{
    return typeid(*this).name();
}

const std::type_info& GameLayerBase::getType()
{
    return typeid(*this);
}

