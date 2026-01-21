#ifndef S21_STRING_H
#define S21_STRING_H

#include <limits.h>
#include <math.h>
#include <stdarg.h>  // для работы с va_list
#include <stddef.h>

typedef size_t s21_size_t;

#define S21_NULL ((void *)0)
#define s21_NULL S21_NULL  // для совместимости с тестами

int s21_sscanf(const char *str, const char *format, ...);

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strcat(char *dest, const char *src);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
char *s21_to_upper(const char *str);
char *s21_to_lower(const char *str);
char *s21_insert(const char *src, const char *str, s21_size_t start_index);
char *s21_trim(const char *src, const char *trim_chars);

typedef struct {
  int minus;
  int plus;
  int space;
  int width;
  int precision;
  int has_precision;
  char length;
  char specifier;
  int zero;
  int hash;
} flags_options;

int s21_sprintf(char *str, const char *format, ...);
const char *parameter_parsing(const char **format, flags_options *flags,
                              va_list *args);
char *process_specifier(char *str, flags_options *flags, va_list *args);

void parsing_flags(const char **format, flags_options *flags);
void parsing_width(const char **format, flags_options *flags, va_list *args);
void parsing_precision(const char **format, flags_options *flags,
                       va_list *args);
void parsing_length(const char **format, flags_options *flags);

char *handle_char(char *str, flags_options *flags, va_list *args);
char *handle_int(char *str, flags_options *flags, va_list *args);
char *handle_string(char *str, flags_options *flags, va_list *args);
char *handle_float(char *str, flags_options *flags, va_list *args);
char *handle_unsigned(char *str, flags_options *flags, va_list *args);
char *handle_percent(char *str, flags_options *flags, va_list *args);

void s21_itoa(long long n, char *buffer);
void reverse_string(char *s);
void s21_utoa(unsigned long long n, char *buffer);
void s21_apply_width(char **str, flags_options *flags, char *tmp_buffer);

int s21_is_digit(char c);
void s21_f_to_str(long double val, char *buffer, int precision, int hash_flag);
void s21_translate(unsigned long long n, char *buffer, int base, int uppercase);
char *handle_uint_hex_oct(char *str, flags_options *flags, va_list *args);
int s21_get_exponent(long double *val);
char *handle_scientific(char *str, flags_options *flags, va_list *args);
void s21_remove_trailing_zeros(char *buffer);
char *handle_g(char *str, flags_options *flags, va_list *args);

void s21_float_to_buffer(long double val, char *buffer, flags_options *flags);
void s21_scientific_to_buffer(long double val, char *buffer,
                              flags_options *flags);
char *s21_strcpy(char *dest, const char *src);

#endif