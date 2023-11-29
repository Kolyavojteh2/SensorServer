#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

namespace utils
{
class MAC
{
public:
    static std::string toString(const std::vector<uint8_t> &vec);
    static std::vector<uint8_t> toVector(const std::string &str);
};

};

#endif // UTILS_H
