// student.h
// One student record. Plain data only - no GUI code and no file code here.
#pragma once

#include <string>

struct Student {
    std::string name;
    std::string rollNo;      // unique: used to find a record when updating or deleting
    std::string fatherName;
    std::string caste;
    std::string district;
    double gpa = 0.0;
};
