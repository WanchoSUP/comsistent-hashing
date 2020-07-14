#ifndef __CONSISTENHASHING_H__
#define __CONSISTENHASHING_H__

#include <cstdint>
#include <memory>
#include <string>
#include <map>
#include <cstdlib>
#include <experimental/random>
#include <algorithm>

namespace ConsistenHashing
{
    class HashRing
    {
    public:
        void addNode(const std::string &nodeKey, int virtualNums)
        {
            for (int i = 0; i < virtualNums; i++)
            {
                nodeMap.emplace(getHash(nodeKey + std::to_string(i)), nodeKey);
            }
        }

        void removeNode(const std::string &nodeKey)
        {
            auto finder = [&](decltype(nodeMap)::const_reference pair) { return nodeKey == pair.second; };

            for (auto it = nodeMap.begin(); (it = std::find_if(it, nodeMap.end(), finder)) != nodeMap.end(); ++it)
            {
                nodeMap.erase(it);
            }
        }

        std::string getNode(const std::string &nodeKey)
        {
            if (nodeMap.empty())
                return "";

            auto greater = nodeMap.upper_bound(getHash(nodeKey));

            if (nodeMap.end() != greater)
            {
                return greater->second;
            }
            else
            {
                return nodeMap.begin()->second;
            }
        }

    private:
        static uint32_t getHash(const std::string &key)
        {
            std::size_t hash = std::hash<std::string>{}(key);
            return static_cast<uint32_t>(hash % 0xffff'ffff);
        }

        std::map<uint32_t, std::string> nodeMap;
    };

} // namespace ConsistenHashing

#endif