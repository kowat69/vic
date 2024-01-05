bits 64
default rel
global main
section .text
main:
push rbp
mov rbp, rsp
push 125255
push 1516
pop rcx
pop rax
add rax, rcx
push rax
push 63
pop rcx
pop rax
cqo
idiv rcx
push rax
push 63
pop rcx
pop rax
imul rax, rcx
push rax
mov rsp, rbp
pop rbp
ret
