// mainwindow.h
// The window: form fields, buttons and the logic behind each button.
// All file work is done by StudentStore - this class only handles the screen.
#pragma once

#include <QMainWindow>
#include "studentstore.h"

class QLabel;
class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:          // functions that run when a button is clicked
    void onAdd();
    void onUpdate();
    void onDelete();
    void onNext();
    void onPrevious();
    void onNew();

private:
    // Entry  = blank form, ready to add a new student
    // Browse = a saved student is on screen, ready to update or delete
    enum class Mode { Entry, Browse };

    // --- building the screen ---
    void buildUi();
    QPushButton *makeButton(const QString &text);

    // --- changing what the screen shows ---
    void setMode(Mode mode);          // enables/disables buttons for the mode
    void startNewRecord();            // blank form + Entry mode
    void showRecord(int index);       // fill the form + Browse mode

    // --- reading and checking what the user typed ---
    bool readForm(Student &student);
    bool lettersOnly(const QString &text, const QString &fieldName);

    StudentStore store_;
    int currentIndex_ = -1;           // which record is shown (-1 = none)

    QLineEdit *nameEdit_ = nullptr;
    QLineEdit *rollEdit_ = nullptr;
    QLineEdit *fatherEdit_ = nullptr;
    QLineEdit *casteEdit_ = nullptr;
    QLineEdit *districtEdit_ = nullptr;
    QLineEdit *gpaEdit_ = nullptr;

    QLabel *statusLabel_ = nullptr;

    QPushButton *addButton_ = nullptr;
    QPushButton *updateButton_ = nullptr;
    QPushButton *deleteButton_ = nullptr;
    QPushButton *nextButton_ = nullptr;
    QPushButton *previousButton_ = nullptr;
    QPushButton *newButton_ = nullptr;
};
