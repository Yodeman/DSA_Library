//#error "WORK IN PROGRESS!!!"
#include <iostream>
#include <any>

class EmptyList: public std::runtime_error{
    public:
	EmptyList(const std::string err): std::runtime_error(err)
	{}
};
struct Node{
    std::any elem;
    std::any next;
};

class SLinkedList{
    public:
	SLinkedList();
	~SLinkedList();

	bool is_empty() const;
	const auto& front() const;	// get front element
	
	//template<typename T>
	void addFront(const auto& arg);
	void removeFront();
	size_t size() { return sz; }

    private:
	size_t sz;
	Node* head;
};

SLinkedList::SLinkedList()
{
    head = new Node{};
    head->elem = 0;
    head->next = nullptr;
    sz = 0;
}

const auto& SLinkedList::front() const
{
    if(is_empty()) throw EmptyList("List is empty!!!");
    Node* node = std::any_cast<Node*>(head->next);
    return node->elem;
}

SLinkedList::~SLinkedList()
{
    while(!is_empty()) removeFront();
    delete head;
}

bool SLinkedList::is_empty() const
{
    return sz==0;
}

//template<typename T>
void SLinkedList::addFront(const auto& arg)
{
    Node* new_node = new Node{};
    new_node->elem = arg;
    new_node->next = nullptr;
    if (is_empty()){
	head->next = new_node;
    }
    else{
	Node* old_node = std::any_cast<Node*>(head->next);
	head->next = new_node;
	new_node->next = old_node;
    }
    ++sz;
    return;
}

void SLinkedList::removeFront()
{
    if(is_empty()) throw EmptyList("List is empty!!!");
    Node* old_node = std::any_cast<Node*>(head->next);
    head->next = old_node->next;
    delete old_node;
    --sz;
    return;
}
