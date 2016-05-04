.pos 0x100
                 ld $0x0, r0            # r0 = temp_i = 0
                 ld $a, r1              # r1 = address of a
                 ld $0x0, r2            # r2 = temp_s = 0
                 ld $5, r3				# r3 = 5
                 not r3					# r3 = ~5
                 inc r3					# r3 = -5
loop:            mov r0, r4             # r4 = temp_i
                 add r3, r4             # r4 = temp_i - 5
                 beq r4, end_loop       # if temp_i = 5 goto end_loop
                 ld (r1, r0, 4), r5     # r5 = a[temp_i]
                 bgt r5, then			# if a[temp_i] > 0 goto then
				 br end_if				# goto end_if
then:			 add r5, r2             # temp_s += a[temp_i]
end_if:          inc r0                 # temp_i++
                 br loop                # goto loop
end_loop:        ld $s, r3              # r3 = address of s
                 st r2, (r3)            # s = temp_s
                 ld $i, r1				# r1 = address of i
                 st r0, (r1)            # i = temp_i
                 halt                     

.pos 0x1000
s:               .long 0x00000000         # s
i:               .long 0x0000000a         # i
a:               .long 0x0000000a         # a[0]
                 .long 0xffffffe2         # a[1]
                 .long 0xfffffff4         # a[2]
                 .long 0x00000004         # a[3]
                 .long 0x00000008         # a[4]