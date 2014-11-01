
#ifndef Grow_Goods_test_TypeBase_h
#define Grow_Goods_test_TypeBase_h

enum
{
    TypeStone   = 101,

};

class TypeBase
{
public:
    TypeBase():_type(0){}
    TypeBase(char type):_type(type){}
    void setType(char type){_type = type;}
    virtual char getType(){return _type;}
    virtual bool  isDirtLine(){return _type == TypeDirtLine;}
    virtual bool  isStone(){return _type ==  TypeStone; }
    virtual bool  isMustCrashType(){return _type > 100;}
    virtual bool  isNeedCallBackType(){ return _type < 99;}
    
    char _type;
};


#endif
