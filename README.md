# Data Analysis
[comment]: \mainpage

A collection of tools for analyzing different types of outputs: MCNP tally output files, X-ray and gamma spectra,...

## Requirements

 * ROOT version 6.00 or later
 * GCC version 4.7 or later (support C++11 standard)
 * GNU make

## Install

#### Linux
 * Download and install ROOT6 from https://root.cern.ch/downloading-root .
 * Checkout the source: `git clone https://github.com/phuong84/data-analysis`.
 * Move to the `data-analysis` directory and type `make` to compile all, `make mcnp` to compile MCNPAnalysis or `make spec` to compile SpectrumAnalysis.
 * The compiled binary files will be put to the `bin` directory.

#### Windows
 * Download and install Cygwin from https://cygwin.com/install.html 
 * Install GCC, Make, Git along with Cygwin.
 * Download ROOT6 binary distribution `root_v6.00.02.win32gcc-gcc-4.8.tar.gz` from https://root.cern.ch/content/release-60002, and unzip the file.
 * Open Cygwin terminal
 * Open and edit the file `.bashrc` by adding these lines into it 
```bash
  export ROOTSYS=/directory/of/ROOT/binary
  export PATH=$PATH:$ROOTSYS/bin
  export LD_LIBRARY_PATH=$ROOTSYS/lib
```
(you may need to type `bash` after editing and closing the `~/.bashrc` file to update environment variables)
 * Checkout the source: `git clone https://github.com/phuong84/data-analysis`.
 * Move to the `data-analysis` directory and type `make` to compile all, `make mcnp` to compile MCNPAnalysis or `make spec` to compile SpectrumAnalysis.
 * The compiled binary files will be put to the `bin` directory.
 

## Getting Start


## Examples

