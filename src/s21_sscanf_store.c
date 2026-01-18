#include "s21_sscanf_internal.h"

/*
назначение файла: записывать распарсенные значения в переменные пользователя через va_list
конвертеры читают и вычисляют значение, а эти функции отвечают за корректный тип (hh/h/l/ll/L) и безопасную запись по указателю
*/

// Записывает знаковое целое значение в переменную нужного типа в зависимости от модификатора длины.
void s21_store_signed(va_list *ap, s21_len_t len, long long v) {
  if (ap != NULL) {
    if (len == S21_LEN_HH) {
      signed char *dst = va_arg(*ap, signed char *);
      if (dst != NULL) *dst = (signed char)v;
    } else if (len == S21_LEN_H) {
      short *dst = va_arg(*ap, short *);
      if (dst != NULL) *dst = (short)v;
    } else if (len == S21_LEN_L) {
      long *dst = va_arg(*ap, long *);
      if (dst != NULL) *dst = (long)v;
    } else if (len == S21_LEN_LL || len == S21_LEN_CAP_L) {
      long long *dst = va_arg(*ap, long long *);
      if (dst != NULL) *dst = (long long)v;
    } else {
      int *dst = va_arg(*ap, int *);
      if (dst != NULL) *dst = (int)v;
    }
  }
}

// Записывает беззнаковое целое значение в переменную нужного типа в зависимости от модификатора длины.
void s21_store_unsigned(va_list *ap, s21_len_t len, unsigned long long v) {
  if (ap != NULL) {
    if (len == S21_LEN_HH) {
      unsigned char *dst = va_arg(*ap, unsigned char *);
      if (dst != NULL) *dst = (unsigned char)v;
    } else if (len == S21_LEN_H) {
      unsigned short *dst = va_arg(*ap, unsigned short *);
      if (dst != NULL) *dst = (unsigned short)v;
    } else if (len == S21_LEN_L) {
      unsigned long *dst = va_arg(*ap, unsigned long *);
      if (dst != NULL) *dst = (unsigned long)v;
    } else if (len == S21_LEN_LL || len == S21_LEN_CAP_L) {
      unsigned long long *dst = va_arg(*ap, unsigned long long *);
      if (dst != NULL) *dst = (unsigned long long)v;
    } else {
      unsigned int *dst = va_arg(*ap, unsigned int *);
      if (dst != NULL) *dst = (unsigned int)v;
    }
  }
}

// Записывает результат %p: кладёт указатель p в переменную пользователя (ожидается void **)
void s21_store_pointer(va_list *ap, void *p) {
  if (ap != NULL) {
    void **dst = va_arg(*ap, void **);
    if (dst != NULL) {
      *dst = p;
    }
  }
}

// Записывает число с плавающей точкой в зависимости от модификатора длины: float / double / long double.
void s21_store_float(va_list *ap, s21_len_t len, long double v) {
  if (ap != NULL) {
    if (len == S21_LEN_CAP_L) {
      long double *dst = va_arg(*ap, long double *);
      if (dst != NULL) *dst = (long double)v;
    } else if (len == S21_LEN_L) {
      double *dst = va_arg(*ap, double *);
      if (dst != NULL) *dst = (double)v;
    } else {
      float *dst = va_arg(*ap, float *);
      if (dst != NULL) *dst = (float)v;
    }
  }
}

// Реализация %n: записывает количество уже потреблённых символов входа в переменную нужного типа.
void s21_store_n(va_list *ap, s21_len_t len, int consumed) {
  if (ap != NULL) {
    if (len == S21_LEN_HH) {
      signed char *dst = va_arg(*ap, signed char *);
      if (dst != NULL) *dst = (signed char)consumed;
    } else if (len == S21_LEN_H) {
      short *dst = va_arg(*ap, short *);
      if (dst != NULL) *dst = (short)consumed;
    } else if (len == S21_LEN_L) {
      long *dst = va_arg(*ap, long *);
      if (dst != NULL) *dst = (long)consumed;
    } else if (len == S21_LEN_LL || len == S21_LEN_CAP_L) {
      long long *dst = va_arg(*ap, long long *);
      if (dst != NULL) *dst = (long long)consumed;
    } else {
      int *dst = va_arg(*ap, int *);
      if (dst != NULL) *dst = (int)consumed;
    }
  }
}
