/* -----------------------------------------------------------------
 * Consumer.cpp
 *
 * A SystemC consumer module.
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
#include "Consumer.h"

#include <systemc>

using std::cout;
using std::endl;

sc_event block_written;

// The consumer process
void Consumer::consumeTokens()
{    
    int buf[12];
    unsigned int blk = 0;

    cout << "Consumer: starting up" << endl;

	while (true) {
        
        for (int i = 0; i < 12; ++i) {
            buf[i] = inp.read();
        }

		pipe.write((char*) buf, sizeof(int32_t) * 12);
        pipe.flush();
        
        
        cout << "Consumer: Wrote block " << ++blk << " to pipe" << endl;
        
        block_written.notify();
        wait();
	}
	pipe.close();
}
