_DATA SEGMENT 


ALIGN 8



_DATA ENDS


_TEXT SEGMENT 
__global __myvnniloop

__myvnniloop PROC
l1:
    db 0c4h
    db 0e2h
    db 07dh 
    db 050h
    db 0d1h

    db 0c4h
    db 0e2h
    db 07dh 
    db 050h
    db 0d9h

    db 0c4h
    db 0e2h
    db 07dh 
    db 050h
    db 0e1h

    db 0c4h
    db 0e2h
    db 07dh 
    db 050h
    db 0e9h

    db 0c4h
    db 0e2h
    db 07dh 
    db 050h
    db 0f1h

    db 0c4h
    db 0e2h
    db 07dh 
    db 050h
    db 0f9h

    db 0c4h
    db 062h
    db 07dh 
    db 050h
    db 0c1h

    db 0c4h
    db 062h
    db 07dh 
    db 050h
    db 0c9h

    db 0c4h
    db 062h
    db 07dh 
    db 050h
    db 0d1h

    db 0c4h
    db 062h
    db 07dh 
    db 050h
    db 0d9h

    db 0c4h
    db 062h
    db 07dh 
    db 050h
    db 0e1h

    db 0c4h
    db 062h
    db 07dh 
    db 050h
    db 0e9h

    jmp l1
    ret
__myvnniloop ENDP

_TEXT ENDS
END
