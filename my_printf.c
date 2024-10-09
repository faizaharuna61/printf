#include <stdarg.h>
#include <unistd.h>

#define BUFFER_SIZE 64
static void ptr_to_hex(void *stk, char *buffer);
static void my_itoa(int dig, char *buffer, int bottom);

static int my_strlen(const char *str) {
    int len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

static void write_string(const char *str) {
    write(1, str, my_strlen(str));
}

static void write_char(char c) {
    write(1, &c, 1);
}

static void my_itoa(int dig, char *buffer, int bottom) {
    int i = 0;
    int anti = 0;

    if (dig < 0 && bottom == 10) {
        anti = 1;
        dig = -dig;
    }

    
    if (dig == 0) {
        buffer[i++] = '0';
    } else {
        while (dig != 0) {
            int r = dig % bottom;
            buffer[i++] = (r < 10) ? ('0' + r) : ('A' + (r - 10)); //
            dig /= bottom;
        }
    }

    if (anti) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[len - j - 1];
        buffer[len - j - 1] = temp;
    }
}

static void ptr_to_hex(void *stk, char *buffer) {
    unsigned long dig = (unsigned long) stk; 
    int index = 0;
    do {
        int r = dig % 16;
        buffer[index++] = (r < 10) ? ('0' + r) : ('a' + (r - 10));
        dig /= 16;
    } while (dig != 0);

    buffer[index++] = 'x';
    buffer[index++] = '0';

    int len = index;
    for (int i = 0; i < len / 2; i++) {
        char temp = buffer[i];
        buffer[i] = buffer[len - i - 1];
        buffer[len - i - 1] = temp;
    }
    buffer[len] = '\0';
}

int my_printf(char * restrict format, ...) {
    va_list args;
    va_start(args, format);

    int t_wrt = 0;

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd': {
                    int dig = va_arg(args, int);
                    char buffer[BUFFER_SIZE];
                    my_itoa(dig, buffer, 10);
                    int len = my_strlen(buffer);
                    t_wrt += len;
                    write_string(buffer);
                    break;
                }
                case 'c': {
                    char ch = (char) va_arg(args, int);
                    t_wrt++;
                    write_char(ch);
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char *);
                    if (str == NULL) {
                        str = "(null)";
                    }
                    int len = my_strlen(str);
                    t_wrt += len;
                    write_string(str);
                    break;
                }
                case 'p': {
                    void *stk = va_arg(args, void *);
                    char buffer[BUFFER_SIZE];
                    ptr_to_hex(stk, buffer);
                    int len = my_strlen(buffer);
                    t_wrt += len;
                    write_string(buffer);
                    break;
                }
                case 'x': case 'X': case 'o': case 'u': {
                    int dig = va_arg(args, int);
                    int bottom = (*format == 'o') ? 8 : ((*format == 'x' || *format == 'X') ? 16 : 10);
                    char buffer[BUFFER_SIZE];
                    my_itoa(dig, buffer, bottom);
                    int len = my_strlen(buffer);
                    t_wrt += len;
                    write_string(buffer);
                    break;
                }
                case '%': {
                    t_wrt++;
                    write_char('%'); 
                    break;
                }
                default:
                    
                    t_wrt++;
                    write_char('%');
                    write_char(*format);
                    break;
            }
        } else {
            
            t_wrt++;
            write_char(*format);
        }
        format++;
    }

    va_end(args);
    return t_wrt; 
}

int main() {
    
    int ret_val = my_printf("%d - %d - %d!\n", 2048, 0, -1337);
    return ret_val;
}
