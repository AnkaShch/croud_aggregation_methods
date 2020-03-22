#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <set>

using namespace std;

const string DIR = "../../TlkAgg2/";
const int SIZE = 2;


int main() {
    srand(time(0));
    ifstream fin(DIR + "crowd_labels.tsv");
    ofstream fout(DIR + "mv.tsv");
    map<string, vector<long long>> mv;
    string w, t;
    long long score;

    while (fin >> w >> t >> score) {
        if (mv.find(t) == mv.end()) {
            mv[t].resize(SIZE, 0);
        }
        mv[t][score]++;
    }
    for (auto & it : mv) {
        long long ind_max = 0;
        for (long long i = 0; i < SIZE; ++i) {
            if (it.second[ind_max] < it.second[i]) {
                ind_max = i;
            } if (it.second[ind_max] == it.second[i]) {
                if (rand() % 2) {
                    ind_max = i;
                }
            }
        }
        fout << it.first << '\t' << ind_max << endl;
    }
    fin.close();
    fout.close();
    return 0;
}
