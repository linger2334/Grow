#ifndef __Grow_Demo__LightNode2__
#define __Grow_Demo__LightNode2__
#include "Macro.h"
#include "GameManager.h"
#include "ContorlPoint.h"
class LightNode2 : public Node
{
public:
    enum PositionType
    {
        CPCenter,
        CPLeft,
        CPRight
    };
    
    CREATE_FUNC(LightNode2);
    virtual  bool init();
    virtual  void update(float dt);
    void   updatePosition();
    bool   _isAutoMoving;
    float  _plantHeight;
    float  _speed;

    Node*       _plantNode;
    std::vector<ContorlPointV2>* _cpList;
};

#endif /* defined(__Grow_Demo__LightNode2__) */
