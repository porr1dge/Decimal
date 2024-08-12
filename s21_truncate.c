#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int mb_err = 0;
  if (result == NULL) return 1;
  s21_decimal zero = {0};
  *result = zero;
  s21_long_decimal a = {0};
  s21_long_decimal ten = {0};
  s21_decim_to_long_decim(value, &a);
  int scale = s21_long_checkscale(a);
  int sign = s21_long_checksign(a);
  s21_from_int_to_long_decimal(10, &ten);
  while (scale != 0) {
    s21_long_div(a, ten, &a);
    scale--;
  }
  s21_long_set_scale_and_sign(&a, 0, sign);
  mb_err = s21_long_decim_to_decim(a, result);
  return mb_err;
}