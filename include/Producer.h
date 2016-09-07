/* -----------------------------------------------------------------
 * Producer.h
 *
 * A SystemC producer module.
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
#ifndef PRODUCER_H
#define PRODUCER_H

#include <fstream>
#include <systemc.h>

using std::ifstream;

SC_MODULE(Producer){
	// Output FIFO port
	sc_fifo_out<int32_t> outp;
	sc_in<bool> clk;

	// File stream for input data
    ifstream pipe;

    // The producer process
	void produceTokens();


    // The module constructor
	SC_CTOR(Producer) {
		SC_THREAD(produceTokens);
		sensitive << clk.pos();
        
        // pipe.open(f_out, fstream::binary);
	// "../HW_FIFO_OUT"
	}
};

#endif
