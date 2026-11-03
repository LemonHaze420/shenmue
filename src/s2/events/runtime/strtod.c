 
#include <ctype.h>

#define ERANGE 34

int errno;
extern const float __infinity_float;

static int
accumulate(value, digit)
	float *value;
	int digit;
{
	*value = *value * 10 + digit;
	return 0;
}

static const float powers[] = {1, 10, 100, 1000, 10000};

static double
power(n)
	unsigned int n;
{
	if (n <= 4)
		return powers[n];
	if (n & 1)
		return 10 * power(n - 1);
	return power(n >> 1) * power(n >> 1);
}

static double
scale(value, exp, negative)
	float *value;
	int exp, negative;
{
	float result;

	if (exp > 38) {
		errno = ERANGE;
		return negative ? -__infinity_float : __infinity_float;
	}
	if (exp < -37) {
		errno = ERANGE;
		return 0;
	}
	result = *value;
	if (negative)
		result = -result;
	if (exp == 0)
		return result;
	if (exp >= 0)
		return result * power(exp);
	return result / power(-exp);
}

double
strtod(s, endptr)
	const char *s;
	char **endptr;
{
	const char *start = s;
	float value = 0;
	int e = 0, any = 0, flags = 0, exp = 0;

	while (isspace(*s))
		s++;
	if (*s == '\0') {
		if (endptr)
			*endptr = (char *)start;
		return 0;
	}
	if (*s == '-' || *s == '+') {
		if (*s == '-')
			flags = 1;
		s++;
		if (*s == '\0') {
			if (endptr)
				*endptr = (char *)start;
			return 0;
		}
	}
	for (; isdigit(*s) || *s == '.'; s++) {
		any = 1;
		if (*s == '.')
			flags |= 4;
		else {
			if (accumulate(&value, *s - '0'))
				exp++;
			if (flags & 4)
				exp--;
		}
	}
	if (*s == 'e' || *s == 'E' || *s == 'd' || *s == 'D') {
		any = 1;
		s++;
		if (*s == '\0')
			goto done;
		while (isspace(*s))
			s++;
		if (*s == '\0')
			goto done;
		if (*s == '-' || *s == '+') {
			if (*s++ == '-')
				flags |= 2;
		}
		if (*s == '\0')
			goto done;
		while (isdigit(*s)) {
			if (e <= 37)
				e = e * 10 + (*s - '0');
			s++;
		}
		if (flags & 2)
			exp -= e;
		else
			exp += e;
	}
done:
	if (endptr)
		*endptr = (char *)(any ? s : start);
	return scale(&value, exp, flags & 1);
}
