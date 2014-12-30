//
//  GearReversal.h
//  Grow-LevelEdit
//
//  Created by wlg on 12/25/14.
//
//

#ifndef __Grow_LevelEdit__GearReversal__
#define __Grow_LevelEdit__GearReversal__

#include <stdio.h>
#include "Macro.h"
#include "ItemModel.h"

class GearReversal : public ItemModel
{
public:
    GearReversal();
    ~GearReversal();
    
    static GearReversal* create(Item& item);
    bool init(Item& item);
    
    void createBody();
    
};

#endif /* defined(__Grow_LevelEdit__GearReversal__) */
