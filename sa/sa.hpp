//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// Suffix array construction by induced sorting (SA-IS)
// based on:
//  'Linear Suffix Array Construction by Almost Pure Induced-Sorting' Nong, G., Zhang, S. and Chan, W. Data Compression Conference, 2009
//
// ... and on awesome explanation https://zork.net/~st/jottings/sais.html
//
#ifndef __COMPRESSION_SUFFIX_ARRAY_H__
#define __COMPRESSION_SUFFIX_ARRAY_H__

#pragma once

BEGIN_NAMESPACE(compression::sa)
USINGNAMESPACE(compression)

template <typename ElementType, typename Traits>
class suffix_array : private noncopyable
{
    // virtual sentinel is always = 0
    public:
        using traits_type = Traits;

        using element_type = ElementType;
        using elements_type = std::vector<element_type>;

        using index_type = int;
        using indices_type = std::vector<index_type>;

        using size_type = int;

        using types_type = std::vector<bool>; // t

    public:
        enum class algorithm
        {
            naive = 1,
            sa_is = 2
        };

    private:
        struct bucket
        {
            int size;
            int head;
            int tail;

            element_type element;

            bucket() : size(0), head(0), tail(0)
            {
            }
        };

        using bucket_type = bucket;
        using buckets_type = std::vector<bucket_type>;

    private:
        static bool is_lms(const types_type& t, int i);
        static bool are_lms_strings_equal(const elements_type& s, const types_type& t, int i_a, int i_b);

        static void build_type_map(const elements_type& s, types_type& t);
        static void build_buckets(const elements_type& s, buckets_type& buckets);

        static void arrange_lms(const elements_type& s, const types_type& t, const buckets_type& buckets, indices_type& sa);

        static void induce_sort_ltype(const elements_type& s, const types_type& t, const buckets_type& buckets, indices_type& sa);
        static void induce_sort_stype(const elements_type& s, const types_type& t, const buckets_type& buckets, indices_type& sa);

    private:
        static bool build_naive(const elements_type& s, indices_type& sa);
        static bool build_sa_is(const elements_type& s, indices_type& sa);

    public:
        static bool build(const elements_type& s, indices_type& sa, algorithm = algorithm::sa_is);
};

template <typename ElementType, typename Traits>
bool suffix_array<ElementType, Traits>::build_naive(const typename suffix_array<ElementType, Traits>::elements_type& s,  // S is the input string
                                                    typename suffix_array<ElementType, Traits>::indices_type& sa)        // SA is the output suffix array of S
{
    bool result = true;

    std::vector<elements_type> suffixes;

    size_type n = static_cast<size_type>(s.size()); // includes +1 for virtual sentinel

    for(int k = 0; k < n; k++)
    {
        elements_type suffix(s.begin() + k, s.end());
        suffixes.emplace_back(suffix);
    }

    std::sort(suffixes.begin(),
              suffixes.end(),
              [](const elements_type& suffix1, const elements_type& suffix2)
              {
                  return suffix1 < suffix2;
              });

    for(const auto& suffix : suffixes)
    {
        sa.emplace_back(static_cast<index_type>(s.size()) - static_cast<index_type>(suffix.size()));
    }

    return result;
}

template <typename ElementType, typename Traits>
bool suffix_array<ElementType, Traits>::build_sa_is(const typename suffix_array<ElementType, Traits>::elements_type& s,  // S is the input string, must have virtual sentinel (0)
                                                    typename suffix_array<ElementType, Traits>::indices_type& sa)        // SA is the output suffix array of S
{
    // input string s must be in this format:
    //  c ... c e
    //  chars   sentinel = 0
    // virtual sentinel is an empty suffix

    bool result = true;

    size_type n = static_cast<size_type>(s.size()); // includes +1 for virtual sentinel

    // init sa
    indices_type sa_aux(n);

    std::fill(sa_aux.begin(), sa_aux.end(), -1);

    // build type map (populate t array)
    std::vector<bool> t;

    build_type_map(s, t);

    // build buckets and its heads/tails
    buckets_type buckets;

    build_buckets(s, buckets);

    // rearrange LMS suffixes, approximate positions
    indices_type sa_aprx;

    arrange_lms(s, t, buckets, sa_aprx);

    // rearrange all other (non-LMS) suffixes, might move sa indices around
    induce_sort_ltype(s, t, buckets, sa_aprx);
    induce_sort_stype(s, t, buckets, sa_aprx);




    if(result)
    {
        sa.swap(sa_aux);
    }

    return result;
}

template <typename ElementType, typename Traits>
inline bool suffix_array<ElementType, Traits>::build(const typename suffix_array<ElementType, Traits>::elements_type& s,
                                                     typename suffix_array<ElementType, Traits>::indices_type& sa,
                                                     typename suffix_array<ElementType, Traits>::algorithm alg)
{
    bool result = true;

    if(alg == algorithm::naive)
    {
        result = build_naive(s, sa);
    }
    else if(alg == algorithm::sa_is)
    {
        result = build_sa_is(s, sa);
    }

    return result;
}

template <typename ElementType, typename Traits>
bool suffix_array<ElementType, Traits>::is_lms(const typename suffix_array<ElementType, Traits>::types_type& t, int i)
{
    // true if the char at index = i is a left-most S-type (LMS)
    // if i == 0 -> true
    // from the paper:
    // 00 Index: 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
    // 01 S:      m  m  i  i  s  s  i  i  s  s  i  i  p  p  i  i  $
    // 02 t:      L  L  S  S  L  L  S  S  L  L  S  S  L  L  L  L  S
    // 03 LMS:          *           *           *                 *
    //                                                            |
    //                                                      always true

    return i > 0 && t[i] && !t[i - 1];
}

template <typename ElementType, typename Traits>
bool suffix_array<ElementType, Traits>::are_lms_strings_equal(const typename suffix_array<ElementType, Traits>::elements_type& s,
                                                              const typename suffix_array<ElementType, Traits>::types_type& t,
                                                              int i_a, // first string start offset
                                                              int i_b) // second string start offset
{
    // controversion:
    // An “LMS substring” is a portion of the input string starting at one LMS character and continuing up to (but not including) the next LMS character.
    //
    // Definition 2.2. (LMS-substring) A LMS-substring is (i) a substring S[i..j] with both S[i] and S[j] being LMS characters,
    // and there is no other LMS character in the substring, for i != j; or (ii) the sentinel itself.

    bool result = true;

    size_type n = static_cast<size_type>(s.size()); // includes +1 for virtual sentinel

    if(i_a == n - 1 || i_b == n - 1)
    {
        // no other substring is equal to the empty suffix
        result = false;
    }

    if(result)
    {
        int i = 0; // iterations

        for(;;)
        {
            bool is_a_lms = is_lms(t, i + i_a);
            bool is_b_lms = is_lms(t, i + i_b);

            // found start of the next LMS strings ...
            if(i > 0 && // at least one element progressed
               is_a_lms && is_b_lms)
            {
                result = true;
                break;
            }

            if(is_a_lms != is_b_lms) // mismatch
            {
                result = false;
                break;
            }

            if(s[i + i_a] != s[i + i_b]) // compare content, element by element
            {
                result = false;
                break;
            }

            i++; // advance to the next element
        }
    }

    return result;
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::build_type_map(const typename suffix_array<ElementType, Traits>::elements_type& s,
                                                       typename suffix_array<ElementType, Traits>::types_type& t)
{
    // builds S/L-type map
    //  t: array [0 ... n] of booleans to represent the S-type and L-type vector,
    //  original paper has [0 ... n - 1] but in our case we have n + 1 elements, + 1 for virtual sentinel

    size_type n = static_cast<size_type>(s.size()); // includes +1 for virtual sentinel

    t.resize(n);

    t[n - 1] = true; // 1) The last suffix suf(S, n −1) (after the last character) consisting of only the single character $ or 0 (the sentinel) is defined as S-type.
                     // 2) The suffix containing only the last character (the last before virtual sentinel) must necessarily be larger than the empty suffix.

    if(n > 1) // real data, handles also 2)
    {
        for(auto i = n - 1; i > 0; i--)
        {
            // properties:
            //  (i)  S[i] is S-type if (i.1)  S[i] < S[i + 1] or (i.2)  S[i] = S[i + 1] and suf(S, i + 1) is S-type
            //  (ii) S[i] is L-type if (ii.1) S[i] > S[i + 1] or (ii.2) S[i] = S[i + 1] and suf(S, i + 1) is L-type
            t[i - 1] = s[i - 1] == s[i] ? t[i] // ... or (i.2) or (ii.2) ...
                                        : element_type(s[i - 1]) < element_type(s[i]); // ... if (i.1) if (ii.1) ...
        }
    }
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::build_buckets(const typename suffix_array<ElementType, Traits>::elements_type& s,
                                                      typename suffix_array<ElementType, Traits>::buckets_type& buckets)
{
    int n = traits_type::abc_size();

    buckets.resize(n);

    // collect stats
    for(const auto& e : s)
    {
        buckets[e].size++;
        buckets[e].element = e;
    }

    // calculate heads
    int offset = 0; // The empty suffix always winds up at the beginning of the suffix array in index 0, so the a bucket begins at index 1.

    for(auto& bucket : buckets)
    {
        if(bucket.size > 0)
        {
            bucket.head = offset;
            offset += bucket.size;
        }
    }

    // calculate tails
    offset = 0;

    for(auto& bucket : buckets)
    {
        if(bucket.size > 0)
        {
            offset += bucket.size;
            bucket.tail = offset - 1;
        }
    }
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::arrange_lms(const typename suffix_array<ElementType, Traits>::elements_type& s,
                                                    const typename suffix_array<ElementType, Traits>::types_type& t,
                                                    const typename suffix_array<ElementType, Traits>::buckets_type& buckets,
                                                    typename suffix_array<ElementType, Traits>::indices_type& sa)
{
    // put all LMS suffixes into their appropriate bucket (to the end)
    //  05 Bucket: $    i                         m       p       s
    //  06 SA:     {16} {-1 -1 -1 -1 -1 10 06 02} {-1 -1} {-1 -1} {-1 -1 -1 -1}
    //                                  |  |  | first insertion
    //                                  |  | second insertion
    //                                  | third insertion

    size_type n = static_cast<size_type>(s.size()); // includes +1 for virtual sentinel

    sa.resize(n);

    std::fill(sa.begin(), sa.end(), -1);

    // collect tails
    indices_type tails;

    for(const auto& bucket : buckets)
    {
        tails.emplace_back(bucket.tail);
    }

    // arrange ...
    for(auto i = 0; i < n - 1; i++) // -1 for excluding virtual sentinel
    {
        if(!is_lms(t, i))
        {
            continue; // not start of LMX suffix
        }

        int k = s[i]; // bucket index

        sa[tails[k]] = i;

        tails[k]--; // move tail index backward to insert the next element in front of the last inserted element
    }

    sa[0] = n - 1; // set the single sentinel LMS-substring, -1 for excluding virtual sentinel
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::induce_sort_ltype(const typename suffix_array<ElementType, Traits>::elements_type& s,
                                                          const typename suffix_array<ElementType, Traits>::types_type& t,
                                                          const typename suffix_array<ElementType, Traits>::buckets_type& buckets,
                                                          typename suffix_array<ElementType, Traits>::indices_type& sa)
{
    // scanning through the suffix array from left-to-right arranging l-type suffixes ...

    // collect heads
    indices_type heads;

    for(const auto& bucket : buckets)
    {
        heads.emplace_back(bucket.head);
    }

    // arrange ...
    size_type m = static_cast<size_type>(sa.size());

    for(int i = 0; i < m; i++)
    {
        if(sa[i] == -1)
        {
            continue; // skip non initialized entries
        }

        // get the index of the suffix that begins to the left of the suffix this entry points to
        int j = sa[i] - 1; // -1 is left of the current position

        if(j < 0)
        {
            continue;
        }

        if(t[j])
        {
            continue; // considering only l-type suffixes
        }

        int k = s[j]; // bucket index

        sa[heads[k]] = j;

        heads[k]++; // move head index forward where to insert the next element
    }
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::induce_sort_stype(const typename suffix_array<ElementType, Traits>::elements_type& s,
                                                          const typename suffix_array<ElementType, Traits>::types_type& t,
                                                          const typename suffix_array<ElementType, Traits>::buckets_type& buckets,
                                                          typename suffix_array<ElementType, Traits>::indices_type& sa)
{
    // scanning through the suffix array from right-to-left arranging s-type suffixes ...

    // collect tails
    indices_type tails;

    for(const auto& bucket : buckets)
    {
        tails.emplace_back(bucket.tail);
    }

    // arrange ...
    for(int i = static_cast<size_type>(sa.size()) - 1; i >= 0; i--)
    {
        int j = sa[i] - 1;

        if(j < 0)
        {
            continue;
        }

        if(!t[j])
        {
            continue; // considering only s-type suffixes
        }

        int k = s[j]; // bucket index

        sa[tails[k]] = j;

        tails[k]--; // move head index backward where to insert the next element
    }
}

END_NAMESPACE

#endif // __COMPRESSION_SUFFIX_ARRAY_H__




    //bool rc = are_lms_strings_equal(s, t, 1, 7);

        //??
    //buckets_type aux;
    //std::for_each(buckets.begin(),
    //              buckets.end(),
    //              [&aux](const auto& bucket)
    //              {
    //                  if(bucket.size > 0)
    //                  {
    //                      aux.emplace_back(bucket);
    //                  }
    //              });
    //buckets.swap(aux);



    //for(auto k = 0; k < n; k++) //??
    //{
    //    std::cout << is_lms(t, k);
    //}
