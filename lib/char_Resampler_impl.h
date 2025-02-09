/* -*- c++ -*- */
/*
 * Copyright 2025 CLoud_A.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NCDSSS_CHAR_RESAMPLER_IMPL_H
#define INCLUDED_NCDSSS_CHAR_RESAMPLER_IMPL_H

#include <gnuradio/ncdsss/char_Resampler.h>

namespace gr {
namespace ncdsss {

class char_Resampler_impl : public char_Resampler
{
private:
    // Nothing to declare in this block.

public:
    char_Resampler_impl(int symbol, int data);
    ~char_Resampler_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace ncdsss
} // namespace gr

#endif /* INCLUDED_NCDSSS_CHAR_RESAMPLER_IMPL_H */
