#ifndef Grow_Demo_test_PathListHelper_hpp
#define Grow_Demo_test_PathListHelper_hpp
namespace PathListHelper
{

    template<class _Type,class _ListType>
    class PathList
    {
    public:
        typedef  float (*PathGetLengthCall)(const _Type&,const _Type&);
        typedef   _Type (*PathGetPointCall)(float, const _Type& ,const _Type&);

        enum
        {
            BeginToEnd = true,
            EndToBegin = false
        };
        PathList():_list(nullptr){}
        PathList(_ListType* list):_list(list){
            _preLength =0;
            _preReverseLength =0;
         if(_list)
         {
             _preIterator = _list->begin();
             _preReverseIterator = _list->rbegin();
         }
        }

        int  getTypePointByLength(float length , bool isBeginToEnd,_Type* out);
        
        float getTotalLength();
        
        _Type& getLastFindObject()
        {
            return _findType;
        }
        
        _ListType* _list;
        _Type      _findType;
        float     _preLength;
        float     _preReverseLength;
        typename _ListType::iterator _preIterator;
        typename _ListType::reverse_iterator _preReverseIterator;
        PathGetLengthCall PathGetLength;
        PathGetPointCall   PathGetPoint;
        
    };

    template<class _Type,class _ListType>
    int  PathList<_Type,_ListType>::getTypePointByLength(float length , bool isBeginToEnd,_Type* out)
    {
        if (!_list|| _list->size()<2 || length < 0.0f) {
            return -1;
        }
        int ret = -1;
        float tempLength = 0.0f;
        if (isBeginToEnd) {
            auto ip = _list->begin();
            if (_preIterator!=_list->end() && _preLength < length )
            {
                ip = _preIterator;
            }
            auto end = _list->end();
            _Type* old = &(*ip);
            ip++;
            while (ip!=end) {
                _Type* now = &(*ip);
                float stepLen = fabs(PathGetLength((*old),(*now)));
                if (tempLength + stepLen < length ) {
                    tempLength += stepLen;
                    ip++;
                    old = now;
                    continue;
                }
                else
                {
                    float subLen = length - tempLength;
                    _findType = PathGetPoint(subLen, (*old) ,(*now));
                    ret =1;
                    _preLength = length;
                    _preIterator = ip;
                    break;
                }
            }
        }
       else
       {
           auto ip = _list->rbegin();
           if (_preReverseIterator!=_list->rend() && _preLength < length )
           {
               ip = _preReverseIterator;
           }
           auto end = _list->rend();
           _Type* old = &(*ip);
           ip++;
           while (ip!=end) {
               _Type* now = &(*ip);
               float stepLen = fabs(PathGetLength((*old),(*now)));
               if (tempLength + stepLen < length ) {
                   tempLength += stepLen;
                   ip++;
                   old = now;
                   continue;
               }
               else
               {
                   float subLen = length - tempLength;
                   _findType = PathGetPoint(subLen, (*old) ,(*now));
                   ret =1;
                   _preReverseLength = length;
                   _preReverseIterator = ip;
                   break;
               }
           }
           
       }
        if(ret>0 && out)*out = _findType;
        return ret;
    }
    
    template<class _Type,class _ListType>
    float  PathList<_Type,_ListType>::getTotalLength()
    {
        float retLen = 0;
        auto ip = _list->begin();
        auto end = _list->end();
        _Type* old = &(*ip);
        while (ip!=end) {
            _Type* now = &(*ip);
            retLen += fabs(PathGetLength((*old),(*now)));
            ip++;
            old = now;
        }
        return retLen;
    }

}


#endif
