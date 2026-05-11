#include "mainWindow.h"
#include "StudentDialog.h"
#include "../exceptions/StudentExceptions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QStatusBar>
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    PersistenceManager::load(manager, filePath);
    refreshTable();
    updateStatusBar();
}

MainWindow::~MainWindow() {}

// ─── Setup UI ─────────────────────────────────────────────────
void MainWindow::setupUI() {
    setWindowTitle("Système de Gestion des Étudiants");
    resize(900, 600);

    // Widget central
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    // Barre de recherche
    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Rechercher par nom...");
    connect(searchBar, &QLineEdit::textChanged, this, &MainWindow::onRechercher);

    // Boutons
    btnAjouter   = new QPushButton("+ Ajouter");
    btnModifier  = new QPushButton("✎ Modifier");
    btnSupprimer = new QPushButton("✖ Supprimer");
    btnRafraichir= new QPushButton("↺ Rafraîchir");

    connect(btnAjouter,    &QPushButton::clicked, this, &MainWindow::onAjouter);
    connect(btnModifier,   &QPushButton::clicked, this, &MainWindow::onModifier);
    connect(btnSupprimer,  &QPushButton::clicked, this, &MainWindow::onSupprimer);
    connect(btnRafraichir, &QPushButton::clicked, this, &MainWindow::onRafraichir);

    // Tableau
    table = new QTableWidget();
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Nom", "Type", "GPA", "Bourse (EUR)"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setAlternatingRowColors(true);

    // Layout boutons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(searchBar);
    btnLayout->addStretch();
    btnLayout->addWidget(btnAjouter);
    btnLayout->addWidget(btnModifier);
    btnLayout->addWidget(btnSupprimer);
    btnLayout->addWidget(btnRafraichir);

    // Layout principal
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(table);
}

// ─── Rafraîchir le tableau ────────────────────────────────────
void MainWindow::refreshTable() {
    table->setRowCount(0);
    for (auto& p : manager.getAll()) {
        auto s = p.second;
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::number(s->getId())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s->getName())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(s->getType())));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(s->getGpa(), 'f', 1)));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(s->computeScholarship(), 'f', 0)));
    }
    updateStatusBar();
}

void MainWindow::refreshTable(const vector<shared_ptr<Student>>& liste) {
    table->setRowCount(0);
    for (auto& s : liste) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::number(s->getId())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s->getName())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(s->getType())));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(s->getGpa(), 'f', 1)));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(s->computeScholarship(), 'f', 0)));
    }
}

// ─── Barre de statut ─────────────────────────────────────────
void MainWindow::updateStatusBar() {
    QString msg = QString("Étudiants : %1  |  Moyenne GPA : %2")
                    .arg(manager.count())
                    .arg(manager.averageGpa(), 0, 'f', 2);
    statusBar()->showMessage(msg);
}

// ─── Slots ───────────────────────────────────────────────────
void MainWindow::onAjouter() {
    StudentDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        try {
            manager.add(dlg.getStudent());
            PersistenceManager::save(manager, filePath);
            refreshTable();
            statusBar()->showMessage("Étudiant ajouté avec succès.", 3000);
        } catch (const DuplicateIDException& e) {
            QMessageBox::critical(this, "Erreur ID", QString(e.what()));
        } catch (const InvalidGradeException& e) {
            QMessageBox::warning(this, "Erreur GPA", QString(e.what()));
        } catch (const InvalidIDException& e) {
            QMessageBox::warning(this, "Erreur ID", QString(e.what()));
        }
    }
}

void MainWindow::onModifier() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un étudiant à modifier.");
        return;
    }
    int id = table->item(row, 0)->text().toInt();
    try {
        auto s = manager.findById(id);
        StudentDialog dlg(this, s);
        if (dlg.exec() == QDialog::Accepted) {
            manager.update(id, dlg.getStudent());
            PersistenceManager::save(manager, filePath);
            refreshTable();
            statusBar()->showMessage("Étudiant modifié avec succès.", 3000);
        }
    } catch (const StudentNotFoundException& e) {
        QMessageBox::critical(this, "Erreur", QString(e.what()));
    }
}

void MainWindow::onSupprimer() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un étudiant à supprimer.");
        return;
    }
    int id = table->item(row, 0)->text().toInt();
    auto reply = QMessageBox::question(this, "Confirmer",
                    "Supprimer cet étudiant ?",
                    QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            manager.remove(id);
            PersistenceManager::save(manager, filePath);
            refreshTable();
            statusBar()->showMessage("Étudiant supprimé.", 3000);
        } catch (const StudentNotFoundException& e) {
            QMessageBox::critical(this, "Erreur", QString(e.what()));
        }
    }
}

void MainWindow::onRafraichir() {
    searchBar->clear();
    refreshTable();
}

void MainWindow::onRechercher(const QString& text) {
    if (text.isEmpty()) {
        refreshTable();
        return;
    }
    auto results = manager.findByName(text.toStdString());
    refreshTable(results);
}