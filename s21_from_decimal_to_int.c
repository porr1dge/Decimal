#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int mb_err = 0;
  if (dst == NULL)
    return 1;
  else {
    *dst = 0;
    s21_decimal res = {0};
    int sign = s21_checksign(src);
    if (sign) sign = -1;
    s21_truncate(src, &res);
    if (res.bits[2] || res.bits[1] || s21_checkbit(res.bits[0], 31)) mb_err = 1;
    *dst = res.bits[0];
    if (sign) *dst *= sign;
  }
  return mb_err;
}