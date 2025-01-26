/* -*- c++ -*- */
/*
 * Copyright 2025 CLoud_A.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NCDSSS_DSSS_DECODER_H
#define INCLUDED_NCDSSS_DSSS_DECODER_H

#include <gnuradio/ncdsss/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace ncdsss {

    /*!
     * \brief <+description of block+>
     * \ingroup ncdsss
     *
     */
    class NCDSSS_API DSSS_Decoder : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<DSSS_Decoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ncdsss::DSSS_Decoder.
       *
       * To avoid accidental use of raw pointers, ncdsss::DSSS_Decoder's
       * constructor is in a private implementation
       * class. ncdsss::DSSS_Decoder::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace ncdsss
} // namespace gr

#endif /* INCLUDED_NCDSSS_DSSS_DECODER_H */
