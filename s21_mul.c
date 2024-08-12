#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int mb_err = 0;
  s21_decimal zero = {0};
  *result = zero;
  s21_long_decimal a = {0};
  s21_long_decimal b = {0};
  s21_long_decimal res = {0};
  int sign_1 = s21_checksign(value_1);
  int sign_2 = s21_checksign(value_2);
  int sign = 0;
  if (sign_1 && sign_2) sign = 0;
  if (sign_1 != sign_2) sign = 1;
  s21_decim_to_long_decim(value_1, &a);
  s21_decim_to_long_decim(value_2, &b);
  int scale_a = s21_long_checkscale(a);
  int scale_b = s21_long_checkscale(b);
  if (scale_a > 28 || scale_b > 28) s21_normalize(&a, &b);
  int scale = s21_long_checkscale(a) + s21_long_checkscale(b);
  if (s21_long_is_zero(a) || s21_long_is_zero(b)) {
    s21_set_scale_and_sign(result, scale, sign);
    return mb_err;
  }
  mb_err = s21_long_mul(a, b, &res);
  s21_long_set_scale_and_sign(&res, scale, sign);
  if (scale > 28 && !mb_err) s21_round_decim(&res, scale - 28);
  s21_round_long_decimal(&res);
  mb_err = s21_long_decim_to_decim(res, result);
  if (mb_err) {
    if (mb_err && (s21_checksign(*result))) mb_err = 2;
    *result = zero;
  }
  return mb_err;
}