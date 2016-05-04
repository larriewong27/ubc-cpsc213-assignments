.pos 0x100
		# a[i] = a[i+1] + b[i+2]
		ld $a, r0				# r0 = &a
		ld $b, r1				# r1 = &b
		ld $i, r2				# r2 = &i
		ld 0(r2), r3			# r3 = i
		ld $1, r4				# r4 = 1
		add r3, r4				# r4 = i + 1
		ld (r0, r4, 4), r5		# r5 = a[i + 1]
		ld $2, r6				# r6 = 2
		add r3, r6				# r6 = i + 2
		ld (r1, r6, 4), r7		# r7 = b[i + 2]
		add r5, r7				# r7 = a[i + 1] + b[i + 2]
		st r7, (r0, r3, 4)		# a[i] = a[i+1] + b[i+2]
		
		# d[i] = a[i] + b[i]
		ld $a, r0				# r0 = &a
		ld $b, r1				# r1 = &b
		ld $d, r2				# r2 = &d
		ld 0(r2), r2			# r2 = d (which is an address of c)
		ld $i, r3				# r3 = &i
		ld 0(r3), r4			# r4 = i
		ld (r0, r4, 4), r5		# r5 = a[i]
		ld (r1, r4, 4), r6		# r6 = b[i]
		add r5, r6				# r6 = a[i] + b[i]
		st r6, (r2, r4, 4)		# d[i] = a[i] + b[i]
		
		# d[i] = a[b[i]] + b[a[i]]
		ld $a, r0				# r0 = &a
		ld $b, r1				# r1 = &b
		ld $d, r2				# r2 = &d
		ld 0(r2), r2			# r2 = d (which is an address of c) 
		ld $i, r3				# r3 = &i
		ld 0(r3), r3			# r3 = i
		ld (r1, r3, 4), r4		# r4 = b[i]
		ld (r0, r3, 4), r5		# r5 = a[i]
		ld (r0, r4, 4), r4		# r4 = a[b[i]]
		ld (r1, r5, 4), r5		# r5 = b[a[i]]
		add r4, r5				# r5 = a[b[i]] + b[a[i]]
		st r5, (r2, r3, 4)		# d[i] = a[b[i]] + b[a[i]]
		
		# d[b[i]] = b[a[i & 3] & 3] - a[b[i & 3] & 3] + d[i]
		ld $a, r0				# r0 = &a
		ld $b, r1				# r1 = &b
		ld $d, r2				# r2 = &d
		ld 0(r2), r2			# r2 = d (which is an address of c) 
		ld $i, r3				# r3 = &i
		ld 0(r3), r3			# r3 = i
		ld $3, r4				# r4 = 3
		mov r4, r5				# r5 = 3
		and r3, r5				# r5 = i & 3
		ld (r0, r5, 4), r6		# r6 = a[i & 3]
		and r4, r6				# r6 = a[i & 3] & 3
		ld (r1, r6, 4), r6		# r6 = b[a[i & 3] & 3]
		ld (r1, r5, 4), r7		# r7 = b[i & 3]
		and r4, r7				# r7 = b[i & 3] & 3
		ld (r0, r7, 4), r7		# r7 = a[b[i & 3] & 3]
		not r7					# r7 = ~(a[b[i & 3] & 3])
		inc r7					# r7 = -(a[b[i & 3] & 3])
		add r6, r7				# r7 = b[a[i & 3] & 3] - a[b[i & 3] & 3]
		ld (r2, r3, 4), r6		# r6 = d[i]
		add r6, r7				# r7 = b[a[i & 3] & 3] - a[b[i & 3] & 3] + d[i]
		ld (r1, r3, 4), r6		# r6 = b[i]
		st r7, (r2, r6, 4)		# d[b[i]] = b[a[i & 3] & 3] - a[b[i & 3] & 3] + d[i]
		
		halt
		
.pos 0x200
# Data area

a:  .long 0             # a[0]
    .long 0             # a[1]
    .long 0             # a[2]
    .long 0             # a[3]
    .long 0             # a[4]
    .long 0             # a[5]
    .long 0             # a[6]
    .long 0             # a[7]
b:  .long 0             # b[0]
    .long 0             # b[1]
    .long 0             # b[2]
    .long 0             # b[3]
    .long 0             # b[4]
    .long 0             # b[5]
    .long 0             # b[6]
    .long 0             # b[7]
c:  .long 0             # c[0]
    .long 0             # c[1]
    .long 0             # c[2]
    .long 0             # c[3]
    .long 0             # c[4]
    .long 0             # c[5]
    .long 0             # c[6]
    .long 0             # c[7]
i:  .long 0             # i
d:  .long c             # d
		