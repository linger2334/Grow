#ifndef __SINGLETON_H__
#define __SINGLETON_H__
template <class _Type>
class Singleton
{
private:
    Singleton(){}
    ~Singleton()
    {
        DestorySingletonObject();
    }
public:
    friend  _Type;
    static  _Type* getInstance()
    {
        if (!_sOneObject)
        {
            _sOneObject = new _Type();
        }
        return _sOneObject;
    }
    static bool isHasSingletonObject()
    {
        return _sOneObject != nullptr;
    }
    static void DestorySingletonObject()
    {
        if(_sOneObject)
        {   delete _sOneObject;
        _sOneObject = nullptr;
        }
    }
private:
    static _Type* _sOneObject;
};

template<class _Type>
_Type* Singleton<_Type>::_sOneObject = nullptr;
#endif 