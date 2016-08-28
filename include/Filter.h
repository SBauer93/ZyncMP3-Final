/* -----------------------------------------------------------------
 * Filter.h
 *
 * Systemc filter module.
 *
 * Author:  Jens Rudolf (Uni Rostock)
 * Date:    2015-24-05
 *
 *
 * Changelog:
 *      2015-24-05 - Initial version
 *
 *
 * Copyright (c) 2015 by Jens Rudolf (Uni Rostock). All rights
 * reserved. For educational use only. NO WARRANTY!
 *
 * Report bugs to:
 *      jens.rudolf@uni-rostock.de
 *
 * -----------------------------------------------------------------
 */
#ifndef FILTER_H
#define FILTER_H

#include <systemc.h>
#include "libc.h"

 // @minimp3.c selfcoded - SBLIMIT
#define SBLIMIT 32
 // Just a comment to check, whether the Git integration works on my computer

 // @minimp3.c Z.59 - FIXHR
#define FIXHR(a) ((int)((a) * (1LL<<32) + 0.5))
 // @minimp3.c Z.57 - FIXR
 //			  Z.55 - FRAC_ONE
 //			  Z.45 - FRAC_BITS 
#define FRAC_BITS   15
#define FRAC_ONE    (1 << FRAC_BITS)
#define FIXR(a)   ((int)((a) * FRAC_ONE + 0.5))

 // @minimp3.c Z.777ff - Cx, icos36, icos36h
#define C1 FIXHR(0.98480775301220805936/2)
#define C2 FIXHR(0.93969262078590838405/2)
#define C3 FIXHR(0.86602540378443864676/2)
#define C4 FIXHR(0.76604444311897803520/2)
#define C5 FIXHR(0.64278760968653932632/2)
#define C6 FIXHR(0.5/2)
#define C7 FIXHR(0.34202014332566873304/2)
#define C8 FIXHR(0.17364817766693034885/2)

 const int icos36[9] = {
	FIXR(0.50190991877167369479),
	FIXR(0.51763809020504152469), //0
	FIXR(0.55168895948124587824),
	FIXR(0.61038729438072803416),
	FIXR(0.70710678118654752439), //1
	FIXR(0.87172339781054900991),
	FIXR(1.18310079157624925896),
	FIXR(1.93185165257813657349), //2
	FIXR(5.73685662283492756461),
};

 const int icos36h[9] = {
	FIXHR(0.50190991877167369479 / 2),
	FIXHR(0.51763809020504152469 / 2), //0
	FIXHR(0.55168895948124587824 / 2),
	FIXHR(0.61038729438072803416 / 2),
	FIXHR(0.70710678118654752439 / 2), //1
	FIXHR(0.87172339781054900991 / 2),
	FIXHR(1.18310079157624925896 / 4),
	FIXHR(1.93185165257813657349 / 4), //2
									   //    FIXHR(5.73685662283492756461),
};

// @libc.h Z.16 - INLINE
// #define INLINE __forceinline


// struct definitions for input buffer 
typedef struct _granule {
	uint8_t scfsi;
	int part2_3_length;
	int big_values;
	int global_gain;
	int scalefac_compress;
	uint8_t block_type;
	uint8_t switch_point;
	int table_select[3];
	int subblock_gain[3];
	uint8_t scalefac_scale;
	uint8_t count1table_select;
	int region_size[3];
	int preflag;
	int short_start, long_end;
	uint8_t scale_factors[40];
	int32_t sb_hybrid[SBLIMIT * 18];
} granule_t;

typedef struct _bitstream {
	const uint8_t *buffer, *buffer_end;
	int index;
	int size_in_bits;
} bitstream_t;

typedef struct _mp3_context {
	uint8_t last_buf[2 * 512 + 24];
	int last_buf_size;
	int frame_size;
	uint32_t free_format_next_header;
	int error_protection;
	int sample_rate;
	int sample_rate_index;
	int bit_rate;
	bitstream_t gb;
	bitstream_t in_gb;
	int nb_channels;
	int mode;
	int mode_ext;
	int lsf;
	int16_t synth_buf[2][512 * 2];
	int synth_buf_offset[2];
	int32_t sb_samples[2][36][SBLIMIT];
	int32_t mdct_buf[2][SBLIMIT * 18];
	int dither_state;
} mp3_context_t;

typedef unsigned char  uint8_t;
typedef   signed int   int32_t;
typedef   signed short int16_t;

/********************************************************************************************/


SC_MODULE(Filter) {
    // Input and output FIFO ports
	sc_fifo_in<int32_t>  inp;
	sc_fifo_out<int32_t> outp;
	sc_in<bool> clk;

    // The filter process
	void filterTokens();

	// The functional functions
	void compute_imdct(mp3_context_t *s, granule_t *g, int32_t *sb_samples, int32_t *mdct_buf);
	void imdct36(int *out, int *buf, int *in, int *win);
	void imdct12(int *out, int *in);

    // The module constructor
	SC_CTOR(Filter) {
		SC_CTHREAD(filterTokens, clk.pos());
	}
};

#endif
