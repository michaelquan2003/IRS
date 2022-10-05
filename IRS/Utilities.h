#pragma once
#include <vector>

namespace Sort {
template <typename A, typename B>
void zip(
  const std::vector<A>& a,
  const std::vector<B>& b,
  std::vector<std::pair<A, B>>& zipped) {
  zipped.reserve(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    zipped.push_back(std::make_pair(a[i], b[i]));
  }
}

template <typename A, typename B>
void unzip(
  const std::vector<std::pair<A, B>>& zipped,
  std::vector<A>& a,
  std::vector<B>& b) {
  for (size_t i = 0; i < a.size(); i++) {
    a[i] = zipped[i].first;
    b[i] = zipped[i].second;
  }
}

template<class T1, class T2>
void sortInPlace(std::vector<std::pair<T1, T2>>& v, int onFirstOrSecond = 1, bool ascending = true)
{
  if (onFirstOrSecond == 1) {
    std::sort(v.begin(), v.end(), [](const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) {return ascending ? lhs.first < rhs.first : lhs.first > rhs.first; });
  }
  else {
    std::sort(v.begin(), v.end(), [](const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) {return ascending ? lhs.second < rhs.second : lhs.second > rhs.second; });
  }
}

template<class T, class K>
void sortInPlace(std::vector<T>& v, std::vector<K>& keys) {
  std::vector<std::pair<T, K>> zipped;
  zip(v, keys, zipped);
  sortInPlace<T, K>(zipped, 2);
  unzip(zipped, v, keys);
}

template<class T1, class T2>
std::vector<std::pair<T1, T2>> sort(std::vector<std::pair<T1, T2>> v, int onFirstOrSecond = 1)
{
  sortInPlace(v, onFirstOrSecond);
  return v;
}

template<class T>
void sortInPlace(std::vector<T>& v, bool ascending=true) {
  std::sort(v.begin(), v.end(), [](const T& lhs, const T& rhs) {return ascending ? lhs < rhs : lhs > rhs; });
}

template<class T>
std::vector<T> sort(std::vector<T> v) {
  sortInPlace(v);
  return v;
}

}
