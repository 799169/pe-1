const int maxp = 100000;
#include "pe"

const int64 mod = 1000000007;

int main()
{
  // Fibonacci sequence
  vector<int64> A{1, -1, -1};
  vector<int64> B{0, 1};
  vector<int64> x1 = gf_first(A, B, 30, mod);
  for (auto& iter: x1)
    cerr << iter << endl;

  cerr << gf_at(A, B, 1000000000, mod) << endl;

  // Dollar exchange.
  // Concret math
  // 7 Generating Functions
  // 7.3 Solving Recurrences
  // Example 4: A closed form for change.
  int64 dp[10000+1] = {1};
  int64 can[5] = {1, 5, 10, 25, 50};
  for (auto each: can)
  {
    for (int j = 0; j + each <= 10000; ++j) if (dp[j])
    {
      dp[j+each] = add_mod(dp[j+each], dp[j], mod);
    }
  }

  int64 coe[100] = {0};
  for (int i = 0; i < 1 << 5; ++i)
  {
    int s = 0;
    int bc = 0;
    for (int j = 0; j < 5; ++j)
      if (i & (1<<j)) ++bc, s += can[j];
    //dbg(s);
    if (bc & 1) --coe[s]; else ++coe[s];
  }

  auto gfresult = gf_first(vector<int64>(coe, coe+92), {1}, 10000, mod);
  for (int i = 9980; i <= 10000; ++i)
    cerr << dp[i] << " " << gfresult[i] << " " << dp[i] - gfresult[i] << endl;

  cerr << gf_at(vector<int64>(coe, coe+92), {1}, 100000000, mod) << endl;
  cerr << from_string("66666793333412666685000001") % mod << endl;
  return 0;
}