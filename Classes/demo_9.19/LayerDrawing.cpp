#include "LayerDrawing.h"

bool LayerDrawing::init()
{
    Layer::init();
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
    
    _quads = TextureAtlas::create("border_2.png", 10); 
    float step = 8/_quads->getTexture()->getContentSize().height;
    for (int i=0; i<6; i++) {
        V3F_C4B_T2F_Quad t;
        t.bl.vertices = Vec3(100,i*8,0);
        t.bl.texCoords = Tex2F(0,i*step);
        t.bl.colors = Color4B::WHITE;
        t.br.vertices = Vec3(160,i*8,0);
        t.br.texCoords = Tex2F(1,i*step);
           t.br.colors = Color4B::WHITE;
        
        t.tl.vertices = Vec3(100,(i+1)*8,0);
        t.tl.texCoords = Tex2F(0,(i+1)*step);
           t.tl.colors = Color4B::WHITE;
        t.tr.vertices = Vec3(160,(i+1)*8,0);
        t.tr.texCoords = Tex2F(1,(i+1)*step);
           t.tr.colors = Color4B::WHITE;
        _quads->insertQuad(&t,i);
    }
    

    return true;
}

void LayerDrawing::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(LayerDrawing::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
    
}
void LayerDrawing::onDraw(const Mat4 &transform, uint32_t flags)
{
    getGLProgram()->use();
    getGLProgram()->setUniformsForBuiltins(transform);
    
    _quads->drawNumberOfQuads(5);

}
