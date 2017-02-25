//
// parallel.h
//

#ifndef IMPULSE_PARALLEL_H
#define IMPULSE_PARALLEL_H

//
// parallel_iterator
//

template<typename... Iterators>
class parallel_iterator {

  public:

    using value_type = tuple<typename Iterators::value_type...>;

    // Constructors

    //parallel_iterator(Iterators... iterators) : _iterators(make_tuple(iterators...)) { }
    parallel_iterator(const tuple<Iterators...>& iterators) : _iterators(iterators) { }

    // Operators

    template<typename... Others>
    bool operator !=(const parallel_iterator& others) const { return !equal(others._iterators, make_index_sequence<sizeof...(Iterators)>()); }

    parallel_iterator& operator ++() { increment(make_index_sequence<sizeof...(Iterators)>()); return *this; }

    tuple<typename Iterators::value_type...> operator *() const { return dereference(make_index_sequence<sizeof...(Iterators)>()); }

  //protected:

    template<size_t... Indexes>
    bool equal(const tuple<Iterators...>& iter_ends, index_sequence<Indexes...>) const
    {
        int equal = false;

        initializer_list<int> result = { (equal |= get<Indexes>(_iterators) == get<Indexes>(iter_ends))... };

        return equal;
    }

    template<size_t... Indexes>
    void increment(index_sequence<Indexes...>)
    {
        initializer_list<int> result = { ((void) ++get<Indexes>(_iterators), 0)... };
    }

    template<size_t... Indexes>
    tuple<typename Iterators::value_type...> dereference(index_sequence<Indexes...>) const
    {
        return make_tuple(*get<Indexes>(_iterators)...);
    }

  //private:

    typename std::tuple<Iterators...> _iterators;

};

//
// parallel_container
//

template<typename... Containers>
class parallel_container {

  public:

    using const_iterator = parallel_iterator<typename Containers::const_iterator...>;
    using value_type     = tuple<typename Containers::value_type...>;

    parallel_container(const tuple<Containers...>& containers)
     : _containers(containers)
    { }

    template<size_t... Indexes>
    const_iterator begin(index_sequence<Indexes...>) const
    {
        return make_tuple(std::begin(get<Indexes>(_containers))...);
    }

    const_iterator begin() const
    {
        //return begin(make_index_sequence<sizeof...(Containers)>());
        const size_t size = tuple_size<tuple<Containers...>>::value;

        return begin(make_index_sequence<size>());
    }

    template<size_t... Indexes>
    const_iterator end(index_sequence<Indexes...>) const
    {
        return make_tuple(std::end(get<Indexes>(_containers))...);
    }

    const_iterator end() const
    {
        //return end(make_index_sequence<sizeof...(Containers)>());
        const size_t size = tuple_size<tuple<Containers...>>::value;

        return end(make_index_sequence<size>());
    }

    const tuple<Containers...> _containers;

};

template<typename Function, typename... Iterator, typename Result = typename result_of<Function(typename Iterator::value_type...)>::type>
Result iterator_apply(const Function& function, const parallel_iterator<Iterator...>& iterator)
{
    return apply_impl(function, *iterator, make_index_sequence<sizeof...(Iterator)>());
}


template<typename... Containers, size_t... Indexes>
tuple<typename remove_reference<Containers>::type::const_iterator...> begin(const tuple<Containers...>& containers, index_sequence<Indexes...>)
{
    return make_tuple(begin(get<Indexes>(containers))...);
}

template<typename... Containers>
tuple<typename remove_reference<Containers>::type::const_iterator...> begin(const tuple<Containers...>& containers)
{
    return begin(containers, make_index_sequence<sizeof...(Containers)>());
}

template<typename... Containers, size_t... Indexes>
tuple<typename remove_reference<Containers>::type::const_iterator...> end(const tuple<Containers...>& containers, index_sequence<Indexes...>)
{
    return make_tuple(end(get<Indexes>(containers))...);
}

template<typename... Containers>
tuple<typename remove_reference<Containers>::type::const_iterator...> end(const tuple<Containers...>& containers)
{
    return end(containers, make_index_sequence<sizeof...(Containers)>());
}

#endif
