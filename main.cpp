#include <iostream>
#include <fstream>
#include <vector>
#include "clikit/clikit.hpp"

using namespace std;

struct Password {
    string name;
    string password;
};

class PasswordManager {
private:
    vector<Password> passwords;

public:
    PasswordManager() {
        ifstream file("password.lock");
        if (!file.is_open()) {
            ofstream createFile("password.lock");
            createFile.close();
        } else {
            Load();
        }
    }

    void Add(const Password& password) {
        passwords.push_back(password);
    }

    void Load() {
        passwords.clear();
        ifstream file("password.lock");
        string line;
        print("Loading Passwords", 0.05);
        while (getline(file, line)) {
            int split = line.find(":");
            Password password;
            password.name = line.substr(0, split);
            password.password = line.substr(split + 1);
            passwords.push_back(password);
        }
        print("Passwords Loaded!", 0.05);
    }

    void Save() {
        ofstream file("password.lock");
        for (const Password& password : passwords) {
            file << password.name << ":" << password.password << endl;
        }
    }

    void ListPasswords() {
        int counter = -1;
        Menu<int> passwordsMenu("Passwords List", {}, 0.01,20);
        
        for (const Password& password : passwords) {
            counter++;
            passwordsMenu.Insert({to_string(counter+1)+". "+password.name, defaultColor, counter});
        }

        passwordsMenu.Insert({"<-Back", "\e[0;35m", -1});

        while (true) {
            int index = passwordsMenu.Get();
            if (index != -1) {
                while (true) {
                    const Password& password = passwords[index];
                    cls();

                    string info = "-> Name: " + password.name + "\n-> Password: " + password.password;
                    Menu<int> passwordMenu(info, {
                        {"Edit Name", defaultColor, 0},
                        {"Edit Password", defaultColor, 1},
                        {"Remove", defaultColor, 2},
                        {"<-Back", "\e[0;35m", -1}
                    });

                    int op = passwordMenu.Get();
                    switch (op) {
                        case 0: {
                            Input<string> nameInput("Enter new name:", 0.05);
                            passwords[index].name = nameInput.Read();
                            break;
                        }
                        case 1: {
                            Input<string> passwordInput("Enter new password:", 0.05);
                            passwords[index].password = passwordInput.ReadSecret('*'); // Update password
                            break;
                        }
                        case 2: {
                            passwords.erase(passwords.begin() + index);
                            Save();
                            return;
                        }
                        default:
                            break;
                    }
                    if (op == -1) break;
                }
            } else {
                break;
            }
        }
    }
};

int main() {
    PasswordManager pm;
    Menu<int> menu("Home Menu", {
        {"Add new password", "\e[0;37m", 0},
        {"List of passwords", "\e[0;37m", 1},
        {"Exit", "\e[0;31m", 2}
    });
    
    Menu<int> yesnoMenu("Are you sure?", {
        {"Yes", "\e[0;31m", 0},
        {"No", "\e[0;37m", 1}
    });

    while (true) {
        int op = menu.Get();
        switch (op) {
            case 0: {
                if (yesnoMenu.Get() == 1) break;
Password password;
                Input<string> nameInput("-> Enter name: ", 0.05);
                Input<string> passwordInput("-> Enter password: ", 0.05);
                password.name = nameInput.Read();
                password.password = passwordInput.ReadSecret('*');
                pm.Add(password);
                pm.Save();
                break;
            }
            case 1:
                break;
            case 2: {
                if (yesnoMenu.Get() == 0) exit(0);
                break;
            }
            default:
                break;
        }
    }
}
