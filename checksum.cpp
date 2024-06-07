#include <string>

uint16_t checksum( const std::string& text ) {
  auto sum = uint16_t{0};
  const auto upper_size = uint16_t{4u};
  const auto upper_shift = (sizeof(sum)*8 - upper_size);
  const auto lower_mask = ~((~0u) << upper_shift);
  for( auto c : text ) {
    auto upper_bits = sum >> upper_shift;
    sum &= lower_mask;
    sum ^= upper_bits;
    sum += static_cast<uint16_t>( c );
  }
  return sum;
}

