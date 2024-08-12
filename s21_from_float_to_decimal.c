#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int mb_err = 0;
  s21_decimal zero = {0};
  if (dst == NULL)
    return 1;
  else if (src == 0.0) {
    mb_err = 0;
    *dst = zero;
    if (src < 0) s21_set_scale_and_sign(dst, 0, 1);
  } else if (isinf(src) || isnan(src)) {
    mb_err = 1;
    *dst = zero;
    if (src < 0) s21_set_scale_and_sign(dst, 0, 1);
  } else if (fabs(src) > MAX_FLOAT) {
    mb_err = 1;
    *dst = zero;
    if (src < 0) s21_set_scale_and_sign(dst, 0, 1);
  } else if (fabs(src) < MIN_FLOAT) {
    mb_err = 1;
    *dst = zero;
    if (src < 0) s21_set_scale_and_sign(dst, 0, 1);
  } else {
    *dst = zero;
    s21_decimal res = {0};
    char str[100] = {0};
    sprintf(str, "%.6e", fabsf(src));
    int scale = s21_check_scale_string(str);
    if (scale <= -23) {
      int prec = scale + 28;
      sprintf(str, "%.*e", prec, fabsf(src));
    }
    res = s21_string_to_decim(str);
    if (src < 0) s21_setbit(&res.bits[3], 31, 1);
    *dst = res;
  }
  return mb_err;
}