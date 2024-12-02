#include <bits/stdc++.h>
using namespace std;

struct Assignment {
    int teacher;  
    int slot;     
    int classroom;
};

int NUM_SUBJECTS;
int NUM_TEACHERS;
int NUM_SLOTS;
int NUM_CLASSROOMS;

vector<int> subject_student_count;  
vector<int> room_capacity;      
unordered_map<int, set<int>> teacher_preferences;

bool is_valid_assignment(int subject, const Assignment &current, const vector<Assignment> &assignments) {
    if (subject_student_count[subject] > room_capacity[current.classroom]) {
        return false;
    }


    if (!teacher_preferences[current.teacher].empty() &&
        teacher_preferences[current.teacher].find(subject) == teacher_preferences[current.teacher].end()) {
        return false;
    }

    for (int i = 0; i < assignments.size(); i++) {
        if (i != subject) {
            const Assignment &other = assignments[i];
            if (other.slot == current.slot) {
                if (other.classroom == current.classroom) {
                    return false;
                }
                if (other.teacher == current.teacher) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool backtrack(int subject, vector<Assignment> &assignments) {
    if (subject == NUM_SUBJECTS) {
        return true;
    }

    vector<Assignment> domain;
    for (int teacher = 0; teacher < NUM_TEACHERS; teacher++) {
        for (int slot = 0; slot < NUM_SLOTS; slot++) {
            for (int classroom = 0; classroom < NUM_CLASSROOMS; classroom++) {
                domain.push_back({teacher, slot, classroom});
            }
        }
    }


    sort(domain.begin(), domain.end(), [&](const Assignment &a, const Assignment &b) {
        int conflicts_a = 0, conflicts_b = 0;
        for (const auto &other : assignments) {
            if (a.slot == other.slot) {
                if (a.classroom == other.classroom) conflicts_a++;
                if (a.teacher == other.teacher) conflicts_a++;
            }
            if (b.slot == other.slot) {
                if (b.classroom == other.classroom) conflicts_b++;
                if (b.teacher == other.teacher) conflicts_b++;
            }
        }
        return conflicts_a < conflicts_b;
    });

    for (const auto &current : domain) {
        if (is_valid_assignment(subject, current, assignments)) {
            assignments[subject] = current; 
            if (backtrack(subject + 1, assignments)) {
                return true; 
            }
        
            assignments[subject] = {-1, -1, -1};
        }
    }

    return false;
}

int main() {
    cout << "Enter the number of subjects: ";
    cin >> NUM_SUBJECTS;

    cout << "Enter the number of teachers: ";
    cin >> NUM_TEACHERS;

    cout << "Enter the number of slots: ";
    cin >> NUM_SLOTS;

    cout << "Enter the number of classrooms: ";
    cin >> NUM_CLASSROOMS;

    subject_student_count.resize(NUM_SUBJECTS);
    cout << "Enter the number of students for each subject:\n";
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        cout << "Subject " << i + 1 << ": ";
        cin >> subject_student_count[i];
    }

    room_capacity.resize(NUM_CLASSROOMS);
    cout << "Enter the capacity of each classroom:\n";
    for (int i = 0; i < NUM_CLASSROOMS; ++i) {
        cout << "Classroom " << i + 1 << ": ";
        cin >> room_capacity[i];
    }

    for (int i = 0; i < NUM_TEACHERS; ++i) {
        int num_preferences;
        cout << "Enter the number of preferred subjects for Teacher " << i + 1 << ": ";
        cin >> num_preferences;
        cout << "Enter the preferred subjects (1-based index): ";
        for (int j = 0; j < num_preferences; ++j) {
            int subject;
            cin >> subject;
            teacher_preferences[i].insert(subject - 1); 
        }
    }

    
    vector<Assignment> assignments(NUM_SUBJECTS, {-1, -1, -1}); 

    if (backtrack(0, assignments)) {
        cout << "Optimal Schedule:\n";
        for (int i = 0; i < NUM_SUBJECTS; ++i) {
            cout << "Subject S" << i + 1 << ": "
                 << "Teacher T" << assignments[i].teacher + 1 << ", "
                 << "Slot Slot" << assignments[i].slot + 1 << ", "
                 << "Classroom C" << assignments[i].classroom + 1 << "\n";
        }
    } else {
        cout << "No solution found.\n";
    }

    return 0;
}
