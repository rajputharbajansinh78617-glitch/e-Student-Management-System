// studentstore.h
// Keeps all student records in memory and saves them to a text file.
// Each line of the file is one student, fields separated by commas:
//   name,rollNo,fatherName,caste,district,gpa
#pragma once

#include <string>
#include <vector>
#include "student.h"

class StudentStore {
public:
    explicit StudentStore(const std::string& filePath);

    void load();                                // read the file into memory
    int count() const;                          // number of records
    const Student& at(int index) const;         // record at position 0..count()-1
    bool rollExists(const std::string& rollNo) const;

    bool add(const Student& student);           // false if roll number already used
    bool update(const Student& student);        // finds record by roll number
    bool remove(const std::string& rollNo);     // finds record by roll number

private:
    int findByRoll(const std::string& rollNo) const;   // -1 if not found
    bool save() const;                                 // rewrite the whole file

    static std::string toLine(const Student& student);
    static bool fromLine(const std::string& line, Student& student);

    std::string filePath_;
    std::vector<Student> students_;
};
