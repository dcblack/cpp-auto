#include "graphemes.hpp"
#include <stdexcept>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////
auto Grapheme::to_upper() const -> Grapheme {
  // Only handles ASCII
  if ( 'a' <= m_grapheme and m_grapheme <= 'z' ) {

    return Grapheme{ m_grapheme - static_cast<uint32_t>('a') + static_cast<uint32_t>('A') };
  }
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
auto Grapheme::to_lower() const -> Grapheme {
  // Only handles ASCII
  if ( 'A' <= m_grapheme and m_grapheme <= 'Z' ) {
    auto upper = m_grapheme - static_cast<uint32_t>('A') + static_cast<uint32_t>('a');
    return Grapheme{ upper };
  }
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
auto Grapheme::to_string() const -> std::string {
  auto result = std::string{};
  result.reserve( max_bytes_in_utf8 );
  auto grapheme = m_grapheme;
  do {
    auto u32 = U32{ grapheme };
    grapheme >>= bits_in_byte;
    result.insert( 0, 1, u32.int8[ 0 ] );
  } while ( grapheme != 0 );
  return result;
}

////////////////////////////////////////////////////////////////////////////////
auto Grapheme::operator|=( uint32_t value ) -> Grapheme& {
  m_grapheme |= value;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
Grapheme_string::Grapheme_string( const std::string& text )
{
  auto grf = Grapheme{};
  if ( text.empty() ) { return; }
  m_data.reserve( text.capacity() ); // bigger than needed
  auto remaining = 0;
  constexpr int msb1 = 0x80;
  constexpr int msb2 = 0xC0;
  constexpr int msb3 = 0xE0;
  constexpr int msb4 = 0xF0;
  constexpr int msb5 = 0xF8;
  constexpr int bits_in_byte = 8;
  for ( unsigned char chr : text ) {
    if ( ( chr & msb1 ) == 0 ) { // ASCII
      remaining = 0;
      grf = Grapheme{ chr };
    } else if ( ( chr & msb3 ) == msb2 ) {
      remaining = 1;
      auto utf32 = static_cast<uint32_t>(chr) << ( bits_in_byte * remaining );
      grf = Grapheme{ utf32 };
    } else if ( ( chr & msb4 ) == msb3 ) {
      remaining = 2;
      auto utf32 = static_cast<uint32_t>(chr) << ( bits_in_byte * remaining );
      grf = Grapheme{ utf32 };
    } else if ( ( chr & msb5 ) == msb4 ) {
      remaining = 3;
      auto utf32 = static_cast<uint32_t>(chr) << ( bits_in_byte * remaining );
      grf = Grapheme{ utf32 };
    } else if ( ( chr & msb2 ) == msb1 ) {
      if ( remaining <= 0 ) {
        throw std::runtime_error{ "Bad UTF-8 character" };
      }
      --remaining;
      auto utf32 = static_cast<uint32_t>(chr) << ( bits_in_byte * remaining );
      grf |= utf32;
    } else {
      throw std::runtime_error{ "Bad UTF-8 character" };
    }
    if ( remaining == 0 ) {
      m_data.push_back( grf );
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
auto Grapheme_string::to_string() const -> std::string {
  auto result = std::string{};
  if ( length() == 0 ) { return result; }
  for ( auto grf : m_data ) { //looks inside
    result.append( grf.to_string() );
  }
  return result;
}

//TAF!
