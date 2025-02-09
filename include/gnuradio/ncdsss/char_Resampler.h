/* -*- c++ -*- */
/*
 * Copyright 2025 CLoud_A.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NCDSSS_CHAR_RESAMPLER_H
#define INCLUDED_NCDSSS_CHAR_RESAMPLER_H

#include <gnuradio/block.h>
#include <gnuradio/ncdsss/api.h>

namespace gr {
namespace ncdsss {

/*!
 * \brief <+description of block+>
 * \ingroup ncdsss
 *
 */
class NCDSSS_API char_Resampler : virtual public gr::block
{
public:
    typedef std::shared_ptr<char_Resampler> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of ncdsss::char_Resampler.
     *
     * To avoid accidental use of raw pointers, ncdsss::char_Resampler's
     * constructor is in a private implementation
     * class. ncdsss::char_Resampler::make is the public interface for
     * creating new instances.
     */
    static sptr make(int symbol = 3069000, int data = 16384);
};

} // namespace ncdsss
} // namespace gr

#endif /* INCLUDED_NCDSSS_CHAR_RESAMPLER_H */
