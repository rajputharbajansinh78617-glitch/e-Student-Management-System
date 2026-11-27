# Student Record Management System (C++ / Qt 6)

SW112 Programming Fundamentals - Open Ended Lab.

## How to build and run (Qt Creator)
1. Install Qt 6 (6.5 LTS or newer) with **Qt Creator** and a **MinGW or MSVC** kit.
2. Qt Creator: File > Open File or Project > select `CMakeLists.txt`.
3. Choose a Qt 6 kit, click **Configure Project**, then press **Run** (Ctrl+R).

The data is saved in `students.txt` inside the build folder (next to the .exe).
To start with example data, copy `sample_data/students.txt` there.

## How it works
| Mode | Add | Update | Delete | New | Next / Previous |
|------|-----|--------|--------|-----|-----------------|
| Entry (blank form, at start) | on | off | off | off | on if records exist |
| Browse (a saved record shown) | off | on | on | on | on |

## Files
| File | Job |
|------|-----|
| `student.h` | The `Student` data (6 fields) |
| `studentstore.h/.cpp` | Loads and saves the comma-separated file; add / update / remove by roll number |
| `mainwindow.h/.cpp` | The form, buttons, validation and mode logic |
| `main.cpp` | Starts the program |
| `tests/test_store.cpp` | Tests for the file handling (no GUI needed) |

## File format
One student per line: `name,rollNo,fatherName,caste,district,gpa`

## Input rules
- No empty fields.
- Name, Father's Name, Caste, District: letters and spaces only.
- Roll Number: letters, digits and `-`; must be unique; locked while editing.
- GPA: number from 0.0 to 4.0.

## Run the file-handling tests (optional)
    g++ -std=c++17 -I. tests/test_store.cpp studentstore.cpp -o test_store
    ./test_store
