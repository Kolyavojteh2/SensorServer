#include "utils.h"
#include <sstream>
#include <iomanip>
#include <iostream>

namespace utils
{
std::string MAC::toString(const std::vector<uint8_t> &vec)
{
    if (vec.size() != 6)
        return "";

    std::ostringstream oss;
    for (size_t i = 0; i < 6; ++i)
    {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(vec[i]);
        if (i < 5)
            oss << ":";
    }
    return oss.str();
}

std::vector<uint8_t> MAC::toVector(const std::string &str)
{
    std::vector<uint8_t> macAddress;
    std::istringstream iss(str);
    std::string byteStr;

    while (std::getline(iss, byteStr, ':'))
    {
        try
        {
            uint8_t byte = std::stoul(byteStr, nullptr, 16);
            macAddress.push_back(byte);
        }
        catch (std::invalid_argument const &e)
        {
            // Обробка помилок перетворення
            std::cerr << "Convert MAC address error. String to Vector" << e.what() << std::endl;
            macAddress.clear();
            break;
        }
    }

    if (macAddress.size() != 6) {
        macAddress.clear();
    }

    return macAddress;
}
}
