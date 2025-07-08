# gr-ncdsss
### Non-coherent direct sequence spread spectrum modulation and demodulation

Chip rate: `3.069Mbps`

Data rate: `16384bps`

use `10th order gold code`

## Dependents

    gnuradio 3.10
    cmake, make, git etc...

## Build && Install
run in Terminal:

    git clone https://github.com/CLA-179/gr-ncdsss.git
    cd gr-ncdsss
    mkdir build && cd build
    cmake ..
    make -j$(nproc)
    sudo make install
    sudo ldconfig