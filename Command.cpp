#include "Command.h"
#include "rb_tree.cpp"

//==============================================================================================
//class Department
//==============================================================================================

bool Department::set_ptr() {
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

bool Department::create() {
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

void Department::clear() {
    if (ptr)
        delete ptr;
    ptr = nullptr;
}

//==============================================================================================
//class DepartmentAdd
//==============================================================================================

DepartmentAdd::~DepartmentAdd() {
    if (ptr) {
        data->remove(ptr);
        clear();
    }
}

void DepartmentAdd::execute() {
    if (create()) {
        std::cin >> ptr->key;
        std::cout << ptr->key;
        data->insert(ptr);
    }
}

void DepartmentAdd::undo() {
    if (ptr)
        data->remove(ptr);
}

void DepartmentAdd::redo() {
    if (ptr)
        data->insert(ptr);
}

//==============================================================================================
//class DepartmentTitle
//==============================================================================================

DepartmentTitle::~DepartmentTitle() {
    title_buf.clear();
}

void DepartmentTitle::execute() {
    if (set_ptr()) {
        title_buf = ptr->key.title;
        std::cout << "Enter new Department title: ";
        std::cin >> ptr->key.title;
    }
}

void DepartmentTitle::undo() {
    std::string tmp = title_buf;
    title_buf = ptr->key.title;
    ptr->key.title = tmp;
}

void DepartmentTitle::redo() {
    std::string tmp = title_buf;
    title_buf = ptr->key.title;
    ptr->key.title = tmp;
}

//==============================================================================================
//class DepartmentRm
//==============================================================================================

void DepartmentRm::execute() {
    if (set_ptr())
        data->remove(ptr);
}

void DepartmentRm::undo() {
    (*data).insert(ptr);
}

void DepartmentRm::redo() {
    (*data).remove(ptr);
}

//==============================================================================================
//class Worker
//==============================================================================================

bool Worker::set_ptr_d() {
    if (data->isEmpty()) {
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

bool Worker::set_ptr_w() {
    if (ptr_d) {
        std::string prompt = "Enter Worker:\n";
        worker tmp_w;
        do {
            std::cout << prompt;
            tmp_w = enter_for_srch();
            prompt = "Not found! Try again!\n";
        } while (!(ptr_w = ptr_d->key.employment.iterative_tree_search(tmp_w)));
        std::cout << "Found worker!" << std::endl;
        return true;
    }
    else {
        ptr_w = nullptr;
        std::cout << "Worker tree is empty!" << std::endl;
        return false;
    }
}

worker Worker::enter_for_srch() {
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

bool Worker::create() {
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

void Worker::clear() {
    if (ptr_w)
        delete ptr_w;
    ptr_w = nullptr;
}

//==============================================================================================
//class WorkerAdd
//==============================================================================================

WorkerAdd::~WorkerAdd() {
    if (ptr_w) {
        ptr_d->key.employment.remove(ptr_w);
        clear();
    }
}

void WorkerAdd::execute() {
    if (set_ptr_d())
        if (create()) {
            std::cin >> ptr_w->key;
            ptr_d->key.employment.insert(ptr_w);
            ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk + ptr_w->key.salary) / ++ptr_d->key.amount_wrk;
        }
}

void WorkerAdd::undo() {
    if (ptr_d && ptr_w) {
        ptr_d->key.employment.remove(ptr_w);
        ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk - ptr_w->key.salary) / --ptr_d->key.amount_wrk;
    }
}

void WorkerAdd::redo() {
    if (ptr_d && ptr_w) {
        ptr_d->key.employment.insert(ptr_w);
        ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk + ptr_w->key.salary) / ++ptr_d->key.amount_wrk;
    }
}

//==============================================================================================
//class WorkerRm
//==============================================================================================

void WorkerRm::execute() {
    if (set_ptr_d())
        if (set_ptr_w()) {
            ptr_d->key.employment.remove(ptr_w);
            ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk - ptr_w->key.salary) / --ptr_d->key.amount_wrk;
        }
}

void WorkerRm::undo() {
    if (ptr_d && ptr_w) {
        ptr_d->key.employment.insert(ptr_w);
        ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk + ptr_w->key.salary) / ++ptr_d->key.amount_wrk;
    }
}

void WorkerRm::redo() {
    if (ptr_d && ptr_w) {
        ptr_d->key.employment.remove(ptr_w);
        ptr_d->key.average_slr = (ptr_d->key.average_slr * ptr_d->key.amount_wrk - ptr_w->key.salary) / --ptr_d->key.amount_wrk;
    }
}

//==============================================================================================
//class WorkerSurname
//==============================================================================================

WorkerSurname::~WorkerSurname() {
    surname_buf.clear();
}

void WorkerSurname::execute() {
    if (set_ptr_d())
        if (set_ptr_w()) {
            surname_buf = ptr_w->key.surname;
            std::cout << "Enter new Worker surname: ";
            std::cin >> ptr_w->key.surname;
        }
}

void WorkerSurname::undo() {
    std::string tmp = surname_buf;
    surname_buf = ptr_w->key.surname;
    ptr_w->key.surname = tmp;
}

void WorkerSurname::redo() {
    std::string tmp = surname_buf;
    surname_buf = ptr_w->key.surname;
    ptr_w->key.surname = tmp;
}

//==============================================================================================
//class WorkerName
//==============================================================================================

WorkerName::~WorkerName() {
    name_buf.clear();
}

void WorkerName::execute() {
    if (set_ptr_d())
        if (set_ptr_w()) {
            name_buf = ptr_w->key.name;
            std::cout << "Enter new Worker name: ";
            std::cin >> ptr_w->key.name;
        }
}

void WorkerName::undo() {
    std::string tmp = name_buf;
    name_buf = ptr_w->key.name;
    ptr_w->key.name = tmp;
}

void WorkerName::redo() {
    std::string tmp = name_buf;
    name_buf = ptr_w->key.name;
    ptr_w->key.name = tmp;
}

//==============================================================================================
//class WorkerPatronymic
//==============================================================================================

WorkerPatronymic::~WorkerPatronymic() {
    patronymic_buf.clear();
}

void WorkerPatronymic::execute() {
    if (set_ptr_d())
        if (set_ptr_w()) {
            patronymic_buf = ptr_w->key.patronymic;
            std::cout << "Enter new Worker patronymic: ";
            std::cin >> ptr_w->key.patronymic;
        }
}

void WorkerPatronymic::undo() {
    std::string tmp = patronymic_buf;
    patronymic_buf = ptr_w->key.patronymic;
    ptr_w->key.patronymic = tmp;
}
void WorkerPatronymic::redo() {
    std::string tmp = patronymic_buf;
    patronymic_buf = ptr_w->key.patronymic;
    ptr_w->key.patronymic = tmp;
}

//==============================================================================================
//class WorkerPost
//==============================================================================================

WorkerPost::~WorkerPost() {
    post_buf.clear();
}

void WorkerPost::execute() {
    if (set_ptr_d())
        if (set_ptr_w()) {
            post_buf = ptr_w->key.post;
            std::cout << "Enter new Worker post: ";
            std::cin >> ptr_w->key.post;
        }
}

void WorkerPost::undo() {
    std::string tmp = post_buf;
    post_buf = ptr_w->key.post;
    ptr_w->key.post = tmp;
}

void WorkerPost::redo() {
    std::string tmp = post_buf;
    post_buf = ptr_w->key.post;
    ptr_w->key.post = tmp;
}

//==============================================================================================
//class WorkerSalary
//==============================================================================================

void WorkerSalary::execute() {
    if (set_ptr_d())
        if (set_ptr_w()) {
            salary_buf = ptr_w->key.salary;
            std::cout << "Enter new Worker salary: ";
            std::cin >> ptr_w->key.salary;
            ptr_d->key.average_slr += (ptr_w->key.salary - salary_buf) / ptr_d->key.amount_wrk;
        }
}

void WorkerSalary::undo() {
    int tmp = salary_buf;
    salary_buf = ptr_w->key.salary;
    ptr_w->key.salary = tmp;
    ptr_d->key.average_slr += (ptr_w->key.salary - salary_buf) / ptr_d->key.amount_wrk;
}

void WorkerSalary::redo() {
    int tmp = salary_buf;
    salary_buf = ptr_w->key.salary;
    ptr_w->key.salary = tmp;
    ptr_d->key.average_slr += (ptr_w->key.salary - salary_buf) / ptr_d->key.amount_wrk;
}