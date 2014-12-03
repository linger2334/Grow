
#ifndef __Grow_beta_test01__PlantDrawingNode__
#define __Grow_beta_test01__PlantDrawingNode__

#include "common.h"
#include "ContorlPoint.h"
class PlantNode;
class GameLayerPlant;
class PlantDrawingNode : public Node
{
public:
    typedef std::vector<ContorlPointV2> CPListType ;
    virtual ~PlantDrawingNode()
    {
        if(_texture) _texture->release();
    }
    CREATE_FUNC(PlantDrawingNode);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
    void onDrawPlant(const Mat4 &transform, uint32_t flags);
    void setTexture(Texture2D* texture);

public:
    Texture2D*      _texture;
    GLProgram*      _glprogram;
    GLProgramState* _glprogramstate;
    CustomCommand   _customCommand;
    //opengl
    bool            _isDirt;
    V3F_T2F*        _vertices;
    unsigned int    _verticesCount;
    unsigned int    _capacity;
    unsigned int    _capacityAddSteplen;
    
    GameLayerPlant* _layerPlant;
};
#endif /* defined(__Grow_beta_test01__PlantDrawingNode__) */
