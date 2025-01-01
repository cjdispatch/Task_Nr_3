#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <string>
#include <chrono>
#include <iomanip>

// Structure to represent a student
struct Student {
    std::string name;
    double grade;

    // Constructor
    Student(std::string name, double grade) : name(std::move(name)), grade(grade) {}
};

// Function to display a container of students
template <typename Container>
void displayStudents(const Container& students, const std::string& title) {
    std::cout << title << " (" << students.size() << " students)\n";
    for (const auto& student : students) {
        std::cout << "  Name: " << student.name << ", Grade: " << student.grade << '\n';
    }
    std::cout << '\n';
}

// Function to measure execution time
template <typename Func>
void measureExecutionTime(const std::string& taskName, Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << taskName << " took " << elapsed.count() << " seconds.\n\n";
}

// Strategy 1: Split into two new containers (failed and passed)
template <typename Container>
void strategy1(const Container& students) {
    Container failed, passed;

    std::partition_copy(students.begin(), students.end(),
                        std::back_inserter(failed), std::back_inserter(passed),
                        [](const Student& s) { return s.grade < 50.0; });

    displayStudents(failed, "Failed Students (Strategy 1)");
    displayStudents(passed, "Passed Students (Strategy 1)");
}

// Strategy 2: Modify the base container (keep only passed in base and move failed to a new container)
template <typename Container>
void strategy2(Container students) {
    Container failed;

    auto it = std::remove_if(students.begin(), students.end(), [&](const Student& s) {
        if (s.grade < 50.0) {
            failed.push_back(s);
            return true;
        }
        return false;
    });
    students.erase(it, students.end());

    displayStudents(failed, "Failed Students (Strategy 2)");
    displayStudents(students, "Passed Students (Strategy 2)");
}

// Function to generate a large dataset
template <typename Container>
Container generateDataset(size_t size) {
    Container students;
    for (size_t i = 0; i < size; ++i) {
        students.emplace_back("Student_" + std::to_string(i), rand() % 101); // Random grade between 0 and 100
    }
    return students;
}

// Function to test and measure performance for different strategies and containers
template <typename Container>
void testPerformance(const std::string& containerName, size_t datasetSize) {
    std::cout << "Testing with container: " << containerName << " (" << datasetSize << " students)\n";

    auto students = generateDataset<Container>(datasetSize);

    displayStudents(students, "Original Students");

    // Apply Strategy 1
    measureExecutionTime("Strategy 1", [&]() { strategy1(students); });

    // Apply Strategy 2
    measureExecutionTime("Strategy 2", [&]() { strategy2(students); });

    std::cout << "========================================\n\n";
}

int main() {
    const size_t datasetSize = 10000; // Adjust for large-scale testing

    // Test with different containers
    testPerformance<std::vector<Student>>("Vector", datasetSize);
    testPerformance<std::list<Student>>("List", datasetSize);
    testPerformance<std::deque<Student>>("Deque", datasetSize);

    return 0;
}
