
/* PROBLEM STATEMENT:
 * Jen drives her ice cream truck to her local elementary school at recess. All the kids rush to line up
 * in front of her truck. Jen is overwhelmed with the number of students (there are 2n of them), so
 * she calls up her associate, Berry, to bring his ice cream truck to help her out. Berry soon arrives
 * and parks at the other end of the line of students. He offers to sell to the last student in line, but the
 * other students revolt in protest: “The last student was last! This is unfair!”
 * The students decide that the fairest way to remedy the situation would be to have the back half of
 * the line (the n kids furthest from Jen) reverse their order and queue up at Berry’s truck, so that the
 * last kid in the original line becomes the last kid in Berry’s line, with the (n+ 1)st kid in the original
 * line becoming Berry’s first customer.
 */

#include <iostream>
#include <cmath>
#include "../../DS/LINKED_LIST/singly_linked_list.hpp"

template<typename T>
void reorder_students(SLinkedList<T>& L)
{
	/*
	 * Input: L - linked list with head L.head() and size L.size()
	 * Output: None
	 * This function should modify L to reverse its last half.
	 * This implementation should not instatiante:
	 *		- any additional linked list nodes
	 *		- any other non-constant-sized data structures
	 */
	size_t n = ceil(L.size()/2.0) - 1;
	SNode<T>* cur = nullptr;
	SNode<T>* nxt = nullptr;
	SNode<T>* h = L.header();
	SNode<T>* prv = nullptr;
	for(size_t i=0; i<n; ++i)
		h = h->next;
	cur = h->next;
	while(cur != nullptr){
		nxt = cur->next;
		cur->next = prv;
		prv = cur;
		cur = nxt;
	}
	h->next = prv;
	cur = nxt =  prv = h = nullptr;
}

int main()
{
	SLinkedList<std::string> L{};
	L.addFront("Paul");
	L.addFront("Lydia");
	L.addFront("Peter");
	L.addFront("Mary");
	L.addFront("Michael");
	L.addFront("Alex");
	L.addFront("Esther");
	L.addFront("Enoch");
	std::cout << "\nBefore reversal:" << std::endl;
	std::cout << L << std::endl;
	reorder_students(L);
	std::cout << "\nAfter reversal:" << std::endl;
	std::cout << L << std::endl;

	return 0;
}
