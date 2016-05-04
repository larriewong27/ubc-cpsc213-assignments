.pos 0x1000
code:
				# v = s.x[i]
				ld $v, r0				# r0 = &v
				ld $s, r1				# r1 = &s
				ld $i, r2				# r2 = &i
				ld 0(r2), r2			# r2 = i
				ld (r1, r2, 4), r3		# r3 = s.x[i]
				st r3, 0(r0)			# v = s.x[i]			
				
				# v = s.y[i]
				ld $v, r0				# r0 = &v
				ld $s, r1				# r1 = &s
				ld 8(r1), r1			# r1 = s.y
				ld $i, r2				# r2 = &i
				ld 0(r2), r2			# r2 = i
				ld (r1, r2, 4), r3		# r3 = s.y[i]
				st r3, 0(r0)			# v = s.y[i]
							
				# v = s.z->x[i]
				ld $v, r0				# r0 = &v
				ld $s, r1				# r1 = &s
				ld 12(r1), r1			# r1 = s.z
				ld $i, r2				# r2 = &i
				ld 0(r2), r2			# r2 = i
				ld (r1, r2, 4), r3		# r3 = s.z->x[i]
				st r3, 0(r0)			# v = s.z->x[i]
				
				halt
				
.pos 0x2000
static:
i:    .long 0			# i
v:    .long 0			# v
s:    .long 0			# s.x[0]
      .long 0			# s.x[1]
      .long heap0		# s.y
      .long heap1		# s.z

.pos 0x3000
heap0: 	.long 0			# s.y[0]
 		.long 0			# s.y[1]
heap1:  .long 0			# s.z->x[0]
        .long 0			# s.z->x[1]
		.long 0			# s.z->y
        .long 0			# s.z->z
