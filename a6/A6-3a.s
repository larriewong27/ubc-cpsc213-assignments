.pos 0x0
                 ld   $sb, r5				# address of the bottom of the stack
                 inca r5					# address of element after the stack
                 gpc  $6, r6                # r6 = pc + 6
                 j    0x300             	# call main
                 halt                     

.pos 0x100
arrPtr:          .long 0x00001000           # arrPtr -> arr

.pos 0x200
foo:             ld   0x0(r5), r0           # r0 = a
                 ld   0x4(r5), r1           # r1 = b
                 ld   $0x100, r2            # r2 = address of arrPtr
                 ld   0x0(r2), r2           # r2 = arrPtr
                 ld   (r2, r1, 4), r3       # r3 = arrPtr[b]
                 add  r3, r0                # r0 = arrPtr[b] + a
                 st   r0, (r2, r1, 4)       # arrPtr[b] += a
                 j    0x0(r6)               # return 

.pos 0x300
main:            ld   $0xfffffff4, r0       # r0 = -12 = size of frame for main
                 add  r0, r5                # allocate frame for main
                 st   r6, 0x8(r5)           # stores return address on stack
                 ld   $0x1, r0              # r0 = 1
                 st   r0, 0x0(r5)           # x = 1 (local in main)
                 ld   $0x2, r0              # r0 = 2
                 st   r0, 0x4(r5)           # y = 2 (local in main)
                 ld   $0xfffffff8, r0       # r0 = -8 = size of frame for foo 
                 add  r0, r5                # allocate frame for foo
                 ld   $0x3, r0              # r0 = 3
                 st   r0, 0x0(r5)           # a = 3 (arg1 in foo)
                 ld   $0x4, r0              # r0 = 4
                 st   r0, 0x4(r5)           # b = 4 (arg2 in foo)
                 gpc  $6, r6                # r6 = pc + 6  
                 j    0x200                 # call foo
                 ld   $0x8, r0              # r0 = 8 = size of frame for foo
                 add  r0, r5                # deallocate frame for foo
                 ld   0x0(r5), r1           # r1 = x = 1
                 ld   0x4(r5), r2           # r2 = y = 2
                 ld   $0xfffffff8, r0       # r0 = -8 = size of frame for foo 
                 add  r0, r5                # allocate frame for foo
                 st   r1, 0x0(r5)           # a = x = 1
                 st   r2, 0x4(r5)           # b = y = 2
                 gpc  $6, r6                # r6 = pc + 6  
                 j    0x200                 # call foo
                 ld   $0x8, r0              # r0 = 8 = size of frame for foo
                 add  r0, r5                # deallocate frame for foo
                 ld   0x8(r5), r6           # load return address from stack
                 ld   $0xc, r0              # r0 = 12 = size of frame for main
                 add  r0, r5                # deallocate frame for main
                 j    0x0(r6)               # return 

.pos 0x1000
arr:             .long 0x00000000			# arr[0]         
                 .long 0x00000000           # arr[1]
                 .long 0x00000000           # arr[2]
                 .long 0x00000000           # arr[3]
                 .long 0x00000000           # arr[4]
                 .long 0x00000000           # arr[5]
                 .long 0x00000000           # arr[6]
                 .long 0x00000000           # arr[7]
                 .long 0x00000000           # arr[8]
                 .long 0x00000000           # arr[9]

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
