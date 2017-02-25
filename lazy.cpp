#include <iostream>
#include <vector>
#include <tuple>

#include <boost/fusion/support/detail/index_sequence.hpp>

using std::cout;
using std::endl;
using std::vector;
using std::tuple;
using std::get;
using std::result_of;
using std::make_tuple;
using std::tuple_element;
using std::tuple_size;
using std::remove_reference;
using std::initializer_list;
using boost::fusion::detail::index_sequence;
using boost::fusion::detail::make_index_sequence;

#include "tuple.h"
#include "vector.h"
#include "sequence.h"
#include "parallel.h"


//template<typename Container, typename Function, typename Element = typename std::result_of<Function(typename Container::value_type)>::type>
template<typename Container, typename Function>
sequence<transform_iterator, Function, Container> transform(const Function& function, const Container& container)
{
    return sequence<transform_iterator, Function, Container>(container, function);
}


template<typename Object, typename Method, typename Result = typename std::result_of<Method(Object)>::type>
Result operator | (const Object& object, const Method& method)
{
    return method(object);
}

template<typename Container, typename Selector, typename... Args>
auto map_impl(const tuple<Container, Selector>& objects, const tuple<Args...>& arguments)
-> decltype(sequence<transform_iterator, Selector, Container>(get<0>(objects), get<0>(arguments)))
{
    return sequence<transform_iterator, Selector, Container>(get<0>(objects), get<0>(arguments));
}

template<typename Selector>
struct map_functor {

    map_functor(const Selector& selector)
     : _selector(selector)
    { }

    //template<typename Container, typename Element = typename std::result_of<Selector(typename Container::value_type)>::type>
    template<typename Container>
    sequence<transform_iterator, Selector, Container> operator ()(const Container& container) const
    {
        return sequence<transform_iterator, Selector, Container>(container, _selector);
    }

    Selector _selector;

};

template<typename Selector>
map_functor<Selector> map(const Selector& selector)
{
    return map_functor<Selector>(selector);
}


int main(int argc, const char* argv[])
{
    std::vector<int> v1 = { 1, 2, 3 };
    std::vector<char> v2 = { 'a', 'b', 'c' };

    auto v3 = transform([](int i) { return i * i; }, v1);

    //transform(make_tuple(v1, v2), [](int i, int j) { return i * i; });

    std::cout << (make_vector(1, 2, 3) | map ([](int i) { return i * i; })) << std::endl;

    cout << v3 << endl;

    std::vector<int> v4(v3);

    cout << v4 << endl;

    //auto v6 = parallel_container<vector<int>, vector<int>>(make_tuple(v1, v2));

    //auto v7 = transform([](int i, int j) { return i + j; }, v6);

    //cout << v6 << endl;

    cout << apply([](int i, int j) { return i + j; }, make_tuple(2, 3)) << endl;

    cout << "=====" << endl;

    auto containers = make_tuple(v1, v2);
    parallel_iterator<vector<int>::const_iterator, vector<char>::const_iterator> iterators = begin(containers), iter_ends = end(containers);

    for (; iterators != iter_ends; ++iterators)
    {
        cout << *iterators << ", ";
    }
    cout << endl;

    parallel_container<vector<int>, vector<char>> parallel(make_tuple(v1, v2));

    for (auto iter = begin(parallel), iter_end = end(parallel); iter != iter_end; ++iter)
    {
        cout << *iter << ", ";
    }
    cout << endl;

    // auto selector = [](int i, int j) { return i + j; };
    // sequence<transform_iterator, decltype(selector), decltype(parallel)> seq(parallel, selector);
    // cout << seq << endl;

    return 0;
}

//         parallel_container<Container> parallel(container);
