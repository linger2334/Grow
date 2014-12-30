#include "UIHelper.h"

namespace UIHelper
{
    
    Vec2 getViewPosition(ViwePoistionType _type,bool isabsolute )
    {
        Vec2 point;
        switch (_type)
        {
            case  LeftBottom:
                point = Vec2(0.0f,0.0f);
                break;
            case  LeftCenter:
                 point = Vec2(0.0f,0.5f);
                break;
            case  LeftTop:
                 point = Vec2(0.0f,1.0f);
                break;
            case  RightBottom:
                point = Vec2(1.0f,0.0f);
                break;
            case  RightCenter:
                point = Vec2(1.0f,0.5f);
                break;
            case  RightTop:
                point = Vec2(1.0f,1.0f);
                break;
            case  BottomCenter:
                point = Vec2(0.5f,0.0f);
                break;
            case  Center:
                point = Vec2(0.5f,0.5f);
                break;
            case  TopCenter:
                point = Vec2(1.0f,0.5f);
                break;
            default:
                log("UI Layout Error Position Type");
                assert(0);
                break;
        }
        if(isabsolute)
        {
            cocos2d::Size wsize = Director::getInstance()->getWinSize();
            point = Vec2(wsize.width*point.x,wsize.height*point.y);
        }
        return point;
    }
    void layout(Node* node, ViwePoistionType _type)
    {
        if(!node)return;
        node->setPosition(getViewPosition(_type));
    }
    
    Vec2 getWorldPosition(Node* node)
    {
        Vec2 pt = node->getPosition();
        auto parent = node->getParent();
        if (parent) {
            pt = parent->convertToWorldSpace(pt);
        }
        return pt;
    }
    
    Vec2 getNodePositionByWorld(Node* parent ,Vec2 point)
    {
       return parent->convertToNodeSpace(point);
    }
    Vec2 getPositionToSameParentByWorld(Node* node,Vec2 point)
    {
        Vec2 pt = point;
        auto parent = node->getParent();
        if (parent) {
            pt = parent->convertToNodeSpace(pt);
        }
        return pt;
    }
};