#include "GameLayerExt.h"

bool RenderMaskColorNode::init()
{
    Node::init();
    return true;
}

RenderMaskColorNode* RenderMaskColorNode::create(unsigned char mask)
{
    RenderMaskColorNode *pRet = new RenderMaskColorNode();
    if (pRet && pRet->init())
    {
        pRet->_colorMask = mask;
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

RenderMaskColorNode* RenderMaskColorNode::create(bool red,bool green,bool blue,bool alpha)
{
    RenderMaskColorNode *pRet = new RenderMaskColorNode();
    if (pRet && pRet->init())
    {
        pRet->_colorMask = 0;
        if (red)pRet->_colorMask |= ColorMask::Red;
        if (green)pRet->_colorMask |= ColorMask::Green;
        if (blue)pRet->_colorMask |= ColorMask::Blue;
        if (alpha)pRet->_colorMask |= ColorMask::Alpha;
        
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

void RenderMaskColorNode::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(RenderMaskColorNode::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}
void RenderMaskColorNode::onDraw(const Mat4 &transform, uint32_t flags)
{
    glColorMask(_colorMask & ColorMask::Red, _colorMask & ColorMask::Green, _colorMask & ColorMask::Blue, _colorMask & ColorMask::Alpha);
}

RenderMaskColorNode* RenderMaskColorNode::createReverseMask()
{
    return RenderMaskColorNode::create(!(_colorMask&ColorMask::Red),!(_colorMask&ColorMask::Green),!(_colorMask&ColorMask::Blue),!(_colorMask&ColorMask::Alpha));
}