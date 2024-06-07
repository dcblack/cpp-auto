#pragma ide diagnostic ignored "readability-magic-numbers"

#include <string>
#include <fmt/format.h>
#include "graphemes.hpp"
#include <array>

struct Earth { long double latitude, longitude; };
auto checksum( const std::string& text ) -> uint16_t;

[[maybe_unused]] auto ANSWER = 42UL;
auto doulos_uk = Earth{50.846930L,-1.794880L};

// Arrays & similar
#ifdef WONT_COMPILE
  auto cArr[5] = { 1, 2, 3 };
  struct S { auto i = int{0}; auto s = ""; auto f = 0.; };
  union  U { auto i = int{0}; auto s = ""; auto f = 0.; };
  class  C { auto i = int{0}; auto s = ""; auto f = 0.; };
#endif
[[maybe_unused]] auto iniLst  = { 1, 2, 3 };
[[maybe_unused]] auto cppArr  = std::array<int,5>{ 1, 2, 3 };
[[maybe_unused]] auto cppVec  = std::vector{ 1, 2, 3 };
[[maybe_unused]] auto cppVmt  = std::vector{10,0};
[[maybe_unused]] auto cppVsz  = std::vector(10,0);

auto main() -> int {
  fmt::print("Doulos is located at {}, {}\n",doulos_uk.latitude,doulos_uk.longitude);
  fmt::print("mt:{} sz:{}\n",cppVmt.size(),cppVsz.size());
  [[maybe_unused]] auto sum = int64_t{ 0 };
  auto text = std::string{ "Greetings! 問候 ✋." };
  fmt::print( "text string     length:{1} contents:'{0}'\n", text, text.size());
  auto grapheme = Grapheme_string{ text };

  fmt::print( "grapheme UPPER  length:{} contents:'", grapheme.length() );
  for( auto character : grapheme ) { //looks inside
    auto uppercase = character.to_upper();
    fmt::print( "{}", uppercase.to_string() );
  }
  fmt::print("'\n");

  fmt::print( "grapheme LOWER  length:{} contents:'", grapheme.length() );
  for( auto character : grapheme ) { //looks inside
    auto lowercase = character.to_lower();
    fmt::print( "{}", lowercase.to_string() );
  }
  fmt::print("'\n");

  fmt::print( "grapheme string length:{} contents:'{}'\n", grapheme.length(), grapheme.to_string() );
  auto identity = checksum( text );
  fmt::print("id:{}\n", identity);
}