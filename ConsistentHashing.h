#ifndef __CONSISTENHASHING_H__
#define __CONSISTENHASHING_H__

#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <map>
#include <cstdlib>
#include <experimental/random>
#include <algorithm>

typedef uint32_t HashCode_t;

namespace ConsistenHashing
{

    class NodeRing
    {
    public:
        void addNode(const std::string &nodeKey, int virtualNodeNumber)
        {
            for (int i = 0; i < virtualNodeNumber; i++)
            {
                nodeMap.emplace(std::experimental::randint(0x0000'0000u, 0xffff'ffffu), nodeKey);
            }
        }

        void removeNode(const std::string &nodeKey)
        {
            auto it = nodeMap.begin();
            auto endIter = nodeMap.end();
            while (it != endIter)
            {
                if (nodeKey == it->second)
                {
                    nodeMap.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        std::string getNode(const std::string &key)
        {
            if (nodeMap.empty())
                return "";

            HashCode_t hashcode = hash(key);

            // Maybe we could use a binary search to improve efficiency.
            auto biggerThan = [&](decltype(nodeMap)::const_reference pair) { return std::get<0>(pair) > hashcode; };
            auto firstBiggerThan = std::find_if(nodeMap.begin(), nodeMap.end(), biggerThan);

            if (nodeMap.end() != firstBiggerThan)
            {
                return firstBiggerThan->second;
            }
            else
            {
                return nodeMap.begin()->second;
            }
        }

    private:
        HashCode_t hash(const std::string &key)
        {
            std::size_t hash = std::hash<std::string>{}(key);
            return static_cast<HashCode_t>(hash & 0xffff'ffff);
        }

        std::map<HashCode_t, std::string> nodeMap;
    };

} // namespace ConsistenHashing

#endif