
#ifndef __Grow_beta_test02__RandomHelper__
#define __Grow_beta_test02__RandomHelper__
#include <random>
class RandomHelper
{
public:
    static int rand(int start,int end)
    {
        static std::default_random_engine generator;
        std::uniform_int_distribution<int> dis(start,end);
        return dis(generator);
    }
    static int rand(int end)
    {
        return rand(0,end);
    }
    static float randFloat(float start,float end)
    {
        static std::default_random_engine generator;
        std::uniform_int_distribution<float> dis(start,end);
        return dis(generator);
    }

};

#endif /* defined(__Grow_beta_test02__RandomHelper__) */
