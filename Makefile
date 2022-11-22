bessel:  bessel_i_standalone.o bessel_k_standalone.o bessel_y_standalone.o bessel_j_standalone.o gamma_cody.o sinpi.o cospi.o mlutils.o bessel.o
	cc -o $@ $? -lm

%.o: %.c
	cc -c $<

.PHONY: clean

clean:
	rm -rf *.o
	rm bessel
