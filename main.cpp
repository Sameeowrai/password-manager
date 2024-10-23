#include <iostream>
#include "clikit/clikit.hpp"

using namespace std;


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
