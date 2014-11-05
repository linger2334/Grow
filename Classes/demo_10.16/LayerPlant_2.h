#ifndef __Grow_Demo__LayerPlant_2__
#define __Grow_Demo__LayerPlant_2__
#include "PlantBase.h"
#include "common.h"
#include "CPLineNode.h"
#include "Plant1.h"
#include "GameManager.h"
#include "Box2D/Box2D.h"
#include "LayerPlantBase.h"

class LayerPlant_2 : public LayerPlantBase
{
public:
    CREATE_FUNC(LayerPlant_2);
    virtual  bool init();
    virtual  void onEnter();
    virtual  void onExit();
    virtual void  moveDown(float y);
    virtual void update(float dt);
    void  updateHead(float dt);
    float grow2(float len);
    virtual float getPlantTopHeightInView();
    float getPlantTopHeight();
    void  checkAddLeaf();

    void removeBorderLight();
    void removeLightBorderByCell(int tag);
    void showCrashBorderLight();
    
    void getPlantRayCrashLength(float outLens[3]);
    
    void renderPlant();
    void checkLeafSubHeight(float _len);
    bool crashTest(Vec2);
    
    void createHeadB2Body();
    void updateHeadB2Body();
    
    void getGroeDirListTest( std::vector<FaceDirection>& dirList);
    GrowContext getMinRangeCanGrowContext();
    GrowContext getFirstCanGrowContext();
    GrowContext getRayGrowLength(float vec[3],float testLength);
    
    bool   isRayCastCrash(Vec2 pt);
    bool   isRayCastCrashMapGrid(Vec2 pt);
    bool   isRayCastCrashStone(Vec2 pt);
    
    bool   isCashPoint(Vec2 pt, int type =0);
    bool   isCrashPointGridAndStone(Vec2 pt);
    
    int    getPlantHeadViewWidthType();
    
    void    doReGrow(float speed, float len);
    
    int   reGrow(float dt);
    float       _reGrowLength;
    Plant_1*    _plant;
    bool        _isNeedGrow;
    bool        _isReGrow;
    float       _reGrowSpeed;
    
    std::list<Sprite*> _leafList;
    SpriteBatchNode*   _leafBatch;
    float              _leafTopHeight;
    bool               _left;
    RenderTexture*     _render;
    float              _leafCheckAddHeight;
    Vec2               _leafPoint;
    bool               _isCheckAddleaf;
    
    
    float              _turnGrowLength;
    
    Sprite*             _lightSprite;
    bool                _islightRuningAction;
    SpriteBatchNode*    _crashBorderBatch;
    std::map<GridCell,Sprite*> _crashBorderLightMap;
    
    b2Body*             _headBody;
    b2World*            _world;
    
    int                 _musicId;
};



#endif /* defined(__Grow_Demo__LayerPlant__) */
