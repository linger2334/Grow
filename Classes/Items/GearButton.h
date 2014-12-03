//
//  GearButton.h
//  GrowUp
//
//  Created by wlg on 11/7/14.
//
//

#ifndef __GrowUp__GearButton__
#define __GrowUp__GearButton__

#include <iostream>
#include "Macro.h"
#include "ItemModel.h"

class GearButton : public ItemModel
{
public:
    GearButton();
    ~GearButton();
    
    static GearButton* create(Item& item);
    bool init(Item& item);
    
    void createCollisionAnimation();
    void createBody();
    
    int bindID;
    ActionInterval* _sink;
protected:
    Sprite* _subject;
    Sprite* _lamp;
    
    float sinkSpeed;
};


#endif /* defined(__GrowUp__GearButton__) */
