#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm> 
#include <sstream>
using namespace std;


void filetovector(const string& filename, vector<string>& words) {
    ifstream file(filename);
    string word;
    while (file >> word) {
        words.push_back(word);
    }
    file.close();
};

vector<string> split_into_words(const string& sentence) {
    istringstream iss(sentence);
    vector<string> words;
    string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}


struct student {
    int id;
    string name;
    string surname;
    string patronymic;
    bool working;
    bool scholarship;
    int arr[9];
    double average;

    student(int _id, const string& _surname, const string& _name, const string& _patronymic, const bool _working, const bool _scholarship, const int _arr[9])
        : id(_id), surname(_surname), name(_name), patronymic(_patronymic), working(_working), scholarship(_scholarship) {
        double mid = 0;
        for (int i = 0; i < 9; ++i) {
            arr[i] = _arr[i];
            mid = mid + _arr[i];
        }
        mid = mid / 9.0;
        average = mid;
    }
};

class student_data {
private:
    vector<student> students;
    int id_next;

public:
    student_data() : id_next(1) {};

    void add_st(const string& surname, const string& name, const string& patronymic, const bool working, const bool scholarship, const int arr[9]) {
        students.push_back(student(id_next++, surname, name, patronymic, working, scholarship, arr));
    }

    void delete_st_by_id(int id) {
        for (auto it = students.begin(); it != students.end(); it++) {
            if (it->id == id) {
                students.erase(it);
                break;
            }
        }
        for (auto it = students.begin(); it != students.end(); it++) {
            if (it->id > id) {
                it->id--;
            }
        }
    }

    void delete_st_by_par(const string& surname, const string& name, const string& patronymic, const bool working, const bool scholarship, const int arr[9]) {
        int n = 0;
        for (auto it = students.begin(); it != students.end(); it++) {
            if ((it->name == name) && (it->surname == surname) && (it->patronymic == patronymic) && (it->working == working) && (it->scholarship == scholarship)){
                for (int i = 0; i < 9; i++) {
                    if (it->arr[i] == arr[i]) {
                        n++;
                    }
                }
                if (n == 9) {
                    delete_st_by_id(it->id);
                    n = 0;
                }
            }
        }
    }
    
    void edit_st(int id, const string& surname, const string& name, const string& patronymic, const bool working, const bool scholarship, const int arr[9]) {
        for (auto& student : students) {
            if (student.id == id) {
                student.surname = surname;
                student.name = name;
                student.patronymic = patronymic;
                student.working = working;
                student.scholarship = scholarship;
                student.average = 0;
                for (int i = 0; i < 9; i++) {
                    student.arr[i] = arr[i];
                    student.average = student.average + arr[i];
                }
                student.average = student.average / 9.0;
            }
        }
    }

    void generate(int n) {
        vector<string> name_male;

        vector<string> surname_male;

        vector<string> patronymic_male;

        vector<string> name_fem;

        vector<string> surname_fem;

        vector<string> patronymic_fem;

        filetovector("name_m.txt", name_male);
        filetovector("surname_m.txt", surname_male);
        filetovector("patronymic_m.txt", patronymic_male);
        filetovector("name_f.txt", name_fem);
        filetovector("surname_f.txt", surname_fem);
        filetovector("patronymic_f.txt", patronymic_fem);
        bool w, s;
        for (int i = 0; i < n; i++) {
            const int arr[] = { rand() % 3 + 3, rand() % 3 + 3, rand() % 3 + 3, rand() % 3 + 3, rand() % 3 + 3, rand() % 3 + 3, rand() % 3 + 3, rand() % 3 + 3, rand() % 3 + 3, };
            if (rand() % 2 == 0) {
                w = rand() % 2;
                s = rand() % 2;
                students.push_back(student(id_next++, surname_male[rand() % surname_male.size()], name_male[rand() % name_male.size()],
                    patronymic_male[rand() % patronymic_male.size()], w, s, arr));
            }
            else {
                w = rand() % 2;
                s = rand() % 2;
                students.push_back(student(id_next++, surname_fem[rand() % surname_fem.size()], name_fem[rand() % name_fem.size()],
                    patronymic_fem[rand() % patronymic_fem.size()], w, s, arr));
            }
        }
    }
    
    void print_base() {
        for (auto& student : students) {
            cout << student.surname << " " << student.name << " " << student.patronymic << " ";
            if (student.working == 1) {
                cout << "Working ";
            }
            else {
                cout << "Not working ";
            }
            if (student.scholarship == 1) {
                cout << "Receiving a scholarship ";
            }
            else {
                cout << "Not receiving a scholarship ";
            }
            cout << "Marks: ";
            for (int i = 0; i < 9; i++) {
                cout << student.arr[i];
            }
            cout << " Average: " << student.average;
            cout << endl;
        }
    }

    void print_base_file(ofstream& fout) {
        for (auto& student : students) {
            fout << student.surname << " " << student.name << " " << student.patronymic << " ";
            if (student.working == 1) {
                fout << "Working ";
            }
            else {
                fout << "Not working ";
            }
            if (student.scholarship == 1) {
                fout << "Receiving a scholarship ";
            }
            else {
                fout << "Not receiving a scholarship ";
            }
            fout << "Marks: ";
            for (int i = 0; i < 9; i++) {
                fout << student.arr[i];
            }
            fout << " Average: " << student.average;
            fout << endl;
        }
    }

    void sort_by_avg() {
        std::sort(students.begin(), students.end(),
            [](const student& x, const student& y) {
                
                    return x.average > y.average;
                
            });
    }

    void sort_by_name() {
        std::sort(students.begin(), students.end(),
            [](const student& x, const student& y) {
                if (x.surname != y.surname) {
                    return x.surname < y.surname;
                }
                else {
                    if (x.name != y.name) {
                        return x.name < y.name;
                    }
                    else {
                        return x.patronymic < y.patronymic;
                    }
                }
            });
    }

    void sort_by_mark() {
        std::sort(students.begin(), students.end(),
            [](const student& x, const student& y) {
                for (int i = 0; i < 9; i++) {
                    if (x.arr[i] == y.arr[i]) {
                        i++;
                    }
                    else { return x.arr[i] > y.arr[i]; }
                }
         });
    }

    vector<int> sample(const string& surname, const string& name, const string& patronymic, const bool w, const bool working, const bool s, const bool scholarship, const bool a, const int arr[9]) {
        vector<int>idx;
        int n = 0;
        int j = 0;
        for (auto& student : students) {
            n = 0;
            if ((student.name == name || name == "All") && (student.surname == surname || surname == "All") && (student.patronymic == patronymic || patronymic == "All") && (w == 0 || (w == 1 && student.working == working)) && (s == 0 || (s == 1 && student.scholarship == scholarship))) {
                if (a == 0) {
                    idx.push_back(j);
                }
                else {
                    for (int i = 0; i < 9; i++) {
                        if (student.arr[i] == arr[i]) {
                            n++;
                        }
                    }
                    if (n == 9) {
                        idx.push_back(j);
                    }
                }
            }
            j++;
        }
        return idx;
    }

        void print_by_sample_file(vector<int> idx, ofstream&fout){
            for (int i = 0; i<idx.size(); i++) {
            fout << students[idx[i]].surname << " " << students[idx[i]].name << " " << students[idx[i]].patronymic << " ";

            if (students[idx[i]].working == 1) {
                fout << "Working ";
            }
            else {
                fout << "Not working ";
            }
            if (students[idx[i]].scholarship == 1) {
                fout << "Receiving a scholarship ";
            }
            else {
                fout << "Not receiving a scholarship ";
            }
            fout << "Marks: ";
            for (int k = 0; k < 9; k++) {
                fout << students[idx[i]].arr[k];
            }
            fout << " Average: " << students[idx[i]].average;
            fout << endl;
        }
    }
        void print_by_sample_console(vector<int> idx) {
            for (int i = 0; i < idx.size(); i++) {
                cout << students[idx[i]].surname << " " << students[idx[i]].name << " " << students[idx[i]].patronymic << " ";

                if (students[idx[i]].working == 1) {
                    cout << "Working ";
                }
                else {
                    cout << "Not working ";
                }
                if (students[idx[i]].scholarship == 1) {
                    cout << "Receiving a scholarship ";
                }
                else {
                    cout << "Not receiving a scholarship ";
                }
                cout << "Marks: ";
                for (int k = 0; k < 9; k++) {
                    cout << students[idx[i]].arr[k];
                }
                cout << " Average: " << students[idx[i]].average;
                cout << endl;
            }
        }
        vector<int> search_by_name(const string name){
        const int a[9] = { 5,5,5,5,5,5,5,5,5 };
        vector<int> idx;
        idx = sample("All", name, "All", 0, 0, 0, 0, 0, a);
        return idx;
        
    }

        vector<int>  search_by_surname(const string surname) {
        const int a[9] = {5,5,5,5,5,5,5,5,5};
        vector<int> idx;
        idx = sample(surname, "All", "All", 0, 0, 0, 0, 0, a);
        return idx;
    }

        vector<int>  search_by_patronymic(const string patronymic) {
        const int a[9] = { 5,5,5,5,5,5,5,5,5 };
        vector<int> idx;
        idx = sample("All", "All", patronymic, 0, 0, 0, 0, 0, a);
        return idx;
    }

        vector<int>  search_by_work(const bool work) {
        const int a[9] = { 5,5,5,5,5,5,5,5,5 };
        vector<int> idx;
        idx = sample("All", "All", "All", 1, work, 0, 0, 0, a);
        return idx;
    }

        vector<int>  search_by_scholarship(const bool scholarship) {
        const int a[9] = { 5,5,5,5,5,5,5,5,5 };
        vector<int> idx;
        idx = sample("All", "All", "All", 0, 0, 1, scholarship, 0, a);
        return idx;
    }


        vector<int>  search_by_marks(const int arr[9]) {
        int n = 0;
        vector<int> idx;
        idx = sample("All", "All", "All", 0, 0, 0, 0, 1, arr);
        return idx;
    }

        vector<int> search_by_name_and_surname(const string surname, const string name) {
        const int a[9] = { 5,5,5,5,5,5,5,5,5 };
        vector<int> idx;
        idx = sample(surname, name, "All", 0, 0, 0, 0, 0, a);
        return idx;
    }


        vector<int> search_by_name_and_patronymic(const string name, const string patronymic) {
        const int a[9] = { 5,5,5,5,5,5,5,5,5 };
        vector<int> idx;
        idx = sample("All", name, patronymic, 0, 0, 0, 0, 0, a);
        return idx;
    }

          vector<int>  search_by_surname_and_patronymic(const string surname, const string patronymic) {
        const int a[9] = { 5,5,5,5,5,5,5,5,5 };
        vector<int> idx;
        idx = sample(surname, "All", patronymic, 0, 0, 0, 0, 0, a);
        return idx;
    }

          vector<int> search_by_surname_name_patronymic(const string& surname, const string& name, const string& patronymic) {
        const int a[9] = { 5,5,5,5,5,5,5,5,5 };
        vector<int> idx;
        idx = sample(surname, name, patronymic, 0, 0, 0, 0, 0, a);
        return idx;
    }

          vector<int> search_by_all_factors(const string& surname, const string& name, const string& patronymic, const bool working, const bool scholarship, const int arr[9]) {
              vector<int> idx;
              idx = sample(surname, name, patronymic, 1, working, 1, scholarship, 1, arr);
              return idx;
    }


          void request(string my_sentence, ofstream& fout1) {
              setlocale(LC_ALL, "Russian");
              bool search;
              vector<int> idx;
              vector<string> result = split_into_words(my_sentence);
              
              if ((result[0] == "search") || (result[0] == "generate") || (result[0] == "sort") || (result[0] == "add") || (result[0] == "edit") || (result[0] == "delete") || (result[0] == "print")) {
                  if (result[0] == "delete") {
                      
                      if (result[1] == "id" || result[1] == "parameters") {
                          if (result[1] == "id") {
                              delete_st_by_id(stoi(result[2]));
                          }
                          else {
                              int arr[9] = { stoi(result[6]), stoi(result[7]), stoi(result[8]), stoi(result[9]), stoi(result[10]), stoi(result[11]), stoi(result[12]), stoi(result[13]), stoi(result[14]) };
                              delete_st_by_par(result[2], result[3], result[4], stoi(result[5]), stoi(result[5]), arr);
                          }
                          return;
                      }
                      else {
                              cout << "Wrong request";
                              return;
                          }

                      }
                      if (result[0] == "edit") {
                          
                          auto it = students.begin();
                          while (it->id != stoi(result[1])) {
                              it++;
                          }
                          edit_st(stoi(result[1]), result[2], result[3], result[4], stoi(result[5]), stoi(result[5]), it->arr);
                          return;

                      }
                      if (result[0] == "add") {
                          
                          int arr[9] = { stoi(result[6]), stoi(result[7]), stoi(result[8]), stoi(result[9]), stoi(result[10]), stoi(result[11]), stoi(result[12]), stoi(result[13]), stoi(result[14]) };
                          add_st(result[1], result[2], result[3], stoi(result[4]), stoi(result[5]), arr);
                          return;
                      }
                      if (result[0] == "generate") {
                          
                          generate(stoi(result[1]));
                          return;

                      }
                      if (result[0] == "print") {
                          
                          if (result[1] == "console") {
                              print_base();
                          }
                          if (result[1] == "file") {
                              
                              print_base_file(fout1);
                              
                          }
                          return;
                      }
                      if (result[0] == "sort") {
                          
                          if ((result[1] == "avg") || (result[1] == "name") || (result[1] == "marks")) {
                              if (result[1] == "avg") {
                                  sort_by_avg();
                              }
                              if (result[1] == "name") {
                                  sort_by_name();
                              }
                              if (result[1] == "marks") {
                                  sort_by_mark();
                              }
                          }
                          else {
                              cout << "Wrong request";
                          }
                          return;
                      }

                      if (result[0] == "search") {
                          
                          int n = 1;
                          search = 1;
                          string name = "All", surname = "All", patronymic = "All";
                          bool work=0, scholarship=0, w=0, s=0, a=0;
                          int arr[9] = { 5,5,5,5,5,5,5,5,5 };
                          while (n < result.size() - 1) {
                              if (((result[n] == "name") || (result[n] == "surname") || (result[n] == "patronymic") || (result[n] == "work") || (result[n] == "scholarship") || (result[n] == "marks"))) {
                                  if (result[n] == "surname") {
                                      n++;
                                      surname = result[n];
                                     
                                      
                                  }
                                  if (result[n] == "name") {
                                      n++;
                                      name = result[n];
                                      
                                  }
                                  if (result[n] == "patronymic") {
                                      n++;
                                      patronymic = result[n];
                                      
                                  }
                                  if (result[n] == "work") {
                                      n++;
                                      work = stoi(result[n]);
                                      
                                      w = 1;
                                  }
                                  if (result[n] == "scholarship") {
                                      n++;
                                      scholarship = stoi(result[n]);
                                      
                                      s = 1;
                                  }
                                  if (result[n] == "marks") {
                                      for (int i = 0; i < 9; i++) {
                                          n++;
                                          arr[i] = stoi(result[n]);
                                      }
                                      
                                      a = 1;
                                  }
                              }
                              else {
                                  n++;
                                     
                                  
                              }
                          }
                          
                          idx = sample(surname, name, patronymic, w, work, s, scholarship, a, arr);
                         
              
                          print_by_sample_file(idx, fout1);
                      }
                  }
                  else {
                      cout << "Unknown Operation" <<endl;
                  }
              }
              
              void work(ifstream& fin, ofstream& fout1) {
                  string my_sentence;
                  while (getline(fin, my_sentence, ';')) {

                      request(my_sentence, fout1);
                  }

              }
};

