#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *dataBad = NULL;
structSigAtomic *dataGood = NULL;

static void handlerBad(int sig)
{
    if (dataBad != NULL)
    {
        dataBad->val = 2;
    }
}

static void handlerGood(int sig)
{
    if (dataGood != NULL)
    {
        dataGood->val = 2;
    }
}

void testBad()
{
    structSigAtomic *temp = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (temp == NULL) { exit(-1); }
    dataBad = temp;
    dataBad->val = 1;
    
    signal(SIGINT, handlerBad);
    
    free(dataBad);
    dataBad = NULL; // Vulnerable point

    // Trigger the signal handler
    raise(SIGINT);
    
    if (dataBad == NULL)
    {
        printf("Potential race condition executed. dataBad is NULL\n");
    }
}

void testGood()
{
    structSigAtomic *temp = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (temp == NULL) { exit(-1); }
    dataGood = temp;
    dataGood->val = 1;

    signal(SIGINT, SIG_DFL); // Disable the signal handler
    free(dataGood);
    dataGood = NULL; // Safeguard against race condition
    signal(SIGINT, handlerGood); // Restore signal handler
    
    // Trigger the signal handler
    raise(SIGINT);
    
    if (dataGood == NULL)
    {
        printf("Safely handled dataGood is NULL\n");
    }
}

int main()
{
    printf("Starting test for bad practice:\n");
    testBad();
    
    printf("Starting test for good practice:\n");
    testGood();
    
    return 0;
}