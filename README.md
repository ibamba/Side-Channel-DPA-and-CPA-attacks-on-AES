# Side-Channel-DPA-and-CPA-attacks-on-AES
This project is the implementation of the side channel CPA &amp; DPA Attacks applied to AES-128 algorithm, in C language.

## Side-Channel Attack
In computer security, a side-channel attack is any attack based on information gained from the implementation of a computer system, rather than weaknesses in the implemented algorithm itself (e.g. cryptanalysis and software bugs). Timing information, power consumption, electromagnetic leaks or even sound can provide an extra source of information, which can be exploited. 
There is several type of side channel attacks : Timming Attack, Power-analysis attack, Electromagnetic attack, Differential fault analysis... In this project, we will use Differential Power Analysis and Correlation Power Analysis against the AES algorithm.

## Files
The .h and .c files contain the source code of the attacks.
The Makefile compile the sources code.
The report file contains more detail on the techniques and algorithms used as well as the work achieved. Do not hesitate to read it, it can be very enriching.
And finally, the .ipynb files are jupyter files. It contains details on the work achieved before the attacks: the capture of traces, some tests ...

## Usage
You can execute the code using the command : 
```bash
./main knownkeys_file plaintexts_file traces_file CPA/DPA [ind_bit (if attack is DPA, ind_bit in [0;7] is to partition traces)]
```

Data files (knownkey, plaintexts and traces) should be in a raw format (int8, int8, float32). The program considers that its the same key
used for all traces, so only 1 key entry (the first one) will be read from knownkeys_file.

To execute a CPA attack, run : 
```bash
./main knownkeys_file plaintexts_file traces_file CPA
```

To execute a DPA attack (and bit index 2 to partition the traces for example), run :
```bash
./main knownkeys_file plaintexts_file traces_file DPA 2
```

To modify the number of traces, the size of traces, or the sub-interval of samples in the traces to consider in the attack, modify defined constants in attack.h file:
```
NB_TRACES: Defines the number of traces

SIZE_TRACE : Defines the total number of samples in each trace

SUBINTERVAL1 and SUBINTERVAL2 : Defines the subinterval of samples of each trace to consider for the attack [SUBINTERVAL1; SUBINTERVAL2], if all samples are to be considered use SUBINTERVAL1 0  and SUBINTERVAL2 (SIZE_TRACE - 1)
```