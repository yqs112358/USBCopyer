#include <Windows.h>

char GetFirstDriveFromMask(ULONG unitmask)
{
    char i;

    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 1)
            break;
        unitmask >>= 1;
    }

    return (i + 'A');
}