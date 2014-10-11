#ifndef __Grow_Demo__LightNode__
#define __Grow_Demo__LightNode__
#include "Macro.h"
#include "GameManager.h"
#include "ContorlPoint.h"

class SceneGame;

class LightNode : public Node
{
public:
    enum PositionType
    {
        CPCenter,
        CPLeft,
        CPRight
    };
    
    CREATE_FUNC(LightNode);
    virtual  bool init();
    virtual  void update(float dt);
    void   updatePosition();
    
    float _speedHeight;
    bool  _isUpdateByHeight;
    float _lightHeight;
    float _plantHeight;
    
    float  _rotateLen;
    PositionType   _positionType;
    
    Node*       _plantNode;
    std::vector<ContorlPointV2>* _cpList;
};

struct LigthListNode
{
public:
    LigthListNode():_node(nullptr),_height(0.0f),_isRuningAction(false){}
    LigthListNode(Node* node,float height):_node(node),_height(height),_isRuningAction(false){}
    
    bool operator == (const Node* node)
    {
        return _node == node;
    }
    Node*   _node;
    float   _height;
    bool    _isRuningAction;
};
class LightNodeListHelper
{
public:
    void addLight(Node* node);
    void removeLight(Node* node);
    
    void subItemHeight(float len);
   
    void updateLight();
    void update(float dt);
    
    SceneGame* _sceneGame;
    std::list<LigthListNode> _lightList;
    float  _lightAddSpeedStep;
};

#endif /* defined(__Grow_Demo__LightNode__) */
