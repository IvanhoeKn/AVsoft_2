#include <iostream>
#include "Company_state.h"
#include <windows.h>

int main(int argc, char* argv[]) {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	std::string file = std::string(argv[1]);
	CompanyState wk;
	wk.setFile(file);
	wk.readXML();
	return 0;
}