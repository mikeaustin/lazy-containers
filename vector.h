//
// vector.h
//

#include <vector>

template<typename T, typename... Ts>
std::vector<T> make_vector(const T&& value, const Ts&&... rest)
{
    return std::vector<T> { value, rest... };
}

template<typename T>
std::vector<T> make_vector()
{
    return std::vector<T>();
}

template<typename T>
std::ostream& operator <<(std::ostream& stream, const std::vector<T>& value) {
    stream << "[";

    for (auto iter = begin(value); iter != end(value); ++iter)
    {
        if (iter != begin(value)) stream << ", ";

        stream << *iter;
    }

    stream << "]";

    return stream;
}
