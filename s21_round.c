#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int mb_err = 0;
  if (result == NULL) return 1;
  s21_decimal zero = {0};
  *result = zero;
  s21_decimal tmp = {0};
  int sign = s21_checksign(value);
  int scale = s21_checkscale(value);
  s21_decimal one = {0};
  s21_from_int_to_decimal(1, &one);
  if (sign) s21_set_scale_and_sign(&value, scale, 0);
  s21_truncate(value, result);
  s21_decimal mod = {{5, 0, 0, 65536}};
  s21_sub(value, *result, &tmp);
  if (s21_is_greater_or_equal(tmp, mod)) mb_err = s21_add(*result, one, result);
  if (sign) s21_set_scale_and_sign(result, 0, sign);
  if (mb_err) mb_err = 1;
  return mb_err;
}