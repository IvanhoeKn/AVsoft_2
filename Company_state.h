#ifndef _COMPANY_STATE_H
#define _COMPANY_STATE_H

#include <stack>
#include <string>
#include "Command.h"
#include "rb_tree.h"
#include "pugixml-1.11/src/pugixml.hpp"

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
	~CompanyState();
	bool AddDepartment();
	bool AddWorker();
	bool RmDepartment();
	bool RmWorker();
	bool NewTitleDepartment();
	bool NewNameWorker();
	bool NewSurnameWorker();
	bool NewPatronymicWorker();
	bool NewPostWorker();
	bool NewSalaryWorker();
	void undo_last_change();
	void redo_last_change();
	void show() const;
	void readXML();
	void writeXML();
	void clear();
protected:
	bool load();
	void clear_stack(std::stack<Command*>& tmp);
	std::string UTF8_to_CP1251(std::string const& utf8);
};

#endif