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

The original VCL is [HERE](https://github.com/vectorclass/version2)
