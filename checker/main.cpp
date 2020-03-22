#include <iostream>
#include <map>
#include <fstream>
#include <vector>

using namespace std;

const string DIR = "../../TlkAgg2/";
const string MOD = "mv";

int main() {
    ifstream fin(DIR + MOD + ".tsv");
    map<string, long long> agg;
    string t;
    long long score;
    while (fin >> t >> score) {
        agg[t] = score;
    }
    fin.close();
    map<string, vector<long long>> gold;
    fin.open(DIR + "golden_labels.tsv");
    while (fin >> t >> score) {
        gold[t].push_back(score);
    }

    long long k = 0, sum = 0;
    for (auto &it : agg) {
        t = it.first;
        score = it.second;
        if (gold.find(t) != gold.end()) {
            k++;
            for (auto e : gold[t]) {
                if (e == score) {
                    sum++;
                    break;
                }
            }
        }
    }
    fin.close();
    cout << sum << ' ' << k << '\n';
    cout << sum * 1.0 / k * 100;
}

