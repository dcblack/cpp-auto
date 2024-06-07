#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <cassert>

struct Grapheme {
  explicit Grapheme( unsigned char c = '\0' ) : m_grapheme{ static_cast<uint32_t>(c)} {}
  explicit Grapheme( uint32_t v ) : m_grapheme{ v} {
    auto little_endian = U32{1};
    assert( little_endian.byte[0] == 1 );
  }

  [[nodiscard]] auto to_upper() const -> Grapheme;
  [[nodiscard]] auto to_lower() const -> Grapheme;
  [[nodiscard]] auto to_string() const -> std::string;
  auto operator|=( uint32_t rhs ) -> Grapheme&;
  friend auto operator<<( std::ostream& os, const Grapheme& rhs ) -> std::ostream& {
    os << rhs.to_string();
    return os;
  }
private:
  static constexpr int bytes_in_word = 4;
  static constexpr int bits_in_byte = 8;
  static constexpr int max_bytes_in_utf8 = 16;
  union U32 {
    uint32_t word;
    uint8_t byte[bytes_in_word];
    int8_t  int8[bytes_in_word];
  };
  uint32_t m_grapheme;
};

struct Grapheme_string {
  std::vector<Grapheme> m_data{};
  explicit Grapheme_string( const std::string& text );
  [[nodiscard]] auto length() const -> size_t { return m_data.size(); }
  [[nodiscard]] auto to_string() const -> std::string;
  auto begin() { return m_data.begin(); }
  auto end() { return m_data.end(); }
  auto operator[](size_t index) -> Grapheme& { return m_data[index]; }
  auto at(size_t index) -> Grapheme& { return m_data.at(index); }
  friend auto operator<<( std::ostream& os, const Grapheme_string& rhs ) -> std::ostream& {
    os << rhs.to_string();
    return os;
  }
};

#include <fmt/format.h>

template <>
class fmt::formatter<Grapheme> {
  // format specification storage
  char presentation_ = 's';
public:
  // parse format specification and store it:
  constexpr auto parse (format_parse_context& ctx) {
    auto i = ctx.begin();
    auto end = ctx.end();
    if (i != end && (*i == 's')) {
      presentation_ = *i++;
    }
    if (i != end && *i != '}') {
      throw format_error("invalid format");
    }
    return i;
  }
  // format a value using stored specification:
  template <typename FmtContext>
  constexpr auto format (Grapheme const& grf, FmtContext& ctx) const {
    switch (presentation_) {
      default:
        // 'ctx.out()' is an output iterator
      case 's': return format_to(ctx.out(), "{}", grf.to_string());
    }
  }
};

template <>
class fmt::formatter<Grapheme_string> {
  // format specification storage
  char presentation_ = 's';
public:
  // parse format specification and store it:
  constexpr auto parse (format_parse_context& ctx) {
    auto i = ctx.begin();
    auto end = ctx.end();
    if (i != end && (*i == 's')) {
      presentation_ = *i++;
    }
    if (i != end && *i != '}') {
      throw format_error("invalid format");
    }
    return i;
  }
  // format a value using stored specification:
  template <typename FmtContext>
  constexpr auto format (Grapheme_string const& grf_str, FmtContext& ctx) const {
    switch (presentation_) {
      default:
        // 'ctx.out()' is an output iterator
      case 's': return format_to(ctx.out(), "{}", grf_str.to_string());
    }
  }
};

//TAF!
