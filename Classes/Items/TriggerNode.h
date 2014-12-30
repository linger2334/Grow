//
//  TriggerNode.h
//  Grow-LevelEdit
//
//  Created by wlg on 12/18/14.
//
//

#ifndef __Grow_LevelEdit__TriggerNode__
#define __Grow_LevelEdit__TriggerNode__

#include <stdio.h>
#include "ItemModel.h"

class TriggerNode : public ItemModel
{
public:
    TriggerNode();
    ~TriggerNode();
    
    static TriggerNode* create(TriggerInfo& triggerInfo);
    bool init(TriggerInfo& triggerInfo);
    
    void createBody(std::vector<Vec2>& vertexes);
    inline std::vector<int>& getBindIDs() { return _bindIDs;}
protected:
    bool isConvex;
    std::vector<int> _bindIDs;
};




#endif /* defined(__Grow_LevelEdit__TriggerNode__) */
