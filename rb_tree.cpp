#include "rb_tree.h"

//==============================================================================================
//struct node
//==============================================================================================

template <typename t_key>
node<t_key>::node() : key(), parent(nullptr), left(nullptr), right(nullptr), red(true) {}

template <typename t_key>
node<t_key>::node(t_key tmp) : parent(nullptr), left(nullptr), right(nullptr), key(tmp) {}

template <typename t_key>
node<t_key>::node(const node<t_key>& node_tmp) {
	key = node_tmp.key;
	parent = node_tmp.parent;
	left = node_tmp.left;
	right = node_tmp.right;
}

template <typename t_key>
node<t_key>& node<t_key>::operator = (const node<t_key>& node_tmp) {
	key = node_tmp.key;
	parent = node_tmp.parent;
	left = node_tmp.left;
	right = node_tmp.right;
	return *this;
}

template <typename t_key>
node<t_key>& node<t_key>::operator = (node<t_key>&& node_tmp) {
	t_key tmp_key = node_tmp.key;
	node_tmp.key = key;
	key = tmp_key;
	node* tmp_ptr;
	tmp_ptr = node_tmp.parent;
	node_tmp.parent = parent;
	parent = tmp_ptr;
	tmp_ptr = node_tmp.left;
	node_tmp.left = left;
	left = tmp_ptr;
	tmp_ptr = node_tmp.right;
	node_tmp.right = right;
	right = tmp_ptr;
	return *this;
}

//==============================================================================================
//class rb_tree
//==============================================================================================

template <typename t_key>
bool rb_tree<t_key>::isEmpty() const {
	if (root)
		return true;
	else
		return false;
}

template <typename t_key>
void rb_tree<t_key>::clear() {
	if (root)
		recursion_clear(root);
}

template <typename t_key>
void rb_tree<t_key>::recursion_clear(node<t_key>* ptr) {
	if (ptr->left)
		recursion_clear(ptr->left);
	if (ptr->right)
		recursion_clear(ptr->right);
	delete ptr;
	ptr = nullptr;
}

template <typename t_key>
bool rb_tree<t_key>::insert(node<t_key>*& ptr) {
	node<t_key>* previous = nullptr;
	node<t_key>* current = root;
	bool flag = false;
	while (!flag && current != nullptr) {
		previous = current;
		if (ptr->key < current->key)
			current = current->left;
		else
			if (ptr->key > current->key)
				current = current->right;
			else 
				flag = true;
	}
	if (!flag) {
		ptr->parent = previous;
		if (previous == nullptr)
			root = ptr;
		else
			if (ptr->key < previous->key)
				previous->left = ptr;
			else
				previous->right = ptr;
		ptr->left = nullptr;
		ptr->right = nullptr;
		ptr->red = true;
		insert_fixup(ptr);
		return true;
	}
	return false;
}

template <typename t_key>
void rb_tree<t_key>::insert_fixup(node<t_key>* ptr) {
	node<t_key>* tmp;
	while (ptr != root && ptr->parent->red) {
		if (ptr->parent == ptr->parent->parent->left) {
			tmp = ptr->parent->parent->right;
			if (tmp != nullptr && tmp->red) {
				ptr->parent->red = false;
				tmp->red = false;
				ptr->parent->parent->red = true;
				ptr = ptr->parent->parent;
			}
			else {
				if (ptr == ptr->parent->right) {
					ptr = ptr->parent;
					left_rotate(ptr);
				}
				ptr->parent->red = false;
				ptr->parent->parent->red = true;
				right_rotate(ptr->parent->parent);
			}
		}
		else {
			tmp = ptr->parent->parent->left;
			if (tmp != nullptr && tmp->red) {
				ptr->parent->red = false;
				tmp->red = false;
				ptr->parent->parent->red = true;
				ptr = ptr->parent->parent;
			}
			else {
				if (ptr == ptr->parent->left) {
					ptr = ptr->parent;
					right_rotate(ptr);
				}
				ptr->parent->red = false;
				ptr->parent->parent->red = true;
				left_rotate(ptr->parent->parent);
			}
		}
	}
	root->red = false;
}

template <typename t_key>
void rb_tree<t_key>::left_rotate(node<t_key>* ptr) {
	node<t_key>* tmp = ptr->right;
	ptr->right = tmp->left;
	if (tmp->left != nullptr)
		tmp->left->parent = ptr;
	tmp->parent = ptr->parent;
	if (ptr->parent == nullptr)
		root = tmp;
	else
		if (ptr == ptr->parent->left)
			ptr->parent->left = tmp;
		else
			ptr->parent->right = tmp;
	tmp->left = ptr;
	ptr->parent = tmp;
}

template <typename t_key>
void rb_tree<t_key>::right_rotate(node<t_key>* ptr) {
	node<t_key>* tmp = ptr->left;
	ptr->left = tmp->right;
	if (tmp->right != nullptr)
		tmp->right->parent = ptr;
	tmp->parent = ptr->parent;
	if (ptr->parent == nullptr)
		root = tmp;
	else
		if (ptr == ptr->parent->right)
			ptr->parent->right = tmp;
		else
			ptr->parent->left = tmp;
	tmp->right = ptr;
	ptr->parent = tmp;
}

template <typename t_key>
node<t_key>* rb_tree<t_key>::remove(node<t_key>* ptr) {
	node<t_key>* tmp = ptr;
	node<t_key>* fix_ptr;
	bool tmp_original_color = tmp->red;
	if (ptr->left == nullptr) {
		fix_ptr = ptr->right;
		rb_transplant(ptr, ptr->right);
	}
	else
		if (ptr->right == NULL) {
			fix_ptr = ptr->left;
			rb_transplant(ptr, ptr->left);
		}
		else {
			tmp = tree_minimum(ptr->right);
			tmp_original_color = tmp->red;
			fix_ptr = tmp->right;
			if (tmp->parent == ptr)
				fix_ptr->parent = tmp;
			else {
				rb_transplant(tmp, tmp->right);
				tmp->right = ptr->right;
				tmp->right->parent = tmp;
			}
			rb_transplant(ptr, tmp);
			tmp->left = ptr->left;
			tmp->left->parent = tmp;
			tmp->red = ptr->red;
		}
	if (!tmp_original_color && fix_ptr != nullptr)
		remove_fixup(fix_ptr);
	return ptr;
}

template <typename t_key>
void rb_tree<t_key>::rb_transplant(node<t_key>* u_ptr, node<t_key>* v_ptr) {
	if (u_ptr->parent == nullptr)
		root = v_ptr;
	else
		if (u_ptr == u_ptr->parent->left)
			u_ptr->parent->left = v_ptr;
		else
			u_ptr->parent->right = v_ptr;
	if (v_ptr != nullptr)
		v_ptr->parent = u_ptr->parent;
}

template <typename t_key>
void rb_tree<t_key>::remove_fixup(node<t_key>* ptr) {
	while (ptr != root && !ptr->red)
		if (ptr == ptr->parent->left) {
			node<t_key>* tmp = ptr->parent->right;
			if (tmp->red) {
				tmp->red = false;
				ptr->parent->red = true;
				left_rotate(ptr->parent);
				tmp = ptr->parent->right;
			}
			if (!tmp->left->red && !tmp->right->red) {
				tmp->red = true;
				ptr = ptr->parent;
			}
			else {
				if (!tmp->right->red) {
					tmp->left->red = false;
					tmp->red = true;
					right_rotate(tmp);
					tmp = ptr->parent->right;
				}
				tmp->red = ptr->parent->red;
				ptr->parent->red = false;
				tmp->right->red = false;
				left_rotate(ptr->parent);
				ptr = root;
			}
		}
		else {
			node<t_key>* tmp = ptr->parent->left;
			if (tmp->red) {
				tmp->red = false;
				ptr->parent->red = true;
				right_rotate(ptr->parent);
				tmp = ptr->parent->left;
			}
			if (!tmp->right->red && !tmp->left->red) {
				tmp->red = true;
				ptr = ptr->parent;
			}
			else {
				if (!tmp->left->red) {
					tmp->right->red = false;
					tmp->red = true;
					left_rotate(tmp);
					tmp = ptr->parent->left;
				}
				tmp->red = ptr->parent->red;
				ptr->parent->red = false;
				tmp->left->red = false;
				right_rotate(ptr->parent);
				ptr = root;
			}
		}
	ptr->red = false;
}

template <typename t_key>
node<t_key>* rb_tree<t_key>::tree_minimum(node<t_key>* ptr) const {
	while (ptr->left != nullptr)
		ptr = ptr->left;
	return ptr;
}

template <typename t_key>
node<t_key>* rb_tree<t_key>::tree_maximum(node<t_key>* ptr) const {
	while (ptr->right != nullptr)
		ptr = ptr->right;
	return ptr;
}

template <typename t_key>
node<t_key>* rb_tree<t_key>::find(const t_key& tmp) const {
	return recursion_search(root, tmp);
}

template <typename t_key>
node<t_key>* rb_tree<t_key>::recursion_search(node<t_key>* ptr, const t_key& key_srch) const {
	if (ptr == nullptr || key_srch == ptr->key)
		return ptr;
	if (key_srch < ptr->key)
		return recursion_search(ptr->left, key_srch);
	else
		return recursion_search(ptr->right, key_srch);
}

template <typename t_key>
node<t_key>* rb_tree<t_key>::iterative_tree_search(t_key& key_srch) const {
	node<t_key>* ptr = root;
	while (ptr != nullptr && key_srch != ptr->key)
		if (key_srch < ptr->key)
			ptr = ptr->left;
		else
			ptr = ptr->right;
	return ptr;
}

template <typename t_key>
void rb_tree<t_key>::show_NLR() const {
	node<t_key>* ptr = root;
	if (ptr)
		tree_walk_NLR(ptr);
	else
		std::cout << "Tree is empty!\n";
}

template <typename t_key>
void rb_tree<t_key>::tree_walk_NLR(node<t_key>* ptr) const {
	if (ptr != nullptr) {
		std::cout << ptr->key << std::endl;
		tree_walk_NLR(ptr->left);
		tree_walk_NLR(ptr->right);
	}
}

template <typename t_key>
void rb_tree<t_key>::show_LNR() const {
	node<t_key>* ptr = root;
	if (ptr)
		tree_walk_LNR(ptr);
	else
		std::cout << "Tree is empty!\n";
}

template <typename t_key>
void rb_tree<t_key>::tree_walk_LNR(node<t_key>* ptr) const {
	if (ptr != nullptr) {
		tree_walk_LNR(ptr->left);
		std::cout << ptr->key << std::endl;
		tree_walk_LNR(ptr->right);
	}
}

template <typename t_key>
void rb_tree<t_key>::show_LRN() const {
	node<t_key>* ptr = root;
	if (ptr)
		tree_walk_LRN(ptr);
	else
		std::cout << "Tree is empty!\n";
}

template <typename t_key>
void rb_tree<t_key>::tree_walk_LRN(node<t_key>* ptr) const {
	if (ptr != nullptr) {
		tree_walk_LRN(ptr->left);
		tree_walk_LRN(ptr->right);
		std::cout << ptr->key << std::endl;
	}
}