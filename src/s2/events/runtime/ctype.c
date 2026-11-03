 
#include <ctype.h>

#undef isalpha
#undef isupper
#undef islower
#undef isdigit
#undef isalnum
#undef iscntrl
#undef isgraph
#undef isprint
#undef ispunct
#undef isspace
#undef isxdigit
#undef isascii
#undef toascii
#undef tolower
#undef toupper
#undef _tolower
#undef _toupper

int
isalpha(c)
	int c;
{
	return (_ctype_ + 1)[c] & (_U | _L);
}

int
isupper(c)
	int c;
{
	return (_ctype_ + 1)[c] & _U;
}

int
islower(c)
	int c;
{
	return (_ctype_ + 1)[c] & _L;
}

int
isdigit(c)
	int c;
{
	return (_ctype_ + 1)[c] & _N;
}

int
isalnum(c)
	int c;
{
	return (_ctype_ + 1)[c] & (_U | _L | _N);
}

int
iscntrl(c)
	int c;
{
	return (_ctype_ + 1)[c] & _C;
}

int
isgraph(c)
	int c;
{
	return (_ctype_ + 1)[c] & (_P | _U | _L | _N);
}

int
isprint(c)
	int c;
{
	return (_ctype_ + 1)[c] & (_P | _U | _L | _N | _B);
}

int
ispunct(c)
	int c;
{
	return (_ctype_ + 1)[c] & _P;
}

int
isspace(c)
	int c;
{
	return (_ctype_ + 1)[c] & _S;
}

int
isxdigit(c)
	int c;
{
	return (_ctype_ + 1)[c] & (_X | _N);
}

int
isascii(c)
	int c;
{
	return (unsigned)c <= 0177;
}

int
toascii(c)
	int c;
{
	return c & 0177;
}

int
tolower(c)
	int c;
{
	return isupper(c) ? c - 'A' + 'a' : c;
}

int
toupper(c)
	int c;
{
	return islower(c) ? c - 'a' + 'A' : c;
}

int
_tolower(c)
	int c;
{
	return isupper(c) ? c - 'A' + 'a' : c;
}

int
_toupper(c)
	int c;
{
	return islower(c) ? c - 'a' + 'A' : c;
}
