#pragma once
#include <iostream>

//-------------------------------
// Semigroup and Group

template<typename ID = size_t>
class Semigroup {
public:
	virtual size_t getSize() const = 0;
	virtual ~Semigroup() {};
	virtual ID op(const ID& lhs, const ID& rhs) const = 0;

	typedef ID IDType;
};

template<typename ID = size_t>
class Group: public Semigroup<ID> {
public:
	virtual ID getInverse(const ID& index) const = 0;
	virtual ID getNeutral() const = 0;

	typedef ID IDType;
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
public:
	typedef typename Container::IDType IDType;
private:
	IDType op(const IDType& lhs, const IDType& rhs) const override {
		return multiply(lhs, rhs);
	}
	IDType getInverse(const IDType& index) const override {
		return getReciprocal(index);
	}
	IDType getNeutral() const override {
		return getIdentity();
	}

public:
	virtual IDType multiply(const IDType& lhs, const IDType& rhs) const {
		return Container::op(lhs, rhs);
	}
	virtual IDType getReciprocal(const IDType& index) const {
		return Container::getInverse(index);
	}
	virtual IDType getIdentity() const {
		return Container::getNeutral();
	}
	virtual size_t getSize() const {
		return Container::getSize();
	}
};

template<typename Container>
class Additive : public Container {
public:
	typedef typename Container::IDType IDType;
private:
	IDType op(const IDType& lhs, const IDType& rhs) const override {
		return this->add(lhs, rhs);
	}
	IDType getInverse(const IDType& index) const override {
		return this->getNegative(index);
	}
	IDType getNeutral() const override {
		return this->getZero();
	}

public:
	virtual IDType add(const IDType& lhs, const IDType& rhs) const {
		return Container::op(lhs, rhs);
	}
	virtual IDType getNegative(const IDType& index) const {
		return Container::getInverse(index);
	}
	virtual IDType getZero() const {
		return Container::getNeutral();
	}
	virtual size_t getSize() const {
		return Container::getSize();
	}
};

//---------------------------
// Field

template<typename MGroup, typename AGroup>
class Field : public Multiplicative<MGroup>, public Additive<AGroup> {
public:
	typedef typename Additive<AGroup>::IDType IDType;
	typedef MGroup	MultGroup;
	typedef AGroup	AddGroup;
	/* In order to manually set field ops, you can choose abstract Group class as
	both template parameters and override these methods
		Other option is to inherit from proper groups.

	virtual IDType multiply(const IDType& lhs, const IDType& rhs);
	virtual IDType getReciprocal(const IDType& index);
	virtual IDType getIdentity();

	virtual IDType add(const IDType& lhs, const IDType& rhs);
	virtual IDType getNegative(cosnt IDType& index);
	virtual IDType getZero();

	*/
	size_t getSize() const {
		return AGroup::getSize();
	}
};

// sustaining the difference between field and ring has been effectively layed upon users' shoulders
template<typename MGroup, typename AGroup>
using Ring = Field<MGroup, AGroup>;

// dual opeartion wrappers are not present since
// field is its own wrapper

//-----------------------------
// Elements

template<typename Wrapper, typename Derived>
class BaseElement {
public:
	typedef typename Wrapper::IDType IDType;
protected:
	IDType id;
	const Wrapper& container;
public:
	BaseElement() : id(), container(ContainerBuilder<Wrapper>::getContainer()) {}
	BaseElement(const IDType& _id) : id(_id), container(ContainerBuilder<Wrapper>::getContainer()) {}
	BaseElement(const BaseElement<Wrapper, Derived>& other) : id(other.id), container(other.container) {}
	Derived& operator=(const BaseElement<Wrapper, Derived>& other) {
		id = other.id;
		return *static_cast<Derived*>(this);
	}

	IDType getId() const { return id; }
	size_t getSize() const { return container.getSize(); }

	// operations
	Derived& operator*=(const BaseElement<Wrapper, Derived>& other) {
		id = container.multiply(id, other.id);
		return *static_cast<Derived*>(this);
	}
	Derived& operator+=(const BaseElement<Wrapper, Derived>& other) {
		id = container.add(id, other.id);
		return *static_cast<Derived*>(this);
	}
	Derived& operator/=(const BaseElement<Wrapper, Derived>& other) {
		id = container.multiply(id, container.getReciprocal(other.id));
		return *static_cast<Derived*>(this);
	}
	Derived& operator-=(const BaseElement<Wrapper, Derived>& other) {
		id = container.add(id, container.getNegative(other.id));
		return *static_cast<Derived*>(this);
	}
	friend Derived operator*(BaseElement<Wrapper, Derived> lhs, const BaseElement<Wrapper, Derived>& rhs) { return lhs *= rhs; }
	friend Derived operator+(BaseElement<Wrapper, Derived> lhs, const BaseElement<Wrapper, Derived>& rhs) { return lhs += rhs; }
	friend Derived operator/(BaseElement<Wrapper, Derived> lhs, const BaseElement<Wrapper, Derived>& rhs) { return lhs /= rhs; }
	friend Derived operator-(BaseElement<Wrapper, Derived> lhs, const BaseElement<Wrapper, Derived>& rhs) { return lhs -= rhs; }
	Derived operator-() const {
		return *static_cast<Derived*>(&(BaseElement<Wrapper, Derived>(container.getZero()) - *this));
	}
};

template<typename Wrapper>
class Element : public BaseElement<Wrapper, Element<Wrapper>> {
public:
	typedef typename Wrapper::IDType IDType;
	Element() : BaseElement<Wrapper, Element<Wrapper>>() {}
	Element(const IDType& _id) : BaseElement<Wrapper, Element<Wrapper>>(_id) {}
	Element(const Element<Wrapper>& other) : BaseElement<Wrapper, Element<Wrapper>>(other) {}
};

template<typename Wrapper>
std::ostream& operator<<(std::ostream& srm, const Element<Wrapper>& elem) {
	return (srm << "(" << elem.getId() << "#" << elem.getSize() << ")");
}

template<typename ID>
inline size_t Semigroup<ID>::getSize() const
{
	return size_t();
}

template<typename ID>
inline ID Semigroup<ID>::op(const ID & lhs, const ID & rhs) const
{
	return ID();
}

template<typename ID>
inline ID Group<ID>::getInverse(const ID & index) const
{
	return ID();
}

template<typename ID>
inline ID Group<ID>::getNeutral() const
{
	return ID();
}
