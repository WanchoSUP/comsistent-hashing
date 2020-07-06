#include <iostream>
#include <string>
#include <random>
#include <map>
#include <vector>
#include <cmath>

#include "ConsistentHashing.h"

using namespace std;
using namespace ConsistenHashing;

int main(int argc, char **argv)
{
    // Host IPs
    vector<string> nodeList{
        "192.168.100.101",
        "192.168.100.102",
        "192.168.100.103",
        "192.168.100.104",
        "192.168.100.105",
        "192.168.100.106",
        "192.168.100.107",
        "192.168.100.108",
        "192.168.100.109",
        "192.168.100.110"};

    // Create a hash ring
    HashRing hashRing;
    for (auto &node : nodeList)
    {
        hashRing.addNode(node, 200);
    }

    //
    map<string, int> counting;
    for (auto &node : nodeList)
    {
        counting.emplace(node, 0);
    }

    // Hitting test
    for (int i = 0; i < 1000'000; i++)
    {
        string dataKey = to_string(random());
        string nodeKey = hashRing.getNode(dataKey);

        auto hittedItem = counting.find(nodeKey);
        hittedItem->second++;
    }

    // Calculate standard deviation
    double sum = 0;
    for_each(counting.begin(), counting.end(), [&](decltype(counting)::const_reference item) { sum += item.second; });
    double mean = sum / counting.size();
    double variance = 0.;
    for_each(counting.begin(), counting.end(), [&](decltype(counting)::const_reference item) { variance += pow(item.second - mean, 2.); });
    variance = variance / counting.size();
    double standardDeviation = sqrt(variance);

    // Output results
    for (auto &item : counting)
    {
        cout << item.first << ", hits: " << item.second << endl;
    }
    cout << "Standard deviation: " << standardDeviation << endl;

    return 0;
}