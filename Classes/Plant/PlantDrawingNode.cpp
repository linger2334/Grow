#include "PlantDrawingNode.h"
#include "Plant.h"
#include "TypeBase.h"
#include "GameHeaders.h"
#include "GameLayerPlant.h"

bool PlantDrawingNode::init()
{
    Node::init();
    auto glprogram  = GLProgram::createWithByteArrays(ccPositionTexture_vert, ccPositionTexture_frag);
    //    auto glprogram  = GLProgram::createWithFilenames("ccShader_PositionTexture.vert", "ccShader_PositionTexture.frag");
    _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    setGLProgramState(_glprogramstate);
    _ignoreAnchorPointForPosition = false;
    _texture = nullptr;
    setAnchorPoint(Vec2::ZERO);

    //opengl
    _isDirt = false;
    _vertices = nullptr;
    _verticesCount = 0;
    _capacity = 0;
    _capacityAddSteplen = 100;
    return true;
}

void PlantDrawingNode::onEnter()
{
    Node::onEnter();
}

void PlantDrawingNode::setTexture(Texture2D* texture)
{
    if (_texture) {
        _texture->release();
    }
    _texture=texture;
    _texture->retain();
}
void PlantDrawingNode::onExit()
{
    Node::onExit();
}

void PlantDrawingNode::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(PlantDrawingNode::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}
void PlantDrawingNode::onDraw(const Mat4 &transform, uint32_t flags)
{
    getGLProgramState()->getGLProgram()->use();
    getGLProgramState()->getGLProgram()->setUniformsForBuiltins(transform);
  //  Director::getInstance()->setDepthTest(true);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    onDrawPlant(transform,flags);
   // Director::getInstance()->setDepthTest(false);
}

void PlantDrawingNode::onDrawPlant(const Mat4 &transform, uint32_t flags)
{
    auto plantLayer = GameLayerPlant::getRunningLayer();
    int count = plantLayer->getPlantCount();
    for (int i = 0; i<2; i++) {
        PlantNode* node = plantLayer->getPlantNodeByIndex(i);
        node->updateVertices();
        GL::bindTexture2D( node->_texture->getName());
        GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), node->_vertices);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), ((float*)(node->_vertices))+3);
        glDrawArrays(GL_TRIANGLE_STRIP, 0,node->_verticesCount );
    }

//    return;
//    auto plantLayer = GameLayerPlant::getRunningLayer();
//    int count = plantLayer->getPlantCount();
//    PlantNode* nodes[2]=
//    {
//        plantLayer->getPlantNodeByIndex(0),
//        plantLayer->getPlantNodeByIndex(1)
//    };
//    nodes[0]->updateVertices();
//    nodes[1]->updateVertices();
//    int indexs[2] = {0};
//    int countdraw = 0;
//    int index = 0;
//    GL::bindTexture2D( nodes[0]->_texture->getName());
//    int tcount = 0;
//
//    while (nodes[0]->_cpList.size()>indexs[0]||
//           nodes[1]->_cpList.size()>indexs[1]) {
//        bool flag =false;
//        if (nodes[0]->_cpList.size()<=indexs[0]) {
//            index =  1;
//        }
//        else if (nodes[1]->_cpList.size()<=indexs[1])
//        {
//            index = 0;
//        }
//        else
//        {
//            if (nodes[0]->_cpList[indexs[0]]._zPosition<
//                nodes[1]->_cpList[indexs[1]]._zPosition) {
//                index = 0;
//                }
//            else index = 1;
//            flag = true;
//        }
//        
////        if (flag == true) {
////            <#statements#>
////        }
//        int vcount =0;
//        int zorder = nodes[index]->_cpList[indexs[index]]._zPosition;
//        for (int i =indexs[index] ; i< nodes[index]->_cpList.size(); i++) {
//            if (nodes[index]->_cpList[i]._zPosition != zorder) {
//                break;
//            }
//            vcount++;
//        }
//        int start = indexs[index];
//        indexs[index] +=vcount;
//        if (vcount < 2) {
//            continue;
//        }
//        
//        PlantNode* node = nodes[index];
//        GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
//        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), node->_vertices+start);
//        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), ((float*)(node->_vertices+start))+3);
//        glDrawArrays(GL_TRIANGLE_STRIP,0,vcount);
//    }

}

