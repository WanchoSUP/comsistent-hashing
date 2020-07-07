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
    typedef uint32_t HashCode_t;

    template <typename T, typename K>
    struct hash
    {
        virtual T operator()(const K &key) = 0;
    };

    struct stdhash
        : public hash<HashCode_t, std::string>
    {
        HashCode_t operator()(const std::string &key)
        {
            std::size_t hash = std::hash<std::string>{}(key);
            return static_cast<HashCode_t>(hash & 0xffff'ffff);
        }
    };

    class HashRing
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
            auto finder = [&](decltype(nodeMap)::const_reference pair) { return nodeKey == pair.second; };

            auto it = nodeMap.begin();
            while ((it = std::find_if(it, nodeMap.end(), finder)) != nodeMap.end())
            {
                nodeMap.erase(it++);
            }
        }

        std::string getNode(const std::string &key)
        {
            return getNode(key, stdhash());
        }

        template <typename T>
        std::string getNode(const std::string &key, T hash)
        {
            if (nodeMap.empty())
                return "";

            // Maybe we could use a binary search to improve efficiency.
            auto biggerThan = [&](decltype(nodeMap)::const_reference pair) { return std::get<0>(pair) > hash(key); };
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
        std::map<HashCode_t, std::string> nodeMap;
    };

} // namespace ConsistenHashing

#endif