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


    Testbench tb("Testbench");

    sc_start();
    return 0;
}
