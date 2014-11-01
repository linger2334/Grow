//
//  DoubleDragon.h
//  GrowUp
//
//  Created by wlg on 10/22/14.
//
//

#ifndef __GrowUp__DoubleDragon__
#define __GrowUp__DoubleDragon__

#include <iostream>
#include "Macro.h"
#include "ItemModel.h"

class DoubleDragon:public ItemModel
{
public:
    DoubleDragon();
    ~DoubleDragon();
    
    static DoubleDragon* create(Item& item);
    bool init(Item& item);
    
    void createBody();
    inline void setCollisionSign(int sign) { collisionSign = sign;}
    void collisionWithPlant(ItemModel* plantHead);
    
protected:
    Sprite* _belly;
    
    int collisionSign;
};

#endif /* defined(__GrowUp__DoubleDragon__) */
