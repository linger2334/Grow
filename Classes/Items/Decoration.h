//
//  Decoration.h
//  GrowUp
//
//  Created by wlg on 11/7/14.
//
//

#ifndef __GrowUp__Decoration__
#define __GrowUp__Decoration__

#include <iostream>
#include "Macro.h"
#include "ItemModel.h"

class Decoration : public ItemModel
{
public:
    static Decoration* create(Item& item);
    bool init(Item& item);

};



#endif /* defined(__GrowUp__Decoration__) */
