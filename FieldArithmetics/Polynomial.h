#pragma once

#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include "PrimeField.h"

template <typename FieldElement>
class Polynomial {

    typedef std::pair<size_t, FieldElement> Monomial;

    void eraseZeroes() {
        size_t size = coef.size();
        size_t lastNonZeroIndex = 0;
        for (size_t i = 1; i < size; ++i) {
            if (coef[i].second == FieldElement(0)) {
                continue;
            } else {
                coef[lastNonZeroIndex + 1] = coef[i];
                ++lastNonZeroIndex;
            }
        }
        while (lastNonZeroIndex + 1 != coef.size()) {
            coef.pop_back();
        }
    }

    std::vector<Monomial> coef;

public:
    // gives random trinomial or pentanomial
    // tanja lange paper says it's practically enough to find
    // irreducible polynomial
    static Polynomial<FieldElement> getRandomTP() {
        Polynomial<FieldElement> answer;
        answer.coef.pop_back();
        size_t degree = (random() % 2) ? 3 : 5;
        for (size_t i = 0; i < degree; ++i) {
            answer.coef.push_back(std::make_pair(i, FieldElement::getRandom()));
        }
        answer.coef.push_back(std::make_pair(degree, FieldElement(1)));
        answer.eraseZeroes();
        return answer;
    }

    static Polynomial<FieldElement> getIrreducible(int degree) {
        Polynomial<FieldElement> candidate;
        do {
            candidate = getRandomTP();
        } while (!candidate.isIrreducible());
        return candidate;
    }

    Polynomial() {
        coef.push_back(std::make_pair(0, FieldElement(0)));
    }
    Polynomial(const std::vector<Monomial> &a): coef(a) {}
    // builds  monomial
    Polynomial(const size_t degree, const FieldElement &index) {
        coef.push_back(std::make_pair(degree, index));
    }

    size_t getDegree() const {
        return coef.back().first;
    }

    //TODO
    bool isIrreducible() const {
        return true;
    }

    FieldElement operator[](size_t i) const {
        auto iter = std::lower_bound(coef.begin(), coef.end(), std::make_pair(i, FieldElement(0)), [](const Monomial &a, const Monomial &b) {
            return a.first < b.first;
        });

        FieldElement answer;
        if (iter == coef.end()) {
            answer = FieldElement(0);
        } else if (iter->first == i) {
            answer = iter->second;
        } else {
            answer = FieldElement(0);
        }
        return answer;
    }

    template <typename T>
    friend Polynomial<T> operator+(const Polynomial<T> &, const Polynomial<T> &);

    template <typename T>
    friend Polynomial<T> operator-(const Polynomial<T> &, const Polynomial<T> &);

    template <typename T>
    friend Polynomial<T> operator*(const Polynomial<T> &, const Polynomial<T> &);

    template <typename T>
    friend Polynomial<T> operator%(const Polynomial<T> &, const Polynomial<T> &);

    template <typename T>
    friend Polynomial<T> operator,(const Polynomial<T> &, const Polynomial<T> &);

    template <typename T>
    friend bool operator==(const Polynomial<T> &, const Polynomial<T> &);
};

template <typename FieldElement>
Polynomial<FieldElement> operator+(const Polynomial<FieldElement> &a, const Polynomial<FieldElement> &b) {
    Polynomial<FieldElement> answer;
    answer.coef.pop_back();
    auto ai = a.begin();
    auto bi = b.begin();
    while (ai != a.end() && bi != b.end()) {
        if (ai->first == bi->first) {
            answer.coef.push_back(std::make_pair(ai->first, ai->second + bi->second));
            ++ai;
            ++bi;
        } else {
            ++((ai->first < bi->first) ? ai : bi);
        }
    }
    if (ai == a.end()) {
        std::copy(bi, b.end(), std::back_inserter(answer));
    } else {
        std::copy(ai, a.end(), std::back_inserter(answer));
    }
    answer.eraseZeroes();
    return answer;
}

// TODO: decide, whether we should avoid copying here by defining unary minus
template <typename FieldElement>
Polynomial<FieldElement> operator-(const Polynomial<FieldElement> &a, const Polynomial<FieldElement> &b) {
    Polynomial<FieldElement> answer;
    answer.coef.pop_back();
    auto ai = a.begin();
    auto bi = b.begin();
    while (ai != a.end() && bi != b.end()) {
        if (ai->first == bi->first) {
            answer.coef.push_back(std::make_pair(ai->first, ai->second - bi->second));
            ++ai;
            ++bi;
        } else {
            ++((ai->first < bi->first) ? ai : bi);
        }
    }
    if (ai == a.end()) {
        std::copy(bi, b.end(), std::back_inserter(answer));
    } else {
        std::transform(ai, a.end(), std::back_inserter(answer), [](const std::pair<size_t, FieldElement> &a) {
            return std::make_pair(a.first, FieldElement(0) - a.second);
        });
    }
    answer.eraseZeroes();
    return answer;
}

// TODO: maybe change to some kind of fft
template <typename FieldElement>
Polynomial<FieldElement> operator*(const Polynomial<FieldElement> &a, const Polynomial<FieldElement> &b) {
    Polynomial<FieldElement> answer;
    answer.coef.resize(a.getDegree() + b.getDegree() + 1, 0);
    for (size_t i = 0; i < a.coef.size(); ++i) {
        for (size_t j = 0; j < b.coef.size(); ++j) {
            answer[i + j] += a[i] * b[j];
        }
    }
    answer.eraseLeadingZeroes();
    return answer;
}

template <typename FieldElement>
Polynomial<FieldElement> operator%(const Polynomial<FieldElement> &a, const Polynomial<FieldElement> &b) {
    if (b.getDegree() == 0) {
        return Polynomial<FieldElement>();
    }
    Polynomial<FieldElement> lhs = a;
    while (lhs.getDegree() >= b.getDegree()) {
        lhs = lhs - b * Polynomial<FieldElement>(lhs.getDegree() - b.getDegree(), lhs[lhs.getDegree()] * (b[b.getDegree()].getInverse()));
    }
    return lhs;
}

template <typename FieldElement>
bool operator==(const Polynomial<FieldElement> &a, const Polynomial<FieldElement> &b) {
    return a.coef == b.coef;
}

// TODO: maybe change to regular function instead of operator
template <typename FieldElement>
Polynomial<FieldElement> operator,(const Polynomial<FieldElement> &a, const Polynomial<FieldElement> &b) {
    // std::cout << "GCD-left : " << a << std::endl;
    // std::cout << "GCD-right: " << b << std::endl;
    if (a % b == Polynomial<FieldElement>()) {
        return b;
    } else {
        return (b, a % b);
    }
}

template <typename FieldElement>
std::ostream& operator<<(std::ostream &os, const Polynomial<FieldElement> &poly) {
    os << poly[0];
    for (size_t i = 1; i < poly.getDegree() + 1; ++i) {
        os << " + " << poly[i] << " * x ^ " << i;
    }
    return os;
}

//int main() {
//    try {
//        srand(time(0));
//        PrimeFieldElement<7> a = 1;
//        PrimeFieldElement<7> b = 3;
//        std::cout << a + b << std::endl;
//    } catch (const std::exception &e) {
//        std::cout << e.what() << std::endl;
//    } catch (...) {
//        std::cout << "Unknown exception thrown" << std::endl;
//    }
//    return 0;
//}
