; T1
org $1000
LOOP add r6, r15
jmp LOOP

; T2
org $2000
add @r6, &LOOP

; T3
org $3000
add label1,label2
org $3050
swpb label1(r6)

; T4
org $4000
swpb @r7+

; T5
org $5000
push #$1234

;T6
;org $5100
;byte 450
;swpb label1(label3)

; data section
org $6000
label1 byte 6
label2 byte 7
label3 byte 8
label4 byte 9
end

