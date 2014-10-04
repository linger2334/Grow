#ifndef _LAYER_DRAWING_H_
#define  _LAYER_DRAWING_H_

#include "Macro.h"

class LayerDrawing : public Layer{
public:
    static Scene* createScene()
    {
        Scene* scene = Scene::create();
        LayerDrawing* layer = LayerDrawing::create();
        scene->addChild(layer);
        return scene;
    }
    CREATE_FUNC(LayerDrawing);
    
    virtual bool init();
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
       CustomCommand   _customCommand;
    TextureAtlas*  _quads;
};
#endif