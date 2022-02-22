#ifndef __LIST_EXCEPTION__
#define __LIST_EXCEPTION__
#include <iostream>
class EmptyList: std::runtime_error{
	public:
		EmptyList(const std::string err): std::runtime_error{err}
		{}
};
#endif // __LIST_EXCEPTION__
