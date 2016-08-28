/* -----------------------------------------------------------------
 * Filter.cpp
 *
 * A SystemC filter module.
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
#include "Filter.h"

#include <systemc>


using std::cout;
using std::endl;


// The filter process
void Filter::filterTokens() {

    cout << "Filter: Starting up" << endl;
    int in[16];
    int out[12];
    unsigned int blk = 0;

	while (true) {

        // Read 16 elements from software into in[].
		for (int i = 0; i < 16; ++i) {
			in[i] = inp.read();
		}
		
		// -------------->  Compute imdct12 here <-----------------

		imdct12(out, in);		

		// Write 12 elements of out[] back to software.
		for (int i = 0; i < 12; ++i) {
			outp.write(out[i]);
		}
		
        cout << "Filter: Processed block " << ++blk << endl;
        
        // Wait for next clock edge
		wait();
    }
}

void Filter::imdct12(int *out, int *in)
{
    int in0, in1, in2, in3, in4, in5, t1, t2;

    in0= in[0*3];
    in1= in[1*3] + in[0*3];
    in2= in[2*3] + in[1*3];
    in3= in[3*3] + in[2*3];
    in4= in[4*3] + in[3*3];
    in5= in[5*3] + in[4*3];
    in5 += in3;
    in3 += in1;

    in2= (int64_t)(2*in2)*(int64_t)(C3) >> 32;
    in3= (int64_t)(4*in3)*(int64_t)(C3) >> 32;

    t1 = in0 - in4;
    t2 = (int64_t)(2*(in1 - in5)) * (int64_t)(icos36h[4]) >> 32;

    out[ 7]=
    out[10]= t1 + t2;
    out[ 1]=
    out[ 4]= t1 - t2;

    in0 += in4>>1;
    in4 = in0 + in2;
    in5 += 2*in1;
    in1 = (int64_t)((in5 + in3)) * (int64_t)(icos36h[1]) >> 32;
    out[ 8]=
    out[ 9]= in4 + in1;
    out[ 2]=
    out[ 3]= in4 - in1;

    in0 -= in2;
    in5 = (int64_t)(2*(in5 - in3)) * (int64_t)(icos36h[7]) >> 32;
    out[ 0]=
    out[ 5]= in0 - in5;
    out[ 6]=
    out[11]= in0 + in5;
}

