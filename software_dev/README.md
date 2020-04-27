# Building

```clang main.c -Wall -g -o test && ./test```

You should expect incomp. pointer type warnings. 

## Create a .ram for our broken SRAM

```python3 -c "print(' '*79)" | cat - sample1.gif > new.ram```

(Offset 80 bytes forward - start at 0x000050)

Data should be 0x20*79 then 0xA0, then GIF