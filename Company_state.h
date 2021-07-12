#ifndef _COMPANY_STATE_H
#define _COMPANY_STATE_H

#include <stack>
#include <string>
#include <iostream>
#include "Command.h"
#include "rb_tree.cpp"
#include "pugixml-1.11/src/pugixml.cpp"

class CompanyState {
private:
	rb_tree<department> data;
	std::stack<Command*> history;
	std::stack<Command*> trash;
	std::string filename;
	pugi::xml_document docXML;
public:
	CompanyState() : history(), data(), trash(), filename(""), docXML() {}
	CompanyState(std::string& tmp) : history(), trash(), filename(tmp), docXML() {}
	~CompanyState() {
		clear();
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
		ptr->execute();
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
		ptr->execute();
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
		ptr->execute();
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
		ptr->execute();
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
		ptr->execute();
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
		ptr->execute();
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
		ptr->execute();
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
		ptr->execute();
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
		ptr->execute();
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
		ptr->execute();
		history.push(ptr);
		return true;
	}
	void undo_last_change() {
		if (!history.empty()) {
			Command* ptr = history.top();
			history.pop();
			ptr->undo();
			trash.push(ptr);
		}
		else
			std::cout << "History is empty!" << std::endl; 
	}
	void redo_last_change() {
		if (!trash.empty()) {
			Command* ptr = trash.top();
			trash.pop();
			ptr->redo();
			history.push(ptr);
		}
		else
			std::cout << "Trash is empty!" << std::endl;
	}
	void show() const {
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
	void readXML() {
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
							ptr_w->key.surname = wrk_node.child_value("surname");
							ptr_w->key.patronymic = wrk_node.child_value("middleName");
							ptr_w->key.post = wrk_node.child_value("function");
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
	void writeXML() {
		pugi::xml_node node_tmp;
		docXML.reset();
		pugi::xml_node node_departments = docXML.append_child("departments");
		for (auto it = data.begin(); it != data.end(); it++) {
			pugi::xml_node node_d = node_departments.append_child("department");
			node_d.append_attribute("name") = it.get()->key.title.c_str();
			pugi::xml_node node_employments = node_d.append_child("employments");
			rb_tree<worker> data_worker = it.get()->key.employment;
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
	void clear() {
		filename.clear();
		docXML.reset();
		data.clear();
		clear_stack(history);
		clear_stack(trash);
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
	void clear_stack(std::stack<Command*>& tmp) {
		while (!tmp.empty()) {
			Command* ptr = tmp.top();
			tmp.pop();
			delete ptr;
		}
	}
};

#endif