# Fuzz testing
This is a simple fuzz testing suite based on the example examples/simplereader.c.

## Requirements
Fuzzing is done using the tools [American Fuzzy Lop](http://lcamtuf.coredump.cx/afl/) and 
[libFuzzer](https://llvm.org/docs/LibFuzzer.html). Here is an example Dockerfile that could
be used to run the fuzzing, it also includes other usefull tools.
```
FROM archlinux/base:latest

RUN pacman -Syy --noconfirm
RUN pacman -Su --noconfirm
RUN pacman -Sy gcc gdb lib32-gcc-libs wget clang make screen llvm llvm-libs gcc-multilib nano cmake nano git jdk8-openjdk 
libsodium valgrind cppcheck vim findutils which --noconfirm

WORKDIR "/tmp"
wget http://lcamtuf.coredump.cx/afl/releases/afl-latest.tgz
RUN tar -xf afl-latest.tgz && rm afl-latest.tgz && mv afl-* afl-latest
WORKDIR "afl-latest"
RUN CC=clang CXX=clang++ make && LLVM_CONFIG=llvm-config CC=clang CXX=clang++ make -C llvm_mode
RUN LLVM_CONFIG=llvm-config CC=clang CXX=clang++ make install
WORKDIR "/home/developer"
RUN wget http://trust-in-soft.com/tis-interpreter-2016-05.linux-x86_64.tar.gz
RUN tar -xf tis-interpreter-2016-05.linux-x86_64.tar.gz
WORKDIR "/home/developer"
RUN git clone https://github.com/linux-test-project/lcov && make -C lcov install && rm -rf lcov
RUN pacman -Sy sudo grep sed awk openssh nano --noconfirm
RUN useradd -mU -s /bin/bash docker && echo 'docker:docker' | chpasswd
RUN echo "docker ALL=(ALL:ALL) ALL" | (EDITOR="tee -a" visudo)
RUN echo "AllowUsers docker" >> /etc/ssh/sshd_config
EXPOSE 22
CMD [ ! -f /etc/ssh/ssh_host_rsa_key ] && ssh-keygen -A; /bin/sshd -D
```

## Fuzzing
A set of different fuzzing configurations is build:
* libFuzzer without and with adress-, memory-, and undefined behaviour sanitizer (build_libfuzzer*)
* AFL without and with adress sanitizer (build_afl*)
* A debug build (build_debug)

A simple shell script is provided to start fuzzing.
```sh
$ ./fuzz.sh
```

## Fuzzing template
1. Add a new file fuzz_*.c and implement the method:
```c
/**
 * Fuzz one input.
 *
 * When fuzzing using libFuzzer or AFL the return value is not considered.
 * But when running the program in debug mode the return code of the program
 * will depend on the return code of this function. This is useful for finding
 * valid/invalid input for later usage.
 *
 * @param data   Pointer to input data from fuzzer.
 * @param size   Size of input data.
 * 
 * @return true  The input is considered valid.
 * @return false The input is considered invalid.
 */
bool fuzz_one_input(const uint8_t *data, size_t size);
```
If the input is valid, true should be returned, false otherwise.
2. Add the new file to CMakeLists.txt
3. Create the directory input/fuzz_*/ and add some initial seeds.

