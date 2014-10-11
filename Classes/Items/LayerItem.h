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

class LayerItem : public GameLayerRollY
{
public:
    LayerItem();
    ~LayerItem();
    
    CREATE_FUNC(LayerItem);
    virtual bool init();
    virtual void moveDown(float y);
    
    void loadItemsAndBodys();
    
};


#endif /* defined(__GrowUp__LayerItem__) */
