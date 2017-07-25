#pragma once
#include <iostream>

//-------------------------------
// Semigroup and Group

class Semigroup {
public:
	virtual size_t getSize() const = 0;
	virtual ~Semigroup() {};
	virtual size_t op(size_t lhs, size_t rhs) const = 0;
};

class Group: public Semigroup {
public:
	virtual size_t getInverse(size_t index) const = 0;
	virtual size_t getNeutral() const = 0;
};

// factory constructing singleton objects

template<typename Container>
class ContainerBuilder {
public:
	static const Container& getContainer() {
		static Container container;
		return container;
	}
};

//-----------------------------
// operator usage enabling wrappers

template<typename Container>
class Multiplicative: public Container {
	size_t op(size_t lhs, size_t rhs) const override {
		return multiply(lhs, rhs);
	}
	size_t getInverse(size_t index) const override {
		return getReciprocal(index);
	}
	size_t getNeutral() const override {
		return getIdentity();
	}

public:
	static size_t mulOp(size_t lhs, size_t rhs) {
		return ContainerBuilder<Multiplicative<Container> >::getContainer().op(lhs, rhs);
	}
	virtual size_t multiply(size_t lhs, size_t rhs) const {
		return Container::op(lhs, rhs);
	}
	virtual size_t getReciprocal(size_t index) const {
		return Container::getInverse(index);
	}
	virtual size_t getIdentity() const {
		return Container::getNeutral();
	}
	typedef Multiplicative<Container> ContainerType;
};

template<typename Container>
class Additive : public Container {
	size_t op(size_t lhs, size_t rhs) const override {
		return add(lhs, rhs);
	}
	size_t getInverse(size_t index) const override {
		return getNegative(index);
	}
	size_t getNeutral() const override {
		return getZero();
	}

public:
	static size_t addOp(size_t lhs, size_t rhs) {
		return ContainerBuilder<Additive<Container> >::getContainer().op(lhs, rhs);
	}
	virtual size_t add(size_t lhs, size_t rhs) const {
		return Container::op(lhs, rhs);
	}
	virtual size_t getNegative(size_t index) const {
		return Container::getInverse(index);
	}
	virtual size_t getZero() const {
		return Container::getNeutral();
	}
	typedef Additive<Container> ContainerType;
};

//---------------------------
// Field

template<typename MGroup, typename AGroup>
class Field : public Multiplicative<MGroup>, public Additive<AGroup> {
public:
	/* In order to manually set field ops, you can choose abstract Group class as
	both template parameters and override these methods
		Other option is to inherit from proper groups.

	virtual size_t multiply(size_t lhs, size_t rhs);
	virtual size_t getReciprocal(size_t index);
	virtual size_t getIdentity();

	virtual size_t add(size_t lhs, size_t rhs);
	virtual size_t getNegative(size_t index);
	virtual size_t getZero();

	In case if you're inheriting from group, in order to use Element class,
	you shold typedef type of your field into ContainerType
	*/
	size_t getSize() const {
		return AGroup::getSize();
	}
	typedef Field<MGroup, AGroup> ContainerType;
};

// dual opeartion wrappers are not present since
// field is its own wrapper

//-----------------------------
// Elements

template<typename Wrapper>
class Element {
protected:
	size_t id;
public:
	Element() {}
	Element(size_t _id) : id(_id) {}
	size_t getId() const { return id; }
	size_t getSize() const { return ContainerBuilder<Wrapper::ContainerType>::getContainer().getSize(); }

	// operations
	Element<Wrapper>& operator*=(const Element<Wrapper>& other) {
		id = Wrapper::mulOp(id, other.id);
		return *this;
	}
	friend Element<Wrapper> operator*(Element<Wrapper> lhs, const Element<Wrapper>& rhs) {
		return lhs *= rhs;
	}
	Element<Wrapper>& operator+=(const Element<Wrapper>& other) {
		id = Wrapper::addOp(id, other.id);
		return *this;
	}
	friend Element<Wrapper> operator+(Element<Wrapper> lhs, const Element<Wrapper>& rhs) {
		return lhs += rhs;
	}
	Element<Wrapper>& operator/=(const Element<Wrapper>& other) {
		id = Wrapper::mulOp(id, ContainerBuilder<Wrapper::ContainerType>::getContainer().getReciprocal(other.id));
		return *this;
	}
	friend Element<Wrapper> operator/(Element<Wrapper> lhs, const Element<Wrapper>& rhs) {
		return lhs /= rhs;
	}
	Element<Wrapper>& operator-=(const Element<Wrapper>& other) {
		id = Wrapper::addOp(id, ContainerBuilder<Wrapper::ContainerType>::getContainer().getNegative(other.id));
		return *this;
	}
	friend Element<Wrapper> operator-(Element<Wrapper> lhs, const Element<Wrapper>& rhs) {
		return lhs -= rhs;
	}
};

template<typename Wrapper>
std::ostream& operator<<(std::ostream& srm, const Element<Wrapper>& elem) {
	return (srm << elem.getId() << " @ " << elem.getSize());
}