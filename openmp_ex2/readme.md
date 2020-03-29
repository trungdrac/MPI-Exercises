Run:

```
gcc -o mm -fopenmp ./mm.c && ./mm
gcc -o mm_compare ./mm_compare.c && ./mm_compare
gcc -o pi -fopenmp ./pi.c -lm && ./pi
gcc -o pi_compare ./pi_compare.c -lm && ./pi_compare
```