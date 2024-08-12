#ifndef _S21_DECIMAL_H_
#define _S21_DECIMAL_H_

#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAX_FLOAT 79228157791897854723898736640.0f
#define MIN_FLOAT \
  0.00000000000000000000000000010000000031710768509710513471352647538147514756461109f

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[8];
} s21_long_decimal;

// Arithmetic

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Comparison

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

// Convertors

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Another functions

int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);

// Help functions

int s21_long_add(s21_long_decimal a, s21_long_decimal b,
                 s21_long_decimal *result);
void s21_long_sub(s21_long_decimal a, s21_long_decimal b,
                  s21_long_decimal *result);
int s21_long_mul(s21_long_decimal a, s21_long_decimal b,
                 s21_long_decimal *result);
s21_long_decimal s21_long_div(s21_long_decimal a, s21_long_decimal b,
                              s21_long_decimal *result);
int s21_checkbit(unsigned int value, int index);
void s21_setbit(unsigned int *value, int index, int bit);
int s21_checksign(s21_decimal x);
int s21_checkscale(s21_decimal x);
int s21_long_checksign(s21_long_decimal x);
int s21_long_checkscale(s21_long_decimal x);
int s21_shiftleft(s21_long_decimal *x);
int s21_shiftright(s21_long_decimal *x);
int s21_h_bit(s21_long_decimal x);
void s21_set_scale_and_sign(s21_decimal *x, int scale, int sign);
void s21_long_set_scale_and_sign(s21_long_decimal *x, int scale, int sign);
void s21_decim_to_long_decim(s21_decimal x, s21_long_decimal *long_x);
int s21_long_is_less(s21_long_decimal *a, s21_long_decimal *b);
int s21_long_is_equal(s21_long_decimal *a, s21_long_decimal *b);
void s21_normalize(s21_long_decimal *a, s21_long_decimal *b);
void s21_round_decim(s21_long_decimal *x, int scale);
void s21_long_set_scale_and_sign(s21_long_decimal *x, int scale, int sign);
void s21_round_long_decimal(s21_long_decimal *x);
int s21_if_long(s21_long_decimal x);
int s21_long_decim_to_decim(s21_long_decimal long_x, s21_decimal *x);
int s21_more_or_less(s21_long_decimal a, s21_long_decimal b);
int s21_from_int_to_long_decimal(int src, s21_long_decimal *dst);
int s21_check_scale_string(char *str);
s21_decimal s21_string_to_decim(char *str);
int s21_check_bit_zero(s21_decimal x);
int s21_long_is_zero(s21_long_decimal x);

#endif  //  _S21_DECIMAL_H_