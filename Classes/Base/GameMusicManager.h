#ifndef __Grow_Demo_test__GameMusicManager__
#define __Grow_Demo_test__GameMusicManager__
#include "common.h"
#include "Singleton.hpp"

class GameMusicManager : public Singleton<GameMusicManager>
{
public:
    CC_SYNTHESIZE(bool ,_isBackgroundQuite,IsBackgroundQuite);
    CC_SYNTHESIZE(bool ,_isEffectQuite,IsEffectQuite);
    CC_SYNTHESIZE(float ,_backgroundVolume,BackgroundVolume);
    CC_SYNTHESIZE(float ,_effectVolume,EffectVolume);
    CC_SYNTHESIZE(std::string,_playingBackgroundMusic,PlayingBackgroundMusic);
};

#endif /* defined(__Grow_Demo_test__GameMusicManager__) */
