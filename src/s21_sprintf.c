#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *start = str; // указатель на начало строки

    while (*format != '\0') {
        if(*format == '%') {
            format++;
            flags_options flags = {0}; // обнуление структуры
            parameter_parsing(&format, &flags, &args); // парсинг формата
            str = process_specifier(str, &flags, &args);

        } else {
            *str++ = *format;
        }
        format++;
    }

    *str = '\0';
    va_end(args);
    return (int)(str - start); // кол-во записанных символов
}

char *process_specifier(char *str, flags_options *flags, va_list *args) {
    switch (flags->specifier) {
        case 'd':
        case 'i':
            str = handle_int(str, flags, args);
            break;
        case 'c':
            str = handle_char(str, flags, args);
            break;
        case 's':
            str = handle_string(str, flags, args);
            break;
        case 'f':
            str = handle_float(str, flags, args);
            break;
        case 'u':
            str = handle_unsigned(str, flags, args);
            break;
        case '%':
            str = handle_percent(str, flags, args);
            break;
        case 'o':
        case 'x':
        case 'X':
        case 'p':
            str = handle_uint_hex_oct(str, flags, args);
            break;
        case 'e':
        case 'E':
            str = handle_scientific(str, flags, args);
            break;
        case 'g':
        case 'G':
            str = handle_g(str, flags, args);
            break;
        default:
            break;
    }
    return str;
}

// %[флаги][ширина][.точность][длина][тип]
const char *parameter_parsing(const char **format, flags_options *flags, va_list *args) {
    parsing_flags(format, flags);
    parsing_width(format, flags, args);
    parsing_precision(format, flags, args);
    parsing_length(format, flags);

    flags->specifier = **format;

    return *format;
}

char *s21_strcpy(char *dest, const char *src) {
    char *ptr = dest;
    while ((*ptr++ = *src++)) ;
    return dest;
}

void s21_apply_width(char **str, flags_options *flags, char *tmp_buffer) {
    int len = (int)s21_strlen(tmp_buffer);
    int spaces = flags->width - len;
    char fill = ' ';
  
    // ИСПРАВЛЕНИЕ 1: Логика флага 0
    if (flags->zero && !flags->minus) {
        // Для c, s, p флаг 0 игнорируется (безопаснее использовать пробелы)
        if (s21_strchr("csp", flags->specifier)) {
            fill = ' ';
        }
        // Для чисел (d, i, u, o, x, X) точность отменяет флаг 0
        else if (s21_strchr("diuoxX", flags->specifier) && flags->has_precision) {
            fill = ' ';
        } 
        else {
            fill = '0';
        }
    }

    // ИСПРАВЛЕНИЕ 2: Обработка знака при заполнении нулями
    // Если мы заполняем нулями, знак должен быть напечатан ДО нулей
    if (fill == '0' && (tmp_buffer[0] == '+' || tmp_buffer[0] == '-' || tmp_buffer[0] == ' ')) {
        **str = tmp_buffer[0]; // Печатаем знак сразу
        (*str)++;
        tmp_buffer++; // Сдвигаем указатель буфера (чтобы не напечатать знак второй раз)
        len--;        // Уменьшаем длину оставшейся части строки
        spaces = flags->width - len - 1; // Пересчитываем пробелы (-1 т.к. знак уже записан)
    }

    // Печать отступов (заполнителя) слева
    if (!flags->minus && spaces > 0) {
        for (int i = 0; i < spaces; i++) {
            **str = fill;
            (*str)++;
        }
    }

    // Печать самой строки (или остатка строки без знака)
    for (int i = 0; i < len; i++) {
        **str = tmp_buffer[i];
        (*str)++;
    }

    // Печать отступов справа (флаг минус)
    if (flags->minus && spaces > 0) {
        for (int i = 0; i < spaces; i++) {
            **str = ' ';
            (*str)++;
        }
    }
}

// парсинг параметров 
void parsing_flags(const char **format, flags_options *flags) {
    int parsing = 1;
    while (parsing) {
        switch (**format) {
            case '-': flags->minus = 1; break;
            case '+': flags->plus = 1; break;
            case ' ': flags->space = 1; break;
            case '0': 
                if (!flags->minus) flags->zero = 1; 
                break;
            case '#': flags->hash = 1; break;
            default: parsing = 0; break;
        }
        if (parsing) (*format)++;
    }
}

int s21_is_digit(char c) {
    return (c >= '0' && c <= '9');
}

void parsing_width(const char **format, flags_options *flags, va_list *args) {
    if (**format == '*') {
        (*format)++;
        int width = va_arg(*args, int); // лезет в список и достает там следующее число в списке данных

        if (width < 0) {
            flags->minus = 1;
            flags->width = -width;
        } else {
            flags->width = width;
        }

    } else {
        while (s21_is_digit(**format)) {
            flags->width = flags->width * 10 + (**format - '0');
            (*format)++;
        }

    }
}

void parsing_precision(const char **format, flags_options *flags, va_list *args) {
    if (**format == '.') {
        flags->has_precision = 1;
        flags->precision = 0;
        (*format)++;

        if (**format == '*') {
            (*format)++;
            int precision = va_arg(*args, int);

            if (precision >= 0) {
                flags->precision = precision;
            } else {
                flags->has_precision = 0;      
                flags->precision = 0;
            }
        } else {
            while (s21_is_digit(**format)) {
                flags->precision = flags->precision * 10 + (**format - '0');
                (*format)++;
            }
        }
    }
}

void parsing_length(const char **format, flags_options *flags) {
    if (**format == 'h' || **format == 'l' || **format == 'L') {
        flags->length = **format;
        (*format)++;
    }
}
// конец парсинга параметров

void s21_print_space(int n, char **p) {
    for (int i = 0; i < n; i++) {
        **p = ' ';
        (*p)++;
    }
}

// все функции на печать 
char *handle_string(char *str, flags_options *flags, va_list *args) {
  char *s = va_arg(*args, char *);
  if (s == S21_NULL) s = "(null)";

  char tmp[1000] = {0};
  int len = (int)s21_strlen(s);
  if (flags->has_precision && flags->precision < len) len = flags->precision;

  s21_strncpy(tmp, s, len);
  s21_apply_width(&str, flags, tmp);
  return str;
}

char *handle_char(char *str, flags_options *flags, va_list *args) {
    char c = (char)va_arg(*args, int);
    
    int spaces = flags->width - 1; 
    
    if (!flags->minus && spaces > 0) {
        for (int i = 0; i < spaces; i++) {
            *str = ' ';
            str++;
        }
    }

    *str = c;
    str++;

    if (flags->minus && spaces > 0) {
        for (int i = 0; i < spaces; i++) {
            *str = ' ';
            str++;
        }
    }
    
    return str;
}

char *handle_percent(char *str, flags_options *flags, va_list *args) {
  (void)args;
  char tmp[2] = {'%', '\0'};
  s21_apply_width(&str, flags, tmp);
  return str;
}

// вспомогательная функция (число в строку)
void s21_itoa(long long n, char *buffer) {
  int i = 0;
  unsigned long long num = (n < 0) ? -n : n; // Используем unsigned для безопасности

  if (num == 0) buffer[i++] = '0';
  
  while (num > 0) {
    buffer[i++] = (num % 10) + '0';
    num /= 10;
  }
  buffer[i] = '\0';
  reverse_string(buffer);
}

// вспомогательная функция (переворот строки)
void reverse_string(char *s) {
    int len = s21_strlen(s);
    for (int i = 0; i < len / 2; i++) {
        char temp = s[i];
        s[i] = s[len - i - 1];
        s [len - i - 1] = temp;
    }
}

char *handle_int(char *str, flags_options *flags, va_list *args) {
  long long val;
  if (flags->length == 'l') val = va_arg(*args, long long);
  else if (flags->length == 'h') val = (short)va_arg(*args, int);
  else val = va_arg(*args, int);

  char digits[500] = {0};
  char final_num[1000] = {0};
  int f_idx = 0;

  s21_itoa(val, digits);
  int len_d = (int)s21_strlen(digits);
  if (flags->has_precision && flags->precision == 0 && val == 0) len_d = 0;

  // 1. Определяем знак
  char sign = 0;
  if (val < 0) sign = '-';
  else if (flags->plus) sign = '+';
  else if (flags->space) sign = ' ';

  // 2. Сборка "тела" числа: [Знак] + [Нули точности] + [Цифры]
  if (sign) {
      final_num[f_idx++] = sign;
  }

  int prec = flags->precision - len_d;
  for (int i = 0; i < prec; i++) final_num[f_idx++] = '0';
  for (int i = 0; i < len_d; i++) final_num[f_idx++] = digits[i];

  s21_apply_width(&str, flags, final_num);
  return str;
}

void s21_utoa(unsigned long long n, char *buffer) {
    int i = 0;
    if (n == 0) buffer[i++] = '0';
    
    while (n > 0) {
        buffer[i++] = (n % 10) + '0';
        n /= 10;
    }
    buffer[i] = '\0';
    reverse_string(buffer);
}

char *handle_unsigned(char *str, flags_options *flags, va_list *args) {
  unsigned long long val;
  if (flags->length == 'l') val = va_arg(*args, unsigned long long);
  else if (flags->length == 'h') val = (unsigned short)va_arg(*args, int);
  else val = va_arg(*args, unsigned int);

  char digits[500] = {0};
  char final_num[1000] = {0};
  int f_idx = 0;

  s21_utoa(val, digits);
  int len_d = (int)s21_strlen(digits);
  if (flags->has_precision && flags->precision == 0 && val == 0) len_d = 0;

  int prec = flags->precision - len_d;
  for (int i = 0; i < prec; i++) final_num[f_idx++] = '0';
  for (int i = 0; i < len_d; i++) final_num[f_idx++] = digits[i];

  s21_apply_width(&str, flags, final_num);
  return str;
}

char *handle_float(char *str, flags_options *flags, va_list *args) {
    long double val = (flags->length == 'L') ? va_arg(*args, long double) : va_arg(*args, double);
    char buffer[1024] = {0};
    
    // Установка точности по умолчанию
    if (!flags->has_precision) flags->precision = 6;
    
    s21_float_to_buffer(val, buffer, flags);
    s21_apply_width(&str, flags, buffer);
    return str;
}

void s21_f_to_str(long double val, char *buffer, flags_options *flags) {
    int precision = flags->has_precision ? flags->precision : 6;
    
    // Округление
    long double round_val = 0.5;
    for (int i = 0; i < precision; i++) round_val /= 10.0;
    val += round_val;

    long double ipart;
    long double fpart = modfl(val, &ipart);
 
    s21_utoa((unsigned long long)ipart, buffer);

    if (precision > 0 || flags->hash) {
        int len = s21_strlen(buffer);
        buffer[len] = '.';
        buffer[len + 1] = '\0';
    }

    if (precision > 0) {
        for (int i = 0; i < precision; i++) {
            fpart *= 10.0;
            int digit = (int)fpart;
            int curr_len = s21_strlen(buffer);
            buffer[curr_len] = digit + '0';
            buffer[curr_len + 1] = '\0';
            fpart -= digit;
        }
    }
}
// парт 3 - блок, где реализация оставшихся спецификаторов

void s21_translate(unsigned long long n, char *buffer, int base, int uppercase) {
    char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    char temp[512] = {0};
    int i = 0;

    if (n == 0) temp[i++] = '0';
    while (n != 0) {
        temp[i++] = digits[n % base];
        n /= base;
    }
    
    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - 1 - j];
    }
    buffer[i] = '\0';
}
// Помощник для знака и префикса float
void s21_add_float_sign(char *buffer, long double val, flags_options *flags) {
    char sign_str[2] = {0};
    if (val < 0) sign_str[0] = '-';
    else if (flags->plus) sign_str[0] = '+';
    else if (flags->space) sign_str[0] = ' ';
    
    if (sign_str[0]) {
        char temp[1024] = {0};
        s21_strcat(temp, sign_str);
        s21_strcat(temp, buffer);
        s21_strcpy(buffer, temp);
    }
}

void s21_float_to_buffer(long double val, char *buffer, flags_options *flags) {
    long double abs_val = (val < 0) ? -val : val;
    s21_f_to_str(abs_val, buffer, flags);
    s21_add_float_sign(buffer, val, flags);
}

void s21_scientific_to_buffer(long double val, char *buffer, flags_options *flags) {
    long double abs_val = (val < 0) ? -val : val;
    int pow = s21_get_exponent(&abs_val);
    
    // Формируем мантиссу
    s21_f_to_str(abs_val, buffer, flags);
    
    // Формируем экспоненту
    int abs_pow = (pow < 0) ? -pow : pow;
    
    s21_strcat(buffer, (flags->specifier == 'e' || flags->specifier == 'g') ? "e" : "E");
    s21_strcat(buffer, (pow >= 0) ? "+" : "-");
    
    // Если экспонента меньше 10, добавляем ведущий ноль
    if (abs_pow < 10) {
        s21_strcat(buffer, "0");
    }
    
    // Преобразуем число степени в строку корректно (даже если там 3 цифры)
    char pow_num_str[20];
    s21_itoa(abs_pow, pow_num_str); 
    s21_strcat(buffer, pow_num_str);

    // Добавляем знак к началу всего числа
    s21_add_float_sign(buffer, val, flags);
}

char *handle_uint_hex_oct(char *str, flags_options *flags, va_list *args) {
    unsigned long long val;
    
    if (flags->specifier == 'p') {
        val = (unsigned long long)va_arg(*args, void *);
    } else if (flags->length == 'l') {
        val = va_arg(*args, unsigned long long);
    } else if (flags->length == 'h') {
        val = (unsigned short)va_arg(*args, unsigned int);
    } else {
        val = va_arg(*args, unsigned int);
    }

    char num_str[512] = {0};
    char final_num[1024] = {0};
    
    int base = (flags->specifier == 'o') ? 8 : 16;
    int upper = (flags->specifier == 'X');
    
    s21_translate(val, num_str, base, upper);
    int len = (int)s21_strlen(num_str);
    
    if (flags->has_precision && flags->precision == 0 && val == 0) {
        if (flags->specifier != 'o' || !flags->hash) len = 0;
    }

    if (flags->specifier == 'p') {
        s21_strcat(final_num, "0x");
    } else if (flags->hash && val != 0) {
        if (base == 8) {
            if (flags->precision <= len) s21_strcat(final_num, "0");
        } else if (base == 16) {
            s21_strcat(final_num, upper ? "0X" : "0x");
        }
    } else if (flags->hash && flags->specifier == 'o' && val == 0 && flags->has_precision && flags->precision == 0) {
        s21_strcat(final_num, "0");
    }

    int zeros = flags->precision - len;
    if (flags->specifier == 'o' && flags->hash && val != 0 && flags->precision > len) {
        zeros--; 
    }
    
    while (zeros-- > 0) s21_strcat(final_num, "0");
    
    if (len > 0) s21_strcat(final_num, num_str);

    s21_apply_width(&str, flags, final_num);
    
    return str;
}

int s21_get_exponent(long double *val) {
    int pow = 0;
    if (*val == 0) return 0;

    if (*val >= 10.0) {
        while (*val >= 10.0) {
            *val /= 10.0;
            pow++;
        }
    } 

    else if (*val > 0 && *val < 1.0) {
        while (*val < 1.0) {
            *val *= 10.0;
            pow--;
        }
    }
    return pow;
}

char *handle_scientific(char *str, flags_options *flags, va_list *args) {
    long double val = (flags->length == 'L') ? va_arg(*args, long double) : va_arg(*args, double);
    char buffer[1024] = {0};
    if (!flags->has_precision) flags->precision = 6;
    
    s21_scientific_to_buffer(val, buffer, flags);
    s21_apply_width(&str, flags, buffer);
    return str;
}

void s21_remove_trailing_zeros(char *buffer) {
    if (!s21_strchr(buffer, '.')) return;

    int len = s21_strlen(buffer);
    char *e_pos = s21_strchr(buffer, 'e');
    if (!e_pos) e_pos = s21_strchr(buffer, 'E');
    
    int i = e_pos ? (int)(e_pos - buffer) - 1 : len - 1;

    while (i > 0 && buffer[i] == '0') {
        i--;
    }
    
    if (i > 0 && buffer[i] == '.') {
        i--;
    }

    if (e_pos) {
        char temp_e[20];
        s21_strcpy(temp_e, e_pos);
        buffer[i + 1] = '\0';
        s21_strcat(buffer, temp_e);
    } else {
        buffer[i + 1] = '\0';
    }
}

char *handle_g(char *str, flags_options *flags, va_list *args) {
    long double val = (flags->length == 'L') ? va_arg(*args, long double) : va_arg(*args, double);
    
    // 1. Установка точности по умолчанию (6 значащих цифр)
    if (!flags->has_precision) flags->precision = 6;
    // Точность 0 трактуется как 1 значащая цифра
    if (flags->precision == 0) flags->precision = 1;

    int saved_prec = flags->precision;
    char saved_spec = flags->specifier;

    long double temp_val = (val < 0) ? -val : val;
    int pow = (val == 0) ? 0 : s21_get_exponent(&temp_val);

    char buffer[1024] = {0};
    
    // 2. Выбор между 'f' и 'e' согласно стандарту:
    // Используется 'e', если pow < -4 или pow >= precision
    if (pow < -4 || pow >= flags->precision) {
        flags->specifier = (saved_spec == 'g') ? 'e' : 'E';
        // Для %g точность P означает P значащих цифр. 
        // В %e это превращается в P-1 знаков после запятой.
        flags->precision = saved_prec - 1;
        s21_scientific_to_buffer(val, buffer, flags);
    } else {
        // Для %g точность P означает P значащих цифр.
        // В %f это превращается в P - 1 - pow знаков после запятой.
        flags->precision = saved_prec - 1 - pow;
        s21_float_to_buffer(val, buffer, flags);
    }

    // 3. Удаление незначащих нулей (если нет флага #)
    if (!flags->hash) {
        s21_remove_trailing_zeros(buffer);
    }

    // Восстанавливаем флаги для корректной работы s21_apply_width
    flags->precision = saved_prec;
    flags->specifier = saved_spec;
    
    s21_apply_width(&str, flags, buffer);
    return str;
}