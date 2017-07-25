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

class Field : public Group {
public:
	virtual size_t mulOp(size_t lhs, size_t rhs) const = 0;
	virtual size_t addOp(size_t lhs, size_t rhs) const = 0;
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
	size_t getSize() const { return getContainer().getSize(); }

	// operations
	Element<Container>& operator*=(const Element<Container>& other) {
		id = getContainer().mulOp(id, other.id);
		return *this;
	}
	friend Element<Container> operator*(Element<Container> lhs, const Element<Container>& rhs) {
		return lhs *= rhs;
	}
	Element<Container>& operator+=(const Element<Container>& other) {
		id = getContainer().addOp(id, other.id);
		return *this;
	}
	friend Element<Container> operator+(Element<Container> lhs, const Element<Container>& rhs) {
		return lhs += rhs;
	}
	Element<Container> getInverse() const {
		return Element<Container>(getContainer().getInverse(id));
	}
};

template<typename Container>
std::ostream& operator<<(std::ostream& srm, const Element<Container>& elem) {
	return (srm << elem.getId() << " @ " << elem.getSize());
}