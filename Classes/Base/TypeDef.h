#ifndef PlantOri_TypeDef_h
#define PlantOri_TypeDef_h


#define V2TV3(T) Vec3(T.x,T.y,0)

enum GridType
{
    None = 0,
    Dirt = 255,
    Border,
};

enum DrawShapeType
{
    Circle=0,
    Rectangle,
    Rhombus,
    ShapeEnd
};
enum BorderDirection
{
    Left,
    Right,
    Top,
    Bottom,
    LeftTop,
    LeftBottom,
    RightTop,
    RightBottom,
    LeftTop_In,
    LeftBottom_In,
    RightTop_In,
    RightBottom_In
};

enum BorderDirectionMask
{
    Left_Mask = 0x01,
    Right_Mask = 0x02,
    Top_Mask = 0x04,
    Bottom_Mask = 0x08,
    LeftTop_Mask = 0x10,
    LeftBottom_Mask = 0x20,
    RightTop_Mask = 0x40,
    RightBottom_Mask = 0x80,
    LeftTop_In_Mask = 0x100,
    LeftBottom_In_Mask = 0x200,
    RightTop_In_Mask = 0x400,
    RightBottom_In_Mask = 0x800
};
enum BorderMaskType
{
    border_row = 0,
    l_lb_lt = Left_Mask | LeftTop_Mask | LeftBottom_Mask,
    l_lb    = Left_Mask | LeftBottom_Mask,
    l_lt    = Left_Mask | LeftTop_Mask,

    r_rb_rt = Right_Mask | RightBottom_Mask | RightTop_Mask,
    r_rb    = Right_Mask | RightBottom_Mask ,
    r_rt    = Right_Mask | RightTop_Mask ,
    
    t_lt_rt = Top_Mask | LeftTop_Mask | RightTop_Mask,
    t_lt    = Top_Mask | LeftTop_Mask ,
    t_rt    = Top_Mask | RightTop_Mask ,
    
    b_lb_rb = Bottom_Mask | LeftBottom_Mask | RightBottom_Mask,
    b_lb    = Bottom_Mask | LeftBottom_Mask ,
    b_rb    = Bottom_Mask | RightBottom_Mask,

    lt_l_t_lb_rt = LeftTop_Mask | Left_Mask | Top_Mask | LeftBottom_Mask | RightTop_Mask,
    lt_l_t_lb = LeftTop_Mask | Left_Mask | Top_Mask | LeftBottom_Mask,
    lt_l_t_rt = LeftTop_Mask | Left_Mask | Top_Mask | RightTop_Mask,
    lt_l_t = LeftTop_Mask | Left_Mask | Top_Mask ,
    
    lb_l_b_lt_rb = LeftBottom_Mask |Left_Mask | Bottom_Mask |LeftTop_Mask|RightBottom_Mask,
    lb_l_b_lt = LeftBottom_Mask | Left_Mask | Bottom_Mask | LeftTop_Mask,
    lb_l_b_rb = LeftBottom_Mask | Left_Mask | Bottom_Mask | RightBottom_Mask ,
    lb_l_b =  LeftBottom_Mask | Left_Mask | Bottom_Mask ,
    
    rt_r_t_rb_lt = RightTop_Mask | Right_Mask | Top_Mask | RightBottom_Mask |LeftTop_Mask,
    rt_r_t_rb = RightTop_Mask | Right_Mask | Top_Mask | RightBottom_Mask,
    rt_r_t_lt = RightTop_Mask | Right_Mask | Top_Mask |LeftTop_Mask,
    rt_r_t = RightTop_Mask | Right_Mask | Top_Mask,
    
    rb_r_b_lb_rt = RightBottom_Mask | Right_Mask|Bottom_Mask |LeftBottom_Mask|RightTop_Mask,
    rb_r_b_lb = RightBottom_Mask | Right_Mask | Bottom_Mask |LeftBottom_Mask,
    rb_r_b_rt = RightBottom_Mask | Right_Mask | Bottom_Mask |RightTop_Mask,
    rb_r_b = RightBottom_Mask | Right_Mask | Bottom_Mask ,
    
    lt = LeftTop_Mask,
    lb = LeftBottom_Mask,
    rt = RightTop_Mask,
    rb = RightBottom_Mask
    
};
enum FaceDirection
{
    FaceLeft = 0,
    FaceTop,
    FaceRight
};



enum ColorMask
{
    Red = 0x01,
    Green = 0x02,
    Blue = 0x04,
    Alpha = 0x08
};


enum HeadCursorPoint
{
    left_bottom = 0x00,
    left_top = 0x02,
    right_bottom = 0x04,
    right_top = 0x08,
    center_bottom = 0x10,
    center_top = 0x20
};
struct V3F_C4F
{
    //! vertices (3F)
    Vec3     vertices;            // 12 bytes
    //! colors (4B)
    Color4F      colors;              // 4 bytes
};


//Use In LayerBorer

class BorderContext
{
public:
    enum
    {
        NormalBorder,
        OnlyQuad,
    };
    BorderContext():_type(NormalBorder),_quadIndex(0),_quadStartHeight(0){}
    BorderContext(BorderMaskType mask,float startHeight,int quadIndex ,char type = NormalBorder):_maskType(mask),_type(type),_quadIndex(quadIndex),_quadStartHeight(startHeight){}
    
    float getTopHeight;
    char _type;
    BorderMaskType _maskType;
    float _quadStartHeight;
    int _quadIndex;
};

class QuadVec
{
public:
    Vec3 lb,lt,rb,rt;
};
enum
{
    TypeBorderLine = 111,
    TypeStone   = 101,
    
};

enum SceneGameChildZorder
{
    MapBackground = 0,
    MapAlphaDrawing = MapBackground+10,
    MapDirt = MapAlphaDrawing+10,
   
    MapDirtBorder = MapDirt+10,
    MapItemExt  = MapDirtBorder + 4,
    MapPlant = MapDirtBorder+10,
    MapStone = MapPlant+10,
    MapGoods = MapStone+10,
    //////////////////////////////////////道具和关卡编辑器
    MapItem = MapGoods + 10,
    MapUIBorder = MapItem + 10,
    MapLight = MapUIBorder + 10,
    MapUI   =  1000,
    MapLevelEditorMenu
    //  MapLevelEditorMenu = 888
    ////////////////////////////////////////////
};
enum GameLayerZorder
{
    ZorderLayerBackground,
    ZorderLayerPlant,
    ZorderLayerMap,
    ZorderLayerUI,
};

enum RayCastDirection
{
    RayCastLeft,
    RayCastTop,
    RayCastRight,
    RayCastBottom,
};
#endif
