//
//  GearGate.h
//  GrowUp
//
//  Created by wlg on 11/7/14.
//
//

#ifndef __GrowUp__GearGate__
#define __GrowUp__GearGate__

#include <iostream>
#include "ItemModel.h"
#include "Macro.h"

class GearGate : public ItemModel
{
public:
    GearGate();
    ~GearGate();
    
    static GearGate* create(Item& item);
    bool init(Item& item);
    
    void createBody();
    void openGate();
    void collisionWithPlant(ItemModel* plantHead);
protected:
    Sprite* _left;
    Sprite* _right;
    Sprite* _leftEye;
    Sprite* _rightEye;
    
    int gap;
    float startRate;
};


#endif /* defined(__GrowUp__GearGate__) */
