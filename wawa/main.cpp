#include <iostream>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

double eps = 10e-16;
const int SIZE = 6;
const string DIR = "../../TlkAgg5/";

class worker {
public:
    map<string, long long> tasks;
    long long correct = 0, incorrect = 0;
    double wawa = 0;

    void set_task(string task, long long score) {
        tasks[task] = score;
    }

    void set_wawa() {
        if (correct + incorrect == 0) {
            wawa  = 0;
            return;
        }
        wawa = correct * 1.0 / (correct + incorrect);
        if (wawa < eps) {
            wawa = eps;
        }
    }
};

class label {
public:
    vector <long long> arr;
    vector <double> new_arr;
    long long mv, wawa;

    label() {
        arr.resize(SIZE, 0);
        new_arr.resize(SIZE, 0);
        mv = 0;
        wawa = 0;
    }

    void set_score(long long sc) {
        arr[sc]++;
    }

    void set_new_score(long long ans, double sc) {
        new_arr[ans] += sc;
    }

    void calc_mv() {
        mv = 0;
        for (int i = 0; i < SIZE; ++i) {
            if (arr[i] > arr[mv]) {
                mv = i;
            } if (arr[i] == arr[mv]) {
                if (rand() % 2) {
                    mv = i;
                }
            }
        }
    }

    void calc_wawa() {
        wawa = 0;
        for (int i = 0; i < SIZE; ++i) {
            if (new_arr[i] > new_arr[wawa]) {
                wawa = i;
            }
        }
    }
};

int main() {
    srand(time(0));
    map<string, label> tasks;
    map<string, worker> workers;
    ifstream fin(DIR + "crowd_labels.tsv");
    string input_worker, input_task;
    long long sc;
    while (fin >> input_worker >> input_task >> sc) {
        workers[input_worker].set_task(input_task, sc);
        tasks[input_task].set_score(sc);
    }
    fin.close();

    for (auto & task : tasks) {
        task.second.calc_mv();
    }

    for (auto & worker : workers) {
        for (auto & t : worker.second.tasks) {
            if (tasks[t.first].mv == t.second) {
                worker.second.correct++;
            } else {
                worker.second.incorrect++;
            }
        }
        worker.second.set_wawa();
        for (auto t = worker.second.tasks.begin(); t != worker.second.tasks.end(); ++t) {
           tasks[t->first].set_new_score(t->second ,worker.second.wawa);
        }
    }

    ofstream fout(DIR + "wawa.tsv");
    for (auto & task : tasks) {
        task.second.calc_wawa();
        fout << task.first << '\t' << task.second.wawa << endl;
    }
    fout.close();
    return 0;
}
