//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ALGORITHMS_CODES_H__
#define __ALGORITHMS_CODES_H__

#pragma once

BEGIN_NAMESPACE(compression::codes)

// α β ε λ ∅ ∈ ∉ Σ ∪ ⊕

template <typename ElementType>
class codes : private noncopyable
{
    public:
        using element_type = ElementType;
        using elements_type = std::vector<element_type>;

        using size_type = int;

    private:
        static void         put_one_integer(const element_type& x,
                                            const size_type& nbits,
                                            string_type& result);

        static void         encode_interpolative_block(const elements_type& L,
                                                       const size_type& m,
                                                       const size_type& lo,
                                                       const size_type& hi,
                                                       string_type& result);
        static void         encode_centered_binary_in_range(const size_type& x,
                                                            const size_type& lo,
                                                            const size_type& hi,
                                                            string_type& result);
        static void         encode_centered_minimal_binary(const size_type& x,
                                                           const size_type& n,
                                                           string_type& result);

    public:
        static string_type  encode_minimal_binary(const element_type& symbol,
                                                  const size_type& abc_size);

        static string_type  decode_minimal_binary(const element_type& symbol,
                                                  const size_type& abc_size);

        static string_type  encode_interpolative_block(const elements_type& block);
};

template <typename ElementType>
string_type codes<ElementType>::encode_minimal_binary(const typename codes<ElementType>::element_type& symbol,
                                                      const typename codes<ElementType>::size_type& abc_size)
{
    // Compression and Coding Algorithms, A. Moffat and A. Turpin p.31
    string_type result;

    auto x = symbol;
    auto n = abc_size;

    auto b = static_cast<size_type>(std::ceil(std::log2(n)));
    auto d = static_cast<size_type>(std::pow(2, b)) - n;

    if(x > d)
        put_one_integer(x - 1 + d, b, result);
    else
        put_one_integer(x - 1, b - 1, result);

    return result;
}

template <typename ElementType>
string_type codes<ElementType>::decode_minimal_binary(const typename codes<ElementType>::element_type& symbol,
                                                      const typename codes<ElementType>::size_type& abc_size)
{
    string_type result;

    return result;
}

template <typename ElementType>
void codes<ElementType>::put_one_integer(const typename codes<ElementType>::element_type& x,
                                         const typename codes<ElementType>::size_type& nbits,
                                         string_type& result)
{
    for(auto i = nbits - 1; i >= 0; i--)
    {
        auto b = (x / static_cast<size_type>(std::pow(2, i))) % 2;
        result += b ? L'1' : L'0';
    }
}

template <typename ElementType>
string_type codes<ElementType>::encode_interpolative_block(const typename codes<ElementType>::elements_type& M)
{
    // Compression and Coding Algorithms, A. Moffat and A. Turpin p.54
    string_type result;

    int m = (int)M.size(); // M, m
    
    elements_type L((int)M.size());

    L[0] = M[0];

    for(int i = 1; i < (int)M.size(); i++)
        L[i] = L[i - 1] + M[i];

    auto B = L[(int)L.size() - 1];

    encode_interpolative_block(L, m, 1, B, result);

    return result;
}

template <typename ElementType>
void codes<ElementType>::encode_interpolative_block(const typename codes<ElementType>::elements_type& L,
                                                    const typename codes<ElementType>::size_type& m,
                                                    const typename codes<ElementType>::size_type& lo,
                                                    const typename codes<ElementType>::size_type& hi,
                                                    string_type& result)
{
    if(m == 0)
        return;

    auto h = m % 2 == 0 ? (m - 1) / 2 : m / 2;

    auto m1 = h;
    auto m2 = (m - 1) - h;

    elements_type L1;
    for(auto k = 0; k < h; k++) //  L[1 ... (h-1)]
        L1.emplace_back(L[k]);

    elements_type L2;
    for(auto k = h + 1; k < m; k++) //  L[(h+1) ... m]
        L2.emplace_back(L[k]);

    encode_centered_binary_in_range(L[h], lo + m1, hi - m2, result);

    encode_interpolative_block(L1, m1, lo, L[h] - 1, result);
    encode_interpolative_block(L2, m2, L[h] + 1, hi, result);
}

template <typename ElementType>
void codes<ElementType>::encode_centered_binary_in_range(const typename codes<ElementType>::size_type& x,
                                                         const typename codes<ElementType>::size_type& lo,
                                                         const typename codes<ElementType>::size_type& hi,
                                                         string_type& result)
{
    //encode_centered_minimal_binary(x - lo + 1, hi - lo + 1, result);
    result += encode_minimal_binary(x - lo + 1, hi - lo + 1);
}

template <typename ElementType>
void codes<ElementType>::encode_centered_minimal_binary(const typename codes<ElementType>::size_type& x0,
                                                        const typename codes<ElementType>::size_type& n,
                                                        string_type& result)
{
    double lg = std::log2(n);
    double cl = std::ceil(lg);
    double pw = std::pow(2, cl);

    int lng = (int)(2.0 * x0 - pw);

    int x = x0 - lng / 2;

    if(x < 1)
        x = x + n;

    result += encode_minimal_binary(static_cast<size_type>(x), n);
}

END_NAMESPACE

#endif // __ALGORITHMS_CODES_H__
