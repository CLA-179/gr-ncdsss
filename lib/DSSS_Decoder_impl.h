/* -*- c++ -*- */
/*
 * Copyright 2025 CLoud_A.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NCDSSS_DSSS_DECODER_IMPL_H
#define INCLUDED_NCDSSS_DSSS_DECODER_IMPL_H

#include <gnuradio/ncdsss/DSSS_Decoder.h>

namespace gr {
  namespace ncdsss {

    class DSSS_Decoder_impl : public DSSS_Decoder
    {
     private:
      // Nothing to declare in this block.

     public:
      DSSS_Decoder_impl();
      ~DSSS_Decoder_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace ncdsss
} // namespace gr

#endif /* INCLUDED_NCDSSS_DSSS_DECODER_IMPL_H */
