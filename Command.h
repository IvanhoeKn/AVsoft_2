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
    bool set_ptr() {
        if (!data->isEmpty()) {
            std::string prompt = "Enter Department:\n";
            department tmp;
            do {
                std::cout << prompt;
                std::cin >> tmp;
                prompt = "Not found! Try again!\n";
            } while (!(ptr = data->iterative_tree_search(tmp)));
            std::cout << "Found!" << std::endl;
            return true;
        }
        else {
            ptr = nullptr;
            std::cout << "Department tree is empty!" << std::endl;
            return false;
        }
    }
    bool create() {
        try {
            ptr = new node<department>;
        }
        catch (const std::exception& er) {
            std::cout << er.what() << std::endl;
            ptr = nullptr;
            return false;
        }
        return true;
    }
    void clear() {
        if (ptr)
            delete ptr;
        ptr = nullptr;
    }
};

//==============================================================================================

class DepartmentAdd : public Department {
public:
    DepartmentAdd(rb_tree<department>& tmp) : Department(tmp) {}
    ~DepartmentAdd() {
        if (ptr) {
            data->remove(ptr);
            clear();
        }
    }
    virtual void execute() {
        if (create()) {
            std::cin >> ptr->key;
            data->insert(ptr);
        }
    }
    virtual void undo() {
        if (ptr)
            data->remove(ptr);
    }
    virtual void redo() {
        if (ptr)
            data->insert(ptr);
    }
};

class DepartmentTitle : public Department {
private:
    std::string title_buf;
public:
    DepartmentTitle(rb_tree<department>& tmp) : Department(tmp) {}
    ~DepartmentTitle() {
        title_buf.clear();
    }
    virtual void execute() {
        if (set_ptr()) {
            title_buf = ptr->key.title;
            std::cout << "Enter new Department title: ";
            std::cin >> ptr->key.title;
        }
    }
    virtual void undo() {
        std::string tmp = title_buf;
        title_buf = ptr->key.title;
        ptr->key.title = tmp;
    }
    virtual void redo() {
        std::string tmp = title_buf;
        title_buf = ptr->key.title;
        ptr->key.title = tmp;
    }
};

class DepartmentRm : public Department {
public:
    DepartmentRm(rb_tree<department>& tmp) : Department(tmp) {}
    ~DepartmentRm() {}
    virtual void execute() {
        if (set_ptr())
            data->remove(ptr);
    }
    virtual void undo() {
        (*data).insert(ptr);
    }
    virtual void redo() {
        (*data).remove(ptr);
    }
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
    bool set_ptr_d() {
        if (!data->isEmpty()) {
            std::string prompt = "Enter Department:\n";
            department tmp_d;
            do {
                std::cout << prompt;
                std::cin >> tmp_d;
                prompt = "Not found! Try again!\n";
            } while (!(ptr_d = data->iterative_tree_search(tmp_d)));
            std::cout << "Found department!" << std::endl;
            return true;
        }
        else {
            ptr_d = nullptr;
            std::cout << "Department tree is empty!" << std::endl;
            return false;
        }
    }
    bool set_ptr_w() {
        if (ptr_d) {
            std::string prompt = "Enter Worker:\n";
            worker tmp_w;
            do {
                std::cout << prompt;
                tmp_w = enter_for_srch();
                prompt = "Not found! Try again!\n";
            } while (!(ptr_w = ptr_d->key.employment.iterative_tree_search(tmp_w)));
            std::cout << "Found worker!" << std::endl;
        }
        else {
            ptr_w = nullptr;
            std::cout << "Worker tree is empty!" << std::endl;
            return false;
        }
    }
    worker& enter_for_srch() {
        worker tmp;
        std::cout << "Enter Surname: ";
        std::cin >> tmp.surname;
        std::cout << "Enter Name: ";
        std::cin >> tmp.name;
        std::cout << "Enter Patronymic: ";
        std::cin >> tmp.patronymic;
        std::cout << "Enter Post: ";
        std::cin >> tmp.post;
        return tmp;
    }
    bool create() {
        try {
            ptr_w = new node<worker>;
        }
        catch (const std::exception& er) {
            std::cout << er.what() << std::endl;
            ptr_w = nullptr;
            return false;
        }
        return true;
    }
    void clear() {
        if (ptr_w)
            delete ptr_w;
        ptr_w = nullptr;
    }
};

//==============================================================================================

class WorkerAdd : public Worker {
public:
    WorkerAdd(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerAdd() {
        if (ptr_w) {
            ptr_d->key.employment.remove(ptr_w);
            clear();
        }
    }
    virtual void execute() {
        if (set_ptr_d())
            if (create()) {
                std::cin >> ptr_w->key;
                ptr_d->key.employment.insert(ptr_w);
                ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk + ptr_w->key.salary) / ++ptr_d->key.amount_wrk;
            }
    }
    virtual void undo() {
        if (ptr_d && ptr_w) {
            ptr_d->key.employment.remove(ptr_w);
            ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk - ptr_w->key.salary) / --ptr_d->key.amount_wrk;
        }
    }
    virtual void redo() {
        if (ptr_d && ptr_w) {
            ptr_d->key.employment.insert(ptr_w);
            ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk + ptr_w->key.salary) / ++ptr_d->key.amount_wrk;
        }
    }
};

class WorkerRm : public Worker {
public:
    WorkerRm(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerRm() {}
    virtual void execute() {
        if (set_ptr_d())
            if (set_ptr_w()) {
                ptr_d->key.employment.remove(ptr_w);
                ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk - ptr_w->key.salary) / --ptr_d->key.amount_wrk;
            }
    }
    virtual void undo() {
        if (ptr_d && ptr_w) {
            ptr_d->key.employment.insert(ptr_w);
            ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk + ptr_w->key.salary) / ++ptr_d->key.amount_wrk;
        }
    }
    virtual void redo() {
        if (ptr_d && ptr_w) {
            ptr_d->key.employment.remove(ptr_w);
            ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk - ptr_w->key.salary) / --ptr_d->key.amount_wrk;
        }
    }
};

class WorkerSurname : public Worker {
private:
    std::string surname_buf;
public:
    WorkerSurname(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerSurname() {
        surname_buf.clear();
    }
    virtual void execute() {
        if (set_ptr_d()) 
            if (set_ptr_w()) {
                surname_buf = ptr_w->key.surname;
                std::cout << "Enter new Worker surname: ";
                std::cin >> ptr_w->key.surname;
            }
    }
    virtual void undo() {
        std::string tmp = surname_buf;
        surname_buf = ptr_w->key.surname;
        ptr_w->key.surname = tmp;
    }
    virtual void redo() {
        std::string tmp = surname_buf;
        surname_buf = ptr_w->key.surname;
        ptr_w->key.surname = tmp;
    }
};

class WorkerName : public Worker {
private:
    std::string name_buf;
public:
    WorkerName(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerName() {
        name_buf.clear();
    }
    virtual void execute() {
        if (set_ptr_d())
            if (set_ptr_w()) {
                name_buf = ptr_w->key.name;
                std::cout << "Enter new Worker name: ";
                std::cin >> ptr_w->key.name;
            }
    }
    virtual void undo() {
        std::string tmp = name_buf;
        name_buf = ptr_w->key.name;
        ptr_w->key.name = tmp;
    }
    virtual void redo() {
        std::string tmp = name_buf;
        name_buf = ptr_w->key.name;
        ptr_w->key.name = tmp;
    }
};

class WorkerPatronymic : public Worker {
private:
    std::string patronymic_buf;
public:
    WorkerPatronymic(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerPatronymic() {
        patronymic_buf.clear();
    }
    virtual void execute() {
        if (set_ptr_d())
            if (set_ptr_w()) {
                patronymic_buf = ptr_w->key.patronymic;
                std::cout << "Enter new Worker patronymic: ";
                std::cin >> ptr_w->key.patronymic;
            }
    }
    virtual void undo() {
        std::string tmp = patronymic_buf;
        patronymic_buf = ptr_w->key.patronymic;
        ptr_w->key.patronymic = tmp;
    }
    virtual void redo() {
        std::string tmp = patronymic_buf;
        patronymic_buf = ptr_w->key.patronymic;
        ptr_w->key.patronymic = tmp;
    }
};

class WorkerPost : public Worker {
private:
    std::string post_buf;
public:
    WorkerPost(rb_tree<department>& tmp) : Worker(tmp) {}
    ~WorkerPost() {
        post_buf.clear();
    }
    virtual void execute() {
        if (set_ptr_d())
            if (set_ptr_w()) {
                post_buf = ptr_w->key.post;
                std::cout << "Enter new Worker post: ";
                std::cin >> ptr_w->key.post;
            }
    }
    virtual void undo() {
        std::string tmp = post_buf;
        post_buf = ptr_w->key.post;
        ptr_w->key.post = tmp;
    }
    virtual void redo() {
        std::string tmp = post_buf;
        post_buf = ptr_w->key.post;
        ptr_w->key.post = tmp;
    }
};

class WorkerSalary : public Worker {
private:
    int salary_buf;
public:
    WorkerSalary(rb_tree<department>& tmp) : Worker(tmp), salary_buf(0) {}
    ~WorkerSalary() {}
    virtual void execute() {
        if (set_ptr_d())
            if (set_ptr_w()) {
                salary_buf = ptr_w->key.salary;
                std::cout << "Enter new Worker salary: ";
                std::cin >> ptr_w->key.salary;
                ptr_d->key.average_slr += (ptr_w->key.salary - salary_buf) / ptr_d->key.amount_wrk;
            }
    }
    virtual void undo() {
        int tmp = salary_buf;
        salary_buf = ptr_w->key.salary;
        ptr_w->key.salary = tmp;
        ptr_d->key.average_slr += (ptr_w->key.salary - salary_buf) / ptr_d->key.amount_wrk;
    }
    virtual void redo() {
        int tmp = salary_buf;
        salary_buf = ptr_w->key.salary;
        ptr_w->key.salary = tmp;
        ptr_d->key.average_slr += (ptr_w->key.salary - salary_buf) / ptr_d->key.amount_wrk;
    }
};

#endif