// studentstore.cpp
#include "studentstore.h"

#include <fstream>
#include <iomanip>
#include <sstream>

StudentStore::StudentStore(const std::string& filePath)
    : filePath_(filePath) {}

// ---------- Reading ----------

void StudentStore::load() {
    students_.clear();

    std::ifstream file(filePath_);
    if (!file.is_open()) {
        return;   // no file yet (first run) -> start with an empty list
    }

    std::string line;
    while (std::getline(file, line)) {
        Student student;
        if (fromLine(line, student)) {   // blank or damaged lines are skipped
            students_.push_back(student);
        }
    }
}

int StudentStore::count() const {
    return static_cast<int>(students_.size());
}

const Student& StudentStore::at(int index) const {
    return students_.at(index);
}

bool StudentStore::rollExists(const std::string& rollNo) const {
    return findByRoll(rollNo) != -1;
}

// ---------- Changing ----------

bool StudentStore::add(const Student& student) {
    if (rollExists(student.rollNo)) {
        return false;
    }
    students_.push_back(student);
    return save();
}

bool StudentStore::update(const Student& student) {
    int index = findByRoll(student.rollNo);
    if (index == -1) {
        return false;
    }
    students_[index] = student;
    return save();
}

bool StudentStore::remove(const std::string& rollNo) {
    int index = findByRoll(rollNo);
    if (index == -1) {
        return false;
    }
    students_.erase(students_.begin() + index);
    return save();
}

// ---------- Helpers ----------

int StudentStore::findByRoll(const std::string& rollNo) const {
    for (int i = 0; i < count(); ++i) {
        if (students_[i].rollNo == rollNo) {
            return i;
        }
    }
    return -1;
}

bool StudentStore::save() const {
    std::ofstream file(filePath_, std::ios::trunc);   // overwrite the file
    if (!file.is_open()) {
        return false;
    }
    for (const Student& student : students_) {
        file << toLine(student) << '\n';
    }
    return file.good();
}

std::string StudentStore::toLine(const Student& student) {
    std::ostringstream line;
    line << student.name << ','
         << student.rollNo << ','
         << student.fatherName << ','
         << student.caste << ','
         << student.district << ','
         << std::fixed << std::setprecision(2) << student.gpa;
    return line.str();
}

bool StudentStore::fromLine(const std::string& line, Student& student) {
    // Split the line at every comma.
    std::vector<std::string> parts;
    std::stringstream stream(line);
    std::string part;
    while (std::getline(stream, part, ',')) {
        parts.push_back(part);
    }

    // A valid line has exactly 6 fields.
    if (parts.size() != 6) {
        return false;
    }

    // Remove a stray '\r' if the file was saved with Windows line endings.
    if (!parts[5].empty() && parts[5].back() == '\r') {
        parts[5].pop_back();
    }

    try {
        student.gpa = std::stod(parts[5]);
    } catch (...) {
        return false;   // GPA is not a number
    }

    student.name = parts[0];
    student.rollNo = parts[1];
    student.fatherName = parts[2];
    student.caste = parts[3];
    student.district = parts[4];
    return true;
}
