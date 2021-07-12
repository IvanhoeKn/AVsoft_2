#include <iostream>
#include <limits>
#include <windows.h>
#include "Company_state.h"

#ifdef max
#undef max
#endif

const std::string Menu[] = { "1. Show", "2. Add department", "3. Add worker", "4. Remove department", "5. Remove worker", "6. Change department", "7. Change worker surname", "8. Change worker name", "9. Change worker middlename", "10. Change worker function", "11. Change worker salary", "12. Undo last", "13. Redo last", "0. Quit" };
const int Num = sizeof(Menu) / sizeof(Menu[0]);
std::string Choice = "Make your choice";
std::string Msg = "You are wrong; repeate please!";

int Answer(const std::string alt[], int n);

int main(int argc, char* argv[]) {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "ru_RU.UTF-8");
	if (argc < 2) {
		printf("Not enough arguments!");
		return -1;
	}
	std::string file = std::string(argv[1]);
	CompanyState company_tree(file);
	company_tree.readXML();
	int index;
	while (index = Answer(Menu, Num))
        switch (index) {
        case 0:
            break;
        case 1:
            company_tree.show();
            break;
        case 2:
            company_tree.AddDepartment();
            break;
        case 3:
            company_tree.AddWorker();
            break;
        case 4:
            company_tree.RmDepartment();
            break;
        case 5:
            company_tree.RmWorker();
            break;
        case 6:
            company_tree.NewTitleDepartment();
            break;
        case 7:
            company_tree.NewSurnameWorker();
            break;
        case 8:
            company_tree.NewNameWorker();
            break;
        case 9:
            company_tree.NewPatronymicWorker();
            break;
        case 10:
            company_tree.NewPostWorker();
            break;
        case 11:
            company_tree.NewSalaryWorker();
            break;
        case 12:
            company_tree.undo_last_change();
            break;
        case 13:
            company_tree.redo_last_change();
            break;
        }
	return 0;
}

int Answer(const std::string alt[], int n) {
    int answer = -1;
    std::string prompt = Choice;
    std::cout << std::endl << "What do you want to do:" << std::endl;
    for (int i = 0; i < n; i++)
        std::cout << alt[i].c_str() << std::endl;
    do {
        std::cout << prompt.c_str() << ": -> ";
        prompt = Msg;
        if (!((std::cin >> answer) && (std::cin.good()))) {
            std::cout << "Error when number of choice was entered;" << std::endl << "Repeat, please." << std::endl;
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            answer = -1;
        }
    } while (answer < 0 || answer >= n);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return answer;
}