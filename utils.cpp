#include "utils.hpp"
#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */
#define FIRSTH 37 /* also prime */

std::vector<std::string> Utils::split(const std::string &input, const std::string &token)
{
    std::vector<std::string>result;
    std::string str = input;
    while(str.size())
    {
        int index = str.find(token);
        if(index != std::string::npos)
        {
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }
        else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

unsigned int Utils::hash_str(const char *s)
{
    unsigned int h = FIRSTH;
       while (*s) {
         h = (h * A) ^ (s[0] * B);
         s++;
       }
       return h % C;
}

bool Utils::AreStepsEqual(const Step &first, const Step &second)
{
    const Tile& tileStart1 = first.GetStart();
    const Tile& tileEnd1 = first.GetEnd();
    const Tile& tileStart2 = second.GetStart();
    const Tile& tileEnd2 = second.GetEnd();
    return tileStart1 == tileStart2 && tileEnd1 == tileEnd2;
}

bool Utils::AreMovesEqual(const Move &first, const Move &second, std::function<bool (const Step &, const Step &)> pred)
{
    const int stepCountFirst = first.StepCount();
    const int stepCountSecond = second.StepCount();
    if(stepCountFirst != stepCountSecond)
        return false;

    const size_t min = stepCountFirst;
    for(size_t i = 0; i < min; ++i)
    {
        const Step& step1 = first.GetStep(i);
        const Step& step2 = second.GetStep(i);
        if(!pred(step1,step2))
        {
            return false;
        }
    }

    return true;
}

bool Utils::IsSubset(const Move &first, const Move &second, std::function<bool(const Step &,const Step &)> pred)
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
            if(!pred(step1,step2))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
