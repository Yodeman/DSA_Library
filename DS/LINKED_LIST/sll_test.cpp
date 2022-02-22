#include <iostream>
//#include "heterogenous_sll.hpp"
#include "singly_linked_list.hpp"

int main()
{
    SLinkedList<int> l{};
    //SLinkedList l{};
    l.addFront(2);
    l.addFront(3);
    l.addFront(4);
    l.addFront(5);
    l.addFront(6);
    std::cout << l << std::endl;
    l.reverse();
    std::cout << l << std::endl;
    return 0;
}
