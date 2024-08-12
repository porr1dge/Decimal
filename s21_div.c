#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int mb_err = 0;
  s21_long_decimal long_zero = {0};
  s21_decimal zero = {0};
  *result = zero;
  s21_long_decimal mod = {0};
  s21_long_decimal tmp_mod = {0};
  s21_long_decimal a = {0};
  s21_long_decimal b = {0};
  s21_long_decimal res = {0};
  s21_long_decimal ten = {0};
  s21_from_int_to_long_decimal(10, &ten);
  int sign_1 = s21_checksign(value_1);
  int sign_2 = s21_checksign(value_2);
  int sign = 0;
  if (sign_1 && sign_2) sign = 0;
  if (sign_1 || sign_2) sign = 1;
  s21_decim_to_long_decim(value_1, &a);
  s21_decim_to_long_decim(value_2, &b);
  if (!s21_more_or_less(b, long_zero)) return 3;
  if (!s21_more_or_less(a, long_zero)) return 0;
  int scale = 0;
  int scale_a = s21_long_checkscale(a);
  int scale_b = s21_long_checkscale(b);
  if (scale_a > scale_b)
    scale = scale_a;
  else if (scale_a < scale_b)
    scale = scale_b;
  else
    scale = scale_a;
  int i = 1;
  if (!s21_more_or_less(a, b) && (scale_a < scale_b)) {
    s21_normalize(&a, &b);
    scale = 0;
    i = 0;
    s21_long_div(a, b, &res);
  } else {
    mod = s21_long_div(a, b, &res);
  }
  while (i) {
    if (s21_more_or_less(mod, long_zero)) {
      while ((s21_more_or_less(mod, b) == -1) && (scale < 28)) {
        s21_long_mul(mod, ten, &mod);
        scale++;
      }
      tmp_mod = long_zero;
      tmp_mod = s21_long_div(mod, b, &mod);
      if (s21_more_or_less(mod, long_zero)) {
        s21_long_mul(res, ten, &res);
        s21_long_add(res, mod, &res);
      }
    }
    if (s21_more_or_less(tmp_mod, long_zero)) mod = tmp_mod;
    if (scale == 28 || !s21_more_or_less(tmp_mod, long_zero)) break;
  }
  s21_long_set_scale_and_sign(&res, scale, sign);
  mb_err = s21_long_decim_to_decim(res, result);
  return mb_err;
}