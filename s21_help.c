#include "s21_decimal.h"

int s21_checkbit(unsigned int value, int index) {
  return (value & (1 << index)) != 0;
}

void s21_setbit(unsigned int *value, int index, int bit) {
  if (bit)
    *value = (*value | (1 << index));
  else
    *value = (*value & ~(1 << index));
}

int s21_checksign(s21_decimal x) { return s21_checkbit(x.bits[3], 31); }

int s21_long_checksign(s21_long_decimal x) {
  return s21_checkbit(x.bits[7], 31);
}

int s21_checkscale(s21_decimal x) { return (x.bits[3] & (127 << 16)) >> 16; }

int s21_long_checkscale(s21_long_decimal x) {
  return (x.bits[7] & (127 << 16)) >> 16;
}

void s21_set_scale_and_sign(s21_decimal *x, int scale, int sign) {
  x->bits[3] = (scale << 16) + (sign << 31);
}

void s21_long_set_scale_and_sign(s21_long_decimal *x, int scale, int sign) {
  x->bits[7] = (scale << 16) + (sign << 31);
}

void s21_decim_to_long_decim(s21_decimal x, s21_long_decimal *long_x) {
  s21_long_decimal zero = {0};
  *long_x = zero;
  long_x->bits[0] = x.bits[0];
  long_x->bits[1] = x.bits[1];
  long_x->bits[2] = x.bits[2];
  long_x->bits[7] = x.bits[3];
}

int s21_from_int_to_long_decimal(int src, s21_long_decimal *dst) {
  int mb_err = 0;
  s21_long_decimal zero = {0};
  if (dst == NULL) {
    mb_err = 1;
  } else {
    *dst = zero;
    dst->bits[0] = src;
    if (src < 0) {
      dst->bits[7] |= 1 << 31;
      dst->bits[0] = ~dst->bits[0] + 1;
    }
  }
  return mb_err;
}

int s21_long_decim_to_decim(s21_long_decimal long_x, s21_decimal *x) {
  s21_decimal zero = {0};
  *x = zero;
  x->bits[0] = long_x.bits[0];
  x->bits[1] = long_x.bits[1];
  x->bits[2] = long_x.bits[2];
  x->bits[3] = long_x.bits[7];
  for (int i = 3; i < 7; i++) {
    if (long_x.bits[i] != 0) return 1;
  }
  return 0;
}

int s21_long_is_zero(s21_long_decimal x) {
  int i = 223;
  for (; i >= 0 && !s21_checkbit(x.bits[i / 32], i % 32); i--)
    ;
  return i == -1;
}

int s21_long_is_less(s21_long_decimal *a, s21_long_decimal *b) {
  int err = 0;
  s21_normalize(a, b);
  int sign_a = s21_long_checksign(*a);
  int sign_b = s21_long_checksign(*b);
  if (s21_long_is_zero(*a) && s21_long_is_zero(*b)) {
    err = 0;
  } else if (sign_a != sign_b) {
    err = sign_a;
  } else {
    for (int i = 191; i >= 0; i--) {
      if ((s21_checkbit(a->bits[i / 32], i % 32)) >
          (s21_checkbit(b->bits[i / 32], i % 32))) {
        err = sign_a == 0 ? 0 : 1;
        break;
      } else if ((s21_checkbit(a->bits[i / 32], i % 32)) <
                 (s21_checkbit(b->bits[i / 32], i % 32))) {
        err = sign_a == 0 ? 1 : 0;
        break;
      }
    }
  }
  return err;
}

int s21_long_is_equal(s21_long_decimal *a, s21_long_decimal *b) {
  int err = 0;
  s21_normalize(a, b);
  int sign_a = s21_long_checksign(*a);
  int sign_b = s21_long_checksign(*b);
  if (s21_long_is_zero(*a) && s21_long_is_zero(*b)) {
    err = 1;
  } else if (sign_a == sign_b) {
    err = 1;
    for (int i = 191; i >= 0; i--) {
      if ((s21_checkbit(a->bits[i / 32], i % 32)) !=
          (s21_checkbit(b->bits[i / 32], i % 32))) {
        err = 0;
        break;
      }
    }
  }
  return err;
}

int s21_if_long(s21_long_decimal x) {
  for (int i = 3; i < 7; i++) {
    if (x.bits[i] != 0) return 1;
  }
  return 0;
}

void s21_round_long_decimal(s21_long_decimal *x) {
  s21_long_decimal max = {0};
  max.bits[0] = 4294967295, max.bits[1] = 4294967295, max.bits[2] = 4294967295;
  s21_long_decimal tmp = *x;
  if (s21_if_long(tmp) &&
      ((s21_more_or_less(tmp, max) == 1) || (!s21_more_or_less(tmp, max)))) {
    while (s21_if_long(tmp) && s21_long_checkscale(tmp)) {
      s21_round_decim(&tmp, 1);
    }
  }
  *x = tmp;
}

int s21_check_scale_string(char *str) {
  int res = 0;
  int sign = 1;
  char *ptr = str;
  while (*ptr) {
    if (*ptr == 'e') {
      ptr++;
      while (*ptr) {
        if (*ptr == '-') sign = -1;
        if (*ptr >= '0' && *ptr <= '9') {
          res *= 10;
          res += *ptr - '0';
        }
        ptr++;
      }
      res *= sign;
      break;
    }
    ptr++;
  }

  return res;
}

s21_decimal s21_string_to_decim(char *str) {
  s21_decimal ten = {0};
  s21_from_int_to_decimal(10, &ten);
  s21_decimal res = {0};
  char *ptr = str;
  int scale = s21_check_scale_string(str);
  while (*ptr) {
    if (*ptr == '.') {
      ptr++;
      continue;
    } else if (*ptr >= '0' && *ptr <= '9') {
      s21_decimal tmp = {0};
      s21_from_int_to_decimal(*ptr - '0', &tmp);
      s21_mul(res, ten, &res);
      s21_add(res, tmp, &res);
      ptr++;
    } else
      break;
  }
  scale = scale - 6;
  if (scale > 0) {
    int a = scale;
    while (a) {
      s21_mul(res, ten, &res);
      a--;
    }
  } else if (scale < 0) {
    if (scale < -28) {
      int a = 28;
      while (a) {
        s21_div(res, ten, &res);
        a--;
      }
      scale += 28;
    }
    int a = scale;
    while (a) {
      s21_div(res, ten, &res);
      a++;
    }
  }
  return res;
}

void s21_normalize(s21_long_decimal *a, s21_long_decimal *b) {
  int scale_1 = s21_long_checkscale(*a);
  int scale_2 = s21_long_checkscale(*b);
  int min_scale = 0, max_scale = 0;
  int sign_a = s21_long_checksign(*a);
  int sign_b = s21_long_checksign(*b);
  int sign = 0;
  s21_long_decimal *min = NULL, *max = NULL;
  if (scale_1 < scale_2) {
    min_scale = scale_1;
    min = a;
    sign = sign_a;
    max_scale = scale_2;
    max = b;
  } else if (scale_1 > scale_2) {
    min_scale = scale_2;
    min = b;
    sign = sign_b;
    max_scale = scale_1;
    max = a;
  }
  s21_long_decimal mul_ten = {0};
  s21_from_int_to_long_decimal(10, &mul_ten);
  while (min_scale != max_scale) {
    if (min_scale < 28 && !s21_long_mul(*min, mul_ten, min)) {
      min_scale++;
      min->bits[7] = 0;
      s21_long_set_scale_and_sign(min, min_scale, sign);
    } else {
      s21_round_decim(max, max_scale - min_scale);
      break;
    }
  }
}

void s21_round_decim(s21_long_decimal *x, int scale) {
  int scale_x = s21_long_checkscale(*x) - scale;
  int sign_x = s21_long_checksign(*x);
  while (scale > 0) {
    s21_long_decimal zero = {0};
    s21_long_decimal tmp_mod = {0};
    s21_long_decimal ten = {0};
    s21_from_int_to_long_decimal(10, &ten);
    s21_long_decimal two = {0};
    s21_from_int_to_long_decimal(2, &two);
    s21_long_decimal one = {0};
    s21_from_int_to_long_decimal(1, &one);
    s21_long_decimal mod = {0};
    mod = s21_long_div(*x, ten, x);
    if (mod.bits[0] > 5) {
      s21_long_add(*x, one, x);
    } else if (mod.bits[0] == 5)
      tmp_mod = s21_long_div(*x, two, &zero);
    if (tmp_mod.bits[0] == one.bits[0]) s21_long_add(*x, one, x);
    scale--;
  }
  s21_long_set_scale_and_sign(x, scale_x, sign_x);
}

int s21_long_add(s21_long_decimal a, s21_long_decimal b,
                 s21_long_decimal *result) {
  int tmp = 0, check = 0, add = 0;
  for (int i = 0; i < 192 && !check; i++) {
    add = s21_checkbit(a.bits[i / 32], i % 32) +
          s21_checkbit(b.bits[i / 32], i % 32);
    if (add == 2 && tmp == 1) {
      s21_setbit(&result->bits[i / 32], i % 32, 1);
      tmp = 1;
    } else if (add == 2 && tmp == 0) {
      s21_setbit(&result->bits[i / 32], i % 32, 0);
      tmp = 1;
    } else if (add == 1 && tmp == 1) {
      s21_setbit(&result->bits[i / 32], i % 32, 0);
      tmp = 1;
    } else if (add == 1 && tmp == 0) {
      s21_setbit(&result->bits[i / 32], i % 32, 1);
    } else if (add == 0 && tmp == 1) {
      s21_setbit(&result->bits[i / 32], i % 32, 1);
      tmp = 0;
    } else {
      s21_setbit(&result->bits[i / 32], i % 32, 0);
    }
    if (i == 191 && tmp == 1) check = 1;
  }
  return check;
}

void s21_long_sub(s21_long_decimal a, s21_long_decimal b,
                  s21_long_decimal *result) {
  int sub = 0, tmp = 0;
  for (int i = 0; i < 192; i++) {
    sub = s21_checkbit(a.bits[i / 32], i % 32) -
          s21_checkbit(b.bits[i / 32], i % 32) - tmp;
    if (!sub) {
      s21_setbit(&result->bits[i / 32], i % 32, 0);
      if (tmp) tmp = 0;
    } else if (sub == 1) {
      s21_setbit(&result->bits[i / 32], i % 32, 1);
      if (tmp) tmp = 0;
    } else if (sub == -1) {
      s21_setbit(&result->bits[i / 32], i % 32, 1);
      tmp = 1;
    }
  }
}

int s21_shiftleft(s21_long_decimal *x) {
  for (int i = 191; i >= 0; i--) {
    if (i != 0)
      s21_setbit(&x->bits[i / 32], i % 32,
                 s21_checkbit(x->bits[(i - 1) / 32], (i - 1) % 32));
    else
      s21_setbit(&x->bits[i / 32], i % 32, 0);
  }
  return 0;
}

int s21_shiftright(s21_long_decimal *x) {
  for (int i = 0; i < 192; i++) {
    if (i != 191)
      s21_setbit(&x->bits[i / 32], i % 32,
                 s21_checkbit(x->bits[(i + 1) / 32], (i + 1) % 32));
    else
      s21_setbit(&x->bits[i / 32], i % 32, 0);
  }
  return 0;
}

int s21_long_mul(s21_long_decimal a, s21_long_decimal b,
                 s21_long_decimal *result) {
  s21_long_decimal tmp = {0};
  s21_long_decimal long_zero = {0};
  *result = long_zero;
  int check = 0;
  for (int i = 0; i < 192; i++) {
    if (s21_checkbit(a.bits[i / 32], i % 32)) {
      tmp = b;
      for (int j = 0; j < i; j++) s21_shiftleft(&tmp);
      check = s21_long_add(tmp, *result, result);
      if (check) return 1;
    }
  }
  return 0;
}

int s21_more_or_less(s21_long_decimal a, s21_long_decimal b) {
  int check = 0, res1 = 0, res2 = 0;
  for (int i = 191; i >= 0 && !check; i--) {
    res1 = s21_checkbit(a.bits[i / 32], i % 32);
    res2 = s21_checkbit(b.bits[i / 32], i % 32);
    if (res1 > res2) {
      check = 1;
      break;
    } else if (res1 < res2) {
      check = -1;
      break;
    }
  }
  return check;
}

s21_long_decimal s21_long_div(s21_long_decimal a, s21_long_decimal b,
                              s21_long_decimal *result) {
  s21_long_decimal tmp = {0};
  s21_long_decimal mod = {0};
  s21_long_decimal zero = {0};
  *result = zero;
  s21_setbit(&tmp.bits[0], 0, 1);
  s21_long_decimal copy_b = b;
  while (1) {
    int hbit = s21_h_bit(a) - s21_h_bit(copy_b);
    for (int i = 0; i < hbit; i++) {
      s21_shiftleft(&copy_b);
      s21_shiftleft(&tmp);
    }
    if (s21_more_or_less(a, copy_b) == -1) {
      s21_shiftright(&copy_b);
      s21_shiftright(&tmp);
    }
    if (s21_more_or_less(a, copy_b) == 1 || s21_more_or_less(a, copy_b) == 0) {
      s21_long_sub(a, copy_b, &a);
      s21_long_add(tmp, *result, result);
      copy_b = b;
      tmp = zero;
      s21_setbit(&tmp.bits[0], 0, 1);
    }
    if (s21_more_or_less(a, b) == -1) break;
  }
  mod = a;
  return mod;
}

int s21_h_bit(s21_long_decimal x) {
  int i = 191;
  for (; i >= 0; i--) {
    if (s21_checkbit(x.bits[i / 32], i % 32)) break;
  }
  return i;
}

int s21_check_bit_zero(s21_decimal x) {
  int check = 0;
  for (int i = 0; i < 16; i++) {
    check += s21_checkbit(x.bits[3], i);
  }
  for (int i = 24; i < 31; i++) {
    check += s21_checkbit(x.bits[3], i);
  }
  return check;
}