#include <iostream>
#include <typeinfo>

#include <boost/fusion/support/detail/index_sequence.hpp>

using namespace std;
using namespace boost::fusion::detail;

#include "tuple.h"
#include "vector.h"

template<typename R, typename... Args>
vector<R> zip(function<R (Args...)> func)
{
    return make_vector(func());
}

template<typename R, typename... Args>
vector<R> zip(R (*func)(Args...))
{
    return zip(function<R (Args...)>(func));
}

//template<typename F, typename R = decltype(&F::operator()), typename... Args>
template<typename Function, typename... Args, typename Return = typename result_of<Function (Args...)>::type>
vector<Return> zip(Function func)
{
    return zip(function<Return (Args...)>(func));
}


template <typename Function, typename... Args, typename Return = typename result_of<Function& (Args...)>::type>
std::vector<Return> select(Function func, const vector<Return>& c)
{
    std::vector<Return> v;
//    std::transform(std::begin(c), std::end(c), std::back_inserter(v), f);
    return v;
}


template<typename Iterator>
struct iter_range {
    iter_range(Iterator begin, Iterator end) : _begin(begin), _end(end) { }

    Iterator _begin;
    Iterator _end;
};

template<typename Iterator>
iter_range<Iterator> make_iter_range(Iterator begin, Iterator end) {
    return iter_range<Iterator>(begin, end);
}

template<typename... Containers>
auto make_iter_range(Containers&... containers) -> decltype(make_tuple(make_iter_range(containers.begin()..., containers.end()...)))
{
    return make_tuple(make_iter_range(containers.begin()..., containers.end()...));
}


template<typename... Objects>
struct zip_with_ : public tuple<Objects...> {

    zip_with_(Objects... objects) : tuple<Objects...>(objects...) { }

    template<size_t... Indexes>
    tuple<typename Objects::iterator...> iterators(index_sequence<Indexes...>)
    {
        return make_tuple(get<Indexes>(*this).begin()...);
    }

    template<size_t... Indexes>
    void increment(tuple<typename Objects::iterator...>& iters, index_sequence<Indexes...>)
    {
        make_tuple(++get<Indexes>(iters)...);
    }

    template<typename Function, typename Iters, size_t... Indexes, typename Return = typename result_of<Function(typename Objects::value_type...)>::type>
    Return apply(Function function, Iters& iters, index_sequence<Indexes...>)
    {
        return function(*get<Indexes>(iters)...);
    }

    template<size_t Index, typename Iters, typename Conts>
    struct not_at_end {
        bool operator ()(Iters& iters, Conts& objects)
        {
            if (get<Index>(iters) != end(get<Index>(objects))) {
                return not_at_end<Index - 1, Iters, Conts>()(iters, objects);
            }

            return false;
        }
    };

    template<typename Iters, typename Conts>
    struct not_at_end<0, Iters, Conts> {
        bool operator ()(Iters& iters, Conts& objects)
        {
            return get<0>(iters) != end(get<0>(objects));
        }
    };

    template<typename Selector, template<typename ...> class Container = vector, typename Return = typename result_of<Selector(typename Objects::value_type...)>::type>
    //template<typename Selector, typename Return = typename result_of<Selector(typename Objects::value_type...)>::type>
    Container<Return> operator ()(Selector selector)
    {
        Container<Return> container;

        auto sequence = make_index_sequence<sizeof...(Objects)>();
        auto iters = iterators(sequence);

        //while (get<0>(iters) != end(get<0>(*this)))
        while (not_at_end<sizeof...(Objects) - 1, decltype(iters), tuple<Objects...>>()(iters, *this))
        {
            container.emplace_back(apply(selector, iters, sequence));

            increment(iters, sequence);
        }

        return container;
    }

};

template<typename Function, typename Iters, size_t... Indexes>
void each2(Function function, const Iters& iters, index_sequence<Indexes...>)
{
    make_tuple(function(get<Indexes>(iters))...);
}

template<typename Function, typename Iters, size_t... Indexes>
void each2(Function function, Iters iters)
{
    each2(function, iters, make_index_sequence<tuple_size<Iters>::value>());
}

// template<int Index, typename F, typename Tuple>
// struct each_ {
//     void operator()(F f, const Tuple& t) {
//         each_<Index - 1, F, Tuple>()(f, t);

//         f(std::get<Index>(t));
//     }
// };

// template<typename Function, typename Tuple>
// struct each_<0, Function, Tuple> {
//     void operator()(Function function, const Tuple& tuple) {
//         function(std::get<0>(tuple));
//     }
// };

// template<typename F, typename Tuple>
// void each(F f, const Tuple& t) {
//     each_<tuple_size<Tuple>::value - 1, F, Tuple>()(f, t);
// }


int main(int argc, const char* argv[])
{
    cout << make_vector(make_tuple(1, 2, 3)) << endl;

    zip([]() { cout << "here" << endl; return 0; });

    select([](int) { return 0; }, vector<int>());

    //map([](int i) { cout << "here" << endl; return 0; }, make_vector(1, 2, 3));

    //zip_with([](int i) { return i; }, make_vector(1, 2, 3));

    cout << endl << "zip_with" << endl;
    cout << zip_with_<vector<int>, vector<char>>(make_vector(1, 2, 3), make_vector('a', 'b', 'c', 'd')) ([](int i, char c) { return i + c; }) << endl;

    cout << endl << "each" << endl;
//    each([](int i) { cout << i << endl; }, make_tuple(1, 2, 3));
    each2([](int i) { cout << i << endl; return nullptr; }, make_tuple(1, 2, 3));

    return 0;
}
