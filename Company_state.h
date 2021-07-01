#ifndef _COMPANY_STATE_H
#define _COMPANY_STATE_H

#include <stack>
#include "Command.h"
#include <string>
#include <iostream>
#include "rb_tree.h"
#include "department.h"
#include "worker.h"
#include "pugixml-1.11/src/pugixml.cpp"

class CompanyState {
private:
	rb_tree<department> data;
	std::stack<Command*> history;
	std::stack<Command*> trash;
	std::string filename;
	pugi::xml_document docXML;
public:
	CompanyState() : history(), trash(), filename(""), docXML() {}
	CompanyState(std::string& tmp) : history(), trash(), filename(tmp), docXML() {}
	~CompanyState() {
		filename.clear();
		docXML.reset();
	}
	void setFile(std::string tmp) {
		filename = tmp;
	}
	bool AddDepartment() {
		Command* ptr;
		try {
			ptr = new DepartmentAdd(data);
		}
		catch (const std::exception& er) {
			std::cout << er.what() << std::endl;
			ptr = nullptr;
			return false;
		}
		history.push(ptr);
		return true;
	}
	bool AddWorker() {
		Command* ptr;
		try {
			ptr = new WorkerAdd(data);
		}
		catch (const std::exception& er) {
			std::cout << er.what() << std::endl;
			ptr = nullptr;
			return false;
		}
		history.push(ptr);
		return true;
	}
	bool RmDepartment() {
		Command* ptr;
		try {
			ptr = new DepartmentRm(data);
		}
		catch (const std::exception& er) {
			std::cout << er.what() << std::endl;
			ptr = nullptr;
			return false;
		}
		history.push(ptr);
		return true;
	}
	bool RmWorker() {
		Command* ptr;
		try {
			ptr = new WorkerRm(data);
		}
		catch (const std::exception& er) {
			std::cout << er.what() << std::endl;
			ptr = nullptr;
			return false;
		}
		history.push(ptr);
		return true;
	}
	bool NewTitleDepartment() {
		Command* ptr;
		try {
			ptr = new DepartmentTitle(data);
		}
		catch (const std::exception& er) {
			std::cout << er.what() << std::endl;
			ptr = nullptr;
			return false;
		}
		history.push(ptr);
		return true;
	}
	bool NewNameWorker() {
		Command* ptr;
		try {
			ptr = new WorkerName(data);
		}
		catch (const std::exception& er) {
			std::cout << er.what() << std::endl;
			ptr = nullptr;
			return false;
		}
		history.push(ptr);
		return true;
	}
	bool NewSurnameWorker() {
		Command* ptr;
		try {
			ptr = new WorkerSurname(data);
		}
		catch (const std::exception& er) {
			std::cout << er.what() << std::endl;
			ptr = nullptr;
			return false;
		}
		history.push(ptr);
		return true;
	}
	bool NewPatronymicWorker() {
		Command* ptr;
		try {
			ptr = new WorkerPatronymic(data);
		}
		catch (const std::exception& er) {
			std::cout << er.what() << std::endl;
			ptr = nullptr;
			return false;
		}
		history.push(ptr);
		return true;
	}
	bool NewPostWorker() {
		Command* ptr;
		try {
			ptr = new WorkerPost(data);
		}
		catch (const std::exception& er) {
			std::cout << er.what() << std::endl;
			ptr = nullptr;
			return false;
		}
		history.push(ptr);
		return true;
	}
	bool NewSalaryWorker() {
		Command* ptr;
		try {
			ptr = new WorkerSalary(data);
		}
		catch (const std::exception& er) {
			std::cout << er.what() << std::endl;
			ptr = nullptr;
			return false;
		}
		history.push(ptr);
		return true;
	}
	void show() const {
		//итератор нужен 
	}
	void readXML() {
		if (load()) {
			pugi::xml_node dep_node = docXML.child("departments").child("department");
			for (dep_node; dep_node; dep_node = dep_node.next_sibling("department")) {
				std::cout << "department " << dep_node.attribute("Filename").value() << std::endl;
				node<department>* ptr;
				try {
					ptr = new node<department>;
				}
				catch (const std::exception& er) {
					std::cout << er.what() << std::endl;
					ptr = nullptr;
				}
				//ptr->key.title = (dep_node.attribute("name").value());
				//data.insert(ptr);
				pugi::xml_node wrk_node = dep_node.child("employments").child("employment");
				for (wrk_node; wrk_node; wrk_node = wrk_node.next_sibling("employment")) {
					std::cout << "     worker " << wrk_node.child_value("name") << std::endl;
				}
			}
		}
	}
	void writeXML() {

	}
protected:
	bool load() {
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
};

#endif