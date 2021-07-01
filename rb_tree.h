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
class rb_tree {
private:
	node<t_key>* root;
public:
	rb_tree() : root(nullptr) {};
	~rb_tree() {}
	bool isEmpty() const;
	void clear();
	bool insert(node<t_key>* ptr);
	node<t_key>* remove(node<t_key>* ptr);
	node<t_key>* find(const t_key& tmp) const;
	node<t_key>* iterative_tree_search(t_key& key_srch) const;
	void show_NLR() const;
	void show_LNR() const;
	void show_LRN() const;
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