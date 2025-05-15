/* -*- c++ -*- */
/*
 * Copyright 2025 CLoud_A.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "DSSS_Decoder_impl.h"
#include <gnuradio/io_signature.h>

typedef unsigned char uint8_t;

#define MAX_DATABUF_LEN 2048

uint8_t gold[1023];
uint8_t find_flag;
u_int16_t gold_index = 0;
u_int16_t data_index = 0;

uint32_t gold_buf[1023];
uint32_t data_buf[MAX_DATABUF_LEN];
uint8_t un_full_data_buf_len;
uint8_t un_full_data_buf;
uint16_t data_buf_used;


int count_one(unsigned short in)
{
    static int out[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

    int o = out[in & 0x0f];
    o += out[(in >> 4) & 0x0f];
    o += out[(in >> 8) & 0x0f];
    o += out[(in >> 12) & 0x0f];

    // printf ("%d\n", o);

    return o;
}

int count_one32(unsigned int in)
{
    static int out[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

    int o = out[in & 0x0f];
    o += out[(in >> 4) & 0x0f];
    o += out[(in >> 8) & 0x0f];
    o += out[(in >> 12) & 0x0f];
    o += out[(in >> 16) & 0x0f];
    o += out[(in >> 20) & 0x0f];
    o += out[(in >> 24) & 0x0f];
    o += out[(in >> 28) & 0x0f];

    // printf ("%d\n", o);

    return o;
}

void gold_gen(unsigned short _x1,
              unsigned short _x2,
              unsigned short _n1,
              unsigned short _n2)
{
    unsigned short x1, x2, n1, n2, temp;

    x1 = _x1;
    n1 = _n1;
    x2 = _x2;
    n2 = _n2;

    for (int i = 0; i < 1023; i++) {
        gold[i] = ((x1 ^ x2) & 0x200) >> 9;
        printf("%d", ((x1 ^ x2) & 0x200) >> 9);

        temp = (count_one(x1 & n1) & 0x01);
        x1 = ((x1 << 1) & 0x3fe) | temp;

        temp = (count_one(x2 & n2) & 0x01);
        x2 = ((x2 << 1) & 0x3fe) | temp;
    }
    printf("\n");
}

void gold_buf_gen(void)
{
    for (uint16_t i = 0; i < 1023; i ++)
    {
        for (uint8_t j = 0; j < 32; j++)
        {
            gold_buf[i] <<= 1;
            gold_buf[i] |= (i + j < 1023 ? gold[i + j] : gold[i + j - 1023]) & 0x01U;
            
        }
        for (uint8_t i = 0; i < 32; i++)
        {
            printf("%d", (gold_buf[i] >> i) & 0x01);
        }
        printf("\n");
        
        
    }
    // exit(0);
}


namespace gr {
namespace ncdsss {

// #pragma message("set the following appropriately and remove this warning")
using input_type = char;
// #pragma message("set the following appropriately and remove this warning")
using output_type = char;
DSSS_Decoder::sptr DSSS_Decoder::make(uint16_t x1, uint16_t x2, uint16_t n1, uint16_t n2)
{
    return gnuradio::make_block_sptr<DSSS_Decoder_impl>(x1, x2, n1, n2);
}


/*
 * The private constructor
 */
DSSS_Decoder_impl::DSSS_Decoder_impl(uint16_t x1, uint16_t x2, uint16_t n1, uint16_t n2)
    : gr::block("DSSS_Decoder",
                gr::io_signature::make(
                    1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
    gold_gen(x1, x2, n1, n2);
    gold_buf_gen();
    find_flag = 0;
}

/*
 * Our virtual destructor.
 */
DSSS_Decoder_impl::~DSSS_Decoder_impl() {}

void DSSS_Decoder_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{

    ninput_items_required[0] = noutput_items;
    // ninput_items_required[0] = 32;

    /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
}

int DSSS_Decoder_impl::general_work(int noutput_items,
                                    gr_vector_int& ninput_items,
                                    gr_vector_const_void_star& input_items,
                                    gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    //  Do <+signal processing+>
    //  Tell runtime system how many input items we consumed on
    //  each input stream.
    consume_each(noutput_items);

    int zero_count = 0;


    // printf("%d\n", gold_index);

    if (noutput_items > MAX_DATABUF_LEN * 32)
    {
        printf("data in too long....");
        exit(1);
    }
    

    if (!find_flag) {
        int i ;
        for (i = 0; i <= noutput_items / 32 - 1; i += 1) {
            for (uint8_t j = 0; j < 32; j++)
            {
                
                data_buf[i] <<= 1;
                data_buf[i] |= in[i * 32 + j];
            }
            
            // if (data_buf[i] == 0) {
            //     zero_count++;
            // }
        }

        data_buf_used = i;

        un_full_data_buf_len = noutput_items - i * 32;
        if (un_full_data_buf_len > 0)
        {
            for (uint8_t j = 0; j < un_full_data_buf_len; j++)
            {
                data_buf[i] <<= 1;
                data_buf[i] |= in[i * 32 + j];
            }
        
            i ++;
            data_buf_used = i;


        }
        

        // printf("noutput: %d\ndatabug:%d\n", noutput_items, i * 32);
    }


    // if (zero_count < noutput_items / 32) // if all data is zero, dont poccess to save the time.
    {
        if (!find_flag) {
            if (data_buf_used > 1 || un_full_data_buf_len > 16)
            {
                for (uint16_t i = 0; i < data_buf_used && find_flag == 0; i++)
                {
                    for (uint16_t j = 0; j < 1023; j++)
                    {
                        uint32_t result = data_buf[i] ^ gold_buf[j];

                        result ^= result << 1;
                        result &= 0xfffffffe;
                        uint8_t ones = count_one32(result);
                        if (ones< 3)
                        {
                            data_index = i;
                            gold_index = j;
                            find_flag = 1;
                            printf("find start!!\n");
                            break;
                        }
                        // else printf("not:%x\n", result);
                        
                    }
                    
                }
                
            }

            #if 0
            if (noutput_items >= 187) {
                for (int i = 0; i < 1023 - 187; i++) {
                    uint32_t change_count = 0;
                    uint8_t last = 0;
                    uint8_t now;
                    for (uint8_t j = 0; j < 187; j++) {
                        now = (in[j]) ^ (gold[i + j]);
                        // printf("%d", now);

                        if (now != last) {
                            change_count++;
                        }

                        last = now;
                    }
                    if (change_count < 3) {
                        // printf("%d\n", noutput_items);
                        printf("find!!,start\n");

                        gold_index = i;
                        find_flag = 1;
                        break;
                    } else {
                        // printf("\n");
                        // printf("%d\n", change_count);
                    }
                }
            }
            #endif
        }

        
    } 
    if (find_flag) {
        int last = 0;
        int now;
        int i = 0;
        for ( i = 0; i < data_index; i++)
        {
            out[i]=in[i];
        }
        data_index = 0;
        
        for (; i < noutput_items - 187 && i > -1 && find_flag; i += 187) {
            int change_count = 0;
            for (int j = 0; j < 187; j++) {
                now = ((in[i + j]) ^ gold[gold_index++]);
                
                if (now != last) {
                    change_count++;
                    // printf("changed\n");
                }
                last = now;

                out[i + j] = now;

                if (gold_index == 1023)
                    gold_index = 0;
            }
            if (change_count > 30) {
                find_flag = 0;
                printf("refinding....\n");
                break;
            } else {
                // printf("%d\n", change_count);
            }
        }
        for (; i < noutput_items; i++) {
            out[i] = ((in[i]) ^ gold[gold_index++]);
            if (gold_index == 1023)
                gold_index = 0;
        }


    } else {
        for (int i = 0; i < noutput_items; i++) {
            out[i] = 0;
        }
    }


    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace ncdsss */
} /* namespace gr */
