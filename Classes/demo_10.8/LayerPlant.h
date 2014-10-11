#ifndef __Grow_Demo__LayerPlant__
#define __Grow_Demo__LayerPlant__
#include "PlantBase.h"
#include "common.h"
#include "CPLineNode.h"
#include "Plant1.h"
#include "GameManager.h"

class LayerPlant_1 : public GameLayerRollY
{
public:
    CREATE_FUNC(LayerPlant_1);
    virtual  bool init();
    virtual  void onEnter();
    virtual  void onExit();
    virtual void  moveDown(float y);
    virtual void update(float dt);
    void  updateHead(float dt);
    float grow(float len);
    
    float getPlantTopHeight();
    
    void  checkAddLeaf();
    void  setGrowSpeed(float  speed)
    {
        if(speed<0)return;
        _growSpeed =speed;
        //_plant->_turnSpeed = speed*2 ;
        _plant->_turnSpeed = 60 ;
    }
    float getGrowSpeed()
    {
        return _growSpeed;
    }
    
    void removeBorderLight();
    void removeLightBorderByCell(int tag);
    void  showCrashBorderLight();
    
    void renderPlant();
    void   checkLeafSubHeight(float _len);
    bool crashTest(Vec2);
    Plant_1*  _plant;
    bool    _isNeedGrow;
    
    FaceDirection      _preDir;
    float              _preGrowLen;
    
    std::list<Sprite*> _leafList;
    SpriteBatchNode*   _leafBatch;
    float              _leafTopHeight;
    bool               _left;
    RenderTexture*     _render;
    float              _growSpeed;
    float              _turnGrowLength;
    
    Sprite*            _lightSprite;
    bool                _islightRuningAction;
    SpriteBatchNode*    _crashBorderBatch;
    std::map<GridCell,Sprite*> _crashBorderLightMap;
};



#endif /* defined(__Grow_Demo__LayerPlant__) */
