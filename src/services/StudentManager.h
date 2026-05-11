#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include "../models/Student.h"


class StudentManager {
private:
    map<int, shared_ptr<Student>> students;

public:
    void add(shared_ptr<Student> s);
    void remove(int id);
    void update(int id, shared_ptr<Student> updated);

    shared_ptr<Student> findById(int id) const;
    vector<shared_ptr<Student>> findByName(const string& n) const;

    vector<shared_ptr<Student>> sortByGpa() const;
    vector<shared_ptr<Student>> sortByName() const;

    double averageGpa() const;
    int count() const { return students.size(); }

    const map<int, shared_ptr<Student>>& getAll() const { return students; }
};

#endif