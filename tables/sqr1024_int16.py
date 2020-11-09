## generates a square-shaped table with values -32768 to 32767

import array
import os
import textwrap
import math

def generate(outfile, tablename, tablelength, samplerate):
    fout = open(os.path.expanduser(outfile), "w")
    fout.write('#ifndef ' + tablename + '_H_' + '\n')
    fout.write('#define ' + tablename + '_H_' + '\n \n')
    fout.write('#if ARDUINO >= 100'+'\n')
    fout.write('#include "Arduino.h"'+'\n')
    fout.write('#else'+'\n')
    fout.write('#include "WProgram.h"'+'\n')
    fout.write('#endif'+'\n')
    fout.write('#include "mozzi_pgmspace.h"'+'\n \n')
    fout.write('#define ' + tablename + '_NUM_CELLS '+ str(tablelength)+'\n')
    fout.write('#define ' + tablename + '_SAMPLERATE '+ str(samplerate)+'\n \n')
    outstring = 'CONSTTABLE_STORAGE(int16_t) ' + tablename + '_DATA [] = {'
    try:
        for num in range(tablelength):
            ## range between 0 and 1 first
            x = float(num)/tablelength
            
            if x<0.5:
                 t_x = 1.0
            else:
                 t_x = 0.0

            scaled = int(math.floor(t_x*65535.999))-32768
            
            outstring += str(scaled) + ', '
    finally:
        outstring +=  "};"
        outstring = textwrap.fill(outstring, 80)
        fout.write(outstring)
        fout.write('\n \n #endif /* ' + tablename + '_H_ */\n')
        fout.close()
        print "wrote " + outfile

generate("sqr1024_int16.h", "SQR1024X16", 1024, "1024X16")
