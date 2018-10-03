#include <stdio.h>
#include <stdlib.h>

#include "noun.h"
#include "nock.h"

int32_t main(int32_t argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Please provide noun.\n");
        return 0;
    }
    else if(argc > 2)
    {
        printf("Too many arguments. Perhaps you need to wrap in \"\"?\n");
        return 0;
    }

    char* s = argv[1];

    struct Noun* n = read_noun(&s);

    if(n == NULL)
    {
        return 0;
    }

    printf(" <<< ");
    print_noun(n);

    struct Noun* result = nock(n);

    printf(" >>> ");
    print_noun(result);

    free_noun(n);
    free_noun(result);

    return 0;
}
