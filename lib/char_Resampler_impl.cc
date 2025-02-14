/* -*- c++ -*- */
/*
 * Copyright 2025 CLoud_A.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "char_Resampler_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace ncdsss {

#pragma message("set the following appropriately and remove this warning")
using input_type = char;
#pragma message("set the following appropriately and remove this warning")
using output_type = char;
char_Resampler::sptr char_Resampler::make(int symbol, int data)
{
    return gnuradio::make_block_sptr<char_Resampler_impl>(symbol, data);
}


/*
 * The private constructor
 */
char_Resampler_impl::char_Resampler_impl(int symbol, int data)
    : gr::block("char_Resampler",
                gr::io_signature::make(
                    1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
    dev = (double)symbol / data;
    printf("symbol / data = %f", dev);
}

/*
 * Our virtual destructor.
 */
char_Resampler_impl::~char_Resampler_impl() {}

void char_Resampler_impl::forecast(int noutput_items,
                                   gr_vector_int& ninput_items_required)
{
#pragma message( \
    "implement a forecast that fills in how many items on each input you need to produce noutput_items and remove this warning")
    /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
}

int char_Resampler_impl::general_work(int noutput_items,
                                      gr_vector_int& ninput_items,
                                      gr_vector_const_void_star& input_items,
                                      gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

#pragma message("Implement the signal processing in your block and remove this warning")
    // Do <+signal processing+>
    // Tell runtime system how many input items we consumed on
    // each input stream.
    consume_each(noutput_items);

    int last = in[0];
    bool sync_flag = false;
    int resamp_count = 0;
    int zero_count = 0, one_count = 0;
    int out_count = 0;

    for (int i = 0; i < noutput_items; i++) {
        if (in[i] != last) {
            for (int j = 0; j < 10; j++) {
                if (last != in[i + j]) {
                    sync_flag = false;
                    break;
                } else {
                    sync_flag = true;
                }
                last = in[i + j];
            }
        }
        last = in[i];

        if (in[i] == 0) {
            zero_count++;
        } else {
            one_count++;
        }

        if (++resamp_count > dev - 1 || sync_flag) {
            out[out_count++] = one_count > zero_count ? 1 : 0;
            one_count = 0;
            zero_count = 0;
            resamp_count = 0;
            i += sync_flag ? 0 : 1; // jump one bit between two data bit
            sync_flag = false;
        }
    }


    // Tell runtime system how many output items we produced.
    return out_count;
}

} /* namespace ncdsss */
} /* namespace gr */
