#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define WIDTH 176
#define HEIGHT 220
#define IN_SIZE  (WIDTH * HEIGHT * 3)
#define OUT_SIZE (WIDTH * HEIGHT * 2)

int main(int argc, char *argv[])
{
    FILE *fin, *fout;
    uint8_t *in;
    uint16_t *out;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s in.rgb888 out.rgb565\n", argv[0]);
        return 1;
    }

    fin = fopen(argv[1], "rb");
    if (!fin) { perror("open input"); return 1; }

    fout = fopen(argv[2], "wb");
    if (!fout) { perror("open output"); return 1; }

    in  = malloc(IN_SIZE);
    out = malloc(OUT_SIZE);

    fread(in, 1, IN_SIZE, fin);

    for (int i = 0, j = 0; i < IN_SIZE; i += 3, j++) {
        uint8_t r = in[i];
        uint8_t g = in[i + 1];
        uint8_t b = in[i + 2];

        out[j] = ((r & 0xF8) << 8) |
                 ((g & 0xFC) << 3) |
                 (b >> 3);
    }

    fwrite(out, 2, WIDTH * HEIGHT, fout);

    fclose(fin);
    fclose(fout);
    free(in);
    free(out);

    printf("RGB565 image generated successfully\n");
    return 0;
}

