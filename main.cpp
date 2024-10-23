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

};

int main() {
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
