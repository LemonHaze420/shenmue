

#ifdef L_sdivsi3_i4
	.global	___sdivsi3_i4
___sdivsi3_i4:
	sts.l	fpscr,@-r15
	mov.l	r0,@-r15
	mov	#8,r2
	swap.w	r2,r2
	sts	fpscr,r0
	or	r2,r0
	add	r2,r2
	not	r2,r2
	and	r2,r0
	lds	r0,fpscr
	lds	r4,fpul
	float	fpul,dr0
	lds	r5,fpul
	float	fpul,dr2
	fdiv	dr2,dr0
	mov.l	@r15+,r0
	ftrc	dr0,fpul
	rts
	lds.l	@r15+,fpscr
#endif

#ifdef L_udivsi3_i4
	.global	___udivsi3_i4
___udivsi3_i4:
	mov	#1,r1
	cmp/hi	r1,r5
	bf	trivial
	sts.l	fpscr,@-r15
	mov.l	r2,@-r15
	mov	#8,r2
	swap.w	r2,r2
	sts	fpscr,r0
	or	r2,r0
	shll	r2
	not	r2,r2
	and	r2,r0
	lds	r0,fpscr
	mova	L1,r0
	rotr	r1
	xor	r1,r4
	lds	r4,fpul
	fmov.s	@r0+,fr5
	fmov.s	@r0,fr4
	float	fpul,dr0
	xor	r1,r5
	lds	r5,fpul
	float	fpul,dr2
	fadd	dr4,dr0
	fadd	dr4,dr2
	fdiv	dr2,dr0
	mov.l	@r15+,r2
	ftrc	dr0,fpul
	rts
	lds.l	@r15+,fpscr
trivial:
	rts
	lds	r4,fpul

	.align	2
L1:
	.double	2147483648
#endif
