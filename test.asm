label1 BYTE 5 ; comment
ALIGN
label2 bss 34
swpb label1
mov label1, label2
mov @r6, r2
words ascii "thisistext"
add words(r2), r4
end
