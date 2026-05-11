#include "PersistenceManager.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"
#include "../exceptions/StudentExceptions.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// ─── SAUVEGARDE ───────────────────────────────────────────────
void PersistenceManager::save(const StudentManager& mgr, const string& path) {
    ofstream file(path);
    if (!file.is_open()) {
        cerr << "[ERREUR] Impossible d'ouvrir le fichier : " << path << endl;
        return;
    }

    for (auto& p : mgr.getAll()) {
        shared_ptr<Student> s = p.second;
        string type = s->getType();

        if (type == "Licence") {
            auto u = dynamic_pointer_cast<UndergraduateStudent>(s);
            file << "Licence|" << s->getId() << "|"
                 << s->getName() << "|"
                 << s->getGpa() << "|"
                 << u->getMajor() << "\n";

        } else if (type == "Master") {
            auto g = dynamic_pointer_cast<GraduateStudent>(s);
            file << "Master|" << s->getId() << "|"
                 << s->getName() << "|"
                 << s->getGpa() << "|"
                 << g->getResearchTopic() << "\n";

        } else if (type == "Doctorat") {
            auto p2 = dynamic_pointer_cast<PhDStudent>(s);
            file << "Doctorat|" << s->getId() << "|"
                 << s->getName() << "|"
                 << s->getGpa() << "|"
                 << p2->getSupervisor() << "|"
                 << p2->getYearOfStudy() << "\n";
        }
    }

    file.close();
    cout << "[OK] Donnees sauvegardees dans : " << path << endl;
}

// ─── CHARGEMENT ───────────────────────────────────────────────
void PersistenceManager::load(StudentManager& mgr, const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "[INFO] Fichier absent, demarrage avec liste vide." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        try {
            stringstream ss(line);
            string type, idStr, name, gpaStr;

            getline(ss, type,   '|');
            getline(ss, idStr,  '|');
            getline(ss, name,   '|');
            getline(ss, gpaStr, '|');

            int   id  = stoi(idStr);
            float gpa = stof(gpaStr);

            if (type == "Licence") {
                string major;
                getline(ss, major, '|');
                mgr.add(make_shared<UndergraduateStudent>(name, id, gpa, major));

            } else if (type == "Master") {
                string topic;
                getline(ss, topic, '|');
                mgr.add(make_shared<GraduateStudent>(name, id, gpa, topic));

            } else if (type == "Doctorat") {
                string supervisor, yearStr;
                getline(ss, supervisor, '|');
                getline(ss, yearStr,    '|');
                int year = stoi(yearStr);
                mgr.add(make_shared<PhDStudent>(name, id, gpa, supervisor, year));
            }

        } catch (...) {
            cerr << "[AVERTISSEMENT] Ligne ignoree : " << line << endl;
        }
    }

    file.close();
    cout << "[OK] Donnees chargees depuis : " << path << endl;
}