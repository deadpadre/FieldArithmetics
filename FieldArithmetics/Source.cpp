#include <iostream>
#include <exception>
#include "Basic.h"

template<size_t SIZE>
class ResidualGroup : public Group {
public:

	// Inherited via Group
	virtual size_t op(size_t lhs, size_t rhs) const override {
		return (lhs + rhs) % SIZE;
	}
	virtual size_t getInverse(size_t index) const override {
		return (SIZE - index) % SIZE;
	}
	virtual size_t getSize() const override {
		return SIZE;
	}
	virtual size_t getNeutral() const override {
		return 0;
	}
};

int main() {
	Element<ResidualGroup<9> > a(5);
	Element<ResidualGroup<9> > b(6);
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << a + b << std::endl;
	std::cout << (a + b).getInverse() << std::endl;
	
	return 0;
}