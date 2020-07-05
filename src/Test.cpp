#include <iostream>
#include <string>
#include <random>
#include <map>

#include "ConsistentHashing.h"

using namespace std;
using namespace ConsistenHashing;

int main(int argc, char **argv)
{
    HashRing hashRing;

    map<string, int> nodeList{
        {"192.168.100.1", 0},
        {"192.168.100.2", 0},
        {"192.168.100.3", 0},
        {"192.168.100.4", 0},
        {"192.168.100.5", 0},
        {"192.168.100.6", 0}};

    for (auto &node : nodeList)
    {
        hashRing.addNode(node.first, 200);
    }

    for (int i = 0; i < 100'0000; i++)
    {
        auto hitted = nodeList.find(hashRing.getNode(to_string(random())));
        hitted->second++;
    }

    for (auto &node : nodeList)
    {
        cout << node.first << ", hitted count: " << node.second << endl;
    }

    return 0;
}