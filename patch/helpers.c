#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

int
strnhexcmp(const char *hexadecimal, const unsigned char *buffer, size_t sz)
{
    for (size_t i = 0; i < sz; i++) {
        int ret;
        char byte[3];
        if (hexadecimal[i*2] == '\0' ||
            hexadecimal[i*2+1] == '\0') return -1;
        sprintf(byte, "%02hhx", buffer[i]);
        ret = strncasecmp(byte, hexadecimal + i*2, 2);
        if (ret) return ret;
    }
    return 0;
}

char *
hexify(const unsigned char *buffer, size_t sz)
{
    char *result = calloc(sz*2 + 1, 1);
    for (size_t i = 0; i < sz; i++)
        sprintf(result + i*2, "%02hhx", buffer[i]);
    return result;
}

int
fuzzy_strcmp(const char *pathname, char *fuzzy)
{
    if (!strcmp(pathname, fuzzy)) return 0;

    /* We have to handle the case where the file is present on a
     * sublayer. In this case, (deleted) may not appear in
     * pathname. */
    char *nfuzzy = strdup(fuzzy), *p;
    if (!strstr(pathname, " (deleted)") &&
        (p = strstr(nfuzzy, " (deleted)"))) {
        p[0] = '\0';
        if (!strcmp(pathname, nfuzzy)) {
            free(nfuzzy);
            return 0;
        }
    }

    /* Another possibility is to have a prefix before pathname */
    if ((p = strstr(pathname, nfuzzy))) {
        if (!strcmp(p, nfuzzy)) {
            free(nfuzzy);
            return 0;
        }
    }
    free(nfuzzy);
    return -1;
}
