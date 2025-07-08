/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ncdsss author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NCDSSS_DSSS_ENCODER_IMPL_H
#define INCLUDED_NCDSSS_DSSS_ENCODER_IMPL_H

#include <gnuradio/ncdsss/DSSS_Encoder.h>

namespace gr {
  namespace ncdsss {

    class DSSS_Encoder_impl : public DSSS_Encoder
    {
     private:
      void pmt_in_callback(pmt::pmt_t msg);
      pmt::pmt_t d_in_port;

     public:
      DSSS_Encoder_impl(uint16_t x1, uint16_t x2, uint16_t n1, uint16_t n2);
      ~DSSS_Encoder_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace ncdsss
} // namespace gr

#endif /* INCLUDED_NCDSSS_DSSS_ENCODER_IMPL_H */
