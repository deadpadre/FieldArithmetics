#include <iostream>
#include <exception>
#include "Basic.h"
#include "PrimeField.h"
#include "CleverPolynomial.h"

int main() {
	//Element<Additive<ResidualGroup<9> > > a(5);
	//Element<Additive<ResidualGroup<9> > > b(6);
	//Element<PrimeField<7> > a(5);
	//Element<PrimeField<7> > b(6);
	//Element<PrimeField<7> > c;
	//std::cout << a << std::endl;
	//std::cout << b << std::endl;
	//std::cout << c << std::endl;
	//std::cout << a + b << std::endl;
	//std::cout << a * b << std::endl;
	//std::cout << a / (b * a) << std::endl;
	//std::cout << -a << std::endl;
	try {
		Polynom<PrimeField<3>> a;
		Polynom<PrimeField<3>> b;
		for (size_t i = 0; i < 10; ++i) {
			a[i] = Element<PrimeField<3> >((i + 1) % 3);
			b[i] = Element<PrimeField<3> >(i % 3);
		}
		std::cout << a << std::endl;
		std::cout << b << std::endl;
		std::cout << a + b << std::endl;
		std::cout << -b << std::endl;
		std::cout << a * (-b) << std::endl;
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}