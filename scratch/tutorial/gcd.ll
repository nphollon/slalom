; function prototype
define i32 @gcd(i32 %x, i32 %y) {

; label %entry is the entry point for the function
entry:

; `icmp eq` compares its two operands and true iff they are equal
; eq is one of several possible condition codes
%tmp3 = icmp eq i32 %x, %y

; br has two possible syntaxes
; br i1 <cond>, label <iftrue>, label <iffalse>
;     = <cond> ? goto <iftrue> : goto <iffalse>
; br label <dest>
;     = goto <dest>
br i1 %tmp3, label %return, label %cond_false

return:
ret i32

cond_false:
; ult is unsigned less-than (%x and %y are treated as unsigned ints)
%tmp9 = icmp ult i32 %x, %y
br i1 %tmp9, label %cond_true12, label %cond_false19

cond_true12:
; sub subtracts %x from %y
%tmp15 = sub i32 %y, %x
%tmp17 = call i32 @gcd(i32 %x, i32 %tmp15)
ret i32 %tmp17

cond_false19:
%tmp22 = sub i32 %x, %y
%tmp24 = call i32 @gcd(i32 %tmp22, i32 %y)
ret i32 %tmp24
}