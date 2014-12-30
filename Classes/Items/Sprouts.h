//
//  Sprouts.h
//  Grow-LevelEdit
//
//  Created by wlg on 12/23/14.
//
//

#ifndef __Grow_LevelEdit__Sprouts__
#define __Grow_LevelEdit__Sprouts__

#include <stdio.h>
#include "ItemModel.h"

class Sprouts : public ItemModel
{
public:
    Sprouts();
    ~Sprouts();
    
    static Sprouts* create(Item& item);
    bool init(Item& item);
    
    void createSproutingAnimate(float growSpeed = kDefaultSproutsGrowSpeed);
    void runSproutingAnimate();
protected:
    ActionInterval* _sproutingAnimate;
};





#endif /* defined(__Grow_LevelEdit__Sprouts__) */
