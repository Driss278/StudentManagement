#include <iostream>
#include <cassert>
#include "../src/models/UndergraduateStudent.h"
#include "../src/models/GraduateStudent.h"
#include "../src/models/PhDStudent.h"
#include "../src/services/StudentManager.h"
#include "../src/persistence/PersistenceManager.h"
#include "../src/exceptions/StudentExceptions.h"
using namespace std;

// ─── Couleurs pour l'affichage ────────────────────────────────
#define OK   "\033[32m[OK]\033[0m "
#define FAIL "\033[31m[FAIL]\033[0m "

int passed = 0;
int failed = 0;

void printResult(bool success, const string& testName) {
    if (success) {
        cout << OK << testName << endl;
        passed++;
    } else {
        cout << FAIL << testName << endl;
        failed++;
    }
}

// ─────────────────────────────────────────────────────────────
// TEST 1 — Création des 3 types
// ─────────────────────────────────────────────────────────────
void testCreation() {
    cout << "\n=== TEST 1 : Creation des 3 types ===" << endl;

    // Licence
    UndergraduateStudent u("Benali Amira", 1001, 3.8f, "Informatique");
    printResult(u.getType() == "Licence",            "Licence getType()");
    printResult(u.computeScholarship() == 500.0,     "Licence bourse GPA>=3.5 = 500");
    printResult(u.getName() == "Benali Amira",       "Licence getName()");
    printResult(u.getId()   == 1001,                 "Licence getId()");
    printResult(u.getGpa()  == 3.8f,                 "Licence getGpa()");

    UndergraduateStudent u2("Karim Youcef", 1002, 3.0f, "Maths");
    printResult(u2.computeScholarship() == 0.0,      "Licence bourse GPA<3.5 = 0");

    // Master
    GraduateStudent g("Cherif Omar", 1003, 3.5f, "IA");
    printResult(g.getType() == "Master",             "Master getType()");
    printResult(g.computeScholarship() == 3.5*300,   "Master bourse GPA*300");

    // Doctorat
    PhDStudent p("Meziane Riad", 1004, 3.9f, "Dr. Hadj", 2);
    printResult(p.getType() == "Doctorat",           "Doctorat getType()");
    printResult(p.computeScholarship() == 1200.0,    "Doctorat bourse fixe = 1200");
    printResult(p.getSupervisor() == "Dr. Hadj",     "Doctorat getSupervisor()");
    printResult(p.getYearOfStudy() == 2,             "Doctorat getYearOfStudy()");
}

// ─────────────────────────────────────────────────────────────
// TEST 2 — CRUD complet
// ─────────────────────────────────────────────────────────────
void testCRUD() {
    cout << "\n=== TEST 2 : CRUD complet ===" << endl;

    StudentManager mgr;

    // Add
    mgr.add(make_shared<UndergraduateStudent>("Benali Amira", 1001, 3.8f, "Info"));
    mgr.add(make_shared<GraduateStudent>     ("Cherif Omar",  1002, 3.5f, "IA"));
    mgr.add(make_shared<PhDStudent>          ("Meziane Riad", 1003, 3.9f, "Dr. Hadj", 2));
    mgr.add(make_shared<UndergraduateStudent>("Karim Youcef", 1004, 2.9f, "Maths"));
    mgr.add(make_shared<GraduateStudent>     ("Saidi Lina",   1005, 3.2f, "Reseaux"));
    printResult(mgr.count() == 5, "Add : 5 etudiants ajoutes");

    // FindById
    auto s = mgr.findById(1001);
    printResult(s->getName() == "Benali Amira", "FindById(1001) correct");

    // FindByName
    auto results = mgr.findByName("omar");
    printResult(results.size() == 1, "FindByName('omar') retourne 1 resultat");

    // Update
    mgr.update(1001, make_shared<UndergraduateStudent>("Benali Amira Updated", 1001, 3.9f, "Info"));
    auto updated = mgr.findById(1001);
    printResult(updated->getName() == "Benali Amira Updated", "Update correct");

    // Remove
    mgr.remove(1001);
    printResult(mgr.count() == 4, "Remove : 4 etudiants restants");
}

// ─────────────────────────────────────────────────────────────
// TEST 3 — Tri GPA et Nom
// ─────────────────────────────────────────────────────────────
void testTri() {
    cout << "\n=== TEST 3 : Tri ===" << endl;

    StudentManager mgr;
    mgr.add(make_shared<UndergraduateStudent>("Cherif Omar",  1001, 3.5f, "Info"));
    mgr.add(make_shared<GraduateStudent>     ("Benali Amira", 1002, 3.8f, "IA"));
    mgr.add(make_shared<PhDStudent>          ("Meziane Riad", 1003, 3.9f, "Dr. Hadj", 2));
    mgr.add(make_shared<UndergraduateStudent>("Karim Youcef", 1004, 2.9f, "Maths"));
    mgr.add(make_shared<GraduateStudent>     ("Saidi Lina",   1005, 3.2f, "Reseaux"));

    // Tri GPA décroissant
    auto byGpa = mgr.sortByGpa();
    printResult(byGpa[0]->getGpa() >= byGpa[1]->getGpa() &&
                byGpa[1]->getGpa() >= byGpa[2]->getGpa() &&
                byGpa[2]->getGpa() >= byGpa[3]->getGpa() &&
                byGpa[3]->getGpa() >= byGpa[4]->getGpa(),
                "SortByGpa() ordre decroissant correct");

    // Tri alphabétique
    auto byName = mgr.sortByName();
    printResult(byName[0]->getName() <= byName[1]->getName() &&
                byName[1]->getName() <= byName[2]->getName(),
                "SortByName() ordre alphabetique correct");
}

// ─────────────────────────────────────────────────────────────
// TEST 4 — Exceptions
// ─────────────────────────────────────────────────────────────
void testExceptions() {
    cout << "\n=== TEST 4 : Exceptions ===" << endl;

    StudentManager mgr;
    mgr.add(make_shared<UndergraduateStudent>("Benali Amira", 1001, 3.8f, "Info"));

    // InvalidIDException
    bool caught = false;
    try {
        auto s = make_shared<UndergraduateStudent>("Test", 42, 3.0f, "Info");
    } catch (const InvalidIDException&) { caught = true; }
    printResult(caught, "InvalidIDException : ID < 1000");

    // InvalidGradeException — GPA > 4
    caught = false;
    try {
        auto s = make_shared<GraduateStudent>("Test", 1099, 5.5f, "Maths");
    } catch (const InvalidGradeException&) { caught = true; }
    printResult(caught, "InvalidGradeException : GPA > 4.0");

    // InvalidGradeException — GPA < 0
    caught = false;
    try {
        auto s = make_shared<UndergraduateStudent>("Test", 1098, -1.0f, "Info");
    } catch (const InvalidGradeException&) { caught = true; }
    printResult(caught, "InvalidGradeException : GPA < 0.0");

    // DuplicateIDException
    caught = false;
    try {
        mgr.add(make_shared<UndergraduateStudent>("Autre", 1001, 3.0f, "Math"));
    } catch (const DuplicateIDException&) { caught = true; }
    printResult(caught, "DuplicateIDException : ID deja utilise");

    // StudentNotFoundException — remove
    caught = false;
    try {
        mgr.remove(9999);
    } catch (const StudentNotFoundException&) { caught = true; }
    printResult(caught, "StudentNotFoundException : remove(9999)");

    // StudentNotFoundException — findById
    caught = false;
    try {
        mgr.findById(9999);
    } catch (const StudentNotFoundException&) { caught = true; }
    printResult(caught, "StudentNotFoundException : findById(9999)");
}

// ─────────────────────────────────────────────────────────────
// TEST 5 — Persistance
// ─────────────────────────────────────────────────────────────
void testPersistance() {
    cout << "\n=== TEST 5 : Persistance ===" << endl;

    string path = "data/test_students.txt";

    // Sauvegarde
    StudentManager mgr1;
    mgr1.add(make_shared<UndergraduateStudent>("Benali Amira", 1001, 3.8f, "Info"));
    mgr1.add(make_shared<GraduateStudent>     ("Cherif Omar",  1002, 3.5f, "IA"));
    mgr1.add(make_shared<PhDStudent>          ("Meziane Riad", 1003, 3.9f, "Dr. Hadj", 2));
    PersistenceManager::save(mgr1, path);
    printResult(mgr1.count() == 3, "Save : 3 etudiants sauvegardes");

    // Rechargement
    StudentManager mgr2;
    PersistenceManager::load(mgr2, path);
    printResult(mgr2.count() == 3, "Load : 3 etudiants charges");

    // Comparaison
    auto s1 = mgr2.findById(1001);
    printResult(s1->getName() == "Benali Amira" &&
                s1->getGpa()  == 3.8f,
                "Load : donnees identiques apres reload");

    auto s2 = mgr2.findById(1002);
    printResult(s2->getType() == "Master", "Load : type Master correct");

    auto s3 = mgr2.findById(1003);
    printResult(s3->getType() == "Doctorat", "Load : type Doctorat correct");

    // Fichier absent
    StudentManager mgr3;
    PersistenceManager::load(mgr3, "data/fichier_inexistant.txt");
    printResult(mgr3.count() == 0, "Fichier absent : liste vide sans plantage");
}

// ─────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────
int main() {
    cout << "╔══════════════════════════════════════╗" << endl;
    cout << "║   TESTS — Gestion des Etudiants      ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;

    testCreation();
    testCRUD();
    testTri();
    testExceptions();
    testPersistance();

    cout << "  RESULTATS FINAUX " << endl;
    cout << "  Passes  : " << passed << "                          ║" << endl;
    cout << "  Echoues : " << failed << "                           ║" << endl;

    return failed == 0 ? 0 : 1;
}