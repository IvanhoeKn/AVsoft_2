#include "department.h"

department::department(std::string& title_tmp, int amount_wrk_tmp, double average_slr_tmp) {
	title = title_tmp;
	amount_wrk = amount_wrk_tmp;
	average_slr = average_slr_tmp;
}

department::department(const department& department_tmp) {
	title = department_tmp.title;
	amount_wrk = department_tmp.amount_wrk;
	average_slr = department_tmp.average_slr;
}

std::istream& operator >> (std::istream& input, department& department_input) {
	std::cout << "Enter Title: ";
	std::getline(input, department_input.title);
	std::cout << "Entered: \"" << department_input.title << "\"" << std::endl;
	return input;
}

std::ostream& operator << (std::ostream& output, department& department_output) {
	std::cout << "Title: \'" << department_output.title << "\'" << std::endl;
	std::cout << "Amount of workers: " << department_output.amount_wrk << std::endl;
	std::cout << "Average salary: " << department_output.average_slr << std::endl;
	return output;
}

department& department::operator = (const department& department_tmp) {
	title = department_tmp.title;
	amount_wrk = department_tmp.amount_wrk;
	average_slr = department_tmp.average_slr;
	return *this;
}

department& department::operator = (department&& department_tmp) {
	std::string buf = title;
	int amount_wrk_tmp = amount_wrk;
	double average_slr_tmp = average_slr;
	title = department_tmp.title;
	department_tmp.title = buf;
	amount_wrk = department_tmp.amount_wrk;
	department_tmp.amount_wrk = amount_wrk_tmp;
	average_slr = department_tmp.average_slr;
	department_tmp.average_slr = average_slr_tmp;
	return *this;
}

bool operator < (department const& first, department const& second) {
	return first.title < second.title;
}

bool operator > (department const& first, department const& second) {
	return first.title > second.title;
}

bool operator == (department const& first, department const& second) {
	return !(first < second) && !(first > second);
}

bool operator != (department const& first, department const& second) {
	return !(first == second);
}