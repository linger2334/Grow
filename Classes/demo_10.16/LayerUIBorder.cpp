
#include "LayerUIBorder.h"
#include "LayerDirt.h"
#define PATH_LAYER_BORDER "layer_border_3.png"


void LayerUIBorder::moveDown(float yLen)
{
    _borderLeftLayer->moveDown(yLen*1.3);
    _borderRightLayer->moveDown(yLen*1.3);
}
bool LayerUIBorder::init()
{
    GameLayerRollY::init();
    _borderLeftLayer = LayerRollImage::create();
    _borderLeftLayer->_itemAnchorPoint =Vec2(0,0);
    _borderLeftLayer->_xPosition = 0;
    _borderLeftLayer->setImage(PATH_LAYER_BORDER);
    _layoutGameLayer(_borderLeftLayer);
    
    _borderRightLayer = LayerRollImage::create();
    _borderRightLayer->_itemAnchorPoint =Vec2(1,0);
    _borderRightLayer->_itemIsFlippedx = true;
    _borderRightLayer->_xPosition = 1;
    _borderRightLayer->setImage(PATH_LAYER_BORDER);
    _layoutGameLayer(_borderRightLayer);
    _borderLeftLayer->setImageStartY(0);
    _borderRightLayer->setImageStartY(0);
    addChild(_borderLeftLayer);
    addChild(_borderRightLayer);
    return true;
}
bool LayerUIBorder::InitGameInfo()
{
    
    return true;
}