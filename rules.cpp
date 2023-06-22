#include "rules.hpp"

bool Rules::IsSubset(const Move &first, const Move &second) const
{
    const int stepCountFirst = first.StepCount();
    const int stepCountSecond = second.StepCount();

    if(stepCountFirst < stepCountSecond)
    {
        const size_t min = stepCountFirst;
        for(size_t i = 0; i < min; ++i)
        {
            const Step& step1 = first.GetStep(i);
            const Step& step2 = second.GetStep(i);
            if(step1 != step2)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
