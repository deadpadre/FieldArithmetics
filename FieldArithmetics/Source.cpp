#include <iostream>
#include <exception>
#include "Basic.h"
#include "PrimeField.h"


int main() {
	//Element<Additive<ResidualGroup<9> > > a(5);
	//Element<Additive<ResidualGroup<9> > > b(6);
	try {
		Element<PrimeField<7> > a(5);
		Element<PrimeField<7> > b(6);
		std::cout << a << std::endl;
		std::cout << b << std::endl;
		std::cout << a + b << std::endl;
		std::cout << a * b << std::endl;
		std::cout << a / (b * a) << std::endl;
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}