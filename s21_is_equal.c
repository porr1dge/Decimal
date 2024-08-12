#include "s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  s21_long_decimal a = {0};
  s21_long_decimal b = {0};
  s21_decim_to_long_decim(value_1, &a);
  s21_decim_to_long_decim(value_2, &b);
  return s21_long_is_equal(&a, &b);
}