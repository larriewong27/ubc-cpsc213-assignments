.pos 0x100
start:
    ld $sb, r5					# address of the bottom of the stack
    inca    r5					# address of element after the stack
    gpc $6, r6					# r6 = pc + 6
    j main						# call main
    halt

f:
    deca r5						# allocate frame for f
    ld $0, r0					# r0 = 0 = ans
    ld 4(r5), r1				# r1 = a
    ld $0x80000000, r2			# r2 = 0x80000000

f_loop:
    beq r1, f_end				# if(a = 0), goto f_end
    mov r1, r3					# r3 = r1 = a
    and r2, r3					# r3 = 0x80000000 & a
    beq r3, f_if1				# if((0x80000000 & a) = 0), goto f_if1
    inc r0						# r0++ = ans++

f_if1:
    shl $1, r1					# a << 1
    br f_loop					# goto f_loop

f_end:
    inca r5						# deallocate frame for f
    j(r6)						# return

main:
    deca r5						# allocate frame for main
    deca r5						# allocate frame for main
    st r6, 4(r5)				# stores return address on stack
    ld $8, r4					# r4 = 8 = i

main_loop:
    beq r4, main_end			# if(i = 0), goto main_end
    dec r4						# i--
    ld $x, r0					# r0 = address of x
    ld (r0,r4,4), r0			# r0 = x[i]
    deca r5						# allocate frame for stack of f
    st r0, (r5)					# store x[i] to stack of f
    gpc $6, r6					# r6 = pc + 6
    j f							# call f
    inca r5						# deallocate frame for stack of f
    ld $y, r1					# r1 = address of y
    st r0, (r1,r4,4)			# y[i] = f(x[i])
    br main_loop				# goto main_loop

main_end:
    ld 4(r5), r6				# loads return address from stack
    inca r5						# deallocate frame for main
    inca r5						# deallocate frame for main
    j (r6)						# return

.pos 0x2000
x:
    .long 1						# x[0]
    .long 2						# x[1]
    .long 3						# x[2]
    .long 0xffffffff			# x[3]
    .long 0xfffffffe			# x[4]
    .long 0						# x[5]
    .long 184					# x[6]
    .long 340057058				# x[7]

y:
    .long 0						# y[0]
    .long 0						# y[1]
    .long 0						# y[2]
    .long 0						# y[3]
    .long 0						# y[4]
    .long 0						# y[5]
    .long 0						# y[6]
    .long 0						# y[7]

.pos 0x8000
# These are here so you can see (some of) the stack contents.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0

