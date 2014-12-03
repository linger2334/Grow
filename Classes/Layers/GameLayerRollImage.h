#ifndef __Grow_Demo_test__GameLayerRollImage__
#define __Grow_Demo_test__GameLayerRollImage__

#include "GameLayerBase.h"
class GameLayerRollImage : public GameLayerBase
{
public:
    CREATE_FUNC(GameLayerRollImage);
    virtual ~GameLayerRollImage()
    {
        CC_SAFE_RELEASE_NULL(_imageTexture);
    }
    virtual  bool  init();
    virtual  void  moveDown(float yLen) override;
    virtual  bool  releaseGameInfo()
    {
        _clearImages();
        return true;
    }
    void  setImage(const std::string& imageName);
    void  setImage(Texture2D* imageTexture);
    
    void  setBlendFunc(BlendFunc);
    
    Sprite*  addImage(bool topDir);
    void  reSet();
    
    void  setImageStartY(float y);
    
    void  autoAddImage();
    
    void  checkReleaseOutViweSprite();
    
    void  _clearImages();
    
    void _LayoutImage(Sprite* sprite,float y);
    
    std::list<Sprite*> _imageList;
    BlendFunc   _blendFunc;
    Texture2D*  _imageTexture;
    float       _xPosition;
    Vec2        _itemAnchorPoint;
    bool        _itemIsFlippedx;
};
#endif /* defined(__Grow_Demo_test__GameLayerRollImage__) */
