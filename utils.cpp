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
