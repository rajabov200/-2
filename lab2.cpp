#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <map>
#include <random>
#include <locale.h>
using namespace std;

struct Equation {
    double a, b, c;
};

struct Solution {
    string name;
    Equation eq;
    vector<double> roots;
};

bool equal(double a, double b) {
    return fabs(a - b) < 1e-6;
}

bool compareVectors(vector<double>& a, vector<double>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (!equal(a[i], b[i])) return false;
    }
    return true;
}

class Student {
public:
    string name;
    virtual Solution solve(Equation eq) = 0;
};

class GoodStudent : public Student {
public:
    Solution solve(Equation eq) override {
        Solution sol;
        sol.name = name;
        sol.eq = eq;

        double D = eq.b * eq.b - 4 * eq.a * eq.c;

        if (D >= 0) {
            sol.roots.push_back((-eq.b + sqrt(D)) / (2 * eq.a));
            if (D > 0)
                sol.roots.push_back((-eq.b - sqrt(D)) / (2 * eq.a));
        }
        return sol;
    }
};

class AverageStudent : public Student {
public:
    Solution solve(Equation eq) override {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);

        if (dis(gen) < 0.7) {
            GoodStudent g;
            g.name = name;
            return g.solve(eq);
        }
        else {
            Solution sol;
            sol.name = name;
            sol.eq = eq;
            sol.roots.push_back(0);
            return sol;
        }
    }
};

class BadStudent : public Student {
public:
    Solution solve(Equation eq) override {
        Solution sol;
        sol.name = name;
        sol.eq = eq;
        sol.roots.push_back(0);
        return sol;
    }
};

class Teacher {
public:
    vector<Solution> queue;
    map<string, int> results;

    void checkAll() {
        for (auto& sol : queue) {
            double a = sol.eq.a, b = sol.eq.b, c = sol.eq.c;
            double D = b * b - 4 * a * c;

            vector<double> correct;

            if (D >= 0) {
                correct.push_back((-b + sqrt(D)) / (2 * a));
                if (D > 0)
                    correct.push_back((-b - sqrt(D)) / (2 * a));
            }

            if (compareVectors(sol.roots, correct)) {
                results[sol.name]++;
            }
        }
    }

    void printResults() {
        cout << "\n=== Результаты ===\n";

        vector<string> all = { "Alice", "Bob", "Charlie" };
        for (auto& name : all) {
            cout << name << ": " << results[name] << endl;
        }
    }
};

int main() {
    vector<Equation> equations;
    setlocale(LC_ALL, "");
    ifstream file("input.txt");

    if (!file.is_open()) {
        cout << "Ошибка: файл input.txt не найден!\n";
        return 1;
    }

    double a, b, c;
    while (file >> a >> b >> c) {
        equations.push_back({ a, b, c });
    }

    file.close();

    GoodStudent s1; s1.name = "Alice";
    AverageStudent s2; s2.name = "Bob";
    BadStudent s3; s3.name = "Charlie";

    vector<Student*> students = { &s1, &s2, &s3 };

    Teacher teacher;

    for (auto& eq : equations) {
        for (auto st : students) {
            teacher.queue.push_back(st->solve(eq));
        }
    }

    teacher.checkAll();
    teacher.printResults();

    return 0;
}