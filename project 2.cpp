#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Stores one course record from the input file
struct Course {
    string number;                 // e.g., "CSCI200"
    string title;                  // e.g., "Data Structures"
    vector<string> prerequisites;  // e.g., {"CSCI101", "MATH201"}
};

// Trim leading/trailing whitespace
static inline string Trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) start++;
    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) end--;
    return s.substr(start, end - start);
}

// Convert a string to uppercase (course numbers are case-insensitive)
static inline string ToUpper(string s) {
    for (char& ch : s) ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
    return s;
}

// Split a line by commas (simple CSV format used by this assignment)
static vector<string> SplitCSVLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream ss(line);
    while (getline(ss, field, ',')) {
        fields.push_back(Trim(field));
    }
    return fields;
}

// Load the CSV file into the data structure (course number -> Course)
bool LoadCourses(const string& filename, unordered_map<string, Course>& courses) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file \"" << filename << "\"." << endl << endl;
        return false;
    }

    courses.clear();

    string line;
    while (getline(file, line)) {
        line = Trim(line);
        if (line.empty()) continue;

        vector<string> fields = SplitCSVLine(line);
        if (fields.size() < 2) continue;

        Course c;
        c.number = ToUpper(fields[0]);
        c.title = fields[1];

        for (size_t i = 2; i < fields.size(); i++) {
            string pre = ToUpper(Trim(fields[i]));
            if (!pre.empty()) c.prerequisites.push_back(pre);
        }

        courses[c.number] = c;
    }

    return true;
}

// Print all courses sorted alphanumerically by course number
void PrintCourseList(const unordered_map<string, Course>& courses) {
    vector<string> keys;
    keys.reserve(courses.size());

    for (const auto& pair : courses) {
        keys.push_back(pair.first);
    }

    sort(keys.begin(), keys.end());

    cout << "Here is a sample schedule:" << endl << endl;
    for (const string& k : keys) {
        const Course& c = courses.at(k);
        cout << c.number << ", " << c.title << endl;
    }
    cout << endl;
}

// Print the title + prerequisites for a single course number
void PrintCourseInfo(const unordered_map<string, Course>& courses, const string& courseNumberInput) {
    string courseNumber = ToUpper(Trim(courseNumberInput));

    auto it = courses.find(courseNumber);
    if (it == courses.end()) {
        cout << "Course not found: " << courseNumber << endl << endl;
        return;
    }

    const Course& c = it->second;
    cout << c.number << ", " << c.title << endl;

    cout << "Prerequisites: ";
    if (c.prerequisites.empty()) {
        cout << "None" << endl << endl;
        return;
    }

    for (size_t i = 0; i < c.prerequisites.size(); i++) {
        cout << c.prerequisites[i];
        if (i + 1 < c.prerequisites.size()) cout << ", ";
    }
    cout << endl << endl;
}

// Display the required menu options
void PrintMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl << endl;
    cout << "What would you like to do? ";
}

int main() {
    unordered_map<string, Course> courses;
    bool loaded = false;

    cout << "Welcome to the course planner." << endl << endl;

    while (true) {
        PrintMenu();

        string choiceLine;
        getline(cin, choiceLine);

        int choice = 0;
        try {
            choice = stoi(choiceLine);
        } catch (...) {
            cout << "Please enter a valid option." << endl << endl;
            continue;
        }

        if (choice == 1) {
            cout << "Enter the file name: ";
            string filename;
            getline(cin, filename);
            loaded = LoadCourses(Trim(filename), courses);

        } else if (choice == 2) {
            if (!loaded) {
                cout << "Please load the data first." << endl << endl;
                continue;
            }
            PrintCourseList(courses);

        } else if (choice == 3) {
            if (!loaded) {
                cout << "Please load the data first." << endl << endl;
                continue;
            }
            cout << "What course do you want to know about? ";
            string courseNumber;
            getline(cin, courseNumber);
            PrintCourseInfo(courses, courseNumber);

        } else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
            break;

        } else {
            cout << choice << " is not a valid option." << endl << endl;
        }
    }

    return 0;
}