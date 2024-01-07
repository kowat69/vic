bits 64
default rel
global main
section .text
main:
push rbp
mov rbp, rsp
push 125255
push 0
push 1516
pop rcx
pop rax
sub rax, rcx
push rax
pop rcx
pop rax
add rax, rcx
push rax
push 64
push 42
pop rcx
pop rax
imul rax, rcx
push rax
pop rcx
pop rax
cqo
idiv rcx
push rax
mov rsp, rbp
pop rbp
ret
