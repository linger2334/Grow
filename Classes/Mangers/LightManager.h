#ifndef __Grow_beta_test02__LightManager__
#define __Grow_beta_test02__LightManager__
#include "common.h"

struct  LightContext
{
public:
    enum
    {
        LightRawState,
        LightIniting,
        LightInPlant,
        LightRemoving
    };
    Node* _node;
    int   _state;
};

class GameLightManager : public Singleton<GameLightManager>
{
public:
    GameLightManager()
    {
    }
    
    int getLightCountByPlantIndex();

    std::vector<LightContext> _lights;
    
};

#endif /* defined(__Grow_beta_test02__LightManager__) */
