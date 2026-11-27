cmake_minimum_required(VERSION 3.16)
project(StudentRecordSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)   # lets Qt process the Q_OBJECT classes automatically

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(StudentRecordSystem
    main.cpp
    mainwindow.cpp
    mainwindow.h
    student.h
    studentstore.cpp
    studentstore.h
)

target_link_libraries(StudentRecordSystem PRIVATE Qt6::Widgets)
