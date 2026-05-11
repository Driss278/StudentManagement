#ifndef UNDERGRADUATESTUDENT_H
#define UNDERGRADUATESTUDENT_H

#include "Student.h"

class UndergraduateStudent : public Student {
private:
    string major; // Filiere (ex: "Informatique")

public:
    UndergraduateStudent(const string& name, int id, float gpa, const string& major);

    void   display()            const override;
    string getType()            const override;
    double computeScholarship() const override;

    string getMajor() const { return major; }
    void   setMajor(const string& m) { major = m; }
};

#endif
