#include <iostream>
#include <typeinfo>

#include <boost/fusion/support/detail/index_sequence.hpp>

using namespace std;
using namespace boost::fusion::detail;

#include "tuple.h"
#include "vector.h"
#include "parallel.h"
#include "sequence.h"


//template<typename... Containers, typename Function, template<typename ...> class Container = vector,
template<typename... Containers, typename Function,
         typename Element = typename result_of<Function(typename remove_reference<Containers>::type::value_type...)>::type>
vector<Element> map(const tuple<Containers...>& containers, const tuple<Function>& arguments)
{
    //return sequence<transform_iterator, Function, tuple<Containers...>>(containers, get<0>(arguments));

    vector<Element> result;

    parallel_iterator<typename remove_reference<Containers>::type::const_iterator...> iterators = begin(containers), iter_ends = end(containers);

    while (iterators != iter_ends)
    {
        //auto value = iterators.apply(get<0>(arguments));
        auto value = iterator_apply(get<0>(arguments), iterators);

        result.emplace_back(value);

        ++iterators;        
    }

    return result;
}

//void test(const tuple<vector<int>&>& v) { }
template<typename... T, typename F, typename E = typename result_of<F(typename remove_reference<T>::type::value_type...)>::type>
void test(const tuple<T...>& v, const F& function)
{
    cout << typeid(decltype(get<0>(v))).name() << endl;
    cout << typeid(decltype(get<1>(v))).name() << endl;
}

int main(int argc, const char* argv[])
{
    auto v1 = make_vector(1, 2, 3);
    auto v2 = make_vector(1, 2, 3);

    //std::tuple<int> x = (1, 2);

    //cout << map(make_tuple(v1, make_vector('a', 'b', 'c')), make_tuple([](int i, char c) { return make_tuple(i, c); })) << endl;
    cout << map(make_tuple(make_vector(1, 2, 3), make_vector('a', 'b', 'c')), make_tuple([](int i, char c) { return make_tuple(i, c); })) << endl;
    cout << map(make_tuple(ref(v1), make_vector('a', 'b', 'c')), make_tuple([](int i, char c) { return make_tuple(i, c); })) << endl;

    //test(make_tuple(ref(v1)));
    //test(tie(v1));

    test(make_tuple(ref(v1), make_vector(1, 2, 3)), [](int i, int j) { });

    return 0;
}
