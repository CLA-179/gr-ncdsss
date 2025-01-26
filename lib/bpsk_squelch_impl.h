/* -*- c++ -*- */
/*
 * Copyright 2025 CLoud_A.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NCDSSS_BPSK_SQUELCH_IMPL_H
#define INCLUDED_NCDSSS_BPSK_SQUELCH_IMPL_H

#include <gnuradio/ncdsss/bpsk_squelch.h>

namespace gr {
  namespace ncdsss {

    class bpsk_squelch_impl : public bpsk_squelch
    {
     private:
      // Nothing to declare in this block.

     public:
      bpsk_squelch_impl();
      ~bpsk_squelch_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace ncdsss
} // namespace gr

#endif /* INCLUDED_NCDSSS_BPSK_SQUELCH_IMPL_H */
