#include "MathHelper.h"

// 弧长公式:  l(弧长) = n（圆心角）x π（圆周率）x r（半径）/180
// 旋转角计算: angle = l(弧长)/ (π（圆周率）x r（半径）/180)
// 直角三角形锐角对边边长: l = sin( (angle*PI.180){弧度})*斜边长
// 直角三角形锐角邻边边长: l = cos( (angle*PI.180){弧度})*斜边长
#define PI 3.1415926f

namespace MathHelper
{
    
Vec3 getRotatePosition(Vec3 pto,float len,float angle)
{
    return getRotatePosition(pto,Vec3(pto.x-len,pto.y,pto.z),angle);
}

Vec3 getRotatePosition(Vec3 pto,Vec3 pt,float angle)
{
//    Vec3 ptRet;
//    float  angleN = angle * PI/180.0f;
//    ptRet.x= (pt.x - pto.x)*cos(angleN) + (pt.y - pto.y)*sin(angleN) + pto.x ;
//    ptRet.y= -(pt.x - pto.x)*sin(angleN) + (pt.y  - pto.y)*cos(angleN) + pto.y ;
//    ptRet.z=pto.z;
//    return ptRet;
    Vec2 ptRetV2 = getRotatePosition(Vec2(pto.x,pto.y),Vec2(pt.x,pt.y),angle);
    return Vec3(ptRetV2.x,ptRetV2.y,pto.z);
}
    
Vec2 getRotatePosition(Vec2 pto,float len,float angle)
{
    return getRotatePosition(pto,Vec2(pto.x-len,pto.y),angle);
}
    
Vec2 getRotatePosition(Vec2 pto,Vec2 pt,float angle)
{
    Vec2 ptRet;
    float  angleN = angle * PI/180.0f;
    ptRet.x= (pt.x - pto.x)*cos(angleN) + (pt.y - pto.y)*sin(angleN) + pto.x ;
    ptRet.y= -(pt.x - pto.x)*sin(angleN) + (pt.y  - pto.y)*cos(angleN) + pto.y ;
    return ptRet;
}

    float getAcrLength(float angle,float radius )
    {
        return (PI*radius/180)*angle;
    }
float getRotateAngle(float acrLength,float radius)
{
    return acrLength/ (PI*radius/180);
}
    
void getWidthHeight(float p[2],float len,float angle)
{
    float  angleN = angle * PI/180.0f;
    p[0]=sin(angleN)*len;
    p[1]=cos(angleN)*len;
}

  

}