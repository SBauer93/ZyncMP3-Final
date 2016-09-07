 /* -----------------------------------------------------------------
 * Producer.cpp
 *
 * Systemc producer module.
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
#include "Producer.h"

#include <systemc>

using std::cout;
using std::endl;

extern sc_event block_written;

// The producer process
void Producer::produceTokens()
{
    int32_t buf[16];
    unsigned int blk = 0;

    while(true) {
        
        // Read one block from file and write it to FIFO
        pipe.read((char*) buf, sizeof(int32_t) * 16);
        
		for (int i = 0; i < 16; ++i) {
			outp.write(buf[i]);
		}
        
        cout << "Producer: Sent block " << ++blk << " to Filter" << endl;
		wait(block_written);
        
	}
	pipe.close();

}
