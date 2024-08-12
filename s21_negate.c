#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (result == NULL) return 1;
  *result = value;
  s21_setbit(&result->bits[3], 31, !s21_checksign(value));
  return 0;
}