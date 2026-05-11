#include "StudentDialog.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"
#include "../exceptions/StudentExceptions.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "ui_mainwindow.h"

StudentDialog::StudentDialog(QWidget* parent, shared_ptr<Student> existing)
    : QDialog(parent) {
    setupUI();
    if (existing) loadExisting(existing);
}

void StudentDialog::setupUI() {
    setWindowTitle("Étudiant");
    setMinimumWidth(400);

    formLayout = new QFormLayout();

    // Type
    typeBox = new QComboBox();
    typeBox->addItems({"Licence", "Master", "Doctorat"});
    connect(typeBox, &QComboBox::currentTextChanged, this, &StudentDialog::onTypeChanged);
    formLayout->addRow("Type :", typeBox);

    // Nom
    nameEdit = new QLineEdit();
    formLayout->addRow("Nom :", nameEdit);

    // ID
    idEdit = new QLineEdit();
    formLayout->addRow("ID :", idEdit);

    // GPA
    gpaSpinBox = new QDoubleSpinBox();
    gpaSpinBox->setRange(0.0, 4.0);
    gpaSpinBox->setSingleStep(0.1);
    gpaSpinBox->setDecimals(1);
    formLayout->addRow("GPA :", gpaSpinBox);

    // Champs dynamiques
    majorEdit      = new QLineEdit();
    topicEdit      = new QLineEdit();
    supervisorEdit = new QLineEdit();
    yearEdit       = new QLineEdit();

    formLayout->addRow("Filière :",    majorEdit);
    formLayout->addRow("Sujet :",      topicEdit);
    formLayout->addRow("Directeur :",  supervisorEdit);
    formLayout->addRow("Année :",      yearEdit);

    // Afficher uniquement les champs Licence par défaut
    onTypeChanged("Licence");

    // Boutons
    QPushButton* btnValider  = new QPushButton("Valider");
    QPushButton* btnAnnuler  = new QPushButton("Annuler");
    connect(btnValider, &QPushButton::clicked, this, &StudentDialog::onValider);
    connect(btnAnnuler, &QPushButton::clicked, this, &QDialog::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(btnValider);
    btnLayout->addWidget(btnAnnuler);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);
}

void StudentDialog::onTypeChanged(const QString& type) {
    // Cacher tous les champs dynamiques
    majorEdit->hide();      formLayout->labelForField(majorEdit)->hide();
    topicEdit->hide();      formLayout->labelForField(topicEdit)->hide();
    supervisorEdit->hide(); formLayout->labelForField(supervisorEdit)->hide();
    yearEdit->hide();       formLayout->labelForField(yearEdit)->hide();

    // Afficher selon le type
    if (type == "Licence") {
        majorEdit->show();
        formLayout->labelForField(majorEdit)->show();
    } else if (type == "Master") {
        topicEdit->show();
        formLayout->labelForField(topicEdit)->show();
    } else if (type == "Doctorat") {
        supervisorEdit->show();
        formLayout->labelForField(supervisorEdit)->show();
        yearEdit->show();
        formLayout->labelForField(yearEdit)->show();
    }
    adjustSize();
}

void StudentDialog::onValider() {
    try {
        string name = nameEdit->text().toStdString();
        int    id   = idEdit->text().toInt();
        float  gpa  = gpaSpinBox->value();
        string type = typeBox->currentText().toStdString();

        if (name.empty()) {
            QMessageBox::warning(this, "Erreur", "Le nom ne peut pas être vide.");
            return;
        }

        if (type == "Licence") {
            string major = majorEdit->text().toStdString();
            result = make_shared<UndergraduateStudent>(name, id, gpa, major);
        } else if (type == "Master") {
            string topic = topicEdit->text().toStdString();
            result = make_shared<GraduateStudent>(name, id, gpa, topic);
        } else if (type == "Doctorat") {
            string supervisor = supervisorEdit->text().toStdString();
            int    year       = yearEdit->text().toInt();
            result = make_shared<PhDStudent>(name, id, gpa, supervisor, year);
        }
        accept();

    } catch (const InvalidIDException& e) {
        QMessageBox::critical(this, "Erreur ID", QString(e.what()));
    } catch (const InvalidGradeException& e) {
        QMessageBox::warning(this, "Erreur GPA", QString(e.what()));
    }
}

void StudentDialog::loadExisting(shared_ptr<Student> s) {
    typeBox->setCurrentText(QString::fromStdString(s->getType()));
    nameEdit->setText(QString::fromStdString(s->getName()));
    idEdit->setText(QString::number(s->getId()));
    gpaSpinBox->setValue(s->getGpa());

    if (s->getType() == "Licence") {
        auto u = dynamic_pointer_cast<UndergraduateStudent>(s);
        majorEdit->setText(QString::fromStdString(u->getMajor()));
    } else if (s->getType() == "Master") {
        auto g = dynamic_pointer_cast<GraduateStudent>(s);
        topicEdit->setText(QString::fromStdString(g->getResearchTopic()));
    } else if (s->getType() == "Doctorat") {
        auto p = dynamic_pointer_cast<PhDStudent>(s);
        supervisorEdit->setText(QString::fromStdString(p->getSupervisor()));
        yearEdit->setText(QString::number(p->getYearOfStudy()));
    }
}

shared_ptr<Student> StudentDialog::getStudent() const {
    return result;
}