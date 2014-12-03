#ifndef __Grow_Demo_test__UIHelper__
#define __Grow_Demo_test__UIHelper__
#include "common.h"

namespace UIHelper
{
    enum ViwePoistionType
    {
        LeftBottom,
        LeftCenter,
        LeftTop,
        RightBottom,
        RightCenter,
        RightTop,
        BottomCenter,
        Center,
        TopCenter,
    };
    
    Vec2 getViewPosition(ViwePoistionType _type,bool isabsolute =true);
    void layout(Node* node,ViwePoistionType _type);
};

#endif /* defined(__Grow_Demo_test__UIHelper__) */
