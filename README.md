pe
==

[![Build Status](https://travis-ci.org/baihacker/pe.svg?branch=master)](https://travis-ci.org/baihacker/pe)
[![Build status](https://ci.appveyor.com/api/projects/status/scaji00tde2gb7uy?svg=true)](https://ci.appveyor.com/project/baihacker/pe)
[![Github Releases](https://img.shields.io/github/release/baihacker/pe.svg)](https://github.com/baihacker/pe/releases)

It is my basic code for solving problem on project euler.

Latest release: [Pe4.0](https://github.com/baihacker/pe/releases)

Installation:
-------------
* Put all the files in a folder (directory). For example, we put them in the directory D:\usr\include.
* Make sure the environment variable CPLUS_INCLUDE_PATH contain D:\usr\include.
* Use gen_config.py to generate pe_config automatically according to your environment, or
* Configure this library manually in [pe_config](https://github.com/baihacker/pe/blob/master/pe_config) or [pe](https://github.com/baihacker/pe/blob/master/pe). 
  * ENABLE_ASSERT whether to assert some inputs or conditions.
  
  * TRY_TO_USE_INT128 whether to check whether the compiler support int128 and use it. Some implementations depends on this flag.
  
  * ENABLE_EIGEN whether to use [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) library.
  
  * ENABLE_OPENMP swhether to use [openmp](http://www.openmp.org). The auto generated pe_config doesn't include this configuration since it will be checked and enable automatically in pe (you can still configure it in pe_config). In the configuration in pe, you can disable it evenif the environment supports it.
  
  * ENABLE_GMP whether to use [gmp](https://gmplib.org).
  
  * ENABLE_FLINT whether to use [FLINT](http://www.flintlib.org).
  
  * ENABLE_MPFR whether to use [mpfr](https://www.mpfr.org).
  
  * ENABLE_LIBBF wheter to use [libbf](https://bellard.org/libbf).
  
  * ENABLE_NTL whether to use [ntl](https://www.shoup.net/ntl/download.html).
  
* See [Build and use third party library in pe](https://github.com/baihacker/pe/blob/master/libraries_on_win64.md#build-and-use-third-party-library-in-pe) for the pre-built third party libraries.

* [optional, recommended] Generate precompile header "pe.hpp.gch".
  * Command: "g++ xc++-header pe.hpp". Run in D:\usr\include
  * You can add more options, usually, they are the same as the options to compile your target. e.g. "g++ -xc++-header pe.hpp --std=c++11 -O3 -march=native -fopenmp".

Use:
----
#include <pe.hpp> in your source code.

Use pe().maxPrime(prime limit).init(); to initialize builtin variables such as prime list, prime count . See the end of "pe" for details.

* **maxp** the maximum value in the prime list should be no more than maxp. Use pe().maxPrime(prime limit) to set the limit.
* **plist** plist[i] is the ith prime. (i starts from 0, i < pcnt)
* **pcnt** the number of prime no more than maxp, i.e. the element in plist.
* **mu** mu[i] where i <= maxp, if i > maxp, please use cal_mu(i). Please use pe().calMu() to initialize it.
* **phi** phi[i] where i <= maxp, if i > maxp, please use cal_phi(i). Please use pe().calPhi() to initialize it.

Prerequirements:
----------------
C++11 or above

File list:
-----------
* pe: Including all the files.
* pe.hpp: The file for generating precompile header.
* pe_algo: Algorithms.
* pe_array: An array implementation with compiling time dimension length. The element count can be more than the limit of int32 and you can specify customized allocator.
* pe_base: Some pre-including headers. Some macros and typedef. Some basic inline functions.
* pe_bi32: Big integer whose base is 1 << 32.
* pe_bit: Bit operation tricks.
* pe_config: a centralized place the configure pe.
* pe_fft: Fast fourier transform and polynomial multiplication.
* pe_fraction: Fraction arithmetic.
* pe_gbi: general big integer. The content corresponds to pe_nt.
* pe_geometry: Support Point2D and Point3D.
* pe_int128: Support to output int128 and the corresponding type traits.
* pe_io: methods and macros that simplify or fasten reading from and writing std io.
* pe_mat: Matrix operations.
* pe_memory: Memory manipulation such as allocating large memory. (windows only)
* pe_misc: misc codes.
* pe_mma: support mma: helper method or class to generate mma codes.
* pe_mod: Modular arithmetic.
* pe_nt: Basic code of number theory.
* pe_nt_base: Generate prime list, factorize integer, prime test, compute phi and mu.
* pe_ntt: Number theoretic transform and polynomial multiplication.
* pe_ntt_flint: An adapter which makes use of flint to implement ntt.
* pe_ntt_libbf: An adapter which makes use of libbf to implement ntt.
* pe_ntt_min_25: [Min_25](https://github.com/min-25)'s ntt implementation. The fastest one for mod polynomials integrated into pe.
* pe_ntt_ntl: ntl based ntt implementation.
* pe_parallel: A simple framework to solve problem with multi-threads. (windows only)
* pe_parallel_algo: Parallel algorithms.
* pe_persistance: KVPersistance. (it may support linux but we need change the generated cmdline and check the other codes)
* pe_poly: Polynomial.
* pe_poly_algo: Polynomial algorithms.
* pe_rand: Randome number.
* pe_sym_poly: Symbolic polynomial.
* pe_time: Support TimeDelta, TimeRecorder
* pe_tree: Some tree based data structures.
* pe_type_traits: Type traits.

Design principle:
--------------------
Fast and easy to use.

eg:

The factorizing interface is c++ style. It will be a little slower but it's easy to use.

[Use global variable plist (array of integers), pcnt to store the prime list and prime count, because they are frequently used. phi and miu are a c-style pointers, because they are not frequently used while vector<int> is a little heavier.] But now, the type of plist can be both array of integers and c-stype pointer, because the performance is nearly the same and sometimes we need dynamic allocation to obtain much more primes where a static allocation will make the binary not work.

Example:
--------
```cpp
#include <pe.hpp>

int main() {
  pe().maxPrime(1000000).init();

  int64 result = PARALLEL_RESULT(
  BEGIN_PARALLEL
    FROM 1 TO 100000000 EACH_BLOCK_IS 10000000 CACHE ""
    THREADS 10
    MAP {
        return is_prime_ex(key);
      }
    REDUCE {
        result += value;
        return result;
      }
  END_PARALLEL);
  dbg(result);

  return 0;
}
```
