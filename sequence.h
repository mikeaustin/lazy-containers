//
// sequence.h
//

#ifndef IMPULSE_SEQUENCE_H
#define IMPULSE_SEQUENCE_H

#include "tuple.h"

//
// class sequence
//

template<template<typename ...> class Iterator, typename Parameter, typename Container>
class sequence {

  public:

    using const_iterator = Iterator<Parameter, Container>;
    using value_type     = typename Container::value_type;

    // Constructors

    sequence(const Container& container, const Parameter& parameter)
     : _perameter(parameter), _container(container) { }

    sequence(const Container&& container, const Parameter& parameter)
     : _container(std::move(container)), _perameter(parameter) { }

    // Iterators

    Iterator<Parameter, Container> begin() const
    { return Iterator<Parameter, Container>(_perameter, std::begin(_container)); }

    Iterator<Parameter, Container> end() const
    { return Iterator<Parameter, Container>(_perameter, std::end(_container)); }

    // Conversion

    operator std::vector<typename Container::value_type>() const
    {
        std::vector<typename Container::value_type> result;

        for (auto iter = begin(), iter_end = end(); iter != iter_end; ++iter)
        {
            result.emplace_back(*iter);
        }

        return result;
    }

  private:

    const Parameter _perameter;
    const Container& _container;

};

//
// class transform_iterator
//

template<typename Function, typename Container>
class transform_iterator {

  public:

    using value_type     = typename Container::value_type;

    // Constructors

    transform_iterator(Function function, typename Container::const_iterator iterator)
     : _function(function), _iterator(iterator) { }

    // Operators

    bool operator !=(const transform_iterator& other) const { return _iterator != other._iterator; }

    transform_iterator& operator ++() { ++_iterator; return *this; }

    typename Container::value_type operator *() const
    {
        return iterator_apply(_function, _iterator);
        //return _function(*_iterator); // Might be a value or a tuple
    }

  private:

    typename Container::const_iterator _iterator;
    const Function _function;

};


template<template<typename ...> class Iterator, typename Container, typename Parameter>
std::ostream& operator <<(std::ostream& stream, const sequence<Iterator, Parameter, Container>& sequence)
{
    stream << "[";

    for (auto iter = begin(sequence), iter_begin = iter, iter_end = end(sequence); iter != iter_end; ++iter)
    {
        if (iter != iter_begin) stream << ", ";

        stream << *iter;
    }

    stream << "]";

    return stream;
}

#endif
