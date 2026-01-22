#ifndef S21_SSCANF_INTERNAL_H
#define S21_SSCANF_INTERNAL_H

/*
внутренний заголовок реализации s21_sscanf
*/

#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define S21_SSCANF_SET_SIZE 256

/*
коды состояния одной операции чтения/сопоставления
*/
typedef enum {
  S21_SCAN_OK = 0,  // преобразование прошло успешно
  S21_SCAN_MATCH_FAIL = 1,  // вход не соответствует ожидаемому формату
  S21_SCAN_INPUT_FAIL = 2  // вход закончился (EOF) до начала/в процессе чтения
} s21_scan_status_t;

// модификаторы длины: hh, h, l, ll, L
typedef enum {
  S21_LEN_NONE = 0,
  S21_LEN_HH,
  S21_LEN_H,
  S21_LEN_L,
  S21_LEN_LL,
  S21_LEN_CAP_L
} s21_len_t;

/*
режимы основания системы счисления
AUTO используется для %i: 0x => 16, ведущий 0 => 8, иначе 10
*/
typedef enum {
  S21_BASE_AUTO = 0,
  S21_BASE_8 = 8,
  S21_BASE_10 = 10,
  S21_BASE_16 = 16
} s21_base_mode_t;

/*
контекст сканирования: общее состояние при разборе строки формата
*/
typedef struct {
  // текущая позиция (курсор) во входной строке
  const char *in;

  // сколько символов входа уже потреблено всего
  int consumed;

  // число зачтённых присваиваний (возвращаемое значение)
  int assigned;

  // флаг остановки основного цикла
  int stopped;

  // произошёл EOF/ошибка входа
  int input_fail;

  // произошёл mismatch формата и входа
  int match_fail;

  // список аргументов для записи результатов
  va_list *ap;
} s21_scan_ctx_t;

/*
разобранное одно преобразование из строки формата (то, что идёт после '%')
содержит все параметры, необходимые для выполнения преобразования
*/
typedef struct {
  // подавление присваивания: '*'
  int suppress;

  // ширина поля (0 означает "не задана")
  int width;

  // модификатор длины
  s21_len_t len;

  // спецификатор преобразования
  char spec;

  // 1 для %[...], иначе 0
  int scanset_used;

  // 1 если %[ ^ ... ]
  int scanset_invert;

  // таблица принадлежности для scanset
  unsigned char set[S21_SSCANF_SET_SIZE];
} s21_fmt_token_t;

// ASCII-хелперы (только однобайтный ASCII)
int s21_is_space(unsigned char c);
int s21_is_digit(unsigned char c);
int s21_is_xdigit(unsigned char c);
unsigned char s21_tolower_ascii(unsigned char c);

// примитивы чтения входа
void s21_skip_spaces_in(s21_scan_ctx_t *ctx);

/*
ПОТРЕБИТЬ = прочитать текущий символ и сдвинуть курсор вперед.
*/
// посмотреть текущий байт входа без потребления; 1 если доступен, 0 если '\0'
int s21_peek_in(s21_scan_ctx_t *ctx, unsigned char *out);

// потребить один байт входа; 1 если потребили, 0 если '\0'
int s21_take_in(s21_scan_ctx_t *ctx, unsigned char *out);

/*
потребить не более limit байтов, пока pred возвращает true
возвращает количество реально потреблённых байтов
*/
int s21_take_while(s21_scan_ctx_t *ctx, int limit, int (*pred)(unsigned char));

// парсинг формата
void s21_init_token(s21_fmt_token_t *t);

/*
разобрать одно преобразование, которое начинается в fmt (fmt указывает на '%')
возвращает указатель на следующий символ формата после спецификатора
при ошибке: t->spec = '\0', а возвращаемый указатель == fmt
*/
const char *s21_parse_token(const char *fmt, s21_fmt_token_t *t);

// диспетчер: применить один разобранный токен
void s21_apply_token(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t);

// конвертеры (чтение + опциональная запись результата)
s21_scan_status_t s21_scan_percent(s21_scan_ctx_t *ctx,
                                   const s21_fmt_token_t *t);
s21_scan_status_t s21_scan_n(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t);

s21_scan_status_t s21_scan_string(s21_scan_ctx_t *ctx,
                                  const s21_fmt_token_t *t);
s21_scan_status_t s21_scan_char(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t);
s21_scan_status_t s21_scan_scanset(s21_scan_ctx_t *ctx,
                                   const s21_fmt_token_t *t);

s21_scan_status_t s21_scan_signed(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t,
                                  s21_base_mode_t base_mode);

s21_scan_status_t s21_scan_unsigned(s21_scan_ctx_t *ctx,
                                    const s21_fmt_token_t *t, int base);

s21_scan_status_t s21_scan_pointer(s21_scan_ctx_t *ctx,
                                   const s21_fmt_token_t *t);
s21_scan_status_t s21_scan_float(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t);

/*
общий разбор целых чисел по правилам, близким к scanf
*/
s21_scan_status_t s21_read_int_core(s21_scan_ctx_t *ctx, int width,
                                    s21_base_mode_t base_mode, int allow_sign,
                                    unsigned long long *mag, int *sign);

/*
общий разбор float, парсит вид:
  [знак] (цифры [.(цифры)] | .цифры) [ (e|E) [знак] цифры ]
*/
s21_scan_status_t s21_read_float_core(s21_scan_ctx_t *ctx, int width,
                                      long double *out);

// запись результата (va_list) с учётом модификаторов длины
void s21_store_signed(va_list *ap, s21_len_t len, long long v);
void s21_store_unsigned(va_list *ap, s21_len_t len, unsigned long long v);
void s21_store_pointer(va_list *ap, void *p);
void s21_store_float(va_list *ap, s21_len_t len, long double v);
void s21_store_n(va_list *ap, s21_len_t len, int consumed);

#endif
