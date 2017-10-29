// petest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cstdlib>
#include <iostream>
using namespace std;

const int maxp = 100000;
/*
static int plist[maxp/10];  
static int pmask[maxp+1];
static int pcnt;
int* phi = NULL;
int* miu = NULL;
const int64 pmpi[];

SL int64 estimate_pmpi(int64 n)
SL int is_prime(int64 n)
SL void init_primes()
SL void init_primes(int cal_phi, int cal_miu)

SL vector<pair<int64, int> > factorize(int64 n)
SL vector<int64> get_factors(const vector<pair<int64, int> >& f)
SL vector<int64> get_factors(int64 value)

SL int64 square_root(int64 n)
SL int64 add_mod(int64 a, int64 b, int64 mod)
SL int64 sub_mod(int64 a, int64 b, int64 mod)
SL int64 mul_mod(int64 a, int64 b, int64 mod)
SL int64 mul_mod_ex(int64 a, int64 b, int64 mod)
SL int64 power_mod(int64 x, int64 n, int64 mod)
SL int64 power_mod_ex(int64 x, int64 n, int64 mod)
SL int64 inv_of(int64 x, int64 p)

// Primitive Root
namespace pr{
SL bool is_pr(const vector<pair<int64, int> >& f, int64 x, int64 p)
SL bool is_pr(int64 x, int64 p)
SL int64 find_pr(int64 p)
int64 ind(int64 a, int64 pr, int64 p)
int64 ind1(int64 a, int64 pr, int64 p)
}

template<typename T>
struct Fraction;
*/
#ifndef __PE_NT_H__ 
#define __PE_NT_H__ 
 
// compiler checking
#if defined(__gnuc__) 
#define CMPILER_GNU 
#elif defined(_MSC_VER) 
#define COMPILER_MSVC 
#endif 

#ifdef CMPILER_GNU
	#if __cplusplus < 201103L 
		#error "c++11 or above" 
	#endif
#endif

#ifdef COMPILER_MSVC
	#if _MSC_VER < 1800
		#error "VS2013 or above"
	#endif
#endif
 
#include <cstdint> 
 
#include <cstdio> 
#include <cstdlib> 
#include <cmath> 
#include <ctime> 
#include <cassert> 
 
#include <random> 
#include <algorithm> 
#include <utility> 
#include <map> 
#include <vector> 
#include <unordered_map> 
#include <tuple> 
 
#include <iostream> 
 
using namespace std; 

// function specifier 
#ifndef SL 
#define SL static inline 
#endif 
 
// PE_ASSERT 
#ifdef DISABLE_ASSERT 
    #define PE_ASSERT(x) 
#else 
    #define PE_ASSERT(x) assert((x)) 
#endif 
 
// About numbers 
 
// Integer limits check 
static_assert(INT_MAX == LONG_MAX, "INT_MAX == LONG_MAX"); 
 
// int64 and int128 
typedef std::int64_t int64; 
#ifdef COMPILER_GNU 
     #ifdef _M_X64 
         #define HAS_INT128 1 
         typedef __int128 int128; 
     #else 
         #define HAS_INT128 0 
     #endif 
#endif 
 
// square overflow integers 
static const int64 PE_SOI63 = 3037000499LL;   // x * x > 2^63 - 1 for x > PE_SOI63 
static const int64 PE_SOI64 = 4294967295LL;   // x * x > 2^64 - 1 for x > PE_SOI64 
 
// About primes 
static_assert(maxp >= 100000, "maxp >= 100000"); 
static_assert(maxp <= INT_MAX, "maxp <= INT_MAX"); 
static const int64 maxp2 = (int64)maxp * maxp; 
 
static int pcnt; 
static int plist[maxp/10]; 
static int pmask[maxp+1]; 
int* phi = NULL; 
int* miu = NULL; 
 
const int64 pmpi[] = 
{ 
0LL, 
4LL,                // 10 
25LL,               // 100 
168LL,              // 1000 
1229LL,             // 1e4 
9592LL,             // 1e5 
78498LL,            // 1e6 
664579LL,           // 1e7 
5761455LL,          // 1e8 
50847534LL,         // 1e9 
455052512LL,        // 1e10 
4118054813LL,       // 1e11 
37607912018LL,      // 1e12 
346065536839LL,     // 1e13 
3204941750802LL,    // 1e14 
}; 
 
SL int64 estimate_pmpi(int64 n) 
{ 
    if (n <= 1) return 0; 
    int64 ret = static_cast<int64>(1. * n / log(1.*n));
    return max(ret, 1LL); 
} 
 
SL int is_prime(int64 n) 
{ 
    if (n <= 1) return 0; 
    if (n <= maxp) return pmask[n] == n; 
    PE_ASSERT(n <= maxp2); 
    for (int i = 0; i < pcnt; ++i) 
    { 
        const int64 p = plist[i]; 
        const int64 test = p * p; 
        if (test > n) break; 
        if (n % p == 0) return 0; 
    } 
    return 1; 
} 
 
SL void init_primes() 
{ 
    pcnt = 0; 
    for (int i = 1; i <= maxp; ++i) pmask[i] = i; 
    for (int i = 2; i <= maxp; ++i) 
    { 
        if (pmask[i] == i) 
        { 
            plist[pcnt++] = i; 
        } 
        for (int j = 0; j < pcnt; ++j) 
        { 
            const int64 t = (int64)plist[j] * i; 
            if (t > maxp) break; 
            pmask[t] = plist[j]; 
            if (i % plist[j] == 0) 
            { 
                break; 
            } 
        } 
    } 
} 
 
SL void init_primes(int cal_phi, int cal_miu) 
{ 
    pcnt = 0; 
    if (cal_phi) phi = new int[maxp+1]; 
    if (cal_miu) miu = new int[maxp+1]; 
     
    for (int i = 1; i <= maxp; ++i) pmask[i] = i; 
    if (phi) phi[1] = 1; 
    if (miu) miu[1] = 1; 
     
    for (int i = 2; i <= maxp; ++i) 
    { 
        if (pmask[i] == i) 
        { 
            if (miu) miu[i] = -1; 
            if (phi) phi[i] = i - 1; 
            plist[pcnt++] = i; 
        } 
        for (int j = 0; j < pcnt; ++j) 
        { 
            const int64 t = (int64)plist[j] * i; 
            if (t > maxp) break; 
            pmask[t] = plist[j]; 
            if (i % plist[j] == 0) 
            { 
                if (miu) miu[t] = 0; 
                if (phi) phi[t] = phi[i] * plist[j]; 
                break; 
            } 
            else 
            { 
                if (miu) miu[t] = -miu[i]; 
                if (phi) phi[t] = phi[i] * (plist[j] - 1); 
            } 
        } 
    } 
} 
 
SL vector<pair<int64, int> > factorize(int64 n) 
{ 
    vector<pair<int64, int> > ret; 
    if (n <= 1) 
    { 
        return ret; 
    } 

    if (n <= maxp) 
    { 
        while (n != 1) 
        { 
            int now = pmask[n]; 
            int c = 0; 
            while (n % now == 0) n /= now, ++c; 
            if (c) ret.push_back({(int64)now, c}); 
        } 
    } 
    else 
    { 
        PE_ASSERT(n <= maxp2); 
        for (int i = 0; i < pcnt; ++i) 
        { 
            const int64 p = plist[i]; 
            const int64 test = p * p; 
            if (test > n) break; 
            int c = 0; 
            while (n % p == 0) n /= p, ++c; 
            if (c) ret.push_back({p, c}); 
        } 
        if (n != 1) ret.push_back({n, 1}); 
    } 
    return ret; 
} 
 
void get_factors_impl(const int limit, const vector<pair<int64, int> >& f, int64 value, vector<int64>& result) 
{ 
    result.push_back(value); 
    for (int i = 0; i < limit; ++i) 
    { 
        int64 tvalue = value; 
        const int64 p = f[i].first; 
        const int c = f[i].second; 
        for (int j = 1; j <= c; ++j) 
        { 
            tvalue *= p; 
            get_factors_impl(i, f,  tvalue, result); 
        } 
    } 
} 
 
SL vector<int64> get_factors(const vector<pair<int64, int> >& f) 
{ 
    vector<int64> result; 
    get_factors_impl(static_cast<int>(f.size()), f, 1, result); 
    return result; 
} 
 
SL vector<int64> get_factors(int64 value) 
{ 
    vector<int64> result; 
    if (value <= 1) 
    { 
        result.push_back(1); 
        return result; 
    } 
    auto f = factorize(value); 
    get_factors_impl(static_cast<int>(f.size()), f, 1, result); 
    return result; 
} 
 
SL int64 square_root(int64 n) 
{ 
    if (n <= 0) return 0; 
    if (n == 1) return 1; 
    for (int64 test = static_cast<int64>(sqrt(n));;++test) 
    { 
        int64 t = test * test; 
        if (t == n) return test; 
        if (t > n) return 0; 
    } 
    return 0; 
} 
 
// Modular arithmetic 
SL int64 add_mod(int64 a, int64 b, int64 mod) 
{ 
    a += b; 
    if (a >= mod) a -= mod; 
    return a; 
} 
 
SL int64 sub_mod(int64 a, int64 b, int64 mod) 
{ 
    a -= b; 
    if (a < 0) a += mod; 
    return a; 
} 
 
SL int64 mul_mod(int64 a, int64 b, int64 mod) 
{ 
    return a*b%mod; 
} 
 
SL int64 mul_mod_ex(int64 a, int64 b, int64 mod) 
{ 
#if HAS_INT128 
    return (int128)a * b % mod; 
#else 
    int64 ret = 0; 
    int64 flag = b; 
    for (int64 d = 1; d <= a; d <<= 1) 
    { 
        if (d & a) 
        { 
            ret = (ret + flag) % mod; 
        } 
        flag = (flag << 1) % mod; 
    } 
    return ret; 
#endif 
} 
 
SL int64 power_mod(int64 x, int64 n, int64 mod) 
{ 
    int64 ret = 1; 
    x %= mod; 
    if (x < 0) x += mod; 
    for (; n; n >>= 1) 
    { 
        if (n&1) ret = ret * x % mod; 
        x = x * x % mod; 
    } 
    return ret; 
} 
 
SL int64 power_mod_ex(int64 x, int64 n, int64 mod) 
{ 
#if HAS_INT128 
    int64 ret = 1; 
    x %= mod; 
    if (x < 0) x += mod; 
    for (; n; n >>= 1) 
    { 
        if (n&1) ret = (int128)ret * x % mod; 
        x = (int128)x * x % mod; 
    } 
    return ret; 
#else 
    int64 ret = 1; 
    x %= mod; 
    if (x < 0) x += mod; 
    for (; n; n >>= 1) 
    { 
        if (n&1) ret = mul_mod_ex(x, ret, mod); 
        x = mul_mod_ex(x, x, mod); 
    } 
    return ret; 
#endif 
} 
 
// p is prime 
SL int64 inv_of(int64 x, int64 p) 
{ 
    return power_mod(x, p-2, p); 
} 
 
// calculate C(n, m) % P where P is prime 
template<int64 P, int64 F = P> 
struct comb_moder 
{ 
    static_assert(P <= PE_SOI63, "P <= PE_SOI63"); 
    static_assert(F > 0, "F > 0"); 
    comb_moder() 
    { 
        init(); 
    } 
    static int64 inv(int64 x) 
    { 
        int64 result = 1; 
        int n = P - 2; 
        x %= P; 
        for (; n; n >>= 1, x = x * x % P) if (n&1) result = result * x % P; 
        return result; 
    } 
    static int64* fac; 
    static void init() 
    { 
        if (fac == NULL) 
        { 
            fac = new int64[F]; 
            fac[0] = 1; 
            for (int i = 1; i < F; ++i) fac[i] = fac[i-1] * i % P; 
        } 
    } 
    static void destroy_fac() 
    { 
        delete[] fac; 
        fac = NULL; 
    } 
    static int64 s(int x) {return x & 1 ? -1 : 1;} 
    static int64 comb(int64 m, int64 n) 
    { 
        PE_ASSERT(fac); 
         
        if (m < 0 || n > m) return 0; 
         
        int64 pp = 0; 
        int64 dist = m - n; 
         
        // todo: overflow x *= P; 
        for (int64 x = P; x <= m; x *= P) pp += m / x - n / x - dist / x; 
        if (pp) return 0; 
        int64 l = 1, r = 1; 
        for (int64 x = m; x; x /= P) {PE_ASSERT(x%P<F); l = l * s(x/P) * fac[x%P] % P;} 
        for (int64 x = n; x; x /= P) {PE_ASSERT(x%P<F); r = r * s(x/P) * fac[x%P] % P;} 
        for (int64 x = dist; x; x /= P) {PE_ASSERT(x%P<F); r = r * s(x/P) * fac[x%P] % P;} 
        l = (l + P) % P; 
        r = (r + P) % P; 
        int64 t  = (inv(r) * l % P + P) % P; 
        return t; 
    } 
}; 
template<int64 P, int64 F> 
int64 * comb_moder<P, F>:: fac; 
 
// greatest common divisor and extend greatest common divisor 
#ifndef GNU_COMPILER 
template<typename _EuclideanRingElement> 
_EuclideanRingElement 
__gcd(_EuclideanRingElement __m, _EuclideanRingElement __n) 
{ 
    while (__n != 0) 
    { 
        _EuclideanRingElement __t = __m % __n; 
        __m = __n; 
        __n = __t; 
    } 
    return __m; 
} 
#endif 
 
#define gcd __gcd 
 
int64 exgcd_impl(int64 a, int64 b, int64& x, int64& y) 
{ 
    if (b == 0) 
    { 
        x = 1; 
        y = 0; 
        return a; 
    } 
     
    int64 x1 = 0, y1 = 0; 
    const int64 ret = exgcd_impl(b, a%b, x1, y1); 
    // b * x1 + (a%b) * y1 = ret 
    // b * x1 + (a - a / b * b) * y1 = ret 
    // b * x1 + a * y1 - a / b * b * y1 = ret 
    // y1 * a + (x1 - a / b * y1) b = ret 
    x = y1; 
    y = x1 - a / b * y1; 
    return ret; 
} 
 
SL int64 exgcd(int64 a, int64 b, int64& x, int64& y) 
{ 
    PE_ASSERT(a != 0 || b != 0); 
    const int64 r = exgcd_impl(a, b, x, y); 
    PE_ASSERT(a*x+b*y==r); 
    return r; 
} 
 
SL tuple<int64, int64, int64> exgcd(int64 a, int64 b) 
{ 
    PE_ASSERT(a != 0 || b != 0); 
    int64 x = 0, y = 0; 
    const int64 r = exgcd_impl(a, b, x, y); 
    PE_ASSERT(a*x+b*y==r); 
    return tuple<int64, int64, int64>{r, x, y}; 
} 
 
// solve equation a x = b (mod m) 
// return 0 and set u = -1 and v = 0 if failed 
// return 1, x = v (mod u) 
SL int solve_linear_equation(int64 a, int64 b, int64 m, int64& v, int64& u) 
{ 
    v = 0; 
    u = 0; 
     
    PE_ASSERT(m); 
     
    if (m < 0) m = -m; 
     
    PE_ASSERT(m <= PE_SOI63); 
     
    a %= m; 
    b %= m; 
     
    if (a < 0) a += m; 
    if (b < 0) b += m; 
     
    int64 xx = 0, yy = 0, d = exgcd(a, m, xx, yy); 
     
    if (b % d) return 0; 
     
    xx = (xx % m + m) % m; 
    xx = xx * (b / d) % m; 
     
    v = xx; 
    u = m / d; 
    return 1; 
} 
 
// solve equation x = a (mod m1) x = b (mod m2) 
// return 0 and set u = 0 and t = 0 if failed 
// return 1, x = v (mod u) 
SL int crt2(int64 a, int64 m1, int64 b, int64 m2, int64& v, int64& u) 
{ 
    v = 0; 
    u = 0; 
     
    PE_ASSERT(m1 != 0 && m2 != 0); 
    if (m1 < 0) m1 = -m1; 
    if (m2 < 0) m2 = -m2; 
    // x = a (mod m1) 
    // x = b (mod m2) 
    // x = k0 m1 + a 
    // k0 m1 + a = b (mod m2) 
    // k0 m1 = b - a (mod m2) 
    a %= m1; 
    b %= m2; 
    if (a < 0) a = -a; 
    if (b < 0) b = -b; 
     
    int64 s = 0, t = 0; 
    if (!solve_linear_equation(m1, b-a, m2, t, s)) return 0; 
    // k0 = k1 * s + t 
    // x = (k1 * s + t) * m1 + a 
    // x = k1 * s * m1 + (a + t * m1) 
     
    u = s * m1; 
    v = ((a + t * m1) % u + u) % u; 
    return 1; 
} 
 
SL tuple<int, int64, int64> crt2(int64 a, int64 m1, int64 b, int64 m2) 
{ 
    int64 v = 0, u = 0; 
    int r = crt2(a, m1, b, m2, v, u); 
    return tuple<int, int64, int64>{r, v, u}; 
} 
 
SL tuple<int, int64, int64> crtn(int64* val, int64* mod, int n) 
{ 
    PE_ASSERT(val != NULL && mod != NULL); 
    int64 v = 0, u = 1; 
    int have = 1; 
    for (int i = 0; i < n; ++i) 
    { 
        tie(have, v, u) = crt2(v, u, val[i], mod[i]); 
        if (!have) 
        { 
            return tuple<int, int64, int64>{0, 0, 0}; 
        } 
    } 
    return tuple<int, int64, int64>{have, v, u}; 
} 
 
// prime test for n <= 2^63 - 1 
// http://oeis.org/A014233 
const int64 sopp[] = 
{ 
2047LL,                         // 2 
1373653LL,                      // 3 
25326001LL,                     // 5 
3215031751LL,                   // 7 
2152302898747LL,                // 11 
3474749660383LL,                // 13 
341550071728321LL,              // 17 
341550071728321LL,              // 19 
3825123056546413051LL,          // 23 
3825123056546413051LL,          // 29 
3825123056546413051LL,          // 31 
// bigger than 2^64             // 37 
// 2^63 = 9223372036854775808   //  
}; 
const int64 sopp_size = sizeof(sopp) / sizeof(sopp[0]); 
 
SL int MR_test_impl(int s, int64 t, int64 n, int x) 
{ 
    // PE_ASSERT((n&1) == 0) 
     
    if (n <= PE_SOI63) 
    { 
        t = power_mod(x, t, n); 
        if (t == 1 || t == n - 1) return 1; 
 
        for (int i = 1; i < s; ++i) 
        { 
            t = mul_mod(t, t, n); 
            if (t == n - 1) return 1; 
        } 
    } 
    else 
    { 
        t = power_mod_ex(x, t, n); 
        if (t == 1 || t == n - 1) return 1; 
 
        for (int i = 1; i < s; ++i) 
        { 
            t = mul_mod_ex(t, t, n); 
            if (t == n - 1) return 1; 
        } 
    } 
    return 0; 
} 
 
SL int MR_test(int64 n, int x) 
{ 
    if (n <= 1) return 0; 
    if ((n&1) == 0) return 0; 
 
    int s = 0; 
    int64 t = n - 1; 
    while ((t&1) == 0) ++s, t >>= 1; 
 
    return MR_test_impl(s, t, n, x); 
} 
 
SL int is_prime_ex(int64 n) 
{ 
    if (n <= 1) return 0; 
    if (n == 2) return 1; 
    if ((n&1) == 0) return 0; 
    if (n <= maxp) return pmask[n] == n; 
     
    for (int i = 1; i < 20; ++i) 
    if (n % plist[i] == 0) return 0; 
 
    int s = 0; 
    int64 t = n - 1; 
    while ((t&1) == 0) ++s, t >>= 1; 
 
    for (int i = 0; i < sopp_size; ++i) 
    { 
        const int64 val = sopp[i]; 
        if (n == val) return 0; 
        if (!MR_test_impl(s, t, n, plist[i])) return 0; 
        if (n < val) break; 
    } 
    return 1; 
} 
 
// C-style random number generator 
// ANSI_ISO_9899-1999: The value of the RAND_MAX macro shall be at least 32767. 
// We assume RAND_MAX is either 32767 or 2147483647 
#if RAND_MAX == 32767 
 
SL int  crand15() {return rand();} 
SL int  crand30() {return crand15() << 15 | crand15();} 
SL int64 crand45(){return (int64)crand15() << 30 | crand30();} 
SL int64 crand60(){return (int64)crand30() << 30 | crand30();} 
 
SL int crandi() {return crand30();} 
SL double crandd() {static const double coe = 1. / (RAND_MAX + 1); return coe * rand();} 
 
#elif RAND_MAX == 2147483647 
 
SL int crand15() {return rand() & 32767;} 
SL int crand30() {return rand();} 
SL int64 crand45(){return (int64)crand15() << 30 | crand30();} 
SL int64 crand50(){return (int64)crand30() << 30 | crand30();} 
 
SL int crandi() {return crand30();} 
SL double crandd() {static const double coe = 1. / ((int64)RAND_MAX + 1LL); return coe * rand();} 
 
#else 
 
#error "RAND_MAX should be either 32767 or 2147483647" 
 
#endif 
 
// C++ style random number generator 
template<typename RE, typename DS> 
class RandomGenerator 
{ 
public: 
    RandomGenerator(const RE& engine, const DS& distribution) :  
        random_engine_(engine), distribution_(distribution){} 
    int operator ()() 
    { 
        return distribution_(random_engine_); 
    } 
private: 
    RE random_engine_; 
    DS distribution_; 
}; 
 
RandomGenerator<std::mt19937, uniform_int_distribution<int> > 
make_uniform_generator(int seed, int min, int max) 
{ 
    PE_ASSERT(min <= max); 
    return RandomGenerator<std::mt19937, uniform_int_distribution<int> > 
        (std::mt19937(seed), std::uniform_int_distribution<int>(min, max)); 
} 
 
RandomGenerator<std::mt19937, uniform_int_distribution<int> > 
make_uniform_generator(int min, int max) 
{ 
    std::random_device rd; 
    PE_ASSERT(min <= max); 
    return RandomGenerator<std::mt19937, uniform_int_distribution<int> > 
        (std::mt19937(rd()), std::uniform_int_distribution<int>(min, max)); 
} 
 
// Primitive Root 
namespace pr{ 
 
SL bool is_pr(const vector<pair<int64, int> >& f, int64 x, int64 p) 
{ 
    for(auto& it: f) if (power_mod(x, (p-1)/it.first, p) == 1) 
    return 0; 
    return 1; 
} 
 
SL bool is_pr(int64 x, int64 p) 
{ 
    return is_pr(factorize(p-1), x, p); 
} 
 
SL int64 find_pr(int64 p) 
{ 
    if (p == 2) return 1; 
    auto f = factorize(p-1); 
    for (int64 i = 2; ; ++i) 
    if (is_pr(f, i, p)) return i; 
    return -1; 
} 
 
#if 0 
struct pr_pair 
{ 
    int idx; 
    int64 value; 
    pr_pair(int64 _idx=0, int64 _value=0) : idx(_idx), value(_value){} 
    bool operator < (const pr_pair& r) const 
    { 
        return value < r.value; 
    } 
}; 
 
pr_pair     l1[100005], l2[100005]; 
int64 ind(int64 a, int64 pr, int64 p) 
{ 
    int64 inv_pr = power_mod(pr, p-2, p); 
    int64 mx = sqrt(double(p-1)+0.5); 
    if (mx * mx != p - 1) ++mx; 
    int64 t1 = 1, t2 = a; 
    int64 temp = power_mod(pr, mx, p); 
    for (unsigned int i = 0; i <= mx - 1; ++i) 
    { 
        l1[i] = pr_pair(i, t1); 
        l2[i] = pr_pair(i, t2); 
        t2 = t2 * inv_pr % p; 
        t1 = t1 * temp % p; 
    } 
    sort(l1, l1+mx); 
    sort(l2, l2+mx); 
    int _i = -1, _j = -1; 
    for (int i = 0, j = 0;;) 
    if (l1[i].value == l2[j].value) 
    { 
        _i = l1[i].idx; 
        _j = l2[j].idx; 
        break; 
    } 
    else if (l1[i].value > l2[j].value)++j; 
    else ++i; 
    int64 ind_a = mx * _i + _j; 
    return ind_a; 
} 
#endif 
int64 ind1(int64 a, int64 pr, int64 p) 
{ 
    typedef unordered_map<int64, int> map_t; 
 
    int64 inv_pr = inv_of(pr, p); 
    int64 mx = static_cast<int64>(sqrt(double(p-1)+0.5)); 
    if (mx * mx != p - 1) ++mx; 
    map_t mem; 
    int64 t1 = 1, t2 = a; 
    for (unsigned int i = 0; i <= mx - 1; ++i) 
    { 
        mem[t2] = i; 
        t2 = t2 * inv_pr % p; 
    } 
    int64 temp = power_mod(pr, mx, p); 
    for (unsigned int i = 0; i <= mx - 1; ++i) 
    { 
        map_t::iterator where = mem.find(t1); 
        if (where != mem.end()) 
        { 
            return i * mx + where->second; 
        } 
        t1 = t1 * temp % p; 
    } 
    return -1; 
} 
} 
template<typename T> 
struct Fraction 
{ 
    Fraction(T u = 0, T v = 1) : a(u), b(v) 
    { 
        T d = __gcd(abs(a), abs(b)); 
        if (d > 1) a /= d, b /= d; 
        if (b < 0) b = -b, a = -a; 
    } 
    int operator < (const Fraction& o) const 
    { 
        return a * o.b < b * o.a; 
    } 
    int operator > (const Fraction& o) const 
    { 
        return a * o.b > b * o.a; 
    } 
    int operator == (const Fraction& o) const 
    { 
        return a * o.b == b * o.a; 
    } 
    Fraction operator + (const Fraction& o) const 
    { 
        return Fraction(a * o.b + o.a * b, b * o.b); 
    } 
    Fraction operator - (const Fraction& o) const 
    { 
        return Fraction(a * o.b - o.a * b, b * o.b); 
    } 
    Fraction operator * (const Fraction& o) const 
    { 
        return Fraction(a * o.a, b * o.b); 
    } 
    Fraction operator / (const Fraction& o) const 
    { 
        return Fraction(a * o.b, b * o.a); 
    } 
    double ToFloat() 
    { 
        return 1. * a / b; 
    } 
    T a, b; 
}; 
struct time_usage 
{ 
    ~time_usage() 
    { 
        cerr << "time usage: " << clock() << "ms" << endl; 
    } 
}; 
static time_usage __time_usage; 
// 65701 
#endif 

int _tmain(int argc, _TCHAR* argv[])
{
	cerr << __cplusplus << endl;
	cerr << _MSC_VER << endl;
	system("pause");
	return 0;
}
