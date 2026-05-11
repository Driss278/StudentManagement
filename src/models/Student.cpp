#include "Student.h"

Student::Student(const string& name, int id, float gpa) {
    if (id < 1000)
        throw InvalidIDException(id);
    if (gpa < 0.0f || gpa > 4.0f)
        throw InvalidGradeException(gpa);
    this->name = name;
    this->id   = id;
    this->gpa  = gpa;
}

void Student::setName(const string& n) {
    name = n;
}

void Student::setGpa(float g) {
    if (g < 0.0f || g > 4.0f)
        throw InvalidGradeException(g);
    gpa = g;
}
