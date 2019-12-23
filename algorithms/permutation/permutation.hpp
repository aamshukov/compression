//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ALGORITHMS_PERMUTATION_H__
#define __ALGORITHMS_PERMUTATION_H__

#pragma once

BEGIN_NAMESPACE(algorithms)

// α β ε λ ∅ ∈ ∉ Σ ∪ ⊕

template <typename ElementType, typename RankType = std::size_t>
class permutation : private noncopyable
{
    public:
        using element_type = ElementType;
        using elements_type = std::vector<element_type>;

        using multiset_element_type = int;
        using multiset_elements_type = std::vector<multiset_element_type>;;

        using index_type = int;
        using indices_type = std::vector<index_type>;

        using size_type = int;
        using rank_type = RankType;

        struct node
        {
            using element_type = permutation::element_type;
            using node_type = std::shared_ptr<node>;

            element_type value;
            node_type next; // the permutations are stored in a singly-linked list ...

            node() : value{}, next(nullptr)
            {
            }
        };

    private:
        static rank_type    rank(elements_type& p, elements_type& pr, const size_type& n);

    public:
        static rank_type    rank(const elements_type& permutation);
        static void         unrank(rank_type rank, const size_type& permutation_size, elements_type& permutation);

        static void         generate_multiset_permutation(elements_type& multiset, std::vector<elements_type>& permutations);

        static std::experimental::generator<elements_type>
                            generate_multiset_permutation_lazy(elements_type& multiset);

        static rank_type    rank_multiset(const elements_type& multiset, const multiset_elements_type& multiset_domain);
        static void         unrank_multiset(rank_type rank,
                                            const multiset_elements_type& multiset_domain,
                                            const size_type multiset_size,
                                            elements_type& multiset);
};

template <typename ElementType, typename RankType>
typename permutation<ElementType, RankType>::rank_type permutation<ElementType, RankType>::rank(const typename permutation<ElementType, RankType>::elements_type& permutation)
{
    // synopsis:
    //      Wendy Myrvold, Frank Ruskey, April, 2000
    //      "Ranking and Unranking Permutations in Linear Time"
    //      wendym@csr.uvic.ca  fruskey@csr.uvic.ca
    //
    //      ALGORITHM 317 (CACM) April-May-July 1967
    //      Charles L. Robinson
    //      Institute for Computer Research, U. of Chicago, Chicago, Ill.
    size_type n = static_cast<size_type>(permutation.size());

    elements_type p(permutation.begin(), permutation.end());    // permutation
    elements_type pr(n);                                        // reverse permutation

    std::fill(pr.begin(), pr.end(), 0);

    // build reverse permutation
    for(index_type i = 0; i < n; i++)
    {
        pr[p[i]] = i;
    }

    // rank
    rank_type result = rank(p, pr, n);

    return result;
}

template <typename ElementType, typename RankType>
typename permutation<ElementType, RankType>::rank_type permutation<ElementType, RankType>::rank(typename permutation<ElementType, RankType>::elements_type& p,
                                                                                                typename permutation<ElementType, RankType>::elements_type& pr,
                                                                                                const typename permutation<ElementType, RankType>::size_type& n)
{
    if(n == 1)
    {
        return 0;
    }

    element_type s = p[n - 1];

    std::swap(p[n - 1], p[pr[n - 1]]);
    std::swap(pr[s], pr[n - 1]);

    return s + n * rank(p, pr, n - 1);
}

template <typename ElementType, typename RankType>
void permutation<ElementType, RankType>::unrank(typename permutation<ElementType, RankType>::rank_type rank,
                                                const typename permutation<ElementType, RankType>::size_type& permutation_size,
                                                typename permutation<ElementType, RankType>::elements_type& permutation)
{
    rank_type r = rank;
    size_type n = permutation_size;

    elements_type& p(permutation);

    p.resize(n);

    std::fill(p.begin(), p.end(), 0);

    // build identity permutation
    for(index_type i = 0; i < n; i++)
    {
        p[i] = i;
    }

    // unrank
    while(n > 0)
    {
        std::swap(p[n - 1], p[static_cast<element_type>(r % n)]);

        r /= n;
        n--;
    }
}

template <typename ElementType, typename RankType>
void permutation<ElementType, RankType>::generate_multiset_permutation(typename permutation<ElementType, RankType>::elements_type& multiset,
                                                                       std::vector<typename permutation<ElementType, RankType>::elements_type>& permutations)
{
    // Loopless Generation of Multiset Permutations using a Constant Number of Variables by Prefix Shifts. Aaron Williams, 2009

    // code duplication with lazy version ... later ...
    if(multiset.empty()) // ... if E is empty, then init(E) should exit.
    {
        return;
    }

    // ... non-increasing order
    std::sort(multiset.begin(),
              multiset.end(),
              [](const element_type& element1, const element_type& element2)
              {
                  return element1 < element2;
              });

    // init
    typename node::node_type h; // first (head) node
    typename node::node_type i; // second-last node
    typename node::node_type j; // last node

    for(const auto& element : multiset) // build h
    {
        auto node(std::make_shared<node>());

        (*node).value = element;
        (*node).next = h;

        h = node;
    }

    size_type n = static_cast<size_type>(multiset.size());

    auto nth = [&h](index_type n)
    {
        typename node::node_type result(h);

        index_type i = 0;

        while(i < n && (*result).next != nullptr)
        {
            result = (*result).next;
            i++;
        }

        return result;
    };

    i = nth(n - 2); // i
    j = nth(n - 1); // j

    typename node::node_type s;
    typename node::node_type t;

    auto visit = [&h, &permutations]()
    {
        elements_type permutation;

        typename node::node_type v(h);

        while(v != nullptr)
        {
            permutation.emplace_back((*v).value);
            v = (*v).next;
        }

        permutations.emplace_back(permutation);
    };

    // collect the first result out of head
    visit();

    // generate ...
    while(((*j).next != nullptr || (*j).value < (*h).value))
    {
        if((*j).next != nullptr && (*i).value >= (*(*j).next).value)
        {
            s = j;
        }
        else
        {
            s = i;
        }
        
        t = (*s).next;

        (*s).next = (*t).next;
        (*t).next = h;

        if((*t).value < (*h).value)
        {
            i = t;
        }

        j = (*i).next;
        h = t;

        visit();
    }
}

template <typename ElementType, typename RankType>
std::experimental::generator<typename permutation<ElementType, RankType>::elements_type> permutation<ElementType, RankType>::generate_multiset_permutation_lazy(elements_type& multiset)
{
    // Loopless Generation of Multiset Permutations using a Constant Number of Variables by Prefix Shifts. Aaron Williams, 2009

    // code duplication with non-lazy version ... later ...
    if(!multiset.empty()) // ... if E is empty, then init(E) should exit.
    {
        // ... non-increasing order
        std::sort(multiset.begin(),
                  multiset.end(),
                  [](const element_type& element1, const element_type& element2)
                  {
                      return element1 < element2;
                  });

        // init
        typename node::node_type h; // first (head) node
        typename node::node_type i; // second-last node
        typename node::node_type j; // last node

        for(const auto& element : multiset) // build h
        {
            auto node(std::make_shared<node>());

            (*node).value = element;
            (*node).next = h;

            h = node;
        }

        size_type n = static_cast<size_type>(multiset.size());

        auto nth = [&h](index_type n)
        {
            typename node::node_type result(h);

            index_type i = 0;

            while(i < n && (*result).next != nullptr)
            {
                result = (*result).next;
                i++;
            }

            return result;
        };

        i = nth(n - 2); // i
        j = nth(n - 1); // j

        typename node::node_type s;
        typename node::node_type t;

        auto visit = [&h]()
        {
            elements_type permutation;

            typename node::node_type v(h);

            while(v != nullptr)
            {
                permutation.emplace_back((*v).value);
                v = (*v).next;
            }

            return permutation;
        };

        // collect the first result out of head
        co_yield visit();

        // generate ...
        while(((*j).next != nullptr || (*j).value < (*h).value))
        {
            if((*j).next != nullptr && (*i).value >= (*(*j).next).value)
            {
                s = j;
            }
            else
            {
                s = i;
            }
        
            t = (*s).next;

            (*s).next = (*t).next;
            (*t).next = h;

            if((*t).value < (*h).value)
            {
                i = t;
            }

            j = (*i).next;
            h = t;

            co_yield visit();
        }
    }
}

template <typename ElementType, typename RankType>
typename permutation<ElementType, RankType>::rank_type permutation<ElementType, RankType>::rank_multiset(const typename permutation<ElementType, RankType>::elements_type& multiset,
                                                                                                         const typename permutation<ElementType, RankType>::multiset_elements_type& multiset_domain)
{
    // multiset_domain must be zero based
    // simplified version of https://github.com/pyncomb/pyncomb/blob/master/pyncomb/tuplelex.py
    // also ...
    // https://computationalcombinatorics.wordpress.com/2012/09/10/ranking-and-unranking-of-combinations-and-permutations/
    // Ranking Tuples
    // To rank tuples, consider the simplest way to count k-tuples in {0, 1, ..., n-1}:
    //  there are n ways to fix the i-th coordinate and n^i ways to extend the tuple to the previous positions (recall i ∈ {0, ... , k-1}).
    // Therefore, we can rank a tuple X ∈ {0, 1, ..., n-1}^k by
    //
    //             k-1
    // rank(X) = Σ    x(i) * n^i.
    //             i=0
    //
    // ...
    rank_type result = 0; // rank

    index_type position = 1; // weighted position

    size_type multiset_size = static_cast<size_type>(multiset.size());
    size_type multiset_domain_size = static_cast<size_type>(multiset_domain.size());

    for(index_type i = 0; i < multiset_size; i++)
    {
        index_type index = multiset_size - i - 1;

        result += position * multiset_domain[multiset[index]]; // mapping will be added later ... maybe

        // n^i
        position *= multiset_domain_size;
    }

    return result;
}

template <typename ElementType, typename RankType>
void permutation<ElementType, RankType>::unrank_multiset(typename permutation<ElementType, RankType>::rank_type rank,
                                                         const typename permutation<ElementType, RankType>::multiset_elements_type& multiset_domain,
                                                         const typename permutation<ElementType, RankType>::size_type multiset_size,
                                                         typename permutation<ElementType, RankType>::elements_type& multiset)
{
    // multiset_domain must be zero based
    // simplified version of https://github.com/pyncomb/pyncomb/blob/master/pyncomb/tuplelex.py
    // ... To unrank the tuple, iteratively pull out the residue modulo n and then divide by n (using integer division)
    // to find the rank of the remaining tuple of shorter length.
    multiset.resize(multiset_size);

    std::fill(multiset.begin(), multiset.end(), 0);

    size_type multiset_domain_size = static_cast<size_type>(multiset_domain.size());

    size_type multiplication_step = multiset_domain_size;

    auto position = static_cast<std::size_t>(std::pow(multiplication_step, multiset_size)); // max weighted position

    for(index_type i = 0; i < multiset_size; i++)
    {
        position /= multiplication_step;

        index_type index = static_cast<index_type>(rank / position);

        multiset[i] = multiset_domain[index];

        rank %= position;
    }
}

END_NAMESPACE

#endif // __ALGORITHMS_PERMUTATION_H__
