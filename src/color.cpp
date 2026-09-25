#include <color.h>
#include <ctype.h>
#include <assert.h>

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : r(r), g(g), b(b), a(a) {
  rf = r / 255.0f;
  gf = g / 255.0f;
  bf = b / 255.0f;
  af = a / 255.0f;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b)
    : r(r), g(g), b(b), a(0) {
  rf = r / 255.0f;
  gf = g / 255.0f;
  bf = b / 255.0f;
  af = 0.0f;
}

unsigned char hex2dec(char ch) {
  if (isdigit(ch))
    return ch - '0';
  if (isupper(ch))
    return 10 + ch - 'A';
  if (islower(ch))
    return 10 + ch - 'a';
  assert(0);
}

Color::Color(const char *hex) {
  if (hex[0] == '#')
    hex++;
  unsigned char values[4];
  int n_values = hex[6] == '\0' ? 3 : 4;
  for (int i = 0; i < n_values; i++) {
    char upper = hex[2 * i];
    char lower = hex[2 * i + 1];
    values[i] = hex2dec(lower) | (hex2dec(upper) << 4);
  }
  r = values[0];
  g = values[1];
  b = values[2];
  a = n_values == 4 ? values[3] : 0;
  rf = r / 255.0f;
  gf = g / 255.0f;
  bf = b / 255.0f;
  af = a / 255.0f;
}


