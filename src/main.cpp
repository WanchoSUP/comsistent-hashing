#include <iostream>
#include <string>
#include <random>
#include <map>
#include <vector>

#include "ConsistentHashing.h"

using namespace std;
using namespace ConsistenHashing;

int main(int argc, char **argv)
{
    // init params
    vector<string> nodeList{
        "192.168.100.1",
        "192.168.100.2",
        "192.168.100.3",
        "192.168.100.4",
        "192.168.100.5",
        "192.168.100.6"};

    // Create a hash ring
    HashRing hashRing;
    for (auto &node : nodeList)
    {
        hashRing.addNode(node, 200);
    }

    // test
    for (int index = 0; index < nodeList.size(); index++)
    {
        //
        map<string, int> counting;
        for (auto &node : nodeList)
        {
            counting.emplace(node, 0);
        }

        for (int i = 0; i < 100'000; i++)
        {
            auto hitted = counting.find(hashRing.getNode(to_string(random())));
            hitted->second++;
        }

        cout << "round(s) " << index + 1 << ":" << endl;
        for (auto &item : counting)
        {
            cout << item.first << ", hits: " << item.second << endl;
        }

        string toremove = nodeList[index];

        hashRing.removeNode(toremove);
    }

    return 0;
}