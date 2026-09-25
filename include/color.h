struct Color {
  unsigned char r, g, b, a;
  float rf, gf, bf, af;

  Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
  Color(unsigned char r, unsigned char g, unsigned char b);
  Color(const char *hex);
};
#define RGB(color) color.rf, color.gf, color.bf
#define RGBA(color) color.rf, color.gf, color.bf, color.af
