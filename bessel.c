#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <regex.h>
#include <float.h>


double bessel_i(double x, double alpha, bool expon_scaled);
double bessel_j(double x, double alpha);
double bessel_k(double x, double alpha, bool expon_scaled);
double bessel_y(double x, double alpha);


typedef double (*fn_3_arg)(double x, double alpha, bool scale);
typedef double (*fn_2_arg)(double x, double alpha);


static char const * STR_TRUE = "TRUE";
static char const * STR_FALSE = "FALSE";

bool partiallyComp(char *truthy, char const * dict){
    int len = strlen(truthy);
    for (int s = 0; s < strlen(truthy) && s < strlen(dict); s++){
        if (
            toupper(dict[s]) != toupper(truthy[s])
           ){
            return false;
        }
    }
    return true;
}

bool isTrue(char *thruthy){
    return partiallyComp(thruthy, STR_TRUE);
}

bool isFalse(char *thruthy){
    return partiallyComp(thruthy, STR_FALSE);
}

char getOption(char *str){
    if (str == NULL){
        return 0;
    }
    if (!(str[0] == '-' && (str[2] == '=' || str[2] == 0))){
        return 0;
    }
    return str[1];
}

char *getOptionArgument(char *str){
    if (!getOption(str)){
        return NULL;
    }
    if (str[2] != '='){
        return NULL;
    }
    return str+3;
}

 bool parseScaleArgument(int nrArgs, char **cli, bool * exponentScale){
    if (nrArgs < 1){
        fprintf(stderr, "number of arguments for bessel is incorrect, nrArgs=[%d]\n", nrArgs + 3);
        return false;
    }
    if (getOption(cli[0]) != 's'){
        fprintf(stderr, "-s option not found\n");
        return false;
    }
    char * scaleArgument = getOptionArgument(cli[0]);

    
    if (!isTrue(scaleArgument) && !isFalse(scaleArgument)){
        fprintf(stderr, "-s argument should have value \"true\" or \"false\", instead it is [%s]\n", scaleArgument);
        return false;
    }
    *exponentScale = isTrue(scaleArgument) ? true : false;
    //printf("scaleArgument = [%d]\n", *exponentScale);
    return true;
 }
 
// xStart xStop Count are optionArguments to option -x
// alpha is argument to option 
bool parse2Arguments(int nrArgs, char **cli, double *xStart, double *xStop, unsigned long int *count, double *alpha){
    char xstartStringData[150];
    char * xstartStringEndPointer;
    char xstopStringData[150];
    char * xstopStringEndPointer;

    // count, number of steps
    char xCountStringData[150];
    char *xCountStringEndPointer;

    // alpha parameter
    char alphaStringData[150];
    char * alphaStringEndPointer;

    // nrArgs
    if (nrArgs < 2){
        fprintf(stderr, "number of arguments for bessel is incorrect, nrArgs=[%d]\n", nrArgs + 1);
        return false;
    }

    //
    if ( getOption(cli[0]) != 'x') {
        fprintf(stderr, "-x argument option not found\n");
        return false;
    }

    // parse -x option argument

    char * xArguments = getOptionArgument(cli[0]);
    
    regex_t regex;
    
    int reti = regcomp(&regex, "^([^,]+)(,([^,]+))?(,([^,]+))?$", REG_ICASE|REG_EXTENDED);

    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return -10;
    }

    regmatch_t pmatch[6];    

    int status = regexec(&regex, xArguments, 6, pmatch, 0);
    regfree(&regex);

   /*printf("match in pmatch is [0].rm_eo=%d, [0].rm_so=%d\n", pmatch[0].rm_eo, pmatch[0].rm_so);
    printf("match in pmatch is [1].rm_eo=%d, [1].rm_so=%i\n", pmatch[1].rm_eo, pmatch[1].rm_so);
    printf("match in pmatch is [2].rm_eo=%d, [2].rm_so=%d\n", pmatch[2].rm_eo, pmatch[2].rm_so);
    printf("match in pmatch is [3].rm_eo=%d, [3].rm_so=%d\n", pmatch[3].rm_eo, pmatch[3].rm_so);
    printf("match in pmatch is [4].rm_eo=%d, [4].rm_so=%d\n", pmatch[4].rm_eo, pmatch[4].rm_so);
    printf("match in pmatch is [5].rm_eo=%d, [5].rm_so=%d\n", pmatch[5].rm_eo, pmatch[5].rm_so);
    */
    /*
    ./bessel -j -x=4578,4,5999 -a=0
      gives this match
        match in pmatch is [0].rm_eo=11, [0].rm_so=0
        match in pmatch is [1].rm_eo=4, [1].rm_so=0
        match in pmatch is [2].rm_eo=6, [2].rm_so=4
        match in pmatch is [3].rm_eo=6, [3].rm_so=5
        match in pmatch is [4].rm_eo=11, [4].rm_so=6
        match in pmatch is [5].rm_eo=11, [5].rm_so=7
    */
    
    if (status) {
        char errorBuffer[150];
        regerror(status, &regex, errorBuffer, 150); 
        fprintf(stderr, "[debug] regexec status = [%d], [%s]\n", status, errorBuffer);
        fprintf(stderr, "Wrong input for -x option, should be -x=start,stop,nr_of_partitions\n");
        return -11;
    }
    
    // -x should always have at least one argument
    int lenXStart = pmatch[1].rm_eo-pmatch[1].rm_so;
    memcpy(xstartStringData, xArguments, lenXStart);//+pmatch[1].rm_so, pmatch[1].rm_eo-pmatch[1].rm_so);
    xstartStringData[lenXStart] = 0;
    *xStart = strtod(xstartStringData, &xstartStringEndPointer);
    //printf("xstart=[%.23lf]\n", *xStart);

   
    *xStop = *xStart;
    // -x optional has a xstop
    if (pmatch[3].rm_eo > 0) {
        int lenXStop =  pmatch[3].rm_eo-pmatch[3].rm_so;
        memcpy(xstopStringData, xArguments + pmatch[3].rm_so, lenXStop);
        xstopStringData[lenXStop] = 0;
        *xStop = strtod(xstopStringData, &xstopStringEndPointer);
        //printf("xstop=[%.23lf]\n", *xStop);
    }


    
    *count = 0;
    // -x optional has a count (number of devisions between xstart and xstop)
    if ( pmatch[5].rm_eo > 0){
        int lenXCount = pmatch[5].rm_eo-pmatch[5].rm_so;
        memcpy(xCountStringData, xArguments + pmatch[5].rm_so, lenXCount);
        xCountStringData[lenXCount] = 0;
        //printf("counts=[%s]\n", xCountStringData);
        *count = strtoul(xCountStringData, 0, 10);
        // printf("count=[%u]\n", *count);
    }
  
    if (*count == 0L){
        *count = 1;
    }

    if (*xStop < *xStart){
        fprintf(stderr, "Bessel cannot have an [xStop]=[%.23lf] smaller then [xStart]=[%.23lf]\n", *xStop, *xStart);
        return false;
    }

    // check if count will create an underflow
    if ( (*xStop - *xStart) < DBL_EPSILON*(*count) && (*count) > 1) {
        fprintf(stderr, "Bessel cannot have an [count]=[%lu] slicing domain [xStart, xStop]/count, in parts smaller then EPSILON (%.23lf)", *count, DBL_EPSILON);
        return false;
    }
    // parse -a option argument

    if (getOption(cli[1]) != 'a'){
         fprintf(stderr, "-a argument option not found\n");
         return false;
    }

    char * alphaArguments = getOptionArgument(cli[1]);
    //printf("alpha=[%s]\n",alphaArguments);
    *alpha = strtod(alphaArguments, &alphaStringEndPointer);
    // printf("alpha=[%.23lf]\n", *alpha);
    return true;
}

double calcBessel(char type, double x, double alpha, bool expScale){

    fn_2_arg fn_2 = NULL; 

    fn_3_arg fn_3 = NULL;

    switch(type){
        case 'i':
        case 'k':
            fn_3 = type == 'i' ? bessel_i : bessel_k;
            return fn_3(x, alpha, expScale);
        case 'y':
        case 'j':
            fn_2 = type == 'y' ? bessel_y : bessel_j;
            return fn_2(x, alpha);
    }
}

void printBesselData(int idx, double answ, double x, double alpha, bool exponentScale){
     printf("%d,%.22e,%.22e,%c,%.22e\n", idx, x, alpha, exponentScale ? 't':'f', answ);
}

int main(int argc, char **argv) {

    fn_2_arg fn_2 = NULL;

    fn_3_arg fn_3 = NULL;

    if (!(argc == 4 || argc == 5)) {
        fprintf(stderr, "Wrong number of arguments\n");
        fflush(stderr);
        return -1;
    }
    
    /**
        int const * x;       // pointer to constant int   (int const) * x
        int * const x;       // constant pointer to int   int (* const x)
        int const * const x; // constant pointer to constant int  (int const) (* const x)
    */

    // argv[1] must be 
    // 1 "-j" besselJ, | -j -x=0,100,5 -a=0.23           |3+1
    // 2 "-i" besselI  | -i -x=0,100,5 -a=0.23 -s=false  |4+1
    // 3 "-y" besselY  | -y -x=0,100,5 -a=0.23           |3+1
    // 4 "-k" besselK  | -k -x=0,100,5 -a=0.23 -s=true   |4+1

    char besselType = getOption(argv[1]);
    if (!besselType){
        fprintf(stderr, "the first argument must be an option -j, -i, -y, -k\n");
        return -2;
    }

    double xstart;
    double xstop;
    unsigned long int count;
    double alpha;
    bool  exponentScale = 0;

    switch(besselType){
        case 'i':
        case 'k':
            // -1 (for argv[0]), -1 for B. option, -1 (for x) -1 for alpha
            if (!parseScaleArgument(argc - 4, argv + 4, &exponentScale)){
              return -6;
            };
        case 'y':
        case 'j':
           // -1 (for argv[0]), -1 for B. option, (0 for exponentScale optional, so does not count)
           // printf("option y or j selected\n");
           if (!parse2Arguments(argc - 2, argv + 2, &xstart, &xstop, &count, &alpha)){
              return -4;
           }
           break;
        default:
            fprintf(stderr, "first option should be the one of -i,-j,-y,-k\n");
            return -3;
    }
    // printf("ExponentScale=%d, xstart=%f\n", exponentScale, xstart);
    // now we have everything we need
    double answ;

    // always print first point
    answ = calcBessel(besselType, xstart, alpha, exponentScale);
    printBesselData(1, answ, xstart, alpha, exponentScale);
    
    // 1. if xStart < xStop within EPSILON then xStart = xStop its only one pointer
        //
    if ( abs(xstop - xstart) <= DBL_EPSILON ){
        return 0;
    }
    // calculate xstart and xstop is different, if count == 1 then only do xstart and xstop
    if (count == 1){
        answ = calcBessel(besselType, xstop, alpha, exponentScale);
        printBesselData(2, answ, xstop, alpha, exponentScale);
        return 0;
    }
    //
    // if count = 1
    //  |---------------|
    // start           stop 
    
    // if count = 2 (simular if count > 2)
    //  |-------|-------|
    // start           stop 
    //
    const double const partitionSize = ( xstop - xstart ) / count;
    unsigned long int i = 1;
    while (i < count) {
        double x = xstart + partitionSize * i;
        answ = calcBessel(besselType, x, alpha, exponentScale);
        printBesselData(i+1, answ, x, alpha, exponentScale);
        i++;
    }
    answ = calcBessel(besselType, xstop, alpha, exponentScale);
    printBesselData(count + 1, answ, xstop, alpha, exponentScale);
    return 0;
}
