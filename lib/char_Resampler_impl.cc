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

using input_type = char;
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
    printf("rx: symbol / data = %f\n", dev);
}

/*
 * Our virtual destructor.
 */
char_Resampler_impl::~char_Resampler_impl() {}

void char_Resampler_impl::forecast(int noutput_items,
                                   gr_vector_int& ninput_items_required)
{

    ninput_items_required[0] = noutput_items;
}

int char_Resampler_impl::general_work(int noutput_items,
                                      gr_vector_int& ninput_items,
                                      gr_vector_const_void_star& input_items,
                                      gr_vector_void_star& output_items)
{
 
    const uint8_t* in = static_cast<const uint8_t*>(input_items[0]);  // input_type
    uint8_t* out = static_cast<uint8_t*>(output_items[0]);            // output_type

    const int input_len = ninput_items[0];
    const float factor = 3069000.0f / 16384.0f;  // ≈ 187.33
    const float hfactor = 3069000.0f / 16384.0f / 10;  // ≈ 187.33 / 10

    static bool sync_locked = false;
    static float phase = 0.0f;
    static int last_bit = -1;
    static int zero_count = 0, one_count = 0;

    int out_count = 0;

    for (int i = 0; i < input_len; ++i) {
        int bit = in[i];

        // 同步锁定：第一次跳变就锁定
        if (!sync_locked) {
            if (last_bit != -1 && bit != last_bit) {
                sync_locked = true;
                phase = 0.0f;
                zero_count = one_count = 0;
            }
            else
            {
                last_bit = bit;
                continue;
            }
            
        }

        // 统计当前比特
        if (bit)
            one_count++;
        else
            zero_count++;

        phase += 1.0f;

        // 重采样输出
        if (phase >= factor) {
            phase -= factor;
            out[out_count++] = (one_count > zero_count) ? 1 : 0;
            

            if (abs(one_count - zero_count) < hfactor)
                sync_locked = false;

            one_count = zero_count = 0;

            if (out_count >= noutput_items)
                break;
        }

        last_bit = bit;
    }

    // 通知系统输入消耗了多少
    consume_each(input_len);
    return out_count;
}

} /* namespace ncdsss */
} /* namespace gr */
