#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int mb_err = 0;
  s21_decimal zero = {0};
  if (dst == NULL) {
    return 1;
  } else if (s21_is_equal(src, zero)) {
    int sign = s21_checksign(src);
    if (sign)
      *dst = -0.0;
    else
      *dst = 0.0;
  } else {
    int sign = s21_checksign(src);
    int scale = s21_checkscale(src);
    *dst = 0.0;
    double tmp = 0.0;
    for (int i = 0; i < 96; i++) {
      if (s21_checkbit(src.bits[i / 32], i % 32)) tmp += pow(2.0, i);
    }
    double scale_tmp = pow(10, scale);
    tmp = tmp / scale_tmp;
    if (sign) tmp *= -1;
    *dst = (float)tmp;
  }
  return mb_err;
}