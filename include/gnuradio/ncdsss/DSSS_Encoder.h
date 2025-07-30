/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ncdsss author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NCDSSS_DSSS_ENCODER_H
#define INCLUDED_NCDSSS_DSSS_ENCODER_H

#include <gnuradio/ncdsss/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace ncdsss {

    /*!
     * \brief <+description of block+>
     * \ingroup ncdsss
     *
     */
    class NCDSSS_API DSSS_Encoder : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<DSSS_Encoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ncdsss::DSSS_Encoder.
       *
       * To avoid accidental use of raw pointers, ncdsss::DSSS_Encoder's
       * constructor is in a private implementation
       * class. ncdsss::DSSS_Encoder::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint16_t x1, uint16_t x2, uint16_t n1, uint16_t n2, int data_rate);
    };

  } // namespace ncdsss
} // namespace gr

#endif /* INCLUDED_NCDSSS_DSSS_ENCODER_H */
