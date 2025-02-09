/* -*- c++ -*- */
/*
 * Copyright 2025 CLoud_A.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "DSSS_Decoder_impl.h"
#include <gnuradio/io_signature.h>

typedef unsigned char uint8_t;

uint8_t gold[1023];
uint8_t find_flag;
u_int16_t gold_index = 0;

int count_one(unsigned short in)
{
    static int out[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

    int o = out[in & 0x0f];
    o += out[(in >> 4) & 0x0f];
    o += out[(in >> 8) & 0x0f];
    o += out[(in >> 12) & 0x0f];

    // printf ("%d\n", o);

    return o;
}

void gold_gen(unsigned short _x1,
              unsigned short _x2,
              unsigned short _n1,
              unsigned short _n2)
{
    unsigned short x1, x2, n1, n2, temp;

    x1 = _x1;
    n1 = _n1;
    x2 = _x2;
    n2 = _n2;

    for (int i = 0; i < 1023; i++) {
        gold[i] = ((x1 ^ x2) & 0x200) >> 9;
        printf("%d", ((x1 ^ x2) & 0x200) >> 9);

        temp = (count_one(x1 & n1) & 0x01);
        x1 = ((x1 << 1) & 0x3fe) | temp;

        temp = (count_one(x2 & n2) & 0x01);
        x2 = ((x2 << 1) & 0x3fe) | temp;
    }
    printf("\n");
}


namespace gr {
namespace ncdsss {

// #pragma message("set the following appropriately and remove this warning")
using input_type = char;
// #pragma message("set the following appropriately and remove this warning")
using output_type = char;
DSSS_Decoder::sptr DSSS_Decoder::make(uint16_t x1, uint16_t x2, uint16_t n1, uint16_t n2)
{
    return gnuradio::make_block_sptr<DSSS_Decoder_impl>(x1, x2, n1, n2);
}


/*
 * The private constructor
 */
DSSS_Decoder_impl::DSSS_Decoder_impl(uint16_t x1, uint16_t x2, uint16_t n1, uint16_t n2)
    : gr::block("DSSS_Decoder",
                gr::io_signature::make(
                    1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
    gold_gen(x1, x2, n1, n2);
    find_flag = 0;
}

/*
 * Our virtual destructor.
 */
DSSS_Decoder_impl::~DSSS_Decoder_impl() {}

void DSSS_Decoder_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{

    ninput_items_required[0] = noutput_items;
    /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
}

int DSSS_Decoder_impl::general_work(int noutput_items,
                                    gr_vector_int& ninput_items,
                                    gr_vector_const_void_star& input_items,
                                    gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    //  Do <+signal processing+>
    //  Tell runtime system how many input items we consumed on
    //  each input stream.
    consume_each(noutput_items);

    int zero_count = 0;

    // printf("%d\n", gold_index);

    if (!find_flag) {
        for (int i = 0; i < noutput_items; i++) {
            if (in[i] == 0) {
                zero_count++;
            }
        }
    }


    if (zero_count < noutput_items) // if all data is zero, dont poccess to save the time.
    {
        if (!find_flag) {
            if (noutput_items >= 187) {
                for (int i = 0; i < 1023 - 187; i++) {
                    uint32_t change_count = 0;
                    uint8_t last = 0;
                    uint8_t now;
                    for (uint8_t j = 0; j < 187; j++) {
                        now = (in[j]) ^ (gold[i + j]);
                        // printf("%d", now);

                        if (now != last) {
                            change_count++;
                        }

                        last = now;
                    }
                    if (change_count < 3) {
                        // printf("%d\n", noutput_items);
                        printf("find!!\n");

                        gold_index = i;
                        find_flag = 1;
                        break;
                    } else {
                        // printf("\n");
                        // printf("%d\n", change_count);
                    }
                }
            }
        }

        if (find_flag) {
            static int last = 0;
            int i = 0;
            for (i = 0; i < noutput_items; i += 187) {
                int change_count = 0;
                for (int j = 0; j < 187; j++) {
                    out[i * 187 + j] = ((in[i * 187 + j]) ^ gold[gold_index++]);
                    if (out[i * 187 + j] != last)
                    {
                        change_count ++;
                    }
                    last = out[i * 187 + j];

                    if (gold_index == 1023)
                        gold_index = 0;
                }
                if (change_count > 10)
                {
                    find_flag = 0;
                }
                
            }
            for ( ; i < noutput_items; i++)
            {
                out[i] = ((in[i]) ^ gold[gold_index++]);
                if (gold_index == 1023)
                        gold_index = 0;
            }
            

        } else {
            for (int i = 0; i < noutput_items; i++) {
                out[i] = 0;
            }
        }
    } else {
        for (int i = 0; i < noutput_items; i++) {
            out[i] = 0;
            // if (++gold_index == 1023)
            //         gold_index = 0;
        }
    }


    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace ncdsss */
} /* namespace gr */
