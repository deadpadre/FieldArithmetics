#pragma once

#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <iostream>

template <size_t size>
class PrimeFieldElement {
	size_t id;

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
    static PrimeFieldElement getRandom() {
        return PrimeFieldElement(random() % size);
    }

    PrimeFieldElement(): id(0) {
        if (!isPrime(size)) {
            throw std::domain_error("PrimeField can be only of prime size");
        }
    }
    PrimeFieldElement(size_t id_): id(id_ % size) {
        if (!isPrime(size)) {
            throw std::domain_error("PrimeField can be only of prime size");
        }
    }

    size_t getResidualValue() const {
        return id;
    }

    PrimeFieldElement getInverse() const {
        if (id == 0) {
            throw std::domain_error("Attempting to get the inverse of zero");
        }
        PrimeFieldElement answer(1);
        size_t power = size - 2;
        while (power) {
            if (power & 1) {
                answer *= *this;
                --power;
            } else {
                answer *= answer;
                power >>= 1;
            }
        }
        return answer;
    }

    virtual PrimeFieldElement& operator+=(const PrimeFieldElement &other) {
        id = (id + other.id) % size;
        return *this;
    }
    friend PrimeFieldElement<size> operator+(PrimeFieldElement<size> lhs, const PrimeFieldElement<size> &rhs) {
        return lhs += rhs;
    }
    PrimeFieldElement& operator-=(const PrimeFieldElement &other) {
        id = (size + id - other.id) % size;
        return *this;
    }
    friend PrimeFieldElement<size> operator-(PrimeFieldElement<size> lhs, const PrimeFieldElement<size> &rhs) {
        return lhs -= rhs;
    }
    PrimeFieldElement& operator*=(const PrimeFieldElement &other) {
        id = ((long long) id * (long long) other.id) % size;
        return *this;
    }
    friend PrimeFieldElement<size> operator*(PrimeFieldElement<size> lhs, const PrimeFieldElement<size> &rhs) {
        return lhs *= rhs;
    }
};

template <size_t size>
bool operator==(const PrimeFieldElement<size> &a, const PrimeFieldElement<size> &b) {
    return a.getResidualValue() == b.getResidualValue();
}

template <size_t size>
std::ostream& operator<<(std::ostream& os, const PrimeFieldElement<size> &elem) {
    return (os << elem.getResidualValue() << "@" << size);
}

