global disable_int
global enable_int

disable_int:
    cli
    ret

enable_int:
    sti
    ret