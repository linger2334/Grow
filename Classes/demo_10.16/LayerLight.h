#ifndef __Grow_Demo__LayerLight__
#define __Grow_Demo__LayerLight__
#include "common.h"
#include "PlantBase.h"
#include "LightNode2.h"
class LayerLight : public GameLayerRollY
{
public:
    CREATE_FUNC(LayerLight);
    virtual bool  init();
    LightNode2*  createOneLightNode(float height);
    Node*      createOneSpriteLight();
    void lightCallback(Node* node1);
    virtual  void moveDown(float yLen);
    virtual  void update(float dt);
    void subLightHeight(float height);
    
    void addOneLight();
    void addOneLight(Vec2 ptlight);
    void removeOneLight(Vec2 ptlight);
    std::list<LightNode2*> _lightListFirst;
    std::list<LightNode2*> _lightListExt;
    
    SpriteBatchNode*    _lightBatch;
};

#endif /* defined(__Grow_Demo__LayerLight__) */
