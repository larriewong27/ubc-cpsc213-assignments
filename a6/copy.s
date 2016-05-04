.pos 0x100
start:           ld   $stackBtm, r5       # r5 = stack pointer = address of stack bottom
                 gpc  $6, r6              # ra = pc + 6
                 j    copy                # call copy()
                 halt

.pos 0x200
copy:            ld   $-8, r0             # r0 = -8 = -(size of callee frame)
                 add  r0, r5              # allocate copy's frame (callee)
                 st   r6, 0x8(r5)         # save ra on stack
                 ld   $0, r1              # r2 = i = 0
while:           ld   $src, r0            # r0 = &src
                 ld   (r0, r1, 4), r0     # r0 = src[i]
                 beq  r0, end_while       # if src[i]==0 goto end_while
                 st   r0, (r5, r1, 4)     # dst[i] = src[i]
                 inc  r1                  # i++
                 br   while               # goto while
end_while:       ld   $0, r0              # r0 = 0
                 st   r0, (r5, r1, 4)     # dst[i] = 0
                 ld   8(r5), r6           # load ra from stack
                 ld   $8, r0              # r0 = 8 = (size of callee frame)
                 add  r0, r5              # deallocate copy's frame (callee)
                 j    0x0(r6)             # return

.pos 0x1000
stackTop:        .long 0x00000000
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
stackBtm:        .long 0x00000000

.pos 0x2000
src:             .long 0x00000001         # src[0]
                 .long 0x00000001         # src[1]
				 .long 0x00001028         # src[2]
				 .long 0x0000ffff         # src[3]
				 .long 0xffff0100         # src[4]
				 .long 0xffffffff         # src[5]
				 .long 0x0200ffff         # src[6]
				 .long 0xffff0300         # src[7]
				 .long 0xffffffff         # src[8]
				 .long 0x0400ffff         # src[9]
				 .long 0xffff0500         # src[10]
				 .long 0xffffffff         # src[11]
				 .long 0x0600ffff         # src[12]
				 .long 0xffff0700         # src[13]
				 .long 0xffffffff         # src[14]
				 .long 0xf0000000         # src[15]
				 .long 0x00000000         # src[16]