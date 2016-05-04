.pos 0x100
				ld	$0x10, r0		# r0 = 10
				ld	$a, r1			# r1 = address of a
				st	r0, 0x0(r1)		# a = 10
				ld	$b, r3			# r3 = address of b (which stores address of b[0])
                ld  0x0(r3), r3     # r3 = pointer to array
				ld	$0x4, r4		# r4 = 4
				st	r0, (r3, r4, 4)	# b[4] = 16
				ld	(r3, r4, 4), r2	# r2 = b[4]
				mov r4, r5			# r4 -> r5
				add r0, r5			# r0 + r5
				ld	$0x9, r4		# r4 = 9
				and r4, r0			# r4 & r0 -> r0
				inc r5				# r5 + 1
				inca r0				# r0 + 4
				dec r5				# r5 - 1
				deca r0				# r0 - 4
				ld $0x7, r6			# r6 = 7
				not r6				# ~r6
				shl $0x3, r6		# r6 >> 3
				shl $0xffffffff, r5	# r5 << 2
				nop
				halt

.pos 0x1000
a:               .long 0xffffffff         # a
.pos 0x2000
b:               .long 0x00003000         # address of b[0]; loaded dynamically
.pos 0x3000
b_data:          .long 0xffffffff         # b[0]
                 .long 0xffffffff         # b[1]
                 .long 0xffffffff         # b[2]
                 .long 0xffffffff         # b[3]
                 .long 0xffffffff         # b[4]
                 .long 0xffffffff         # b[5]
                 .long 0xffffffff         # b[6]
                 .long 0xffffffff         # b[7]
                 .long 0xffffffff         # b[8]
                 .long 0xffffffff         # b[9]