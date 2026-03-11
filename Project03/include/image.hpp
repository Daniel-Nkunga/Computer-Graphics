#ifndef IMAGE_H
#define IMAGE_H

struct ErodrImage {
    float *data;
    int width;
    int height;
};

ErodrImage image_alloc(int width, int height);
void image_free(ErodrImage *img);
void image_copy(ErodrImage *dst, ErodrImage *src);
bool image_clamp(ErodrImage *img);

#endif