#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include "bloom_filter.h"
#include "murmur_hash3.h"
#include <array>

bloom_filter::bloom_filter(uint64_t size, uint8_t num_hashes)
      : m_bits(size),
        m_num_hashes(num_hashes) {}

std::array<uint64_t, 2> hash(const uint8_t *data,
                             std::size_t len) {
  std::array<uint64_t, 2> hash_value;
  MurmurHash3_x64_128(data, len, 0, hash_value.data());

  return hash_value;
}

inline uint64_t nth_hash(uint8_t n,
                        uint64_t hashA,
                        uint64_t hashB,
                        uint64_t filterSize) {
    return (hashA + n * hashB) % filterSize;
}

void bloom_filter::add(const uint8_t *data, std::size_t len) {
  auto hash_values = hash(data, len);

  for (int n = 0; n < m_num_hashes; n++) {
      m_bits[nth_hash(n, hash_values[0], hash_values[1], m_bits.size())] = true;
  }
}

bool bloom_filter::possibly_contains(const uint8_t *data, std::size_t len) const {
  auto hash_values = hash(data, len);

  for (int n = 0; n < m_num_hashes; n++) {
      if (!m_bits[nth_hash(n, hash_values[0], hash_values[1], m_bits.size())]) {
          return false;
      }
  }

  return true;
}

double bloom_filter::percent_full() {
  double percent = 0.0;
  int bits_set = 0;
  for(auto bit : m_bits) {
    if (bit) {
      bits_set++;
    }
  }
  percent = bits_set / (double)m_bits.size();
  return percent;
}

void bloom_filter::flush() {
  std::fill(m_bits.begin(), m_bits.end(), 0);
  return;
}
