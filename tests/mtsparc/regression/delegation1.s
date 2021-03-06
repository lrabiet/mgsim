/*
 This test does a simple delegation test.
 It creates a family of 4 threads to the next ring, using a global and a shared.
 All threads use the shared and global.
 */
    .file "delegation1.s"
    .text

    .align 64
    .globl main
main:
    
    mov (4 << 1) | 4, %3
    allocates %3         ! PID:4, Size:4
    setlimit %3, 4
    set bar, %1
    crei %1, %3

    mov 42, %1
    mov  2, %2
    putg %1, %3, 0
    puts %2, %3, 0
    
    ! Sync
    sync %3, %1
    release %3
    mov %1, %0
    end

    .align 64
    .registers 1 1 1 0 0 0
bar:
    add %td0, %tg0, %tl0
    swch
    mov %tl0, %ts0
    print %tl0, 0
    end

    .data
    .ascii "PLACES: 16\0"
        
