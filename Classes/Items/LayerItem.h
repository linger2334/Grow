//
//  LayerItem.h
//  GrowUp
//
//  Created by wlg on 9/30/14.
//
//

#ifndef __GrowUp__LayerItem__
#define __GrowUp__LayerItem__

#include <iostream>
#include "Macro.h"
#include "ItemModel.h"
#include "PlantBase.h"
#include "Polygon.h"
#include "GB2ShapeCache-x.h"

class LayerItem : public GameLayerRollY
{
public:
    LayerItem();
    ~LayerItem();
    
    CREATE_FUNC(LayerItem);
    virtual bool init();
    virtual void moveDown(float y);
    
    void loadAllItemsAndBodys();
    void loadItemsAndBodys(float height);
    
    
    inline std::list<ItemModel*>& getItems(){ return _items;}
    inline std::list<class::Polygon*>& getPolygons() { return _polygons;}
    
    void update(float dt);
    
    GB2ShapeCache* _fixturesCache;
protected:
    void loadAllPolygons();
    void loadPolygons(bool needload,float height);
    
    bool firstload;
    int integerHeight;
    std::list<Item> _itemsNeedLoaded;
    std::list<ItemModel*> _items;
    std::map<int,float>_polygonsNeedLoaded;
    std::list<class::Polygon*> _polygons;
};


#endif /* defined(__GrowUp__LayerItem__) */
