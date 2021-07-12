#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>
#include <iostream>
#include "rb_tree.h"
#include "department.h"
#include "worker.h"

class Command {
protected: 
    rb_tree<department>* data;
public:
    Command(rb_tree<department>& tmp) : data(&tmp) {}
    virtual ~Command() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

//==============================================================================================
//==============================================================================================

class Department : public Command {
protected:
    node<department>* ptr;
public:
    Department(rb_tree<department>& tmp) : Command(tmp), ptr(nullptr) {}
    ~Department() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
protected:
    bool set_ptr();
    bool create();
    void clear();
};

//==============================================================================================

class DepartmentAdd : public Department {
public:
    DepartmentAdd(rb_tree<department>& tmp) : Department(tmp) {}
    ~DepartmentAdd();
    virtual void execute();
    virtual void undo();
    virtual void redo();
};

class DepartmentTitle : public Department {
private:
    std::string title_buf;
public:
    DepartmentTitle(rb_tree<department>& tmp) : Department(tmp) {}
    ~DepartmentTitle();
    virtual void execute();
    virtual void undo();
    virtual void redo();
};

class DepartmentRm : public Department {
public:
    DepartmentRm(rb_tree<department>& tmp) : Department(tmp) {}
    ~DepartmentRm() {}
    virtual void execute();
    virtual void undo();
    virtual void redo();
};

//==============================================================================================
//==============================================================================================

class Worker : public Command {
protected:
    node<department>* ptr_d;
    node<worker>* ptr_w;
public:
    Worker(rb_tree<department>& tmp) : Command(tmp), ptr_d(nullptr), ptr_w(nullptr) {}
    ~Worker() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
protected:
    bool set_ptr_d();
    bool set_ptr_w();
    worker enter_for_srch();
    bool create();
    void clear();
};

//==============================================================================================

class WorkerAdd : public Worker {
public:
    WorkerAdd(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerAdd();
    virtual void execute();
    virtual void undo();
    virtual void redo();
};

class WorkerRm : public Worker {
public:
    WorkerRm(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerRm() {}
    virtual void execute();
    virtual void undo();
    virtual void redo();
};

class WorkerSurname : public Worker {
private:
    std::string surname_buf;
public:
    WorkerSurname(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerSurname();
    virtual void execute();
    virtual void undo();
    virtual void redo();
};

class WorkerName : public Worker {
private:
    std::string name_buf;
public:
    WorkerName(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerName();
    virtual void execute();
    virtual void undo();
    virtual void redo();
};

class WorkerPatronymic : public Worker {
private:
    std::string patronymic_buf;
public:
    WorkerPatronymic(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerPatronymic();
    virtual void execute();
    virtual void undo();
    virtual void redo();
};

class WorkerPost : public Worker {
private:
    std::string post_buf;
public:
    WorkerPost(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerPost();
    virtual void execute();
    virtual void undo();
    virtual void redo();
};

class WorkerSalary : public Worker {
private:
    int salary_buf;
public:
    WorkerSalary(rb_tree<department>& tmp) : Worker(tmp), salary_buf(0) {}
    ~WorkerSalary() {}
    virtual void execute();
    virtual void undo();
    virtual void redo();
};

#endif