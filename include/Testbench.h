/* -----------------------------------------------------------------
 * Testbench.h
 *
 * A SystemC testbench module.
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
#ifndef TESTBENCH_H
#define TESTBENCH_H

#ifdef __RTL_SIMULATION__
    #include "Filter_rtl_wrapper.h"
    #define Filter Filter_rtl_wrapper
#else
    #include "Filter.h"
#endif

#include "Consumer.h"
#include "Producer.h"

#include <systemc.h>

SC_MODULE(Testbench) {

    // The modules
    sc_clock clock;

	Producer producer;
    Filter	 filter;
    Consumer consumer;


    // The communication channels
	sc_fifo<int32_t> channelIn;
	sc_fifo<int32_t> channelOut;

    
    // The testbench constructor
	SC_CTOR(Testbench)
	    // Initialize modules and channels
		: clock		("my_clock", 10 ,SC_NS, 0.5, 1, SC_NS)
		, producer	("producer")
        , filter	("filter")
        , consumer	("consumer")
		, channelIn	("channelIn", 4096)
		, channelOut("channelOut", 4096) {

		
		// Connect producer and filter via channelIn
		producer.outp(channelIn);
		filter.inp(channelIn);

		// Connect filter and consumer via channelOut
		filter.outp(channelOut);
		consumer.inp(channelOut);

		producer.clk(clock);
		filter.clk(clock);
		consumer.clk(clock);
	}
};

#endif
