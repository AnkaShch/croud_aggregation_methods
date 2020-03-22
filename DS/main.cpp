#include <iostream>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

const double EPS = 10e-16;
const int STEP = 10;
const int SIZE = 2;
const string DIR = "../../TlkAgg2";

class worker {
public:
    map<string, long long> tasks;
    double table[SIZE][SIZE];

    void set_task(const string& task, long long score) {
        tasks[task] = score;
    }

    void clear_table() {
        for (int i  = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                table[i][j] = 0;
            }
        }
    }

    void upd_table(const double * arr, long long ans) {
        for (int i = 0; i < SIZE; ++i) {
            table[i][ans] += arr[i];
        }
    }

    void normalize() {
        for (int i = 0; i < SIZE; ++i) {
            double sum = 0;
            for (int j = 0; j < SIZE; ++j) {
                sum += table[i][j];
            }
            for (int j = 0; j < SIZE; ++j) {
                table[i][j] = max(table[i][j] / sum, EPS);
            }
        }
    }

    void print_table() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                cout << table[i][j] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }
};

class label {
public:
    double arr[SIZE]{};
    long long ans = 0;

    label() {
        for (auto & e : arr)
            e = 0;
    }

    void set_score(long long sc) {
        arr[sc]++;
    }

    void normalize() {
        double sum = 0;
        for (auto e : arr) {
            sum += e;
        }
        for (auto & e : arr) {
            e = max(e / sum, EPS);
        }
    }

    void upd_arr(long long i, double elm) {
        arr[i] = max(arr[i] * elm, EPS);
    };

    void find_ans() {
        for (int i = 0; i < SIZE; ++i) {
            if (arr[i] > arr[ans]) {
                ans = i;
            } if (arr[i] == arr[ans]) {
                if (rand() % 2 != 0) {
                    ans = i;
                }
            }
        }
    }
};

int main() {
    srand(time(0));
    map<string, label> tasks;
    map<string, worker> workers;
    ifstream fin(DIR + "/crowd_labels.tsv");
    string input_worker, input_task;
    long long sc;
    while (fin >> input_worker >> input_task >> sc) {
        workers[input_worker].set_task(input_task, sc);
        tasks[input_task].set_score(sc);
    }
    fin.close();

    for (int k = 0; k < STEP; ++k) {
        for (auto & [name, label] : tasks) {
            label.normalize();
        }

        for (auto &[worker_name, worker_task] : workers) {
            worker_task.clear_table();
            for (auto &[task_name, _] : worker_task.tasks) {
                worker_task.upd_table(tasks[task_name].arr, worker_task.tasks[task_name]);
            }
            worker_task.normalize();
        }

        vector<double> prior(SIZE, 0);
        for (auto &[task_name, task_label] : tasks) {
            for (int i = 0; i < SIZE; ++i) {
                prior[i] += task_label.arr[i];
            }
        }

        double sum = 0;
        for (auto e : prior) {
            sum += e;
        }
        for (auto &e : prior) {
            e = max(e / sum, EPS);
        }

        for (auto &[task_name, task_label] : tasks) {
            for (int i = 0; i < SIZE; ++i) {
                task_label.arr[i] = prior[i];
            }
        }

        for (auto &[worker_name, worker_task] : workers) {
            for (auto &[task_name, task_label] : worker_task.tasks) {
                for (long long i = 0; i < SIZE; ++i){
                    tasks[task_name].upd_arr(i, worker_task.table[i][task_label]);
                }
            }
        }
    }

    ofstream fout(DIR + "/ds.tsv");
    for (auto &[task_name, task_label] : tasks) {
        task_label.normalize();
        task_label.find_ans();
        fout << task_name << '\t' << task_label.ans << '\n';
    }
    fout.close();
}
