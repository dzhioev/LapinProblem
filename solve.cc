#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using ull = unsigned long long;

unsigned cnm(unsigned n, unsigned m) {
  static std::vector<std::vector<unsigned>> c;
  c.reserve(32);
  for (unsigned i = c.size(); i <= n; ++i) {
    if (!i) {
      c.push_back(std::vector<unsigned>{1});
      continue;
    }
    std::vector<unsigned> row;
    row.reserve(i + 1);
    row.push_back(1);
    for (unsigned j = 1; j < i; ++j) {
      row.push_back(c[i - 1][j - 1] + c[i - 1][j]);
    }
    row.push_back(1);
    c.push_back(std::move(row));
  }
  return c[n][m];
}

std::vector<std::vector<unsigned>> gen_subsets(unsigned n, unsigned m) {
  std::vector<std::vector<unsigned>> result;
  if (!n) return std::move(result);
  result.reserve(cnm(n, m));
  unsigned v = (1 << m) - 1;
  while (v < (1 << n)) {
    std::vector<unsigned> subset;
    subset.reserve(m);
    for (unsigned i = 0; i < n; ++i) {
      if (v & (1 << i)) {
        subset.push_back(i);
      }
    }
    result.push_back(std::move(subset));
    unsigned t = v | (v - 1);
    v = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(v) + 1));
  }
  return std::move(result);
}

const std::vector<std::vector<unsigned>>& get_subsets(unsigned n, unsigned m) {
  static std::vector<std::vector<std::vector<std::vector<unsigned>>>> subsets;
  subsets.reserve(32);
  for (unsigned i = subsets.size(); i <= n; ++i) {
    std::vector<std::vector<std::vector<unsigned>>> v;
    v.reserve(i + 1);
    for (unsigned j = 0; j <= i; ++j) {
      v.push_back(gen_subsets(i, j));
    }
    subsets.push_back(std::move(v));
  }
  return subsets[n][m];
}

ull sqrt_helper(ull x, ull lo, ull hi)
{
  if (lo == hi)
    return lo;

  const ull mid = (lo + hi + 1) / 2;

  if (x / mid < mid)
    return sqrt_helper(x, lo, mid - 1);
  else
    return sqrt_helper(x, mid, hi);
}

// calculates L sqrt(n) J
ull calc_fsn(ull n)
{
  return sqrt_helper(n, 0, n / 2 + 1);
}

// calculates L sqrt(n / d^2) J
// based on:
//   L sqrt(a) J  \in { L sqrt(L a J) J, L sqrt(L a J) J + 1 }
ull calc_fsnbd2(ull n, ull d) {
  ull fsfnbd2 = calc_fsn(n / (d * d));
  if ((fsfnbd2 + 1) * (fsfnbd2 + 1) * d * d <= n)
    return fsfnbd2 + 1;
  else
    return fsfnbd2;
}

std::vector<ull> find_primes_let(ull n) {
  std::vector<ull> primes{2};
  for (ull i = 3; i <= n; ++i) {
    bool is_prime = true;
    for (ull j = 0; primes[j] * primes[j] <= i; ++j) {
      if (!(i % primes[j])) {
        is_prime = false;
        break;
      }
    }
    if (is_prime) primes.push_back(i);
  }
  return std::move(primes);
}

void find_prime_factors(ull i,
                        const std::vector<ull>& primes,
                        std::vector<ull>* result) {
  result->resize(0);
  for (ull p: primes) {
    if (i < p * p)
      break;
    if (!(i % p)) {
      result->push_back(p);
      do {
        i /= p;
      } while (!(i % p));
    }
  }
  if (i != 1)
    result->push_back(i);
}

ull solve(ull n) {
  ull fsn = calc_fsn(n);
  ull total_jobs = 0;
  for (ull d = 1; d <= fsn; ++d) {
    total_jobs += calc_fsnbd2(n, d);
  }

  std::cout << "Finding all primes up to " << calc_fsn(fsn) << "..."
            << std::endl;
  std::vector<ull> primes = find_primes_let(calc_fsn(fsn));
  std::cout << primes.size() << " primes found..." << std::endl;

  std::cout << "Jobs to do: " << total_jobs << std::endl;
  int percents = 0;
  ull jobs_done = 0;

  auto on_job_done = [&percents, &jobs_done, total_jobs] {
    int percents_before = percents;
    percents = (++jobs_done * 100) / total_jobs;
    if (percents != percents_before) {
      std::cout << "Progress: " << percents << "%" << std::endl;
    }
  };

  ull result = 0;
  std::vector<ull> ps;

  for (ull d = 1; d <= fsn; ++d) {
    ull fsnbd2 = calc_fsnbd2(n, d);
    for (ull i = 1; i <= fsnbd2; ++i) {
      find_prime_factors(i, primes, &ps);
      on_job_done();
    }
  }

  result *= 2;
  result += fsn * (fsn + 1) / 2;

  return result;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " N\n";
    return 1;
  }

  ull n;
  try {
    if (argv[1][0] == '-') {
      throw 1;
    }
    n = std::stoull(argv[1]);
  } catch (...) {
    std::cerr << "N is not an nonnegative integer number or is too large.\n";
    return 2;
  }
  ull res = solve(n);
  std::cout << "F(" << n << ") = " << res << std::endl;
}


