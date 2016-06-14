# Test Results - test.asm

## T1
> Type 2,3
> reg-dir

S1-07-1000-0F56-FE3F-46

## T2
> type 2
> reg-indir, absolute

S1-07-2000-A256-0010-D0

## T3
> type 1,2
> relative, indexed
S1-09-3000-9050-0060-0160-25
S1-07-3050-9110-0060-77

## T4
> type 1
> reg-indir-aa

S1-05-4000-B710-F3

## T5
> type 1
> immediate

S1-07-3012-3412-20

## T6
> Errors:
> BYTE a value greater than 1 byte 
> indexed addressing with a register

## Data Section
S1-07-6000-06-07-08-09-7A
