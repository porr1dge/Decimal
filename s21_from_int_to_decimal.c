#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int mb_err = 0;
  s21_decimal zero = {0};
  if (dst == NULL) {
    return 1;
  } else {
    *dst = zero;
    dst->bits[0] = src;
    if (src < 0) {
      dst->bits[3] |= 1 << 31;
      dst->bits[0] = ~dst->bits[0] + 1;
    }
  }
  return mb_err;
}