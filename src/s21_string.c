#include "s21_string.h"

#include <stdlib.h>

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const unsigned char *p = str;
  void *result = S21_NULL;

  for (s21_size_t i = 0; i < n && result == S21_NULL; i++) {
    if (p[i] == (unsigned char)c) {
      result = (void *)(p + i);
    }
  }

  return result;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *a = str1;
  const unsigned char *b = str2;
  int result = 0;

  for (s21_size_t i = 0; i < n && result == 0; i++) {
    if (a[i] != b[i]) {
      result = a[i] - b[i];
    }
  }

  return result;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *d = dest;
  const unsigned char *s = src;
  for (s21_size_t i = 0; i < n; i++) d[i] = s[i];
  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *p = str;
  for (s21_size_t i = 0; i < n; i++) p[i] = (unsigned char)c;
  return str;
}

s21_size_t s21_strlen(const char *str) {
  s21_size_t len = 0;
  while (str && str[len]) len++;
  return len;
}

char *s21_strerror(int errnum) {
#if defined(__APPLE__)
  static const char *errors[] = {
      "Undefined error: 0",
      "Operation not permitted",
      "No such file or directory",
      "No such process",
      "Interrupted system call",
      "Input/output error",
      "Device not configured",
      "Argument list too long",
      "Exec format error",
      "Bad file descriptor",
      "No child processes",
      "Resource deadlock avoided",
      "Cannot allocate memory",
      "Permission denied",
      "Bad address",
      "Block device required",
      "Device busy",
      "File exists",
      "Cross-device link",
      "Operation not supported by device",
      "Not a directory",
      "Is a directory",
      "Invalid argument",
      "Too many open files in system",
      "Too many open files",
      "Inappropriate ioctl for device",
      "Text file busy",
      "File too large",
      "No space left on device",
      "Illegal seek",
      "Read-only file system",
      "Too many links",
      "Broken pipe",
      "Numerical argument out of domain",
      "Result too large",
      "Resource temporarily unavailable",
      "Operation now in progress",
      "Operation already in progress",
      "Socket operation on non-socket",
      "Destination address required",
      "Message too long",
      "Protocol wrong type for socket",
      "Protocol not available",
      "Protocol not supported",
      "Socket type not supported",
      "Operation not supported",
      "Protocol family not supported",
      "Address family not supported by protocol family",
      "Address already in use",
      "Can't assign requested address",
      "Network is down",
      "Network is unreachable",
      "Network dropped connection on reset",
      "Software caused connection abort",
      "Connection reset by peer",
      "No buffer space available",
      "Socket is already connected",
      "Socket is not connected",
      "Can't send after socket shutdown",
      "Too many references: can't splice",
      "Operation timed out",
      "Connection refused",
      "Too many levels of symbolic links",
      "File name too long",
      "Host is down",
      "No route to host",
      "Directory not empty",
      "Too many processes",
      "Too many users",
      "Disc quota exceeded",
      "Stale file handle",
      "Too many levels of remote in path",
      "RPC struct is bad",
      "RPC version wrong",
      "RPC prog. not avail",
      "Program version wrong",
      "Bad procedure for program",
      "No locks available",
      "Function not implemented",
      "Inappropriate file type or format",
      "Authentication error",
      "Need authenticator",
      "Device power is off",
      "Device error",
      "Value too large to be stored in data type",
      "Bad executable (or shared library)",
      "Bad CPU type in executable",
      "Shared library version mismatch",
      "Malformed Mach-o file",
      "Operation canceled",
      "Identifier removed",
      "No message of desired type",
      "Illegal byte sequence",
      "Attribute not found",
      "Bad message",
      "EMULTIHOP (Reserved)",
      "No message available on STREAM",
      "ENOLINK (Reserved)",
      "No STREAM resources",
      "Not a STREAM",
      "Protocol error",
      "STREAM ioctl timeout",
      "Operation not supported on socket",
      "Policy not found",
      "State not recoverable",
      "Previous owner died",
      "Interface output queue is full"};
  int max_err = 106;
#else
  static const char *errors[] = {
      "Success",
      "Operation not permitted",
      "No such file or directory",
      "No such process",
      "Interrupted system call",
      "Input/output error",
      "No such device or address",
      "Argument list too long",
      "Exec format error",
      "Bad file descriptor",
      "No child processes",
      "Resource temporarily unavailable",
      "Cannot allocate memory",
      "Permission denied",
      "Bad address",
      "Block device required",
      "Device or resource busy",
      "File exists",
      "Invalid cross-device link",
      "No such device",
      "Not a directory",
      "Is a directory",
      "Invalid argument",
      "Too many open files in system",
      "Too many open files",
      "Inappropriate ioctl for device",
      "Text file busy",
      "File too large",
      "No space left on device",
      "Illegal seek",
      "Read-only file system",
      "Too many links",
      "Broken pipe",
      "Numerical argument out of domain",
      "Numerical result out of range",
      "Resource deadlock avoided",
      "File name too long",
      "No locks available",
      "Function not implemented",
      "Directory not empty",
      "Too many symbolic links encountered",
      "No message of desired type",
      "Identifier removed",
      "Channel number out of range",
      "Level 2 not synchronized",
      "Level 3 halted",
      "Level 3 reset",
      "Link number out of range",
      "Protocol driver not attached",
      "No CSI structure available",
      "Level 2 halted",
      "Invalid exchange",
      "Invalid request descriptor",
      "Exchange full",
      "No anode",
      "Invalid request code",
      "Invalid slot",
      "Bad font file format",
      "Device not a stream",
      "No data available",
      "Timer expired",
      "Out of streams resources",
      "Machine is not on the network",
      "Package not installed",
      "Object is remote",
      "Link has been severed",
      "Advertise error",
      "Srmount error",
      "Communication error on send",
      "Protocol error",
      "Multihop attempted",
      "RFS specific error",
      "Bad message",
      "Value too large for defined data type",
      "Name not unique on network",
      "File descriptor in bad state",
      "Remote address changed",
      "Can not access a needed shared library",
      "Accessing a corrupted shared library",
      ".lib section in a.out corrupted",
      "Attempting to link in too many shared libraries",
      "Cannot exec a shared library directly",
      "Invalid or incomplete multibyte or wide character",
      "Interrupted system call should be restarted",
      "Streams pipe error",
      "Too many users",
      "Socket operation on non-socket",
      "Destination address required",
      "Message too long",
      "Protocol wrong type for socket",
      "Protocol not available",
      "Protocol not supported",
      "Socket type not supported",
      "Operation not supported",
      "Protocol family not supported",
      "Address family not supported by protocol",
      "Address already in use",
      "Cannot assign requested address",
      "Network is down",
      "Network is unreachable",
      "Network dropped connection on reset",
      "Software caused connection abort",
      "Connection reset by peer",
      "No buffer space available",
      "Transport endpoint is already connected",
      "Transport endpoint is not connected",
      "Cannot send after transport endpoint shutdown",
      "Too many references: cannot splice",
      "Connection timed out",
      "Connection refused",
      "Host is down",
      "No route to host",
      "Operation already in progress",
      "Operation now in progress",
      "Stale file handle",
      "Structure needs cleaning",
      "Not a XENIX named type file",
      "No XENIX semaphores available",
      "Is a named type file",
      "Remote I/O error",
      "Disk quota exceeded",
      "No medium found",
      "Wrong medium type",
      "Operation canceled",
      "Required key not available",
      "Key has expired",
      "Key has been revoked",
      "Key was rejected by service",
      "Owner died",
      "State not recoverable"};
  int max_err = 133;
#endif

  static char buffer[256];

  if (errnum >= 0 && errnum <= max_err) {
    return (char *)errors[errnum];
  }

#if defined(__APPLE__)
  s21_memset(buffer, 0, sizeof(buffer));
  s21_memcpy(buffer, "Unknown error: ", 15);
#else
  s21_memset(buffer, 0, sizeof(buffer));
  s21_memcpy(buffer, "Unknown error ", 14);
#endif

  int num = errnum;
  int i = 0;
  char tmp[32];

  if (num < 0) {
    buffer[14] = '-';
    num = -num;
    i = 1;
  }

  int pos = 0;
  do {
    tmp[pos++] = (char)('0' + num % 10);
    num /= 10;
  } while (num);

  for (int j = pos - 1; j >= 0; j--) {
    buffer[14 + i + pos - 1 - j] = tmp[j];
  }

  return buffer;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t i = s21_strlen(dest);
  s21_size_t j = 0;

  while (j < n && src[j] != '\0') {
    dest[i + j] = src[j];
    j++;
  }
  dest[i + j] = '\0';

  return dest;
}

char *s21_strchr(const char *str, int c) {
  char *result = S21_NULL;

  while (*str && result == S21_NULL) {
    if (*str == (char)c) {
      result = (char *)str;
    }
    str++;
  }

  if (result == S21_NULL && c == '\0') {
    result = (char *)str;
  }

  return result;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int result = 0;

  for (s21_size_t i = 0; i < n && result == 0; i++) {
    if (str1[i] != str2[i] || str1[i] == '\0' || str2[i] == '\0') {
      result = (unsigned char)str1[i] - (unsigned char)str2[i];
    }
  }

  return result;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  s21_size_t i = 0;
  for (; i < n && src[i]; i++) dest[i] = src[i];
  for (; i < n; i++) dest[i] = '\0';
  return dest;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t result = 0;
  int found = 0;

  for (s21_size_t i = 0; str1[i] && !found; i++) {
    for (s21_size_t j = 0; str2[j] && !found; j++) {
      if (str1[i] == str2[j]) {
        result = i;
        found = 1;
      }
    }
    if (!found) result = i + 1;
  }

  return result;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  char *result = S21_NULL;

  for (s21_size_t i = 0; str1[i] && result == S21_NULL; i++) {
    for (s21_size_t j = 0; str2[j] && result == S21_NULL; j++) {
      if (str1[i] == str2[j]) {
        result = (char *)(str1 + i);
      }
    }
  }

  return result;
}

char *s21_strrchr(const char *str, int c) {
  char *res = S21_NULL;
  while (*str) {
    if (*str == (char)c) res = (char *)str;
    str++;
  }
  return (c == '\0') ? (char *)str : res;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *result = S21_NULL;

  if (!*needle) {
    result = (char *)haystack;
  } else {
    for (s21_size_t i = 0; haystack[i] && result == S21_NULL; i++) {
      s21_size_t j = 0;
      while (haystack[i + j] && needle[j] && haystack[i + j] == needle[j]) {
        j++;
      }
      if (!needle[j]) {
        result = (char *)(haystack + i);
      }
    }
  }

  return result;
}

char *s21_strtok(char *str, const char *delim) {
  static char *next = S21_NULL;
  char *result = S21_NULL;

  if (str) {
    next = str;
  }

  if (next) {
    while (*next && s21_strchr(delim, *next)) {
      next++;
    }

    if (*next) {
      result = next;

      while (*next && !s21_strchr(delim, *next)) {
        next++;
      }

      if (*next) {
        *next = '\0';
        next++;
      }
    }
  }

  return result;
}

char *s21_to_upper(const char *str) {
  char *result = S21_NULL;

  if (str) {
    s21_size_t len = s21_strlen(str);
    result = malloc(len + 1);

    if (result) {
      for (s21_size_t i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z')
          result[i] = str[i] - 32;
        else
          result[i] = str[i];
      }
      result[len] = '\0';
    }
  }

  return result;
}

char *s21_to_lower(const char *str) {
  char *result = S21_NULL;

  if (str) {
    s21_size_t len = s21_strlen(str);
    result = malloc(len + 1);

    if (result) {
      for (s21_size_t i = 0; i < len; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z')
          result[i] = str[i] + 32;
        else
          result[i] = str[i];
      }
      result[len] = '\0';
    }
  }

  return result;
}

char *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *result = S21_NULL;

  if (src && str) {
    s21_size_t len1 = s21_strlen(src);
    s21_size_t len2 = s21_strlen(str);

    if (start_index <= len1) {
      result = malloc(len1 + len2 + 1);

      if (result) {
        for (s21_size_t i = 0; i < start_index; i++) result[i] = src[i];

        for (s21_size_t i = 0; i < len2; i++) result[start_index + i] = str[i];

        for (s21_size_t i = start_index; i <= len1; i++)
          result[len2 + i] = src[i];
      }
    }
  }

  return result;
}

char *s21_trim(const char *src, const char *trim_chars) {
  char *result = S21_NULL;

  if (src) {
    if (!trim_chars) trim_chars = " ";

    s21_size_t start = 0;
    s21_size_t end = s21_strlen(src);

    while (start < end && s21_strchr(trim_chars, src[start])) start++;

    while (end > start && s21_strchr(trim_chars, src[end - 1])) end--;

    result = malloc(end - start + 1);

    if (result) {
      for (s21_size_t i = start; i < end; i++) result[i - start] = src[i];

      result[end - start] = '\0';
    }
  }

  return result;
}
