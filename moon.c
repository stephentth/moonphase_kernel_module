int moon_phase(int y, int m, int d) {
  int c, e;
  double jd;
  int b;

  if (m < 3) {
    y--;
    m += 12;
  }
  ++m;
  c = 365.25 * y;
  e = 30.6 * m;

  jd = c + e + d - 694039.09;
  jd /= 29.53; // moon cycle (29.53 days)
  b = jd;
  jd -= b;
  b = jd * 8 + 0.5;
  b = b & 7;
  return b;
}
