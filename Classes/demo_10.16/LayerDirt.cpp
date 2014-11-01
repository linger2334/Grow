#include "LayerDirt.h"


bool  LayerRollImage::init()
{
    GameLayerRollY::init();
    _imageTexture = nullptr;
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
    _xPosition =0.5;
    _itemAnchorPoint = Vec2(0.5,0);
    _itemIsFlippedx = false;
    return true;
}
void  LayerRollImage::moveDown(float yLen)
{
    for (auto i : _imageList) {
        auto old = i->getPositionY();
        i->setPositionY(old-fabs(yLen));
    }
    checkReleaseOutViweSprite();
    autoAddImage();
}
void  LayerRollImage::setBlendFunc(BlendFunc blendFunc)
{
    _blendFunc = blendFunc;
    for (auto i : _imageList) {
        i->setBlendFunc(blendFunc);
    }
}
Sprite*  LayerRollImage::addImage( bool topDir)
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

void   LayerRollImage::setImage(const std::string& imageName)
{
    setImage(Director::getInstance()->getTextureCache()->addImage(imageName));
}
void   LayerRollImage::setImage(Texture2D* imageTexture)
{
    if (!imageTexture) return;
    CC_SAFE_RELEASE(_imageTexture);
    _imageTexture = imageTexture;
    _imageTexture->retain();
}

void   LayerRollImage::reSet()
{
    
}

void   LayerRollImage::setImageStartY(float y)
{
    if (!_imageTexture) return;
    _clearImages();
    auto sp = addImage(true);
    _LayoutImage(sp, -abs(y));
    autoAddImage();
}
void LayerRollImage::_LayoutImage(Sprite* sprite,float y)
{
    sprite->setPosition(Vec2(this->getContentSize().width*_xPosition,y-1));
}
void  LayerRollImage::_clearImages()
{
    for (auto i : _imageList) {
        _imageList.remove(i);
        i->removeFromParent();
    }
}

void   LayerRollImage::autoAddImage()
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

void  LayerRollImage::checkReleaseOutViweSprite()
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



bool LayerDirt::init()
{
    LayerRollImage::init();
    initGameInfo();
    return true;
}
bool LayerDirt::initGameInfo()
{
    _blendFunc =  {GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA };
    setImage("dirt512_3.png");
    setImageStartY(0);
    return true;
}


 bool LayerBackGround::init()
{
    LayerRollImage::init();
    initGameInfo();
    return true;
}
 bool LayerBackGround::initGameInfo()
{
    setImage("background_1_3.jpg");
    setImageStartY(0);
    return true;
}


