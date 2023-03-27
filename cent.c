/*
Copyright (c) 2023, jtgrassie

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ent.h"

void
usage()
{
    fprintf(stderr,
        "USAGE\n"
        "\tcent [-w|--whole] [-h|--help] [file]\n\n"
        "\tfile        If parameter is missing, it will use stdin (if present) "
            "or just use /dev/urandom.\n"
        "\t-w|--whole  Read the whole file. If not set, just reads up to 4096 "
            "bytes.\n"
        "\t-h|--help   This screen.\n\n"
        "EXAMPLES\n"
        "\thead -c 255 /dev/urandom | cent\n"
        "\topenssl enc --aes-128-cbc -pbkdf2 -k test </etc/passwd | cent -w\n"
        "\tperl -e 'for(0..255){print chr}' | cent\n"
        "\tcent /etc/group\n");
}

int main(int argc, char **argv)
{
    unsigned char buf[4096] = {0};
    struct stat st_info;
    size_t blen = 0, elen = 0;
    FILE *in = NULL;
    int whole = 0;
    char *file = NULL;
    ent_ctx ctx = {{0},0};

    if (fstat(0, &st_info))
        perror("fstat"), _exit(1);

    while (--argc)
    {
        if (!strcmp("-h", argv[argc]) || !strcmp("--help", argv[argc]))
            usage(), _exit(1);
        else if (!strcmp("-w", argv[argc]) || !strcmp("--whole", argv[argc]))
            whole = 1;
        else
            file = argv[argc];
    }

    if (file)
    {
        fprintf(stderr, "reading: %s\n", file);
        if (!(in = freopen(file, "r", stdin)))
            perror("freopen"), _exit(2);
    }
    else if (S_ISFIFO(st_info.st_mode) || !isatty(0))
    {
        fprintf(stderr, "reading: stdin\n");
        in = stdin;
    }
    else
    {
        fprintf(stderr, "reading: /dev/urandom\n");
        if (!(in = freopen("/dev/urandom", "r", stdin)))
            perror("freopen"), _exit(3);
    }

    do
    {
        blen = fread(buf, 1, sizeof buf, in);
        elen = ent_add(&ctx, buf, buf+blen);
    }
    while (whole && blen);

    printf("Entropy: %g, over: %lu bytes\n", ent_get(&ctx), elen);

    return 0;
}
