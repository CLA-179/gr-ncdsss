/* -*- c++ -*- */
/*
 * Copyright 2025 CLoud_A.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NCDSSS_BPSK_SQUELCH_H
#define INCLUDED_NCDSSS_BPSK_SQUELCH_H

#include <gnuradio/ncdsss/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace ncdsss {

    /*!
     * \brief <+description of block+>
     * \ingroup ncdsss
     *
     */
    class NCDSSS_API bpsk_squelch : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<bpsk_squelch> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ncdsss::bpsk_squelch.
       *
       * To avoid accidental use of raw pointers, ncdsss::bpsk_squelch's
       * constructor is in a private implementation
       * class. ncdsss::bpsk_squelch::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace ncdsss
} // namespace gr

#endif /* INCLUDED_NCDSSS_BPSK_SQUELCH_H */
