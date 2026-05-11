#include "StudentManager.h"
#include "../exceptions/StudentExceptions.h"
#include <algorithm>
#include <cctype>

// Ajouter un étudiant
void StudentManager::add(shared_ptr<Student> s) {
    if (students.count(s->getId()))
        throw DuplicateIDException(s->getId());
    students[s->getId()] = s;
}

// Supprimer un étudiant
void StudentManager::remove(int id) {
    if (!students.count(id))
        throw StudentNotFoundException(id);
    students.erase(id);
}

// Modifier un étudiant
void StudentManager::update(int id, shared_ptr<Student> updated) {
    if (!students.count(id))
        throw StudentNotFoundException(id);
    students[id] = updated;
}

// Chercher par ID
shared_ptr<Student> StudentManager::findById(int id) const {
    if (!students.count(id))
        throw StudentNotFoundException(id);
    return students.at(id);
}

// Chercher par nom (recherche partielle)
vector<shared_ptr<Student>> StudentManager::findByName(const string& n) const {
    vector<shared_ptr<Student>> result;
    string query = n;
    transform(query.begin(), query.end(), query.begin(), ::tolower);

    for (auto& p : students) {
        string name = p.second->getName();
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.find(query) != string::npos)
            result.push_back(p.second);
    }
    return result;
}

// Tri par GPA décroissant
vector<shared_ptr<Student>> StudentManager::sortByGpa() const {
    vector<shared_ptr<Student>> result;
    for (auto& p : students) result.push_back(p.second);

    sort(result.begin(), result.end(), [](const shared_ptr<Student>& a,
                                          const shared_ptr<Student>& b) {
        return a->getGpa() > b->getGpa();
    });
    return result;
}

// Tri par nom alphabétique (insensible à la casse)
vector<shared_ptr<Student>> StudentManager::sortByName() const {
    vector<shared_ptr<Student>> result;
    for (auto& p : students) result.push_back(p.second);

    sort(result.begin(), result.end(), [](const shared_ptr<Student>& a,
                                          const shared_ptr<Student>& b) {
        string na = a->getName(), nb = b->getName();
        transform(na.begin(), na.end(), na.begin(), ::tolower);
        transform(nb.begin(), nb.end(), nb.begin(), ::tolower);
        return na < nb;
    });
    return result;
}

// Moyenne des GPA
double StudentManager::averageGpa() const {
    if (students.empty()) return 0.0;
    double sum = 0;
    for (auto& p : students) sum += p.second->getGpa();
    return sum / students.size();
}