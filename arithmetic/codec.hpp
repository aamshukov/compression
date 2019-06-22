//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// based on mathematicalmonk's videos
// https://www.youtube.com/playlist?list=PLE125425EC837021F
//
#ifndef __COMPRESSION_ARITHMETIC_CODEC_H__
#define __COMPRESSION_ARITHMETIC_CODEC_H__

#pragma once

BEGIN_NAMESPACE(compression::arithmetic)

template <typename ElementType, typename IntegerType, typename InputStream, typename OutputStream>
class codec
{
    //          r(i)
    //  p(i) =  ---- , R = SUM(r(i)), i=0|n
    //           R
    //
    //  IMPORTANT: R * WHOLE <= 2^PRECISION
    //
    //  c(0) = 0, c(j) = r(0) + ... + r(j-1) for j = 1, ..., n
    //
    //  d(j) = c(j) + r(j) for j = 1, ..., n
    public:
        using integer_type = IntegerType;
        using element_type = ElementType;

        using input_stream_type = std::shared_ptr<InputStream>;
        using output_stream_type = std::shared_ptr<OutputStream>;

        using model_type = std::shared_ptr<model<element_type, integer_type>>;

    private:
        integer_type        my_precision;
        integer_type        my_whole;       // 1.0
        integer_type        my_half;        // 0.5
        integer_type        my_quarter;     // 0.25
        integer_type        my_quarter3;    // 0.75

    public:
                            codec();

        bool                encode(const model_type& model,
                                   const input_stream_type& input_stream,
                                   const output_stream_type& output_stream,
                                   std::size_t& original_size,
                                   std::size_t& encoded_size);

        bool                decode(const model_type& model,
                                   const input_stream_type& input_stream,
                                   const output_stream_type& output_stream,
                                   std::size_t original_size);
};

template <typename ElementType, typename IntegerType, typename InputStream, typename OutputStream>
inline codec<ElementType, IntegerType, InputStream, OutputStream>::codec()
{
    my_precision = std::numeric_limits<integer_type>::digits / 2;
    my_whole = 1 << my_precision;
    my_half = my_whole / 2;
    my_quarter = my_whole / 4;
    my_quarter3 = 3 * my_quarter;
}

template <typename ElementType, typename IntegerType, typename InputStream, typename OutputStream>
inline bool codec<ElementType, IntegerType, InputStream, OutputStream>::encode(const model_type& model,
                                                                               const input_stream_type& input_stream,
                                                                               const output_stream_type& output_stream,
                                                                               std::size_t& original_size,
                                                                               std::size_t& encoded_size)
{
    original_size = 0;
    encoded_size = 0;

    bool result = true;

    std::size_t org_size = 0;
    std::size_t enc_size = 0;

    integer_type R = (*model).r();

    integer_type a = 0;
    integer_type b = my_whole;

    integer_type w = 0; // b - a, interval width

    std::size_t s = 0; // number of middle-splits

    integer_type c = 0;
    integer_type d = 0;

    while(!(*input_stream).eos())
    {
        // update [a, b)
        element_type symbol;

        if(!(*input_stream).read(symbol))
        {
            // if !result - report error and exit
            result = (*input_stream).eos();
            break;
        }

        org_size++;

        w = b - a;

        c = (*model).c_by_symbol(symbol);
        d = (*model).d_by_symbol(symbol);

        if(c == -1 || d == -1)
        {
            // report error and exit
            result = false;
            break;
        }

        b = a + (w * d) / R;
        a = a + (w * c) / R;

        (*model).update(); // update c and d

        // rescale and emit
        while(b < my_half || a > my_half)
        {
            if(b < my_half)
            {
                // emit zero
                (*output_stream).write(static_cast<bit>(0));
                enc_size++;

                // emit s ones
                for(int k = 0; k < s; k++)
                {
                    (*output_stream).write(static_cast<bit>(1));
                    enc_size++;
                }

                // rescale
                a = 2 * a;
                b = 2 * b;

                s = 0;
            }
            else if(a > my_half)
            {
                // emit one
                (*output_stream).write(static_cast<bit>(1));
                enc_size++;

                // emit s zeros
                for(int k = 0; k < s; k++)
                {
                    (*output_stream).write(static_cast<bit>(0));
                    enc_size++;
                }

                a = 2 * (a - my_half);
                b = 2 * (b - my_half);

                s = 0;
            }
        }

        while(a > my_quarter && b < my_quarter3)
        {
            a = 2 * (a - my_quarter);
            b = 2 * (b - my_quarter);

            s = s + 1;
        }
    }

    s = s + 1; // one more time

    if(a <= my_quarter)
    {
        // emit zero
        (*output_stream).write(static_cast<bit>(0));
        enc_size++;

        // emit s ones
        for(int k = 0; k < s; k++)
        {
            (*output_stream).write(static_cast<bit>(1));
            enc_size++;
        }
    }
    else
    {
        // emit one
        (*output_stream).write(static_cast<bit>(1));
        enc_size++;

        // emit s zeros
        for(int k = 0; k < s; k++)
        {
            (*output_stream).write(static_cast<bit>(0));
            enc_size++;
        }
    }

    if(result)
    {
        original_size = org_size;
        encoded_size = enc_size;
    }

    return result;
}

template <typename ElementType, typename IntegerType, typename InputStream, typename OutputStream>
inline bool codec<ElementType, IntegerType, InputStream, OutputStream>::decode(const model_type& model,
                                                                               const input_stream_type& input_stream,
                                                                               const output_stream_type& output_stream,
                                                                               std::size_t original_size)
{
    bool result = true;

    element_type symbol;

    integer_type z = 0; // approximation

    // collect up to min(precision, M) symbols
    std::size_t i = 1; // important to start from 1

    while(i <= my_precision)
    {
        if(!(*input_stream).read(symbol))
        {
            break;
        }

#ifdef _DEBUG
        if(symbol == 1 || symbol == L'1')
#else
        if(symbol == 1)
#endif
        {
            z = z + (1 << (my_precision - i));
        }

        i++;
    }

    // abc
    std::size_t abc_size = (*(*model).abc()).data().size();

    // decode
    std::size_t decoded_size = 0;

    integer_type R = (*model).r();

    integer_type a = 0;
    integer_type b = my_whole;

    integer_type a0 = 0;
    integer_type b0 = 0;

    integer_type w = 0; // b - a, interval width

    std::size_t s = 0; // number of middle-splits

    integer_type c = 0;
    integer_type d = 0;

    for(;;)
    {
        // decode symbol
        for(std::size_t j = 0, n = abc_size; j < n; j++) // over all alphabet symbols (chunks)
        {
            w = b - a;

            c = (*model).c_by_index(j);
            d = (*model).d_by_index(j);

            if(c == -1 || d == -1)
            {
                // report error and exit
                result = false;
                goto _exit;
            }

            b0 = a + (w * d) / R;
            a0 = a + (w * c) / R;

            (*model).update(); // update c and d

            if(a0 <= z && z < b0)
            {
                // emit j
                (*output_stream).write((*model).symbol_by_index(j));

                decoded_size++;

                a = a0;
                b = b0;

                if(decoded_size == original_size)
                {
                    goto _exit;
                }

                break; // considered this chunk, need to break
            }
        }

        // rescale
        while(b < my_half || a > my_half)
        {
            if(b < my_half)
            {
                a = 2 * a;
                b = 2 * b;
                z = 2 * z;

                s = 0;
            }
            else if(a > my_half)
            {
                a = 2 * (a - my_half);
                b = 2 * (b - my_half);
                z = 2 * (z - my_half);

                s = 0;
            }

#ifdef _DEBUG
            if((*input_stream).read(symbol) && (symbol == 1 || symbol == '1'))
#else
            if((*input_stream).read(symbol) && symbol == 1)
#endif
            {
                z = z + 1;
            }
        }

        while(a > my_quarter && b < my_quarter3)
        {
            a = 2 * (a - my_quarter);
            b = 2 * (b - my_quarter);
            z = 2 * (z - my_quarter);

#ifdef _DEBUG
            if((*input_stream).read(symbol) && (symbol == 1 || symbol == '1'))
#else
            if((*input_stream).read(symbol) && symbol == 1)
#endif
            {
                z = z + 1;
            }

            s = s + 1;
        }
    }

_exit:
    if(decoded_size != original_size)
    {
        // report error and exit
        result = false;
    }

    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_CODEC_H__
