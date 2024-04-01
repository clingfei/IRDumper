# IRDumper

A tool that can dump LLVM IR when compiling Linux kernel with LLVM, tested on LLVM-13.0.0. 


## Compile

```
export LLVM_DIR=/path/to/llvm/build
mkdir build && cd build
cmake ..
make 
```

## Usage

```
make LLVM=1 KCFLAGS="-flegacy-pass-manager -Xclang -load -Xclang /home/clf/DI-Pass/build/libIRDumper.so -mllvm input=/path/to/filelist" -j ($nproc)
```

The files listed in filelist must be **absolute path**.