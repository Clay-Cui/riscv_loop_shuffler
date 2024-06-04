#include <stdint.h>
#include <stddef.h>

// HACK FOR TESTING: just use a counter initialized at some value

static uint8_t counter = 50;    // initial value

int randombytes(uint8_t *obuf, size_t len)
{
    // while (len > 4)
    // {
    //     *obuf++ = counter++;
    //     *obuf++ = counter++;
    //     *obuf++ = counter++;
    //     *obuf++ = counter++;
    //     len -= 4;
    // }
    // if (len > 0)
    // {
    //     for (; len > 0; --len)
    //     {
    //         *obuf++ = counter++;;
    //     }
    // }

        *obuf++ = counter++;
        *obuf++ = counter++;
        *obuf++ = counter++;
        *obuf++ = counter++;
        *obuf++ = counter++;
        *obuf++ = counter++;
        *obuf++ = counter++;
        *obuf++ = counter++;
    return 0;
}
