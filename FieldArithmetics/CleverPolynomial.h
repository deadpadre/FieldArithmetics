#pragma once

#include "Basic.h"
#include <map>

template<typename F>
class PolynomialRing : public Ring<Group<std::map<size_t, Element<F>>>, Group<std::map<size_t, Element<F>>>>  {
public:
	typedef typename std::map<size_t, Element<F> > IDType;


	virtual IDType multiply(const IDType& lhs, const IDType& rhs) const override {
		std::map<size_t, Element<F> > answer;
		for (auto& i : lhs) {
			for (auto& j : rhs) {
				answer[i.first + j.first] += i.second * j.second;
			}
		}
		return answer;
	}
	virtual IDType getReciprocal(const IDType& index) const override {
		throw std::domain_error("Attempting to get polynomial reciprocal");
	}
	virtual IDType getIdentity() const override {
		std::map<size_t, Element<F> > answer;
		answer[0] = ContainerBuilder<F>::getContainer().getIdentity();
		return answer;
	}

	virtual IDType add(const IDType& lhs, const IDType& rhs) const override {
		std::map<size_t, Element<F> > answer;
		for (auto& i : lhs) {
			answer[i.first] += i.second;
		}
		for (auto& i : rhs) {
			answer[i.first] += i.second;
		}
		return answer;
	}
	virtual IDType getNegative(const IDType& index) const override {
		std::map<size_t, Element<F> > answer;
		for (auto& i : index) {
			answer[i.first] = (-i.second);
		}
		return answer;
	}
	virtual IDType getZero() const override {
		std::map<size_t, Element<F> > answer;
		return answer;
	}

	virtual size_t getSize() const override {
		return -1;
	}
};

template<typename F>
class Polynom : public BaseElement<PolynomialRing<F>, Polynom<F>> {
public:
	Polynom() : BaseElement<PolynomialRing<F>, Polynom<F>>() {}
	Polynom(const typename PolynomialRing<F>::IDType& value) : BaseElement<PolynomialRing<F>, Polynom<F>>(value) {}
	const Element<F>& operator[](size_t i) const { return id[i]; }
	Element<F>& operator[](size_t i) { return id[i]; }
};

template<typename F>
std::ostream& operator<<(std::ostream& srm, const Polynom<F>& elem) {
	for (auto& i : elem.getId()) {
		srm << i.second << "@" << i.first << ' ';
	}
	return srm;
}