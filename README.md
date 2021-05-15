# ATLHECTB
**A Geant4 simulation of the ATLAS hadronic end-cap calorimeter beam tests.**

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#project-description">Project description</a></li>
    <li><a href="#authors-and-contacts">Authors and contacts</a></li>
    <li>
      <a href="#how-to">How to</a>
      <ul>
        <li><a href="#build-compile-and-execute-on-maclinux">Build, compile and execute on Mac/Linux</a></li>
        <li><a href="#build-compile-and-execute-on-lxplus">Build, compile and execute on lxplus</a></li>
      </ul>
    </li>
    <li><a href="#selected-atlas-hec-references">Selected ATLAS HEC references</a></li>
  </ol>                                           
</details>

<!--Project desription-->
## Project description
The project targets a standalone Geant4 simulation of the ATLAS hadronic end-cap calorimeter beam tests to performe Geant4 regression testing, physics lists comparison and validation against test-beam data. 
- Start date: 11 May 2021 
- Status: under development. 

<!--Authors and contacts-->
## Authors and contacts
- Lorenzo Pezzotti (CERN EP-SFT) - lorenzo.pezzotti@cern.ch 
- Supervisor: Alberto Ribon (CERN EP-SFT)

<!--How to:-->
## How to

### Build, compile and execute on Mac/Linux
1. git clone the repo
   ```sh
   git clone https://github.com/lopezzot/ATLHECTB.git
   ```
2. cmake build directory and make (using geant4.10.07_p01)
   ```sh
   mkdir ATLHECTB-build; cd ATLHECTB-build/
   cmake -DGeant4_DIR=/absolute_path_to/geant4.10.07_p01-install/lib/Geant4-10.7.1/ relative_path_to/ATLHECTB/
   make
   ```
3. source Geant4 env
   ```sh
   source /relative_path_to/geant4.10.07_p01-install/bin/geant4.sh
   ```
4. execute (example with ATLHECTB_run.mac macro card, 2 threads and FTFP_BERT physics list)
   ```sh
   ./ATLHECTB -m ATLHECTB_run.mac -t 2 -pl FTFP_BERT
   ```
Parser options
   * -m macro.mac: pass a Geant4 macro card (example -m ATLHECTB_run.mac available in source directory and automatically copied in build directory) 
   * -t integer: pass number of threads for multi-thread execution (example -t 3, default t=2)
   * -pl Physics_List: select Geant4 physics list (example -pl FTFP_BERT)

### Build, compile and execute on lxplus
1. git clone the repo
   ```sh
   git clone https://github.com/lopezzot/ATLHECTB.git
   ```
2. cmake build directory and make (using geant4.10.07_p01, check for gcc and cmake dependencies for other versions)
   ```sh
   mkdir ATLHECTB-build; cd ATLHECTB-build/
   source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8.3.0/x86_64-centos7/setup.sh 
   source /cvmfs/geant4.cern.ch/geant4/10.7.p01/x86_64-centos7-gcc8-optdeb-MT/CMake-setup.sh 
   export CXX=`which g++`
   export CC=`which gcc`
   cmake3 -DGeant4_DIR= /cvmfs/geant4.cern.ch/geant4/10.7.p01/x86_64-centos7-gcc8-optdeb-MT/lib64/Geant4-10.7.1 ../ATLHECTB/
   make
   ```
3. execute (example with ATLHECTB_run.mac macro card, 2 threads and FTFP_BERT physics list)
   ```sh
   ./ATLHECTB -m ATLHECTB_run.mac -t 2 -pl FTFP_BERT
   ```
Hint: cp and source the ATLHECTB_lxplus_10.7.p01.sh file in the build directory.

<!--Selected ATLAS HEC references-->
## Selected ATLAS HEC references
- Geant4 evaluation with test-beam data (NIM A560 (2006)):  [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://www.sciencedirect.com/science/article/pii/S0168900205026835)
- Beam tests performance (NIM A482 (2002)): [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://www.sciencedirect.com/science/article/pii/S0168900201013389)
