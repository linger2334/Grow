//
//  Item.c
//  GrowUp
//
//  Created by wlg on 9/24/14.
//
//

#include <stdio.h>
#include "Item.h"

bool Itemlesser(Item& item1,Item& item2)
{
    return item1.localZorder < item2.localZorder;
}