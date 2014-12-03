#ifndef __Grow_beta_test02__GameShasers__
#define __Grow_beta_test02__GameShasers__
#include "common.h"

class GameShaders : public Singleton<GameShaders>
{
public:
    GameShaders()
    {
        initShaders();
    }
    void  initShaders()
    {
       _grayGLProgram = GLProgram::createWithFilenames("ccShader_PositionTextureColor_noMVP.vert", "ccShader_PositionTextureColor_noMVP.frag");
        _grayGLProgram->retain();
      
    }

    GLProgramState* getOneGrayShaderState()
    {
        return   GLProgramState::create(_grayGLProgram);
    }
    
    GLProgramState* getNormalSpriteShaderState()
    {
        return GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
    }
    
    GLProgram* _grayGLProgram;
};

#endif /* defined(__Grow_beta_test02__GameShasers__) */
