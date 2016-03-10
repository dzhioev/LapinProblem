#include <iostream>
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

int main() {
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j <= i; ++j) {
      std::cout << cnm(i, j) << ' ';
    }
    std::cout << std::endl;
  }

  std::cout << "==========================\n";
  for (const auto& subset: get_subsets(10, 3)) {
    for (const auto& i: subset) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "==========================\n";
  for (const auto& subset: get_subsets(12, 2)) {
    for (const auto& i: subset) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "==========================\n";
  for (const auto& subset: get_subsets(12, 12)) {
    for (const auto& i: subset) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "==========================\n";

  for (const auto& subset: get_subsets(0, 0)) {
    for (const auto& i: subset) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "==========================\n";
}

