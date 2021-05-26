#include <vector>
#include <cstdint>

struct bloom_filter {
  bloom_filter(uint64_t size, uint8_t num_hashes);

  void add(const uint8_t *data, std::size_t len);
  bool possibly_contains(const uint8_t *data, std::size_t len) const;
  double percent_full();
  void flush();

private:
  uint8_t m_num_hashes;
  std::vector<bool> m_bits;
};

