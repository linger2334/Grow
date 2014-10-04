#include "LayerDirt.h"

bool LayerDirt1::init()
{
    Node::init();
    this->setContentSize(Director::getInstance()->getWinSize());
    this->setAnchorPoint(Vec2::ZERO);
    _blendFunc = {GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA};
    return true;
}

void  LayerDirt1::moveDown(float yLen)
{
    for (auto i : _dirtList) {
        auto old = i->getPositionY();
        i->setPositionY(old-fabs(yLen));
    }
     auto ipBegin = _dirtList.begin();
    if (ipBegin!=_dirtList.end()) {
        auto  head = *ipBegin;
        if (head->getPositionY()+head->getContentSize().height<0) {
            _dirtList.erase(ipBegin);
            head->removeFromParent();
        }
    }
   
    updateSprites();
}

void LayerDirt1::updateSprites()
{
    for (int i = 0 ; i<4; i++) {
        float yTop = 0.0f;
        auto ipRBegin = _dirtList.rbegin();
        if (ipRBegin!=_dirtList.rend()) {
            yTop = (*ipRBegin)->getPositionY()+(*ipRBegin)->getContentSize().height;
        }
        Size visibleSize =Director::getInstance()->getWinSize();
        if (yTop < visibleSize.height) {
            Sprite* tempDirt = Sprite::create("dirt512_2.png");
            tempDirt->setAnchorPoint(Vec2(0.5,0));
            tempDirt->setPosition(Vec2(visibleSize.width*0.5,yTop));
            tempDirt->setBlendFunc(_blendFunc);
            _dirtList.push_back(tempDirt);
            this->addChild(tempDirt);
        }
        else break;
    }
}

bool  LayerRollImage::init()
{
    GameLayerRollY::init();
    _imageTexture = nullptr;
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
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
    tempDirt->setAnchorPoint(Vec2(0.5,0));
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
    sprite->setPosition(Vec2(this->getContentSize().width*0.5,y-1));
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


