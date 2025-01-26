/* -*- c++ -*- */
/*
 * Copyright 2025 CLoud_A.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "bpsk_squelch_impl.h"

namespace gr {
  namespace ncdsss {

    // #pragma message("set the following appropriately and remove this warning")
    using input_type = gr_complex;
    // #pragma message("set the following appropriately and remove this warning")
    using output_type = gr_complex;
    bpsk_squelch::sptr
    bpsk_squelch::make()
    {
      return gnuradio::make_block_sptr<bpsk_squelch_impl>(
        );
    }


    /*
     * The private constructor
     */
    bpsk_squelch_impl::bpsk_squelch_impl()
      : gr::block("bpsk_squelch",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
    {}

    /*
     * Our virtual destructor.
     */
    bpsk_squelch_impl::~bpsk_squelch_impl()
    {
    }

    void
    bpsk_squelch_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    // #pragma message("implement a forecast that fills in how many items on each input you need to produce noutput_items and remove this warning")
      ninput_items_required[0] = noutput_items;
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    bpsk_squelch_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      auto in = static_cast<const input_type*>(input_items[0]);
      auto out = static_cast<output_type*>(output_items[0]);

      // #pragma message("Implement the signal processing in your block and remove this warning")
      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);
      

      int count = 0;
      // static int cou;

      for (int i = 0; i < noutput_items; i++)
      {
        if (in[i].real() < 0.3 && in[i].real() > -0.3) 
        {
          count++;
        }
        
      }
      
      if (count < (noutput_items * 0.8)) // if half of point is in the center of constellation, output const 0
      {
        for (int i = 0; i < noutput_items; i++)
        {
          out[i] = in[i];
        }
        
      }else
      {
        for (int i = 0; i < noutput_items; i++)
        {
          out[i] = gr_complex(0, 0);
        }
        // printf("close:%d\n", cou++);
      }
      

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace ncdsss */
} /* namespace gr */
