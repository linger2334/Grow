#include "GameLayerRollImage.h"
bool  GameLayerRollImage::init()
{
    GameLayerBase::init();
    GameRunningInfo::getInstance()->removeGameLayer(typeid(GameLayerRollImage).name());
    _imageTexture = nullptr;
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
    _xPosition =0.5;
    _itemAnchorPoint = Vec2(0.5,0);
    _itemIsFlippedx = false;
    return true;
}
bool  GameLayerRollImage::releaseGameInfo()
{
    _clearImages();
    CC_SAFE_RELEASE_NULL(_imageTexture);
    return true;
}
void  GameLayerRollImage::moveDown(float yLen)
{
    for (auto i : _imageList) {
        auto old = i->getPositionY();
        i->setPositionY(old-fabs(yLen));
    }
    checkReleaseOutViweSprite();
    autoAddImage();
}
void  GameLayerRollImage::setBlendFunc(BlendFunc blendFunc)
{
    _blendFunc = blendFunc;
    for (auto i : _imageList) {
        i->setBlendFunc(blendFunc);
    }
}
Sprite*  GameLayerRollImage::addImage( bool topDir)
{
    Sprite* tempDirt = Sprite::createWithTexture(_imageTexture);
    tempDirt->setAnchorPoint(_itemAnchorPoint);
    tempDirt->setFlippedX(_itemIsFlippedx);
    tempDirt->setBlendFunc(_blendFunc);
    if(topDir)_imageList.push_back(tempDirt);
    else _imageList.push_front(tempDirt);
    this->addChild(tempDirt);
    return tempDirt;
}

void   GameLayerRollImage::setImage(const std::string& imageName)
{
    setImage(Director::getInstance()->getTextureCache()->addImage(imageName));
}
void   GameLayerRollImage::setImage(Texture2D* imageTexture)
{
    if (!imageTexture) return;
    CC_SAFE_RELEASE_NULL(_imageTexture);
    _imageTexture = imageTexture;
    _imageTexture->retain();
}

void   GameLayerRollImage::reSet()
{
    
}

void   GameLayerRollImage::setImageStartY(float y)
{
    if (!_imageTexture) return;
    _clearImages();
    auto sp = addImage(true);
    _LayoutImage(sp, -abs(y));
    autoAddImage();
}
void GameLayerRollImage::_LayoutImage(Sprite* sprite,float y)
{
    sprite->setPosition(Vec2(this->getContentSize().width*_xPosition,y-1));
}
void  GameLayerRollImage::_clearImages()
{
    for (auto i : _imageList) {
        i->removeFromParent();
       // _imageList.remove(i);
    }
    _imageList.clear();
}

void   GameLayerRollImage::autoAddImage()
{
    if (!_imageTexture||_imageTexture->getContentSize().height<=0) return;
    for (;;) {
        float yTop = 0.0f;
        auto ipRBegin = _imageList.rbegin();
        if (ipRBegin!=_imageList.rend()) {
            yTop = (*ipRBegin)->getPositionY()+(*ipRBegin)->getContentSize().height;
        }
        
        if (yTop < this->getContentSize().height) {
            Sprite* tempDirt = addImage(true);
            _LayoutImage(tempDirt,yTop);
            
        }
        else break;
    }
}

void  GameLayerRollImage::checkReleaseOutViweSprite()
{
    auto ipBegin = _imageList.begin();
    auto ipEnd   = _imageList.end();
    while (ipBegin!=ipEnd) {
        auto  sp = *ipBegin;
        ipBegin++;
        if (sp->getPositionY()+sp->getContentSize().height < 0) {
            _imageList.remove(sp);
            sp->removeFromParent();
        }
        else break;
    }
}