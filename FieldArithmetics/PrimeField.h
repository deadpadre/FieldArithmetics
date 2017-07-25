#pragma once

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

template<size_t SIZE>
class PrimeResidualMGroup : public Group {

	static bool isPrime(int a) {
		if (a < 0) {
			a = -a;
		}
		if (a <= 1) {
			return false;
		}
		for (long long i = 2; i * i <= a; ++i) {
			if (a % i == 0) {
				return false;
			}
		}
		return true;
	}

public:
	PrimeResidualMGroup() {
		if (!isPrime(SIZE)) {
			throw std::domain_error("PrimeResidualMGroup is a group only when it is of prime size");
		}
	}

	// Inherited via Group
	virtual size_t op(size_t lhs, size_t rhs) const override {
		return (lhs * rhs) % SIZE;
	}
	virtual size_t getInverse(size_t index) const override {
		if (index == 0) {
			throw std::domain_error("Attempting to inverse the zero element");
		}
		size_t answer = 1;
		size_t power = SIZE - 2;
		while (power) {
			if (power & 1) {
				answer = op(answer, index);
				--power;
			} else {
				answer = op(answer, answer);
				power >>= 1;
			}
		}
		return answer;
	}
	virtual size_t getSize() const override {
		return SIZE;
	}
	virtual size_t getNeutral() const override {
		return 1;
	}
};

template<size_t SIZE>
using PrimeField = Field<PrimeResidualMGroup<SIZE>, ResidualGroup<SIZE> >;