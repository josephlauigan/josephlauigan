ESE532 Final Project Code

Each C++ file is written in C-to-Gates framework - utilizing HLS Pragmas for conversion to FPGA fabric. 

Project Overview:
Multi-stage project  that centered on optimizing the measurement efficiency of the electron spin of a Nitrogen-Vacancy (NV) center in diamond. This readout method centers on an optical Spin-to-Charge conversion technique for reading out the electron spin state and microwave pulses to coherent control the state. Working in Lee Bassett's Quantum Engineering Lab at the University of Pennsylvania, I developed Verilog code to implement a photon counting threshold and dynamic stop-and-go signal on a Xilinx Artix-7 FPGA. This would allow for deterministic charge state initialization for the NV-center and speedup of the overall measurement time.

Synchronization of the free-running clock of the FPGA with the Arbitrary Waveform Generator and Digital Acquisition Card already in the experimental setup was crucial for the implementation. Verification test cases were developed to ensure correct functionality in controlled, real-time settings.

This work will expand and continue on into my master's thesis, where data will be taken to quantify speedup metrics for the measurement setups and exploration in leveraging the added functional benefits of the FPGA for identifying weakly-coupled Carbon-13 nuclear spins to the NV-center for applications in Quantum Memory Registers.
