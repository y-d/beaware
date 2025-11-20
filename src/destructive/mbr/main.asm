[BITS 16]
[ORG 0x7C00]

%macro sleep 1
    push dx
    mov ah, 86h
    mov cx, %1
    mov dx, 0x6000
    int 15h
    pop dx
%endmacro

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    mov ax, 0x0013
    int 0x10
    mov ax, 0xA000
    mov es, ax

main:
    mov cx, 120
.bars_loop:
    push cx
    mov di, 0
    mov cx, 200
.bars_y:
    push cx
    mov ax, 200
    sub ax, cx
    add ax, [time]
    mov dx, 0
    mov bx, 12
    div bx
    mov cx, 320
.bars_x:
    mov [es:di], al
    inc di
    loop .bars_x
    pop cx
    loop .bars_y
    inc word [time]
    sleep 0x0000
    pop cx
    loop .bars_loop

    mov cx, 120
.plasma_loop:
    push cx
    mov di, 0
    mov cx, 200
.plasma_y:
    push cx
    mov cx, 320
.plasma_x:
    mov ax, cx
    imul ax, [esp+2]
    mov bx, [time]
    shl bx, 1
    add ax, bx
    mov [es:di], al
    inc di
    loop .plasma_x
    pop cx
    loop .plasma_y
    inc word [time]
    sleep 0x0000
    pop cx
    loop .plasma_loop

    mov cx, 120
.fire_loop:
    push cx
    mov dx, 0x3C8
    mov al, 0
    out dx, al
    inc dx
    mov cx, 64
.fire_pal1:
    mov al, 0
    out dx, al
    mov al, 0
    out dx, al  
    mov al, cl
    shl al, 2
    out dx, al
    loop .fire_pal1
    mov cx, 64
.fire_pal2:
    mov al, cl
    shl al, 2
    out dx, al
    mov al, cl
    shl al, 2
    out dx, al
    mov al, 63
    out dx, al
    loop .fire_pal2
    mov di, 320*180
    mov cx, 320*20
.fire_base:
    mov byte [es:di], 100
    inc di
    loop .fire_base
    mov cx, 100
.fire_propagate:
    mov di, 320
    mov bx, 320*180
.fire_calc:
    mov al, [es:di-321]
    add al, [es:di-320]
    adc ah, 0
    add al, [es:di-319]
    adc ah, 0
    add al, [es:di-1]
    adc ah, 0
    shr ax, 2
    mov [es:di-321], al
    inc di
    dec bx
    jnz .fire_calc
    loop .fire_propagate
    sleep 0x0000
    pop cx
    loop .fire_loop

    mov cx, 120
.vortex_loop:
    push cx
    mov di, 0
    mov cx, 200
.vortex_y:
    push cx
    mov cx, 320
.vortex_x:
    push cx
    mov ax, cx
    sub ax, 160
    mov bx, [esp+4]
    sub bx, 100
    push ax
    mov ax, bx
    mov bx, [time]
    add bx, [esp]
    mov [esp], ax
    pop ax
    imul ax, ax
    imul bx, bx
    add ax, bx
    shr ax, 4
    add ax, [time]
    mov [es:di], al
    inc di
    pop cx
    loop .vortex_x
    pop cx
    loop .vortex_y
    inc word [time]
    sleep 0x0000
    pop cx
    loop .vortex_loop

    mov cx, 120
.checker_loop:
    push cx
    mov di, 0
    mov cx, 200
.checker_y:
    push cx
    mov cx, 320
.checker_x:
    mov ax, cx
    add ax, [time]
    xor ax, [esp+2]
    add ax, [time]
    and al, 8
    mov [es:di], al
    inc di
    loop .checker_x
    pop cx
    loop .checker_y
    inc word [time]
    sleep 0x0000
    pop cx
    loop .checker_loop

check_key:
    mov ah, 1
    int 0x16
    jz main
    
    mov cx, 30
.explode_loop:
    push cx
    mov al, 0xB6
    out 0x43, al
    mov ax, cx
    shl ax, 7
    out 0x42, al
    mov al, ah
    out 0x42, al
    in al, 0x61
    or al, 3
    out 0x61, al
    
    mov di, 0
    mov cx, 64000
.explode_pixels:
    mov al, [es:di]
    add al, cl
    mov [es:di], al
    inc di
    loop .explode_pixels
    
    sleep 0x0001
    
    in al, 0x61
    and al, 0xFC
    out 0x61, al
    
    pop cx
    loop .explode_loop
    
    int 0x19

time dw 0

times 510-($-$$) db 0
dw 0xAA55