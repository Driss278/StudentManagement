#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <memory>
#include "../models/Student.h"
#include "ui_mainwindow.h"

class StudentDialog : public QDialog {
    Q_OBJECT

public:
    explicit StudentDialog(QWidget* parent = nullptr,
                           shared_ptr<Student> existing = nullptr);
    shared_ptr<Student> getStudent() const;

private slots:
    void onTypeChanged(const QString& type);
    void onValider();

private:
    QComboBox*      typeBox;
    QLineEdit*      nameEdit;
    QLineEdit*      idEdit;
    QDoubleSpinBox* gpaSpinBox;

    // Champs dynamiques
    QLineEdit*      majorEdit;       // Licence
    QLineEdit*      topicEdit;       // Master
    QLineEdit*      supervisorEdit;  // Doctorat
    QLineEdit*      yearEdit;        // Doctorat

    QFormLayout*    formLayout;
    shared_ptr<Student> result;

    void setupUI();
    void loadExisting(shared_ptr<Student> s);
};

#endif