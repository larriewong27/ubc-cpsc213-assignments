.pos 0x100

		# c = 5
        ld $c, r0                 # r0 = &c
        ld $5, r1                 # r1 = 5
        st r1, 0(r0)              # c = 5
        
        # b = c + 10
        ld $10, r2                # r2 = 10
        add r2, r1                # r1 = c + 10
        ld $b, r3                 # r3 = &b
        st r1, 0(r3)              # b = c + 10
        
        # a[8] = 8
        ld $a, r0				  # r0 = &a
        ld $8, r1				  # r1 = 8
        st r1, (r0, r1, 4)		  # a[8] = 8
        
		# a[4] = a[4] + 4
		ld $4, r1				  # r1 = 4
		ld (r0, r1, 4), r2        # r2 = a[4]
		add r1, r2                # r2 = a[4] + 4
		st r2, (r0, r1, 4)        # a[4] = a[4] + 4
		
		# a[c] = a[8] + b + a[b & 0x7]
		ld 0x20(r0), r1		      # r1 = a[8]
		ld $b, r2				  # r2 = &b
		ld 0(r2), r3			  # r3 = b
		add r3, r1				  # r1 = a[8] + b
		ld $0x7, r4				  # r4 = 0x7
		and r4, r3				  # r3 = b & 0x7
		ld (r0, r3, 4), r5		  # r5 = a[b & 0x7]
		add r5, r1				  # r1 = a[8] + b + a[b & 0x7]
		ld $c, r6				  # r6 = &c
		ld 0(r6), r7 			  # r7 = c
		st r1, (r0, r7, 4)		  # a[c] = a[8] + b + a[b & 0x7]
		
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
    .long 0             # a[8]
    .long 0             # a[9]
b:  .long 0             # b
c:  .long 0             # c
