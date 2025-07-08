/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ncdsss author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "DSSS_Encoder_impl.h"
#include <gnuradio/io_signature.h>

#include "datastruct/gold.h"
#include "datastruct/fifobuf.h"

static uint8_t gold[1023];
// static uint32_t gold_buf[1023];
static uint16_t gold_index;

#define FIFO_SIZE 1024 * 8 * 4
FIFOBuffer fb;
unsigned char fifobuf[FIFO_SIZE];

namespace gr {
namespace ncdsss {

// #pragma message("set the following appropriately and remove this warning")
// using input_type = float;
// #pragma message("set the following appropriately and remove this warning")
using output_type = char;
DSSS_Encoder::sptr DSSS_Encoder::make(uint16_t x1, uint16_t x2, uint16_t n1, uint16_t n2)
{
    return gnuradio::make_block_sptr<DSSS_Encoder_impl>(x1, x2, n1, n2);
}


/*
 * The private constructor
 */
DSSS_Encoder_impl::DSSS_Encoder_impl(uint16_t x1, uint16_t x2, uint16_t n1, uint16_t n2)
    : gr::sync_block("DSSS_Encoder",
                     gr::io_signature::make(
                         0 /* min inputs */, 0 /* max inputs */, 0),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
    gold_gen(x1, x2, n1, n2, gold);
    gold_index = 0;
    // gold_buf_gen(gold, gold_buf);
    d_in_port = pmt::mp("in");
    message_port_register_in(d_in_port);
    
    fifo_init(&fb, fifobuf, FIFO_SIZE);

    set_msg_handler(d_in_port, [this](const pmt::pmt_t& msg) { pmt_in_callback(msg); });
}

/*
 * Our virtual destructor.
 */
DSSS_Encoder_impl::~DSSS_Encoder_impl() {}

void DSSS_Encoder_impl::pmt_in_callback(pmt::pmt_t msg)
{
    // pmt::pmt_t meta(pmt::car(msg));
    pmt::pmt_t bytes(pmt::cdr(msg));
    size_t msg_len, n_path;
    const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);
    printf("pmt_in:%d\n", msg_len);

    for (int i = 0; i < msg_len; i++)
    {
        for (int8_t j = 7; j > -1; j--)
        {
            if(!fifo_isfull(&fb))
            {
                fifo_push(&fb, (bytes_in[i] >> j) & 0x01);
            }
        }
        
    }
    
}

void sender(uint8_t *d, uint32_t len)
{
    static uint8_t dummy_bit = 0;
    uint8_t data_index = 0;
    uint8_t data = 0;

    for (uint32_t i = 0; i < len; i++)
    {
        if (data_index == 0)
        {
            if (!fifo_isempty(&fb))
            {
                data = fifo_pop(&fb);
            }
            else
            {
                data = dummy_bit;
                dummy_bit ^= 0x01U;
            }
            
        }

        data_index ++;

        if (data_index >= 187)
        {
            data_index = 0;
        }
        
        d[i] = data ^ gold[gold_index ++];

        if (gold_index == 1023)
        {
            gold_index = 0;
        }
        
        
    }
    
}

int DSSS_Encoder_impl::work(int noutput_items,
                            gr_vector_const_void_star& input_items,
                            gr_vector_void_star& output_items)
{
    auto out = static_cast<output_type*>(output_items[0]);

    sender((uint8_t *)out, noutput_items);
    
    return noutput_items;
}

} /* namespace ncdsss */
} /* namespace gr */
