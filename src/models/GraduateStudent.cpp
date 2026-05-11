#include "GraduateStudent.h"
#include <iostream>
#include <iomanip>

GraduateStudent::GraduateStudent(const string& name, int id, float gpa, const string& researchTopic)
    : Student(name, id, gpa), researchTopic(researchTopic) {}

void GraduateStudent::display() const {
    cout << fixed << setprecision(1);
    cout << "[Master] ID: " << id
         << " | Nom: "    << name
         << " | GPA: "    << gpa
         << " | Sujet: "  << researchTopic
         << " | Bourse: " << computeScholarship() << " EUR"
         << endl;
}

string GraduateStudent::getType() const {
    return "Master";
}

double GraduateStudent::computeScholarship() const {
    return gpa * 300.0;
}
