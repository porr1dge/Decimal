#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int mb_err = 0;
  if (result == NULL) return 1;
  s21_decimal zero = {0};
  *result = zero;
  int sign = s21_checksign(value);
  s21_long_decimal one = {0};
  s21_from_int_to_long_decimal(1, &one);
  s21_long_decimal res = {0};
  s21_truncate(value, result);
  s21_decim_to_long_decim(*result, &res);
  if (sign && !s21_is_equal(value, *result)) {
    s21_long_add(res, one, &res);
  }
  s21_long_set_scale_and_sign(&res, 0, sign);
  mb_err = s21_long_decim_to_decim(res, result);
  return mb_err;
}