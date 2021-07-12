#ifndef _RB_TREE_H
#define _RB_TREE_H

#include <iostream>

//==============================================================================================
//==============================================================================================

template <typename t_key>
struct node {
	t_key key;
	bool red;
	node* parent;
	node* left;
	node* right;
	node();
	node(t_key tmp);
	//копирующий конструктор
	node(const node& node_tmp);
	//деструктор
	~node() {}
	//копирующий оператор присваивания
	node<t_key>& operator = (const node& node_tmp);
	//перемещающий оператор присваивания
	node<t_key>& operator = (node&& node_tmp);
};

//==============================================================================================
//==============================================================================================

template <typename t_key>
class _tree_iterator {
private:
	node<t_key>* ptr;
public:
	_tree_iterator() {}
	_tree_iterator(node<t_key>* it) : ptr(it) {}
	~_tree_iterator() {
		ptr = nullptr;
	}
	node<t_key>* get() { return ptr; }
	const node<t_key>& operator *() { return *ptr; }
	const node<t_key>* operator ->() { return &*ptr; }
	_tree_iterator<t_key> operator ++ (int) {
		_tree_iterator<t_key> res(*this);
		ptr = tree_successor(ptr);
		return res;
	}
	_tree_iterator<t_key>& operator ++() {
		ptr = tree_successor(ptr);
		return *this;
	}
	int operator != (const _tree_iterator<t_key>& t_tree_it) const {
		return ptr != t_tree_it.ptr;
	}
	int operator == (const _tree_iterator<t_key>& t_tree_it) const {
		return ptr == t_tree_it.ptr;
	}
protected:
	node<t_key>* tree_successor(const node<t_key>* ptr) {
		if (ptr->right != NULL)
			return tree_minimum(ptr->right);
		node<t_key>* previous = ptr->parent;
		while (previous != NULL && ptr == previous->right) {
			ptr = previous;
			previous = ptr->parent;
		}
		return previous;
	}
	node<t_key>* tree_predecessor(const node<t_key>* ptr) {
		if (ptr->left != NULL)
			return tree_maximum(ptr->left);
		node<t_key>* previous = ptr->parent;
		while (previous != NULL && ptr == previous->left) {
			ptr = previous;
			previous = ptr->parent;
		}
		return previous;
	}
	node<t_key>* tree_minimum(node<t_key>* ptr) const {
		while (ptr->left != nullptr)
			ptr = ptr->left;
		return ptr;
	}
	node<t_key>* tree_maximum(node<t_key>* ptr) const {
		while (ptr->right != nullptr)
			ptr = ptr->right;
		return ptr;
	}
};

//==============================================================================================
//==============================================================================================

template <typename t_key>
class rb_tree {
private:
	node<t_key>* root;
public:
	rb_tree() : root(nullptr) {};
	~rb_tree() {}
	bool isEmpty() const;
	void clear();
	bool insert(node<t_key>*& ptr);
	node<t_key>* remove(node<t_key>* ptr);
	node<t_key>* find(const t_key& tmp) const;
	node<t_key>* iterative_tree_search(t_key& key_srch) const;
	void show_NLR() const;
	void show_LNR() const;
	void show_LRN() const;
	friend class _tree_iterator<t_key>;
	typedef _tree_iterator<t_key> _tree_const_it;
	_tree_const_it begin() const { return tree_minimum(root); }
	_tree_const_it end() const { return nullptr; }
private:
	void recursion_clear(node<t_key>* ptr);
	void insert_fixup(node<t_key>* ptr);
	void left_rotate(node<t_key>* ptr);
	void right_rotate(node<t_key>* ptr);
	void rb_transplant(node<t_key>* u_ptr, node<t_key>* v_ptr);
	void remove_fixup(node<t_key>* ptr);
	node<t_key>* tree_minimum(node<t_key>* ptr) const;
	node<t_key>* tree_maximum(node<t_key>* ptr) const;
	node<t_key>* recursion_search(node<t_key>* ptr, const t_key& key_srch) const;
	void tree_walk_NLR(node<t_key>* ptr) const;
	void tree_walk_LNR(node<t_key>* ptr) const;
	void tree_walk_LRN(node<t_key>* ptr) const;
};

#endif // !_RB_TREE_H