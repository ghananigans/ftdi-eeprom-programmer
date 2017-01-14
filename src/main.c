#include <stdio.h>

#ifdef DEBUG
#define DEBUG_PRINT(x) fprintf x
#else
#define DEBUG_PRINT(x) /* DEBUG MODE NOT ENABLED */
#endif

int
main (
    int argc,
    char ** argv
    )
{
    fprintf( stdout, "FTDI Reprogrammer\n" );

    return 0;
}
