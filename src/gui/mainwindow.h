#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "../services/StudentManager.h"
#include "../persistence/PersistenceManager.h"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAjouter();
    void onModifier();
    void onSupprimer();
    void onRafraichir();
    void onRechercher(const QString& text);

private:
    // Widgets
    QTableWidget* table;
    QPushButton*  btnAjouter;
    QPushButton*  btnModifier;
    QPushButton*  btnSupprimer;
    QPushButton*  btnRafraichir;
    QLineEdit*    searchBar;

    // Données
    StudentManager      manager;
    const string        filePath = "data/students.txt";

    // Méthodes utilitaires
    void setupUI();
    void refreshTable();
    void refreshTable(const vector<shared_ptr<Student>>& liste);
    void updateStatusBar();
};

#endif
