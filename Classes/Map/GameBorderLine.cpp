#include "GameBorderLine.h"


#define  TABLE(T,N,P) std::make_pair(T, LineDirContext(N,P))
//#define  TABLE(T,N,P) BorderCell::_sDirTable[0] = LineDirContext(Top,Bottom)
std::map<unsigned char,LineDirContext> BorderCell::_sDirTable =
{
    TABLE(t_lt,Left,RightTop),TABLE(t_lt_rt,Left,Right),TABLE(t_rt,LeftTop,Right),
    TABLE(rt_r_t,LeftTop,RightBottom),
    TABLE(r_rt,Top,RightBottom),TABLE(r_rb_rt,Top,Bottom),TABLE(r_rb,RightTop,Bottom),
    TABLE(rb_r_b,RightTop,LeftBottom),
    TABLE(b_rb,Right,LeftBottom),TABLE(b_lb_rb,Right,Left),TABLE(b_lb,RightBottom,Left),
    TABLE(lb_l_b,RightBottom,LeftTop),
    TABLE(l_lb,Bottom,LeftTop),TABLE(l_lb_lt,Bottom,Top),TABLE(l_lt,LeftBottom,Top),
    TABLE(lt_l_t,LeftBottom,RightTop),
    TABLE(rt_r_t_lt,Left,RightBottom), TABLE(rt_r_t_rb,LeftTop,Bottom),
    TABLE(lt_l_t_rt, LeftBottom, Right), TABLE(lt_l_t_lb, Bottom, RightTop),
    TABLE(lb_l_b_rb,Right,LeftTop),    TABLE(lb_l_b_lt,RightBottom,Top),
    TABLE(rb_r_b_lb, RightTop, Left),TABLE(rb_r_b_rt, Top, LeftBottom),
    TABLE(lt_l_t_lb_rt,Bottom,Right),TABLE(rt_r_t_rb_lt, Left, Bottom),
    TABLE(lb_l_b_lt_rb, Right, Top),TABLE(rb_r_b_lb_rt, Top, Left),
    TABLE(rt_r_t_rb_lt ,Left,Bottom),TABLE(rb_r_b_lb_rt, Left, Top)
};

GridCell  getGridCellByDirection(const GridCell& cell,BorderDirection dir)
{
    GridCell ret = cell;
    
    static GridCell _vgrid[]=
    {
        GridCell(-1,0),GridCell(1,0),GridCell(0,1),GridCell(0,-1),
        GridCell(-1,1),GridCell(-1,-1),GridCell(1,1),GridCell(1,-1)
    };
    assert(dir<=BorderDirection::RightBottom);
    ret._x+=_vgrid[dir]._x;
    ret._y+=_vgrid[dir]._y;
    return ret;
    switch (dir) {
        case BorderDirection::Left:
            ret._x-=1;
            break;
        case BorderDirection::Right:
            ret._x+=1;
            break;
        case BorderDirection::Top:
            ret._y+=1;
            break;
        case BorderDirection::Bottom:
            ret._y-=1;
            break;
        case BorderDirection::LeftTop:
            ret._x-=1;
            ret._y+=1;
            break;
        case BorderDirection::LeftBottom:
            ret._x-=1;
            ret._y-=1;
            break;
        case BorderDirection::RightTop:
            ret._x+=1;
            ret._y+=1;
            break;
        case BorderDirection::RightBottom:
            ret._x+=1;
            ret._y-=1;
            break;
        default:
            assert(0);
            break;
    }
    return ret;
}
bool BorderCell::isValidBorderCell(BorderMaskType type)
{
    return _sDirTable.find(type)!=_sDirTable.end();
}
bool BorderCell::isNearBorderCell(const BorderCell& cell,bool* isNext)
{
    bool ret = false;
    bool tisNext=false;
    if (cell._cell == getNextMapCell() )
    {
        ret=true;
        tisNext = true;
    }
    else if (cell._cell == getPreMapCell())ret=true;
    
    if(isNext && ret)*isNext=tisNext;
    return ret;
}
BorderDirection BorderCell::getNextBorderDirection(bool  clockwise)
{
    BorderDirection nextDir;
    auto ip =_sDirTable.find(_maskDir);
    if (ip == _sDirTable.end()) {
        log("table size %d" ,_sDirTable.size());
        log("Error dirMaskType %d",_maskDir);
        assert(0);
    }
    auto& context = ip->second;
    if (clockwise) {nextDir = context._next;}
    else { nextDir = context._pre;}
    return nextDir;
}
BorderDirection BorderCell::getPreBorderDirection(bool clockwise)
{
    BorderDirection nextDir;
    auto ip =_sDirTable.find(_maskDir);
    if (ip == _sDirTable.end()) {
        log("table size %d" ,_sDirTable.size());
        log("Error dirMaskType %d",_maskDir);
        assert(0);
    }
    auto& context = ip->second;
    if (clockwise) {nextDir = context._pre;}
    else { nextDir = context._next;}
    return nextDir;
}

GridCell  BorderCell::getNextMapCell(bool clockwise)
{
    BorderDirection nextDir;
    auto ip =_sDirTable.find(_maskDir);
    if (ip == _sDirTable.end()) {
        log("table size %d" ,_sDirTable.size());
        log("Error dirMaskType %d",_maskDir);
        assert(0);
    }
    auto& context = ip->second;
    if (clockwise) {nextDir = context._next;}
    else { nextDir = context._pre;}
    
    return getGridCellByDirection(_cell,nextDir);
}


GridCell BorderCell::getNextMapCell()
{
    return getNextMapCell(true);
}
GridCell BorderCell::getPreMapCell()
{
    return getNextMapCell(false);
}

BorderCell BorderCell::getNextBorderCell()
{
    return BorderCell(getNextMapCell());
}
BorderCell BorderCell::getPreBorderCell()
{
    return BorderCell(getPreBorderCell());
    
}

bool  GameBorderLine::isRing()
{
    if(_borderList.empty())return false;
    GridCell cellFirst = getFirstBorderCell()._cell;
    GridCell cellLast = getLastBorderCell()._cell;
    
    int x = abs(cellFirst._x-cellLast._x);
    int y = abs(cellFirst._y-cellLast._y);
    return x <= 1 && y <= 1;
}

void GameBorderLine::push_back(const BorderCell&  cell)
{
    _borderList.push_back(cell);
}
void GameBorderLine::push_fornt(const BorderCell& cell)
{
    _borderList.push_front(cell);
}
BorderCell&  GameBorderLine::getFirstBorderCell()
{
    return *_borderList.begin();
}
BorderCell& GameBorderLine::getLastBorderCell()
{
    return *_borderList.rbegin();
}

void GameBorderLine::mergeBorderLine()
{
    
}

bool GameBorderLine::isLinkBorderLine( GameBorderLine& borderLine)
{
    if(borderLine._borderList.size()<2)return false;
    BorderCell&  cell = *borderLine._borderList.begin();
    return cell.isNearBorderCell(*borderLine._borderList.end());
}

bool GameBorderLine::LinkBorderLine( GameBorderLine& borderLine)
{
    bool bRet =false;
    do
    {
        auto end = getLastBorderCell();
        auto first2 = borderLine.getFirstBorderCell();
        if (end.isNearBorderCell(first2)) {
            auto ip = borderLine._borderList.begin();
            auto endborder =borderLine._borderList.end();
            while (ip!=endborder) {
                _borderList.push_back(*ip);
                ip++;
            }
            bRet= true;
            break;
        }
        
        auto frst1 = getFirstBorderCell();
        auto end2  = borderLine.getLastBorderCell();
        if ( first2.isNearBorderCell(end2)) {
            auto ip = borderLine._borderList.rbegin();
            auto endborder =borderLine._borderList.rend();
            while (ip!=endborder) {
                _borderList.push_front(*ip);
                ip++;
            }
            bRet= true;
            break;
        }
    }while (0);
    
    return bRet;
}

std::list<GameBorderLine> GameBorderLine::getBorderlines(const std::list<BorderCell>& list)
{
    std::list<GameBorderLine> ret;
    if (list.empty()) {
        return  std::move(ret);
    }
    
    std::list<BorderCell> temList = list;
    while(!temList.empty())
    {
        ret.push_back(GameBorderLine());
        GameBorderLine& line = *ret.rbegin();
        BorderCell cell;
        std::list<BorderCell>::const_iterator ip=temList.begin();
        cell = *ip;
        line.push_back(cell);
        temList.erase(ip);
        GridCell tgrid;
        for (int i=0; i<2; i++) {
            BorderCell tcell = cell;
            for (; ; ) {
                if(i==0) tgrid =tcell.getNextMapCell();
                else tgrid = tcell.getPreMapCell();
                if(temList.empty())break;
                auto ipn = std::find(temList.begin(),temList.end(),tgrid);
                if(ipn == temList.end()){ break;}
                if(i==0)line.push_back(*ipn);
                else line.push_fornt(*ipn);
                tcell =*ipn;
                temList.erase(ipn);
            }
        }
    }
    return std::move(ret);
}
