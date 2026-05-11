#ifndef PHDSTUDENT_H
#define PHDSTUDENT_H

#include "Student.h"


class PhDStudent : public Student {
private:
    string supervisor;
    int    yearOfStudy;

public:
    PhDStudent(const string& name, int id, float gpa,
               const string& supervisor, int yearOfStudy);

    void   display()            const override;
    string getType()            const override;
    double computeScholarship() const override;

    string getSupervisor()  const { return supervisor; }
    int    getYearOfStudy() const { return yearOfStudy; }
    void   setSupervisor(const string& s) { supervisor = s; }
    void   setYearOfStudy(int y) { yearOfStudy = y; }
};

#endif
