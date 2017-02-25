//
// tuple.h
//

#include <tuple>

#ifndef IMPULSE_TUPPLE_H
#define IMPULSE_TUPPLE_H

// Base case
template<typename Lhs, typename Rhs>
std::tuple<Lhs&, Rhs&> operator ,(const Lhs& lhs, const Rhs& rhs)
{
    return std::make_tuple(lhs, rhs);
}

// General case when we already have a tuple
template<typename... Lhs, typename Rhs>
std::tuple<Lhs..., Rhs&> operator ,(const std::tuple<Lhs...>&& lhs, const Rhs& rhs)
{
    return std::tuple_cat(lhs, std::make_tuple(rhs));
}

// template<typename Function, typename Tuple, size_t... Indexes,
//          typename Result = typename result_of<Function(typename tuple_element<Indexes, Tuple>::type...)>::type>
// Result apply_impl(const Function& function, const Tuple& tuple, index_sequence<Indexes...>)
// {
//     return function(get<Indexes>(tuple)...);
// }

template<typename Function, typename... Args, size_t... Indexes,
         typename Result = typename result_of<Function(Args...)>::type>
Result apply_impl(const Function& function, const tuple<Args...>& tuple, index_sequence<Indexes...>)
{
    return function(get<Indexes>(tuple)...);
}

//template<typename Function, typename... Tuple, typename Result = typename result_of<Function(typename Tuple::value_type...)>::type>
//template<typename Function, typename Tuple, size_t... Indexes, typename Result = typename result_of<Function(int)>::type>
//Result apply(const Function& function, const tuple<Tuple...>& tuple)

// template<typename Function, typename Tuple>
// auto apply(const Function& function, const Tuple& tuple) -> decltype(apply_impl(function, tuple, make_index_sequence<tuple_size<Tuple>::value>()))
// {
//     return apply_impl(function, tuple, make_index_sequence<tuple_size<Tuple>::value>());
// }


template<typename Function, typename... Args, typename Result = typename result_of<Function(Args...)>::type>
Result apply(const Function& function, const tuple<Args...>& args)
{
    //const size_t size = tuple_size< tuple<Args...> >::value;
    const size_t size = sizeof...(Args);

    return apply_impl(function, args, make_index_sequence<size>());
}

template<typename Function, typename... Args, typename Result = typename result_of<Function(Args...)>::type>
Result apply(const Function& function, const Args&... args)
{
    //const size_t size = tuple_size< tuple<Args...> >::value;
    const size_t size = sizeof...(Args);

    return apply_impl(function, make_tuple(args...), make_index_sequence<size>());
}

template<typename Function, typename Iterator, typename Result = typename result_of<Function(typename Iterator::value_type)>::type>
Result iterator_apply(const Function& function, const Iterator& iterator)
{
    return function(*iterator);
}


template<int index, typename... Ts>
struct print_tuple {
    void operator ()(std::ostream& stream, const std::tuple<Ts...>& values)
    {
        print_tuple<index - 1, Ts...>()(stream, values);

        stream << ", " << std::get<index>(values);
    }
};

template<typename... Ts>
struct print_tuple<0, Ts...> {
    void operator ()(std::ostream& stream, const std::tuple<Ts...>& values)
    {
        stream << std::get<0>(values);
    }
};

template<typename... Iterators>
std::ostream& operator <<(std::ostream& stream, const std::tuple<Iterators...>& values)
{
    //make_index_sequence<sizeof...(Iterators)>()

    stream << "(", print_tuple<std::tuple_size<std::tuple<Iterators...>>::value - 1, Iterators...>()(stream, values), stream << ")";

    return stream;
}

#endif
