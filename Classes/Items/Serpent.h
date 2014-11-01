//
//  Serpent.h
//  GrowUp
//
//  Created by wlg on 10/22/14.
//
//

#ifndef __GrowUp__Serpent__
#define __GrowUp__Serpent__

#include <iostream>
#include "Macro.h"
#include "ItemModel.h"

class Serpent : public ItemModel
{
public:
    Serpent();
    ~Serpent();
    
    static Serpent* create(Item& item);
    bool init(Item& item);
    
    void createBody();
    void collisionWithPlant(ItemModel* plantHead);
    
    void update(float dt);
    
protected:
    Sprite* _hole;
    
    float absorptionRate;
    float holeTransparency;
    
};




#endif /* defined(__GrowUp__Serpent__) */
