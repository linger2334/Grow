#ifndef __Grow_Goods_test__ActionHelper__
#define __Grow_Goods_test__ActionHelper__
#include "common.h"

namespace ActionHelper {
    struct randomBezierConfig
    {
        enum RandomDirType
        {
            RandAll,
            RandomLeft,
            RandomRight,
            RandomAlternation
        };
        Vec2 _startPosition;
        Vec2 _endPosition;
        Vec2* _contorlPoints;
        int   _contorlPointCount;
        int  _minRange;
        int  _maxRange;
    };
    Action* randomBezier(Vec2 start,Vec2 end);
}

#endif /* defined(__Grow_Goods_test__ActionHelper__) */
