# Vectorclass SIMDE (VCLSimde)
This is a fork of Agner Fog's excellent vectorclass library, modified to use SIMDe as an intermediary for emulation of SIMD instruction sets that may not be present.

Explicit goals:
* Use Vec8f or Vec16f as general purpose abstract SIMD blocks
* Zero-cost abstraction when writing to the native ISA
* Transparent fallback to AVX2/SSE as needed
* High performance conversion to WebAssembly and NEON

Explicit non-goals:
* Support of narrower than 128bit SIMD

The original vectorclass supports the first three goals. Targeting SIMDe as an intermediary allows for the fourth.



Below is the original VCL README:

# version2
Vector Class Library, latest version

This is a C++ class library for using the Single Instruction Multiple Data (SIMD) instructions to improve performance on modern microprocessors with the x86 or x86/64 instruction set on Windows, Linux, and Mac platforms. There are no plans to support ARM or other instruction sets.

[Latest release](https://github.com/vectorclass/version2/releases)

[Download manual](https://github.com/vectorclass/manual/raw/master/vcl_manual.pdf)

[Add-on packages for particular applications](https://github.com/vectorclass/add-on)

[Getting-started video.](https://www.youtube.com/watch?v=TKjYdLIMTrI) Video blogger Christopher Rose has made this nice video telling how to get started with the Vector Class Library.

**Help:** You may ask for programming help on [StackOverflow](https://stackoverflow.com) using the tag vector-class-library.
