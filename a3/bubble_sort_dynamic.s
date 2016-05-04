.pos 0x100

		# t = val[i]
		ld $t, r0				# r0 = &t
		ld $i, r1				# r1 = &i
		ld 0(r1), r1			# r1 = i
		ld $val, r2				# r2 = &val
		ld 0(r2), r2			# r2 = val
		ld (r2,r1,4), r3		# r3 = val[i]
		st r3, 0(r0)			# t = val[i]
		
		# val[i] = val[j]
		ld $val, r0				# r0 = &val
		ld 0(r0), r0			# r0 = val
		ld $i, r1				# r1 = &i
		ld 0(r1), r1			# r1 = i
		ld $j, r2				# r2 = &j
		ld 0(r2), r2			# r2 = j
		ld (r0, r2, 4), r3		# r3 = val[j]
		st r3, (r0, r1, 4)		# val[i] = val[j]
		
		# val[j] = t
		ld $val, r0				# r0 = &val
		ld 0(r0), r0			# r0 = val
		ld $j, r1				# r1 = &j
		ld 0(r1), r1			# r1 = j
		ld $t, r2				# r2 = &t
		ld 0(r2), r2			# r2 = t
		st r2, (r0, r1, 4)		# val[j] = t
		
		halt

.pos 0x200
# Data area

	i:  .long 0x3             # i
	j:  .long 0x2             # j
	t:	.long 0				  # t
val:	.long 0               # val[0]
		.long 0               # val[1]
		.long 0x7             # val[2]
		.long 0x9             # val[3]