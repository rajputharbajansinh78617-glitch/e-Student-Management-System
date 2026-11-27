// mainwindow.cpp
#include "mainwindow.h"

#include <QCoreApplication>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QVBoxLayout>

// The data file is saved next to the program.
static std::string dataFilePath() {
    return (QCoreApplication::applicationDirPath() + "/students.txt").toStdString();
}

// ====================================================================
//  Setup
// ====================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), store_(dataFilePath()) {
    store_.load();      // read saved students from the file
    buildUi();
    startNewRecord();   // application starts in Entry mode
}

QPushButton *MainWindow::makeButton(const QString &text) {
    return new QPushButton(text);
}

void MainWindow::buildUi() {
    setWindowTitle("Student Record Management System");

    // Title and status line (shows "Record 2 of 5" etc.)
    QLabel *titleLabel = new QLabel("Student Record Management System");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");

    statusLabel_ = new QLabel;
    statusLabel_->setAlignment(Qt::AlignCenter);

    // The form: one labelled text box per student detail
    nameEdit_ = new QLineEdit;
    rollEdit_ = new QLineEdit;
    fatherEdit_ = new QLineEdit;
    casteEdit_ = new QLineEdit;
    districtEdit_ = new QLineEdit;
    gpaEdit_ = new QLineEdit;
    gpaEdit_->setPlaceholderText("0.00 - 4.00");

    QFormLayout *form = new QFormLayout;
    form->addRow("Name:", nameEdit_);
    form->addRow("Roll Number:", rollEdit_);
    form->addRow("Father's Name:", fatherEdit_);
    form->addRow("Caste:", casteEdit_);
    form->addRow("District:", districtEdit_);
    form->addRow("GPA:", gpaEdit_);

    // Buttons
    addButton_ = makeButton("Add");
    updateButton_ = makeButton("Update");
    deleteButton_ = makeButton("Delete");
    previousButton_ = makeButton("Previous");
    nextButton_ = makeButton("Next");
    newButton_ = makeButton("New");

    QHBoxLayout *actionRow = new QHBoxLayout;      // Add / Update / Delete
    actionRow->addWidget(addButton_);
    actionRow->addWidget(updateButton_);
    actionRow->addWidget(deleteButton_);

    QHBoxLayout *navigationRow = new QHBoxLayout;  // Previous / Next / New
    navigationRow->addWidget(previousButton_);
    navigationRow->addWidget(nextButton_);
    navigationRow->addWidget(newButton_);

    // Put everything in one column
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(titleLabel);
    layout->addWidget(statusLabel_);
    layout->addLayout(form);
    layout->addLayout(actionRow);
    layout->addLayout(navigationRow);

    QWidget *centre = new QWidget;
    centre->setLayout(layout);
    setCentralWidget(centre);
    setMinimumWidth(420);

    // Simple look
    setStyleSheet(
        "#titleLabel { font-size: 18px; font-weight: bold; padding: 8px; }"
        "QPushButton { padding: 6px; }"
        "QLineEdit { padding: 4px; }");

    // Connect each button to its function
    connect(addButton_, &QPushButton::clicked, this, &MainWindow::onAdd);
    connect(updateButton_, &QPushButton::clicked, this, &MainWindow::onUpdate);
    connect(deleteButton_, &QPushButton::clicked, this, &MainWindow::onDelete);
    connect(nextButton_, &QPushButton::clicked, this, &MainWindow::onNext);
    connect(previousButton_, &QPushButton::clicked, this, &MainWindow::onPrevious);
    connect(newButton_, &QPushButton::clicked, this, &MainWindow::onNew);
}

// ====================================================================
//  Button functions
// ====================================================================

void MainWindow::onAdd() {
    Student student;
    if (!readForm(student)) {
        return;     // readForm already showed the error
    }

    if (store_.rollExists(student.rollNo)) {
        QMessageBox::warning(this, "Duplicate", "This roll number already exists.");
        return;
    }

    if (!store_.add(student)) {
        QMessageBox::critical(this, "Error", "Could not save the record to the file.");
        return;
    }

    QMessageBox::information(this, "Saved", "Student record added successfully.");
    startNewRecord();   // blank form, ready for the next student
}

void MainWindow::onUpdate() {
    Student student;
    if (!readForm(student)) {
        return;
    }

    // The roll number box is read-only in Browse mode, so it always
    // matches the record on screen. It is what finds the record in the file.
    if (!store_.update(student)) {
        QMessageBox::critical(this, "Error", "Could not update the record.");
        return;
    }

    QMessageBox::information(this, "Updated", "Student record updated successfully.");
    showRecord(currentIndex_);
}

void MainWindow::onDelete() {
    QMessageBox::StandardButton answer = QMessageBox::question(
        this, "Confirm delete", "Do you really want to delete this record?");
    if (answer != QMessageBox::Yes) {
        return;
    }

    std::string rollNo = store_.at(currentIndex_).rollNo;
    if (!store_.remove(rollNo)) {
        QMessageBox::critical(this, "Error", "Could not delete the record.");
        return;
    }

    QMessageBox::information(this, "Deleted", "Student record deleted.");

    if (store_.count() == 0) {
        startNewRecord();                       // nothing left to show
    } else if (currentIndex_ >= store_.count()) {
        showRecord(store_.count() - 1);         // we deleted the last one
    } else {
        showRecord(currentIndex_);              // the next record moved into this spot
    }
}

void MainWindow::onNext() {
    int next = currentIndex_ + 1;
    if (next >= store_.count()) {
        next = 0;                               // after the last record, go back to the first
    }
    showRecord(next);
}

void MainWindow::onPrevious() {
    int previous = currentIndex_ - 1;
    if (previous < 0) {
        previous = store_.count() - 1;          // before the first record, go to the last
    }
    showRecord(previous);
}

void MainWindow::onNew() {
    startNewRecord();
}

// ====================================================================
//  Changing what the screen shows
// ====================================================================

void MainWindow::setMode(Mode mode) {
    bool browsing = (mode == Mode::Browse);
    bool hasRecords = store_.count() > 0;

    addButton_->setEnabled(!browsing);          // Add only in Entry mode
    updateButton_->setEnabled(browsing);        // Update/Delete only in Browse mode
    deleteButton_->setEnabled(browsing);
    newButton_->setEnabled(browsing);
    nextButton_->setEnabled(hasRecords);        // Next/Previous need saved records
    previousButton_->setEnabled(hasRecords);

    rollEdit_->setReadOnly(browsing);           // roll number identifies the record, so lock it

    if (browsing) {
        statusLabel_->setText(QString("Record %1 of %2")
                                  .arg(currentIndex_ + 1)
                                  .arg(store_.count()));
    } else {
        statusLabel_->setText(QString("New record  |  %1 saved").arg(store_.count()));
    }
}

void MainWindow::startNewRecord() {
    currentIndex_ = -1;
    nameEdit_->clear();
    rollEdit_->clear();
    fatherEdit_->clear();
    casteEdit_->clear();
    districtEdit_->clear();
    gpaEdit_->clear();
    setMode(Mode::Entry);
}

void MainWindow::showRecord(int index) {
    currentIndex_ = index;
    const Student &student = store_.at(index);

    nameEdit_->setText(QString::fromStdString(student.name));
    rollEdit_->setText(QString::fromStdString(student.rollNo));
    fatherEdit_->setText(QString::fromStdString(student.fatherName));
    casteEdit_->setText(QString::fromStdString(student.caste));
    districtEdit_->setText(QString::fromStdString(student.district));
    gpaEdit_->setText(QString::number(student.gpa, 'f', 2));

    setMode(Mode::Browse);
}

// ====================================================================
//  Checking what the user typed
// ====================================================================

// Shows a warning and returns false if the text has anything except letters and spaces.
bool MainWindow::lettersOnly(const QString &text, const QString &fieldName) {
    static const QRegularExpression pattern("^[\\p{L} ]+$");
    if (!pattern.match(text).hasMatch()) {
        QMessageBox::warning(this, "Invalid input",
                             fieldName + " must contain only letters and spaces.");
        return false;
    }
    return true;
}

// Reads the form into 'student'. Returns false (after showing why) if anything is wrong.
bool MainWindow::readForm(Student &student) {
    QString name = nameEdit_->text().trimmed();
    QString rollNo = rollEdit_->text().trimmed();
    QString fatherName = fatherEdit_->text().trimmed();
    QString caste = casteEdit_->text().trimmed();
    QString district = districtEdit_->text().trimmed();
    QString gpaText = gpaEdit_->text().trimmed();

    // 1. Nothing may be empty
    if (name.isEmpty() || rollNo.isEmpty() || fatherName.isEmpty() ||
        caste.isEmpty() || district.isEmpty() || gpaText.isEmpty()) {
        QMessageBox::warning(this, "Missing data", "Please fill in all fields.");
        return false;
    }

    // 2. Text fields: letters and spaces only (this also keeps commas out of the file)
    if (!lettersOnly(name, "Name") || !lettersOnly(fatherName, "Father's Name") ||
        !lettersOnly(caste, "Caste") || !lettersOnly(district, "District")) {
        return false;
    }

    // 3. Roll number: letters, digits and '-' only
    static const QRegularExpression rollPattern("^[A-Za-z0-9-]+$");
    if (!rollPattern.match(rollNo).hasMatch()) {
        QMessageBox::warning(this, "Invalid input",
                             "Roll Number can contain only letters, digits and '-'.");
        return false;
    }

    // 4. GPA: a number from 0.0 to 4.0
    bool isNumber = false;
    double gpa = gpaText.toDouble(&isNumber);
    if (!isNumber || gpa < 0.0 || gpa > 4.0) {
        QMessageBox::warning(this, "Invalid input", "GPA must be a number between 0.0 and 4.0.");
        return false;
    }

    // Everything is fine: copy into the Student
    student.name = name.toStdString();
    student.rollNo = rollNo.toStdString();
    student.fatherName = fatherName.toStdString();
    student.caste = caste.toStdString();
    student.district = district.toStdString();
    student.gpa = gpa;
    return true;
}
