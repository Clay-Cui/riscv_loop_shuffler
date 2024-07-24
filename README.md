
# Configurable Loop Shuffling via Instruction Set Extensions
This repository contains the source code of the implementation of the Configurable loop shuffler for RISC-V.

This ReadMe walks through the steps of recreating the [LiteX](https://github.com/enjoy-digital/litex) framework environment, generating extended CV32E40P core, and creating the SoC file as well as the bitstream used for FPGA evaluation. 


## LiteX Environment

This project relies on LiteX environment to generate SoC wrapper project which is later used to generate the bitstream.

To install Litex:
```sh
wget https://raw.githubusercontent.com/enjoy-digital/litex/master/litex_setup.py
chmod +x litex_setup.py
./litex_setup.py --init --install --user --config=standard
```

## CV32E40P
LiteX has official support for CV32E40P through an old commit. We use this commit as the starting point. 

First, copy all files in `/src/vender/rtl` to the corresponding `pythondata-cpu-cv32e40p` folder.
```sh
cp src/vender/rtl/* litex/pythondata-cpu-cv32e40p/pythondata-cpu-cv32e40p/rtl/
```
Then, copy the config file include the newly added source files. 
```sh
cp src/vender/*.yml litex/pythondata-cpu-cv32e40p/pythondata-cpu-cv32e40p/
cp src/vender/cv32e40p_manifest.flist litex/pythondata-cpu-cv32e40p/pythondata-cpu-cv32e40p/
```


## SoC Generation

Finally, we can use LiteX to generate the SoC wrapper:
```sh
pushd src/SoC
python3 base.py
popd
```
This command will invoke Vivado and create `/build` directory which contains the generated SoC project and compiled software library.
## GCC modification
### Build from scrach
In order to compile the firmware with custom instruction, customized toolchain is needed and needs to be compiled from source code. Note that this may take some time depending on computer configuration. More detailed tutorial can be found [here](https://pcotret.gitlab.io/riscv-custom/).

First, clone the pulp GCC compiler to use the customized hardware loop instructions. 
```sh
git clone https://github.com/pulp-platform/pulp-riscv-gcc
```
Then copy the two files in `riscv-gnu-toolchain` to the correct location.
```sh
cp toolchain/riscv-opc.h pulp-riscv-gnu-toolchain/riscv-binutils/include/opcode/riscv-opc.h
cp toolchain/riscv-opc.c pulp-riscv-gnu-toolchain/riscv-binutils/opcodes/riscv-opc.c
```
To target the installation on Newlib:
```sh
./configure --prefix=/opt/riscv
make
```
### Precompiled version
We provide precompiled binary file (gcc and objdump) in `/toolchain/bin` for easy usage.

## Firmware update:

To generate new firmware:
```sh
pushd src/SoC/firmware
make all CC='path/to/your/riscv/compiler'
popd
```

Litex provides simple but handy tool, the litex_term to update firmware through UART:
```sh
pushd src/SoC
litex_term --speed 115200 --kernel firmware/demo.bin /dev/ttyUSB0
```
We provide python script for easier updating the firmware as well as communication with the board, including sending plaintext, receiving and validating ciphertext.
```sh
pushd src/SoC
python3 aes_validate.py
popd
```
