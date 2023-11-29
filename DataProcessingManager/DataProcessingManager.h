#ifndef DATA_PROCESSING_MANAGER_H
#define DATA_PROCESSING_MANAGER_H

#include <map>
#include <iostream>
#include <vector>

class TCPServer;
class SensorsManager;

struct HandlerArguments_t
{
    TCPServer *server = nullptr;
    std::vector<uint8_t> *binInput = nullptr;
    SensorsManager *manager = nullptr;
};

template<class Key_t, class HandlerFunc>
class DataProcessingManager
{
public:
    void add(const Key_t& key, const HandlerFunc &handler)
    {
        if (m_handlers.find(key) != m_handlers.end())
            std::cerr << "The same key already exists. the HandlerFunc will overwriten" << std::endl;

        m_handlers[key] = handler;
    }

    void remove(const Key_t& key)
    {
        if (m_handlers.find(key) == m_handlers.end())
        {
            std::cerr << "This key doesn't exists." << std::endl;
            return;
        }

        m_handlers.erase(key);
    }

    void runAll(void *inputData) const
    {
        HandlerArguments_t* handlerArgs = (HandlerArguments_t*) inputData;

        while (!handlerArgs->binInput->empty())
        {
            for (auto &handler : m_handlers)
            {
                handler.second(inputData);
                if (handlerArgs->binInput->empty())
                    break;
            }
        }
    }

    void run(const Key_t& key, void *inputData) const
    {
        auto iter = m_handlers.find(key);
        if (iter == m_handlers.end())
        {
            std::cerr << "This key doesn't exists." << std::endl;
            return;
        }

        iter.second(inputData);
    }

private:
    std::map<Key_t, HandlerFunc> m_handlers;
};

#endif // DATA_PROCESSING_MANAGER_H
