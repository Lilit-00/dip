#include <numeric>
#include <exception>

#include "fraction.h"

namespace my {

template<typename Type, size_t csize = 2>
class dynamic_matrix 
{
public:
    dynamic_matrix();
    ~dynamic_matrix();

    Type get(Fraction m, Fraction n) const;

    void  put(Fraction m, Fraction n, const Type& value);
    inline bool is_exist(const Fraction& m, const Fraction& n) const;

    void clear();
private:
    inline void assert_exist_index(const Fraction& m, const Fraction& n) const;
    bool is_gcdr_of(long x, long y) const;

    bool is_pow_of(long x, long y) const;

    struct grid 
    {
        Type value = {};
        grid *matrix[csize][csize] = { nullptr };
        ~grid();
    } * _grid;
};

// --------------------------implementation---------------------------------

template<typename Type, size_t csize>
dynamic_matrix<Type, csize>::dynamic_matrix()
    : _grid(new grid) 
{}

template<typename Type, size_t csize>
dynamic_matrix<Type, csize>::~dynamic_matrix() {
    delete _grid;
}

template<typename Type, size_t csize>
dynamic_matrix<Type, csize>::grid::~grid() {
    for(size_t i = 0; i < csize; ++i)
        for (size_t j = 0; j < csize; ++j)
            delete matrix[i][j];
}

template<typename Type, size_t csize>
Type dynamic_matrix<Type, csize>::get(Fraction m, Fraction n) const {
    assert_exist_index(m, n);
    grid* p_grid = _grid;
    while(p_grid != nullptr && (m - floor(m) || n - floor(n))) {
        p_grid = p_grid->matrix[floor(m)][floor(n)];
        m = csize * (m - floor(m)); m.reduce();
        n = csize * (n - floor(n)); n.reduce();
    }
    if (p_grid == nullptr)
        return Type{};
    p_grid = p_grid->matrix[(long)m][(long)n];
    return p_grid == nullptr ? Type{} : p_grid->value;
}

template<typename Type, size_t csize>
void  dynamic_matrix<Type, csize>::put(Fraction m, Fraction n, const Type& value) {
    assert_exist_index(m, n);
    grid* p_grid = _grid;
    while(m - floor(m) || n - floor(n)) {
        if (p_grid->matrix[floor(m)][floor(n)] == nullptr)
            p_grid->matrix[floor(m)][floor(n)] = new grid;
        p_grid = p_grid->matrix[floor(m)][floor(n)];
        m = csize * (m - floor(m)); m.reduce();
        n = csize * (n - floor(n)); n.reduce();
    }
    if(p_grid->matrix[(long)m][(long)n] == nullptr) {
        p_grid->matrix[(long)m][(long)n] = new grid{value};
    } else {
        p_grid->matrix[(long)m][(long)n]->value = value;
    }
}

template<typename Type, size_t csize>
void dynamic_matrix<Type, csize>::clear() {
    this->~dynamic_matrix<Type, csize>();
    _grid = new grid;
}

template<typename Type, size_t csize>
bool dynamic_matrix<Type, csize>::is_gcdr_of(long x, long y) const {
    long gcd = std::gcd(x, y);
    while(gcd != 1) {
        y /= gcd;
        gcd = std::gcd(x, y);
    }
    return y == 1;
}

template<typename Type, size_t csize>
bool dynamic_matrix<Type, csize>::is_pow_of(long x, long y) const {
    if (x <= 1 || y <= 1) 
        return x == 1 && y == 1;

    const long cx = x;
    while (y % x == 0) {
        y /= x;
        x = cx;
        while(y / (x * x) > 1)
            x *= x;
    }

    return y == 1;
}

template<typename Type, size_t csize>
inline bool dynamic_matrix<Type, csize>::is_exist(const Fraction& m, const Fraction& n) const {
    if (floor(m) >= csize || floor(n) >= csize)
		return false;
    if (m.denominator() != 1 && (!is_gcdr_of(csize, m.denominator()) || !is_gcdr_of(csize, n.denominator())))
		return false;
	return true;
}

template<typename Type, size_t csize>
inline void dynamic_matrix<Type, csize>::assert_exist_index(const Fraction& m, const Fraction& n) const {
	if (is_exist(m, n) == false)
        throw std::out_of_range("dynamic matirx");
}

} // namespace my
