#include <iostream>
#include "doubly_linked_list.hpp"

int main()
{
	DLinkedList<int> d{};
    d.addBack(1);
    d.addBack(8);
    d.addBack(3);
    for(auto i=d.begin(); i!=d.end(); ++i) {
    	if ((*i)->elem > 3) {
    		d.insertBefore(*i, 2);
    		d.insertAfter(*i, 6);
    		break;
    	}
    }
    for(auto i : d)
    	std::cout << i->elem << " ";
    /*
    for(auto i=d.begin(); i!=d.end(); ++i)
    	std::cout << (*i)->elem << " ";
	std::cout << '\n';
	*/

    return 0;
}
