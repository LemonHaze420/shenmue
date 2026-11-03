/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <stdarg.h>

#define MAXEXP 308
#define MAXFRACT 39
#define BUF (MAXEXP + MAXFRACT + 1)
#define DEFPREC 6

#define PUTC(ch) { ++cnt; *s++ = ch; }

#define SARG() \
	(flags & LONGINT ? va_arg(argp, long) : \
	    flags & SHORTINT ? (long)(short)va_arg(argp, int) : \
	    (long)va_arg(argp, int))
#define UARG() \
	(flags & LONGINT ? va_arg(argp, unsigned long) : \
	    flags & SHORTINT ? (unsigned long)(unsigned short)va_arg(argp, int) : \
	    (unsigned long)va_arg(argp, unsigned int))

#define todigit(c) ((c) - '0')
#define tochar(n) ((n) + '0')
#define isdigit(c) ((unsigned)todigit(c) <= 9)

#define LONGINT 0x01
#define LONGDBL 0x02
#define SHORTINT 0x04
#define ALT 0x08
#define LADJUST 0x10
#define ZEROPAD 0x20
#define HEXPREFIX 0x40

char *memchr();
static char *exponent(), *round();
static int cvt();

static double
modf(value, iptr)
	double value, *iptr;
{
	double i, hi;

	if (value > 2147483648.0 || value < -2147483648.0) {
		hi = (double)(long)(value / 2147483648.0) * 2147483648.0;
		i = (double)(long)(value - hi) + hi;
	} else
		i = (double)(long)value;
	*iptr = i;
	return (value - i);
}

static int
cvt(number, prec, flags, signp, fmtch, startp, endp)
	double number;
	register int prec;
	int flags;
	char fmtch;
	char *signp, *startp, *endp;
{
	register char *p, *t;
	register double fract;
	int dotrim, expcnt, gformat;
	double integer, tmp;
	struct ieee {
		unsigned int mantissa:22;
		unsigned int quiet:1;
		unsigned int exponent:8;
		unsigned int sign:1;
	} *ip;

	ip = (struct ieee *)&number;
	if (ip->exponent == 0xff) {
		if (ip->mantissa == 0)
			strcpy(startp, ip->sign ? "-Inf" : "Inf");
		else if (0)
			memcpy(startp, "QNaN", 5);
		else
			memcpy(startp, "SNaN", 5);
		return (strlen(startp));
	}

	dotrim = expcnt = gformat = 0;
	t = ++startp;

	fract = modf(number, &integer);

	for (p = endp - 1; integer; ++expcnt) {
		tmp = modf(integer / 10, &integer);
		*p-- = tochar((int)((tmp + .01) * 10));
	}
	switch (fmtch) {
	case 'f':
		if (expcnt)
			for (; ++p < endp; *t++ = *p);
		else
			*t++ = '0';
		if (prec || flags & ALT)
			*t++ = '.';
		if (fract) {
			if (prec)
				do {
					fract = modf(fract * 10, &tmp);
					*t++ = tochar((int)tmp);
				} while (--prec && fract);
			if (fract)
				startp = round(fract, (int *)0, startp, t - 1, (char)0, signp);
		}
		for (; prec--; *t++ = '0');
		break;
	case 'e':
	case 'E':
eformat:
		if (expcnt) {
			*t++ = *++p;
			if (prec || flags & ALT)
				*t++ = '.';
			for (; prec && ++p < endp; --prec)
				*t++ = *p;
			if (!prec && ++p < endp) {
				fract = 0;
				startp = round((double)0, &expcnt, startp, t - 1, *p, signp);
			}
			--expcnt;
		} else if (fract) {
			for (expcnt = -1;; --expcnt) {
				fract = modf(fract * 10, &tmp);
				if (tmp)
					break;
			}
			*t++ = tochar((int)tmp);
			if (prec || flags & ALT)
				*t++ = '.';
		} else {
			*t++ = '0';
			if (prec || flags & ALT)
				*t++ = '.';
		}
		if (fract) {
			if (prec)
				do {
					fract = modf(fract * 10, &tmp);
					*t++ = tochar((int)tmp);
				} while (--prec && fract);
			if (fract)
				startp = round(fract, &expcnt, startp, t - 1, (char)0, signp);
		}
		for (; prec--; *t++ = '0');
		if (gformat && !(flags & ALT)) {
			while (t > startp && *--t == '0');
			if (*t == '.')
				--t;
			++t;
		}
		t = exponent(t, expcnt, fmtch);
		break;
	case 'g':
	case 'G':
		if (!prec)
			++prec;
		if (expcnt > prec || !expcnt && fract && fract < .0001) {
			--prec;
			fmtch -= 2;
			gformat = 1;
			goto eformat;
		}
		if (expcnt)
			for (; ++p < endp; *t++ = *p, --prec);
		else
			*t++ = '0';
		if (prec || flags & ALT) {
			dotrim = 1;
			*t++ = '.';
		} else
			dotrim = 0;
		if (fract) {
			if (prec) {
				do {
					fract = modf(fract * 10, &tmp);
					*t++ = tochar((int)tmp);
				} while (!tmp);
				while (--prec && fract) {
					fract = modf(fract * 10, &tmp);
					*t++ = tochar((int)tmp);
				}
			}
			if (fract)
				startp = round(fract, (int *)0, startp, t - 1, (char)0, signp);
		}
		if (flags & ALT)
			for (; prec--; *t++ = '0');
		else if (dotrim) {
			while (t > startp && *--t == '0');
			if (*t != '.')
				++t;
		}
	}
	return (t - startp);
}

static char *
round(fract, exp, start, end, ch, signp)
	double fract;
	int *exp;
	register char *start, *end;
	char ch, *signp;
{
	double tmp;

	if (fract)
		(void)modf(fract * 10, &tmp);
	else
		tmp = todigit(ch);
	if (tmp > 4)
		for (;; --end) {
			if (*end == '.')
				--end;
			if (++*end <= '9')
				break;
			*end = '0';
			if (end == start) {
				if (exp) {
					*end = '1';
					++*exp;
				} else {
					*--end = '1';
					--start;
				}
				break;
			}
		}
	else if (*signp == '-')
		for (;; --end) {
			if (*end == '.')
				--end;
			if (*end != '0')
				break;
			if (end == start)
				*signp = 0;
		}
	return (start);
}

static char *
exponent(p, exp, fmtch)
	register char *p;
	register int exp;
	char fmtch;
{
	register char *t;
	char expbuf[MAXEXP];

	*p++ = fmtch;
	if (exp < 0) {
		exp = -exp;
		*p++ = '-';
	} else
		*p++ = '+';
	t = expbuf + MAXEXP;
	if (exp > 9) {
		do {
			*--t = tochar(exp % 10);
		} while ((exp /= 10) > 9);
		*--t = tochar(exp);
		for (; t < expbuf + MAXEXP; *p++ = *t++);
	} else {
		*p++ = '0';
		*p++ = tochar(exp);
	}
	return (p);
}

int
vsprintf(s, fmt0, argp)
	register char *s;
	const char *fmt0;
	va_list argp;
{
	register const char *fmt;
	register int ch;
	register int cnt;
	register int n;
	register char *t;
	double _double;
	unsigned long _ulong;
	int base;
	int dprec;
	int fieldsz;
	int flags;
	int fpprec;
	int prec;
	int realsz;
	int size;
	int width;
	char sign;
	char softsign;
	char *digs;
	char buf[BUF];

	fmt = fmt0;
	digs = "0123456789abcdef";
	for (cnt = 0;; ++fmt) {
		if (!(ch = *fmt)) {
			*s = '\0';
			return (cnt);
		}
		if (ch != '%') {
			PUTC(ch);
			continue;
		}

		flags = 0; dprec = 0; fpprec = 0; width = 0;
		prec = -1;
		sign = '\0';

rflag:
		switch (*++fmt) {
		case ' ':
			if (!sign)
				sign = ' ';
			goto rflag;
		case '#':
			flags |= ALT;
			goto rflag;
		case '*':
			if ((width = va_arg(argp, int)) >= 0)
				goto rflag;
			width = -width;
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '+':
			sign = '+';
			goto rflag;
		case '.':
			if (*++fmt == '*')
				n = va_arg(argp, int);
			else {
				n = 0;
				while (isdigit(*fmt))
					n = 10 * n + todigit(*fmt++);
				--fmt;
			}
			prec = n < 0 ? -1 : n;
			goto rflag;
		case '0':
			flags |= ZEROPAD;
			goto rflag;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			n = 0;
			do {
				n = 10 * n + todigit(*fmt);
			} while (isdigit(*++fmt));
			width = n;
			--fmt;
			goto rflag;
		case 'L':
			flags |= LONGDBL;
			goto rflag;
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			flags |= LONGINT;
			goto rflag;
		case 'c':
			*(t = buf) = va_arg(argp, int);
			size = 1;
			sign = '\0';
			goto pforw;
		case '%':
			t = "%";
			size = 1;
			sign = '\0';
			goto pforw;
		case 'D':
			flags |= LONGINT;
		case 'd':
		case 'i':
			_ulong = SARG();
			if ((long)_ulong < 0) {
				_ulong = -_ulong;
				sign = '-';
			}
			base = 10;
			goto number;
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
			_double = va_arg(argp, double);
			if (prec > MAXFRACT) {
				if (*fmt != 'g' && *fmt != 'G' || (flags & ALT))
					fpprec = prec - MAXFRACT;
				prec = MAXFRACT;
			} else if (prec == -1)
				prec = DEFPREC;
			if (_double < 0) {
				softsign = '-';
				_double = -_double;
			} else
				softsign = 0;
			*buf = 0;
			size = cvt(_double, prec, flags, &softsign, *fmt, buf, buf + sizeof(buf));
			if (softsign)
				sign = '-';
			t = *buf ? buf : buf + 1;
			goto pforw;
		case 'n': {
			void *p = va_arg(argp, void *);

			if (flags & LONGINT)
				*(long *)p = cnt;
			else if (flags & SHORTINT)
				*(short *)p = cnt;
			else
				*(int *)p = cnt;
			break;
		}
		case 'O':
			flags |= LONGINT;
		case 'o':
			_ulong = UARG();
			base = 8;
			goto nosign;
		case 'b':
			_ulong = UARG();
			base = 2;
			goto nosign;
		case 'p':
			_ulong = (unsigned long)va_arg(argp, void *);
			base = 16;
			goto nosign;
		case 's':
			if (!(t = va_arg(argp, char *)))
				t = "(null)";
			if (prec >= 0) {
				char *p;

				if (p = memchr(t, 0, prec)) {
					size = p - t;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = strlen(t);
			sign = '\0';
			goto pforw;
		case 'U':
			flags |= LONGINT;
		case 'u':
			_ulong = UARG();
			base = 10;
			goto nosign;
		case 'X':
			digs = "0123456789ABCDEF";
		case 'x':
			_ulong = UARG();
			base = 16;
			if (flags & ALT && _ulong != 0)
				flags |= HEXPREFIX;
nosign:
			sign = '\0';
number:
			if ((dprec = prec) >= 0)
				flags &= ~ZEROPAD;
			t = buf + BUF;
			if (_ulong != 0 || prec != 0) {
				do {
					*--t = digs[_ulong % base];
					_ulong /= base;
				} while (_ulong);
				digs = "0123456789abcdef";
				if (flags & ALT && base == 8 && *t != '0')
					*--t = '0';
			}
			size = buf + BUF - t;
pforw:
			fieldsz = size + fpprec;
			if (sign)
				fieldsz++;
			if (flags & HEXPREFIX)
				fieldsz += 2;
			realsz = dprec > fieldsz ? dprec : fieldsz;
			if ((flags & (LADJUST | ZEROPAD)) == 0 && width)
				for (n = realsz; n < width; n++)
					PUTC(' ');
			if (sign)
				PUTC(sign);
			if (flags & HEXPREFIX) {
				PUTC('0');
				PUTC((char)*fmt);
			}
			if ((flags & (LADJUST | ZEROPAD)) == ZEROPAD)
				for (n = realsz; n < width; n++)
					PUTC('0');
			for (n = fieldsz; n < dprec; n++)
				PUTC('0');
			n = size;
			while (--n >= 0)
				PUTC(*t++);
			while (--fpprec >= 0)
				PUTC('0');
			if (flags & LADJUST)
				for (n = realsz; n < width; n++)
					PUTC(' ');
			break;
		case '\0':
			*s = '\0';
			return (cnt);
		default:
			PUTC((char)*fmt);
		}
	}
}
