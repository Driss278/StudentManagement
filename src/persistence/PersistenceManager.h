#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include <string>
#include "../services/StudentManager.h"

using namespace std;

class PersistenceManager {
public:
    static void save(const StudentManager& mgr, const string& path);
    static void load(StudentManager& mgr, const string& path);
};

#endif