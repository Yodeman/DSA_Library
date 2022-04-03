#ifndef MY_TWO_FOUR_TREE
#define MY_TWO_FOUR_TREE

#include <iostream>
#include <concepts>
#include <valarray>
#include <memory>

template<std::totally_ordered T>
struct TwoFourTreeNode{
	unsigned char n_entries = 0;
	std::array<std::unique_ptr<T>, 4> entries{};
	std::array<std::shared_ptr<TwoFourTreeNode<T>>, 5> children{};
	std::weak_ptr<TwoFourTreeNode<T>> parent;

	void insert_entry(T&& entry) {
		if (!n_entries) {
			entries[0] = std::make_unique<T>(entry);
			++n_entries;
			return;
		}
		else {
			for (auto i=n_entries; i>0; --i) {
				if (entries[i-1] && *(entries[i-1]) > entry) {
					entries[i] = std::move(entries[i-1]);
				}
				else {
					entries[i] = std::make_unique<T>(entry);
					++n_entries;
					return;
				}
			}
			entries[0] = std::make_unique<T>(entry);
			++n_entries;
		}
	}
	void remove_entry(unsigned char index) {
		for (auto i=index; i<n_entries; ++i)
			entries[i] = std::move(entries[i+1]);
		--n_entries;
	}
};

template<std::totally_ordered T>
class TwoFourTree{
	private:
		size_t size() { return sz; }
		bool is_empty() { return sz==0; }
		void insert(const T&);
		void remove(const T&);
		std::unique_ptr<T> find(const T&);

	public:
		size_t sz = 0;
		std::shared_ptr<TwoFourTreeNode<T>> root_node = nullptr;
		void restructure(std::shared_ptr<TwoFourTreeNode<T>>&);
		std::unique_ptr<T> __search(std::shared_ptr<TwoFourTreeNode<T>>&);
};

template<std::totally_ordered T>
std::shared_ptr<TwoFourTreeNode<T>> TwoFourTree<T>::__search(
			std::shared_ptr<TwoFourTreeNode<T>>& parent_node,
			std::shared_ptr<TwoFourTreeNode<T>>& node,
			const T& entry
		)
{
	if (node) {
		// check the entries of a node to see if it contains the entry
		// of interest.
		for (unsigned char i = 0; i<3; ++i) {
			if ((node->entry)[i] && (*((node->entries)[i]) == entry))
				return node;
		}
		// if the node doesn't contain the entry of interest, pick the child
		// node that is most likely to contain the entry of interest.
		for (unsigned char i = 0; i<4; ++i){
			auto child_node = (node->children)[i];
			// since child nodes are ordered in increasing order, once the
			// iteration hits a null child node from left, it stops since
			// its siblings on its right hand side are definitely going to
			// be null.
			if (!child_node) {
				parent_node = node;
				node = child_node;
				break;
			}
			// if the child node contains some entries, since the entries are
			// ordered in increasing order, checks if the last non-null entry
			// in the node is greater than the entry of interest, then, the
			// node is most likely to contain the entry of interest, else,
			// move to the next sibling.
			else if (child_node) {
				auto n_e = child_node->n_entries;
				//while ((ri != (child_node->entries).rend()) && (*ri == nullptr)) ++ri;
				if (n_e && *((childe_node->entries)[n_e]) >= entry) {
					parent_node = node;
					node = child_node;
					break;
				}
				continue;
			}
		}
		return __search(parent_node, node, entry);
	}
	return parent_node;
}

template<std::totally_ordered T>
void TwoFourTree<T>::insert(T&& entry)
{
	if (sz==0){
		root_node = std::make_shared<TwoFourTreeNode<T>>();
		root_node->insert_entry(std::forward<T>(entry));
		++sz;
		return;
	}
	auto node = __search(nullptr, root_node, entry);
	auto entries = node->entries;
	auto n_e = node->n_entries;
	for (char i = 0; i < n_e; ++i) {
		// do nothing if entry already exists, else insert
		// the new entry.
		if (entries[i] && *(entries[i]) == entry) {
			return;
		}
	}
	node->insert_entry(std::forward<T>(entry));
	// perfom split operation if the number of entries in a node is 4.
	if (entries[3])
		restructure(node);

}

template<std::totally_ordered T>
std::unique_ptr TwoFourTree<T>::find(const T& entry)
{
	auto node = __search(nullptr, root_node, entry);
	auto entries = node->entries;
	auto n_e = node->n_entries;
	for (auto i=0; i < n_e; ++i) {
		if (entries[i] && *(entries[i]) == entry)
			return std::make_unique<T>(entry);
	}
	return nullptr;
}

template<std::totally_ordered T>
void TwoFourTree<T>::remove(const T& entry)
{
	auto node = __search(nullptr, root_node, entry);
	auto entries = node->entries;
	auto n_e = node->n_entries;
	char idx = -1; // the index of the entry to be deleted.
	for (auto i=0; i<n_e; ++i) {
		if (entries[i] && *(entries[i])==entry) {
			idx = i;
			break;
		}
	}

	if (idx == -1) {
		throw std::runtime_error("the given entry doesn't exists in the tree!!!");
	}
	else {
		// remove the entry if the children of the node aren't all internal
		// nodes.
		bool chidren_are_internal = True;
		for (auto i=0; i < n_e+1; ++i){
			if (!(node->children)[i]) {
				children_are_internal = False;
				node->remove_entry(idx);
				if (!entries[0])
					restructure(node);
				--sz;
				return;
			}
		}
		// else find the rightmost child node rooted at child node at
		// index `idx` whose children are all null, and swap its
		// last entry with the entry that's about to be deleted.
		if (children_are_internal){
			auto child_node = (node->children)[idx];
			while ((child_node->children)[child_node->n_entries])
				child_node = (child_node->children)[child_node->n_entries];
			(node->entries)[idx] = std::move((child_node->entries)[child_node->n_entries - 1]);
			if (!(child_node->entries)[0])
				restructure(child_node);
			return;
		}
	}
}

template<std::totally_ordered T>
void TwoFourTree<T>::restructure(std::shared_ptr<TwoFourTreeNode<T>>& node)
{
	// overflow??
	if ((node->entries)[3]) {
		__resolve_overflow(node);
	}
	// underflow??
	else if (!(node->entries)[0]){
		__resolve_underflow(node);
	}
}

template<std::totally_ordered T>
void TwoFourTree<T>::__resolve_overflow(std::shared_ptr<TwoFourTreeNode<T>>& node)
{
	if (node == root_node){
		// new nodes to store entries.
		std::shared_ptr<TwoFourTreeNode<T>> new_node = std::make_shared<TwoFourTreeNode<T>>();
		std::shared_ptr<TwoFourTreeNode<T>> first_node_from_left = std::make_shared<TwoFourTreeNode<T>>();
		std::shared_ptr<TwoFourTreeNode<T>> second_node_from_left = std::make_shared<TwoFourTreeNode<T>>();
		
		// move entries.
		(new_node->entries)[0] = std::move((root_node->entries)[2]);
		(first_node_from_left->entries)[0] = std::move((root_node->entries)[0]);
		(first_node_from_left->entries)[1] = std::move((root_node->entries)[1]);
		(second_node_from_left->entries)[0] = std::move((root_node->entries)[3]);

		(new_node->children)[0]  = first_node_from_left;
		(new_node->children)[1] = second_node_from_left;
		first_node_from_left->parent = new_node;
		second_node_from_left->parent = new_node;
		root_node.reset();
		root_node = new_node;
		return;

	}
	else {
		int entry_idx = node_idx = 3;
		auto parent_node = (node->parent).lock();
		std::shared_ptr<TwoFourTreeNode<T>> first_node = std::make_shared<TwoFourTreeNode<T>>();
		std::shared_ptr<TwoFourTreeNode<T>> second_node = std::make_shared<TwoFourTreeNode<T>>();
		// look for the right position in parent node entries to insert the entry at index 2 of
		// the overflowing child node.
		while(entry_idx > 0) {
			if (((parent_node->entries)[entry_idx]) && (*((parent_node->entry)[entry_idx]) > *((node->entry)[2]))) {
				parent_node->entries[entry_idx+1] = std::move((parent_node->entries)[entry_idx]);
			}
			else if (((parent_node->entries)[entry_idx]) && (*((parent_node->entry)[entry_idx]) < *((node->entry)[2]))) {
				(parent_node->entries)[entry_idx+1] = std::move((node->entries)[2]);
				break;
			}
			--entry_idx;
		}
		
		// create space for the new node that will be inserted in the children of the parent node.
		while(node_idx > 0) {
			if ((parent_node->children)[node_idx] != node)
				(parent_node->children)[node_idx+1] = std::move((parent_node->children)[node_idx]);
			else
				break;
			--node_idx
		}

		(first_node->entries)[0] = std::move((node->entries)[0]);
		(first_node->entries)[1] = std::move((node->entries)[1]);
		(first_node->children)[0] = std::move((node->children)[0]);
		(first_node->children)[1] = std::move((node->children)[1]);
		(first_node->children)[2] = std::move((node->children)[2]);

		(second_node->entries)[0] = std::move((node->entries)[3]);
		(second_node->children)[0] = std::move((node->children)[3]);
		(second_node->children)[1] = std::move((node->children)[4]);

		node.reset();
		(parent_node->children)[node_idx] = first_node;
		(parent_node->children)[node_idx+1] = second_node;
		
		// restructure parent node if it has overflown.
		if ((parent_node->entries)[3])
			__resolve_overflow(parent_node);

	}
}

template<std::totally_ordered T>
void TwoFourTree<T>::__resolve_underflow(std::shared_ptr<TwoFourTreeNode<T>>& node)
{
	auto parent_node = (node->parent).lock();
	char idx;
	for (idx = 0; i < 4; ++i){
		if ((parent_node->children)[i] == node)
			break;
	}
	std::shared_ptr<TwoFourTreeNode<T>> first_sibling = (i > 0) ? (parent->children)[i-1] : nullptr;
	std::shared_ptr<TwoFourTreeNode<T>> second_sibling = (i < 4) ? (parent->children)[i+1] : nullptr;

	if (first_sibling && first_sibling->n_entries >= 2)
		__transfer_operation(parent_node, node, first_sibling, idx);
	else if (second_sibling && second_sibling->n_entries >= 2)
		__tranfer_operation(parent_node, node, second_sibling, idx);
	else if (first_sibling && first_sibling->n_entries == 1)
		__fussion_operation(node, first_sibling, idx);
	else if (second_sibling && second_sibling->n_entries == 1)
		__fussion_operation(node, second_sibling, idx);
}

template<std::totally_ordered T>
void TwoFourTree<T>::__transfer_operation(
			std::shared_ptr<TwoFourTreeNode<T>>& parent_node,
			std::shared_ptr<TwoFourTreeNode<T>>& node,
			std::shared_ptr<TwoFourTreeNode<T>>& sibling,
			char index
		)
{
	// if sibling is on the left.
	if((index-1 > 0) && ((parent_node->children)[index-1] == sibling)) {
		T entry = *((sibling->entries)[sibling->n_entries -1]);
		sibling->remove_entry((sibling->n_entries - 1))
		parent_node->insert_entry(std::forward<T>(entry));
		entry = *((parent_node->entries)[parent_node->n_entries - 1]);
		parent_node->remove_entry((parent_node->n_entries -1));
		node->insert_entry(std::forward<T>(entry));
	}
	else {
		T entry = *((sibling->entries)[0]);
		sibling->remove_entry(0);
		parent_node->insert_entry(std::forward<T>(entry));
		entry = *((parent_node->entries)[0]);
		parent_node->remove_entry(0);
		node->insert_entry(std::forward<T>(entry));
	}
}

template<std::totally_ordered T>
void TwoFourTree<T>::__fussion_operation(
			std::shared_ptr<TwoFourTreeNode<T>>& node,
			std::shared_ptr<TwoFourTreeNode<T>>& sibling,
			char index
		)
{
	// pass.
}

#endif	//MY_TWO_FOUR_TREE
