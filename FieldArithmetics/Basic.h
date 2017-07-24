#pragma once
#include <iostream>

//-------------------------------
// Semigroup and Group

class Semigroup {
public:
	virtual size_t getSize() const = 0;
	virtual ~Semigroup() {};
	virtual size_t op(size_t lhs, size_t rhs) const = 0;

	virtual size_t mulOp(size_t lhs, size_t rhs) const { return op(lhs, rhs); }
	virtual size_t addOp(size_t lhs, size_t rhs) const { return op(lhs, rhs); }
};

class Group: public Semigroup {
public:
	virtual size_t getInverse(size_t index) const = 0;
	virtual size_t getNeutral() const = 0;
};

//-----------------------------
// Elements

template<typename Container>
class Element {
protected:
	size_t id;
	const Container& getContainer() const {
		static Container container;
		return container;
	}
public:
	Element() {}
	Element(size_t _id) : id(_id) {}
	size_t getId() const { return id; }
	Element<Container> getInverse() const {
		return Element<Container>(getContainer().getInverse(id));
	}
	size_t getSize() const { return getContainer().getSize(); }
};

template<typename Container>
std::ostream& operator<<(std::ostream& srm, const Element<Container>& elem) {
	return (srm << elem.getId() << " @ " << elem.getSize());
}


// Multiplicative

template<typename Container>
class MultiplicativeElement : public virtual Element<Container> {
public:
	MultiplicativeElement(size_t _id) : Element<Container>(_id) {}
	MultiplicativeElement<Container>& operator+=(const MultiplicativeElement<Container>& other) {
		id = getContainer().mulOp(id, other.id);
		return *this;
	}
	friend MultiplicativeElement<Container> operator+(MultiplicativeElement<Container> lhs, const MultiplicativeElement<Container>& rhs) {
		return lhs += rhs;
	}
};

// Additive

template<typename Container>
class AdditiveElement : public virtual Element<Container> {
public:
	AdditiveElement(size_t _id) : Element<Container>(_id) {}
	AdditiveElement<Container>& operator+=(const AdditiveElement<Container>& other) {
		id = getContainer().addOp(id, other.id);
		return *this;
	}
	friend AdditiveElement<Container> operator+(AdditiveElement<Container> lhs, const AdditiveElement<Container>& rhs) {
		return lhs += rhs;
	}
};
