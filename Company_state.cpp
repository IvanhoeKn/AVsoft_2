#include "Company_state.h"
#include <vector>
#include <Windows.h>
#include "rb_tree.cpp"
#include "pugixml-1.11/src/pugixml.cpp"

CompanyState::~CompanyState() {
	clear();
}

bool CompanyState::AddDepartment() {
	Command* ptr;
	try {
		ptr = new DepartmentAdd(data);
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
		ptr = nullptr;
		return false;
	}
	ptr->execute();
	history.push(ptr);
	return true;
}

bool CompanyState::AddWorker() {
	Command* ptr;
	try {
		ptr = new WorkerAdd(data);
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
		ptr = nullptr;
		return false;
	}
	ptr->execute();
	history.push(ptr);
	return true;
}

bool CompanyState::RmDepartment() {
	Command* ptr;
	try {
		ptr = new DepartmentRm(data);
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
		ptr = nullptr;
		return false;
	}
	ptr->execute();
	history.push(ptr);
	return true;
}

bool CompanyState::RmWorker() {
	Command* ptr;
	try {
		ptr = new WorkerRm(data);
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
		ptr = nullptr;
		return false;
	}
	ptr->execute();
	history.push(ptr);
	return true;
}

bool CompanyState::NewTitleDepartment() {
	Command* ptr;
	try {
		ptr = new DepartmentTitle(data);
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
		ptr = nullptr;
		return false;
	}
	ptr->execute();
	history.push(ptr);
	return true;
}

bool CompanyState::NewNameWorker() {
	Command* ptr;
	try {
		ptr = new WorkerName(data);
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
		ptr = nullptr;
		return false;
	}
	ptr->execute();
	history.push(ptr);
	return true;
}

bool CompanyState::NewSurnameWorker() {
	Command* ptr;
	try {
		ptr = new WorkerSurname(data);
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
		ptr = nullptr;
		return false;
	}
	ptr->execute();
	history.push(ptr);
	return true;
}

bool CompanyState::NewPatronymicWorker() {
	Command* ptr;
	try {
		ptr = new WorkerPatronymic(data);
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
		ptr = nullptr;
		return false;
	}
	ptr->execute();
	history.push(ptr);
	return true;
}

bool CompanyState::NewPostWorker() {
	Command* ptr;
	try {
		ptr = new WorkerPost(data);
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
		ptr = nullptr;
		return false;
	}
	ptr->execute();
	history.push(ptr);
	return true;
}

bool CompanyState::NewSalaryWorker() {
	Command* ptr;
	try {
		ptr = new WorkerSalary(data);
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
		ptr = nullptr;
		return false;
	}
	ptr->execute();
	history.push(ptr);
	return true;
}

void CompanyState::undo_last_change() {
	if (!history.empty()) {
		Command* ptr = history.top();
		history.pop();
		ptr->undo();
		trash.push(ptr);
	}
	else
		std::cout << "History is empty!" << std::endl;
}

void CompanyState::redo_last_change() {
	if (!trash.empty()) {
		Command* ptr = trash.top();
		trash.pop();
		ptr->redo();
		history.push(ptr);
	}
	else
		std::cout << "Trash is empty!" << std::endl;
}

void CompanyState::show() const {
	for (auto it = data.begin(); it != data.end(); it++) {
		std::cout << "Department------------------------------------------------" << std::endl;
		std::cout << it.get()->key;
		std::cout << "==========================================================" << std::endl;
		rb_tree<worker> data_worker = it.get()->key.employment;
		if (data_worker.isEmpty())
			for (auto it_w = data_worker.begin(); it_w != data_worker.end(); it_w++) {
				std::cout << it_w.get()->key;
				std::cout << "-=-=-=-=-=-=-=-=-" << std::endl;
			}
		else
			std::cout << "There is no workers!" << std::endl;
		std::cout << "End-------------------------------------------------------" << std::endl;
	}
}

void CompanyState::readXML() {
	if (load()) {
		pugi::xml_node dep_node = docXML.child("departments").child("department");
		for (dep_node; dep_node; dep_node = dep_node.next_sibling("department")) {
			node<department>* ptr_d;
			try {
				ptr_d = new node<department>;
			}
			catch (const std::exception& er) {
				std::cout << er.what() << std::endl;
				ptr_d = nullptr;
			}
			if (ptr_d) {
				ptr_d->key.title = dep_node.attribute("name").value();
				ptr_d->key.title = UTF8_to_CP1251(ptr_d->key.title);
				pugi::xml_node wrk_node = dep_node.child("employments").child("employment");
				for (wrk_node; wrk_node; wrk_node = wrk_node.next_sibling("employment")) {
					node<worker>* ptr_w;
					try {
						ptr_w = new node<worker>;
					}
					catch (const std::exception& er) {
						std::cout << er.what() << std::endl;
						ptr_w = nullptr;
					}
					if (ptr_w) {
						ptr_w->key.name = wrk_node.child_value("name");
						ptr_w->key.name = UTF8_to_CP1251(ptr_w->key.name);
						ptr_w->key.surname = wrk_node.child_value("surname");
						ptr_w->key.surname = UTF8_to_CP1251(ptr_w->key.surname);
						ptr_w->key.patronymic = wrk_node.child_value("middleName");
						ptr_w->key.patronymic = UTF8_to_CP1251(ptr_w->key.patronymic);
						ptr_w->key.post = wrk_node.child_value("function");
						ptr_w->key.post = UTF8_to_CP1251(ptr_w->key.post);
						ptr_w->key.salary = std::stoi(wrk_node.child_value("salary"));
						ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk + ptr_w->key.salary) / ++ptr_d->key.amount_wrk;
						ptr_d->key.employment.insert(ptr_w);
					}
				}
				data.insert(ptr_d);
			}
		}
	}
}

void CompanyState::writeXML() {
	pugi::xml_node node_tmp;
	docXML.reset();
	pugi::xml_node node_departments = docXML.append_child("departments");
	if (data.isEmpty())
		for (auto it = data.begin(); it != data.end(); it++) {
			pugi::xml_node node_d = node_departments.append_child("department");
			node_d.append_attribute("name") = it.get()->key.title.c_str();
			pugi::xml_node node_employments = node_d.append_child("employments");
			rb_tree<worker> data_worker = it.get()->key.employment;
			if (data_worker.isEmpty())
				for (auto it_w = data_worker.begin(); it_w != data_worker.end(); it_w++) {
					pugi::xml_node node_w = node_employments.append_child("employment");
					node_w.append_child("surname").append_child(pugi::node_pcdata).set_value(it_w.get()->key.surname.c_str());
					node_w.append_child("name").append_child(pugi::node_pcdata).set_value(it_w.get()->key.name.c_str());
					node_w.append_child("middleName").append_child(pugi::node_pcdata).set_value(it_w.get()->key.patronymic.c_str());
					node_w.append_child("function").append_child(pugi::node_pcdata).set_value(it_w.get()->key.post.c_str());
					node_w.append_child("salary").append_child(pugi::node_pcdata).set_value(std::to_string(it_w.get()->key.salary).c_str());
				}
		}
	std::string new_file = "new_" + filename;
	docXML.save_file(new_file.c_str(), "\t", pugi::format_default, pugi::encoding_utf8);
}

void CompanyState::clear() {
	filename.clear();
	docXML.reset();
	clear_stack(history);
	clear_stack(trash);
	data.clear();
}

bool CompanyState::load() {
	pugi::xml_parse_result result = docXML.load_file(filename.c_str(), pugi::parse_default, pugi::encoding_utf8);
	if (result) {
		std::cout << "XML [" << filename << "] parsed without errors\n";
		return true;
	}
	else {
		std::cout << "XML [" << filename << "] parsed with errors\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..." << (filename.c_str() + result.offset) << "]\n\n";
		return false;
	}
}

void CompanyState::clear_stack(std::stack<Command*>& tmp) {
	while (!tmp.empty()) {
		Command* ptr = tmp.top();
		tmp.pop();
		delete ptr;
	}
}

std::string CompanyState::UTF8_to_CP1251(std::string const& utf8) {
	if (!utf8.empty()) {
		int wchlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
		if (wchlen > 0 && wchlen != 0xFFFD) {
			std::vector<wchar_t> wbuf(wchlen);
			MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), &wbuf[0], wchlen);
			std::vector<char> buf(wchlen);
			WideCharToMultiByte(1251, 0, &wbuf[0], wchlen, &buf[0], wchlen, 0, 0);
			return std::string(&buf[0], wchlen);
		}
	}
	return std::string();
}