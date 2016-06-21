;T1
org $1000
LOOP add r6, r15
jmp LOOP

;T2
org $2000
add @r6,&LOOP

;T3
org $3000
add label1, label2
org $3050
swpb label1(r6)

;T4
org $4000
swpb @r7+

;T5
org $5000
push #$1234

;T6
;org $5100
;byte 450
;swpb label1(label3)

;T7
org $5200
string ascii "text"
align

;T8
org $5250
wrd word 1234
wrd WORD $1234
org $5275
swpb &wrd
equate equ $1204
swpb &equate

org $6000
label1 byte 6
label2 byte 7
label3 byte 8
label4 byte 9

end
