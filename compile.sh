#!/bin/bash

g++ -I. -I/usr/local/systemc-2.3.2/include -L. -L/usr/local/systemc-2.3.2/lib-linux64 -Wl,-rpath=/usr/local/systemc-2.3.2/lib-linux64 -o testbench Testbench.cpp -lsystemc -lm && ./testbench |& tee log.txt
