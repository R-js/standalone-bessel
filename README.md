# standalone-bessel
Dissected Bessel functions from R-base as a command line executable (besselK, besselJ, besselI, besselY)


## Compiling

```bash
cd <project-dir>
make
```

There will be a `bessel` executable in the project dir, you can copy this to `/usr/local/bin` or wherever you fancy
It is fully self contained executable


## Usage 

We show command equivalent and between R and `bessel` command line executable


### For BesselI

R example:

```R
x <- seq(0,1,0.1)
alpha <- 1.5
y <- besselI(x, alpha, TRUE)
```

Equivalent `bessel` command line example

- the -i option selects the `besselI` function_
- the -x option arguments works like the R sequence `seq(start, end , step)`
    - -x=start,stop,step
- the -a option argument is the `alpha` argument
- the -s option argument (true or false) is to scale the answer exponentially to avoid overflow or underflow    

```bash
./bessel -i -x=0,1,0.1 -a=1.5 -s=TRUE
# -> output to stdout

#row number, x,                      alpha,                  scale besselI(x,alpha, scale)

#1,0.0000000000000000000000e+00,1.5000000000000000000000e+00,t,0.0000000000000000000000e+00
#2,1.0000000000000000555112e-01,1.5000000000000000000000e+00,t,7.6176951894028292061600e-03
#3,2.0000000000000001110223e-01,1.5000000000000000000000e+00,t,1.9554246712917208728122e-02
#4,3.0000000000000004440892e-01,1.5000000000000000000000e+00,t,3.2667506227343073854641e-02
#5,4.0000000000000002220446e-01,1.5000000000000000000000e+00,t,4.5827261386007379917462e-02
#6,5.0000000000000000000000e-01,1.5000000000000000000000e+00,t,5.8471662583135769475362e-02
#7,5.9999999999999997779554e-01,1.5000000000000000000000e+00,t,7.0311123038549097619132e-02
#8,6.9999999999999995559108e-01,1.5000000000000000000000e+00,t,8.1206972794260576242742e-02
#9,7.9999999999999993338662e-01,1.5000000000000000000000e+00,t,9.1109494021316142498002e-02
#10,8.9999999999999991118216e-01,1.5000000000000000000000e+00,t,1.0002252762532191920908e-01
#11,9.9999999999999988897770e-01,1.5000000000000000000000e+00,t,1.0798193302637608403938e-01
```


### For BesselJ

R example:

```R
x <- seq(-1,1,0.2)
alpha <- 1.5
y <- besselJ(x, alpha);

data.frame(x,alpha, y);
# //->
#      x alpha           y
#1  -1.0   1.5        NaN
#2  -0.8   1.5        NaN
#3  -0.6   1.5        NaN
#4  -0.4   1.5        NaN
#5  -0.2   1.5        NaN
#6   0.0   1.5 0.00000000
#7   0.2   1.5 0.02369330
#8   0.4   1.5 0.06621313
#9   0.6   1.5 0.11921470
#10  0.8   1.5 0.17840207
#11  1.0   1.5 0.24029784
```

Equivalent `bessel` command line example

- the -j option selects the `besselJ` function_
- the -x option arguments works like the R sequence `seq(start, end , step)`
    - -x=start,stop,step
- the -a option argument is the `alpha` argument

```bash

./bessel -j -x=-1,1,0.2 -a=1.5

# //-> stdout
# nr,  x,                        alpha                        y=besselJ(x, alpha)
# 1,-1.0000000000000000000000e+00,1.5000000000000000000000e+00,nan
# 2,-8.0000000000000004440892e-01,1.5000000000000000000000e+00,nan
# 3,-6.0000000000000008881784e-01,1.5000000000000000000000e+00,nan
# 4,-4.0000000000000007771561e-01,1.5000000000000000000000e+00,nan
# 5,-2.0000000000000006661338e-01,1.5000000000000000000000e+00,nan
# 6,-5.5511151231257827021182e-17,1.5000000000000000000000e+00,nan
# 7,1.9999999999999995559108e-01,1.5000000000000000000000e+00,2.3693304095129236186423e-02
# 8,3.9999999999999996669331e-01,1.5000000000000000000000e+00,6.6213130821221669042309e-02
# 9,5.9999999999999997779554e-01,1.5000000000000000000000e+00,1.1921469870548732916671e-01
# 10,8.0000000000000004440892e-01,1.5000000000000000000000e+00,1.7840206751096521209377e-01
# 11,1.0000000000000000000000e+00,1.5000000000000000000000e+00,2.4029783912342697616005e-01
```

### For BesselK


R example:

```R
x <- seq(-1,1,0.2);
alpha <- 2.5;
y <- besselK(x, alpha, FALSE);

data.frame(x,alpha, y);
# -->
#      x alpha         y
#1  -1.0   2.5       NaN
#2  -0.8   2.5       NaN
#3  -0.6   2.5       NaN
#4  -0.4   2.5       NaN
#5  -0.2   2.5       NaN
#6   0.0   2.5       Inf
#7   0.2   2.5 208.79853
#8   0.4   2.5  36.19751
#9   0.6   2.5  12.72784
#10  0.8   2.5   5.94205
#11  1.0   2.5   3.22748
```

Equivalent `bessel` command line example

- the -k option selects the `besselK` function_
- the -x option arguments works like the R sequence `seq(start, end , step)`
    - -x=start,stop,step
- the -a option argument is the `alpha` argument
- the -s option argument (true or false) is to scale the answer exponentially to avoid overflow or underflow    


```bash

./bessel -k -x=-1,1,0.2 -a=2.5 -s=false

# //-> 
# nr, x,                        , alpha                     scale, bessel(x,alpha, scale)
#1,-1.0000000000000000000000e+00,2.5000000000000000000000e+00,f,nan
#2,-8.0000000000000004440892e-01,2.5000000000000000000000e+00,f,nan
#3,-6.0000000000000008881784e-01,2.5000000000000000000000e+00,f,nan
#4,-4.0000000000000007771561e-01,2.5000000000000000000000e+00,f,nan
#5,-2.0000000000000006661338e-01,2.5000000000000000000000e+00,f,nan
#6,-5.5511151231257827021182e-17,2.5000000000000000000000e+00,f,nan
#7,1.9999999999999995559108e-01,2.5000000000000000000000e+00,f,2.0879852992166129865836e+02
#8,3.9999999999999996669331e-01,2.5000000000000000000000e+00,f,3.6197506660978362447167e+01
#9,5.9999999999999997779554e-01,2.5000000000000000000000e+00,f,1.2727843453151882258112e+01
#10,8.0000000000000004440892e-01,2.5000000000000000000000e+00,f,5.9420503042137688964885e+00
#11,1.0000000000000000000000e+00,2.5000000000000000000000e+00,f,3.2274795311352622029233e+00
```

### For BesselY

R example:

```R
x <- seq(-1,1,0.2)
alpha <- 0.8
y <- besselY(x, alpha);
data.frame(x,alpha, y);
#      x alpha          y
#1  -1.0   0.8        NaN
#2  -0.8   0.8        NaN
#3  -0.6   0.8        NaN
#4  -0.4   0.8        NaN
#5  -0.2   0.8        NaN
#6   0.0   0.8       -Inf
#7   0.2   0.8 -2.4547267
#8   0.4   0.8 -1.4776122
#9   0.6   0.8 -1.0864635
#10  0.8   0.8 -0.8430858
#11  1.0   0.8 -0.6556140
```

Equivalent `bessel` command line example

- the -y option selects the `besselY` function_
- the -x option arguments works like the R sequence `seq(start, end , step)`
    - -x=start,stop,step
- the -a option argument is the `alpha` argument

```bash

./bessel -y -x=-1,1,0.2 -a=0.8 

# //-> 
# nr,  x,                        alpha                        y=besselY(x, alpha)
#1,-1.0000000000000000000000e+00,8.0000000000000004440892e-01,nan
#2,-8.0000000000000004440892e-01,8.0000000000000004440892e-01,nan
#3,-6.0000000000000008881784e-01,8.0000000000000004440892e-01,nan
#4,-4.0000000000000007771561e-01,8.0000000000000004440892e-01,nan
#5,-2.0000000000000006661338e-01,8.0000000000000004440892e-01,nan
#6,-5.5511151231257827021182e-17,8.0000000000000004440892e-01,nan
#7,1.9999999999999995559108e-01,8.0000000000000004440892e-01,-2.4547267285997373953421e+00
#8,3.9999999999999996669331e-01,8.0000000000000004440892e-01,-1.4776121978062599637838e+00
#9,5.9999999999999997779554e-01,8.0000000000000004440892e-01,-1.0864634678296238856632e+00
#10,8.0000000000000004440892e-01,8.0000000000000004440892e-01,-8.4308583260933500636014e-01
#11,1.0000000000000000000000e+00,8.0000000000000004440892e-01,-6.5561396353696488326079e-01
```
