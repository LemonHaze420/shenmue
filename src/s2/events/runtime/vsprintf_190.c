#include <stdarg.h>

int vsnprintf(char *s, unsigned int maximum, const char *format, va_list arguments);

int vsprintf(char *s, const char *format, va_list arguments)
{
    return vsnprintf(s, -1, format, arguments);
}
