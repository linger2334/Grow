//
//  LayerItem.h
//  GrowUp
//
//  Created by wlg on 9/30/14.
//
//

#ifndef __GrowUp__LayerItem__
#define __GrowUp__LayerItem__
#include "Macro.h"
#include "ItemModel.h"
#include "Polygon.h"
#include "GB2ShapeCache-x.h"
#include "GameLayerBase.h"
#include "Singleton.hpp"
class LayerItem : public GameLayerBase,public GameLayerHelper<LayerItem>
{
public:
    LayerItem();
    ~LayerItem();
    
    CREATE_FUNC(LayerItem);
    virtual bool init();
    virtual void moveDown(float y);
    
    void loadAllItemsAndBodys();
    void loadItemsAndBodys(float height);
    void invertStatusInCurrentScreen();
    
    inline std::list<ItemModel*>& getRocks(){ return _rocks;}
    inline std::list<ItemModel*>& getPolygons() { return _polygons;}
    
    void update(float dt);
    
    GB2ShapeCache* _fixturesCache;
    Node* _layerItemExt;
protected:
    void loadAllPolygons();
    void loadPolygons(bool needload,float height);
    
    bool firstload;
    int integerHeight;
    std::list<Item> _itemsNeedLoaded;
    std::list<ItemModel*> _rocks;//
    std::vector<std::pair<int, float>> _polygonsNeedLoaded;
    std::list<ItemModel*> _polygons;
    
};

bool OrderPolygonByHeight(std::pair<int, float>& polygonIndex1,std::pair<int,float>& polygonIndex2);

#endif /* defined(__GrowUp__LayerItem__) */
