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

class Serpent : public ItemModel,public ReversalProtocol
{
public:
    Serpent();
    ~Serpent();
    
    static Serpent* create(Item& item);
    bool init(Item& item);
    
    void createBody();
    void runMoveAction(float dt);
    void switchItemStatus();
    void update(float dt);
protected:
    Sprite* _hole;
    
    float absorptionRate;
    float holeTransparency;
    ///
    float _waitRemoveTime = 0.5 ;
    int   _lightCount = 0;
    DrawNode*   _rangeNode;
};




#endif /* defined(__GrowUp__Serpent__) */
