
#ifndef __PlantOri__LayerBackMask__
#define __PlantOri__LayerBackMask__
#include "common.h"
#include "TypeDef.h"

class DrawLayerCallFunc: public Layer{
public:
    
    CREATE_FUNC(DrawLayerCallFunc);
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override
    {
        _customCommand.init(_globalZOrder);
        _customCommand.func = CC_CALLBACK_0(DrawLayerCallFunc::onDraw, this, transform, flags);
        renderer->addCommand(&_customCommand);
    }
    void onDraw(const Mat4 &transform, uint32_t flags)
    {
        assert(_callBack);
        _callBack(transform,flags);
    }
    std::function< void(const Mat4&, uint32_t)> _callBack;
    CustomCommand _customCommand;
};

class RenderMaskColorNode : public Node
{
    
public:
    static RenderMaskColorNode* create(unsigned char mask);
    
    static RenderMaskColorNode* create(bool red,bool green,bool blue,bool alpha);
    virtual bool init() override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
    
    RenderMaskColorNode* createReverseMask();
    CustomCommand _customCommand;
     unsigned char _colorMask;
};
#endif /* defined(__PlantOri__LayerBackMask__) */
