/* -----------------------------------------------------------------
 * Testbench.cpp
 *
 * SystemC testbench with simulation driver.
 *
 * Author:  Jens Rudolf (Uni Rostock)
 * Date:    2015-29-05
 *
 *
 * Changelog:
 *      2015-29-05 - Initial version
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
#include "Testbench.h"

#include <iostream>
#include <systemc.h>

using std::cerr;
using std::endl;

int sc_main(int argc, char *argv[]) {

    char* files[2];
    
    if(argc == 1) {
	    files[0] = "./HW_FIFO_IN";
	    files[1] = "./HW_FIFO_OUT";
    }
    else {
	    files[0] = argv[2];
	    files[1] = argv[1];
    }

    //cout << "Files :" << files[0] << " " << files[1] << endl;

    Testbench tb("Testbench");
    
    cout << "Model: Opening " << files[1] << endl;
    tb.producer.pipe.open(files[1], fstream::binary | fstream::in);

    cout << "Model: Opening " << files[0] << endl;
    tb.consumer.pipe.open(files[0], fstream::binary | fstream::out);

	cout << "Model: Files opened" << endl;

    sc_report_handler::set_actions(SC_WARNING, SC_DO_NOTHING);

    sc_start();
    return 0;
}
