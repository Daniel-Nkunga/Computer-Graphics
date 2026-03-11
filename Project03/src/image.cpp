#include <assert.h>
#include <image.hpp>

ErodrImage image_alloc(int width, int height)
{
    return ErodrImage{
        .data = new float[width * height],
        .width = width,
        .height = height,
    };
}

void image_free(ErodrImage *img)
{
    delete[] img->data;
}

void image_copy(ErodrImage *dst, ErodrImage *src)
{
    assert(src->width == dst->width);
    assert(src->height == dst->height);
    assert(src->data != nullptr);
    assert(dst->data != nullptr);
    memcpy(dst->data, src->data, sizeof(float) * src->width * src->height);
}

bool image_clamp(ErodrImage *img)
{
    bool clamped = false;
    int size = img->width * img->height;
    for(int i = 0; i < size; i++)
    {
        float value = img->data[i];
        if (value >= 0.0f && value <= 1.0f) continue;
        img->data[i] = (value < 0.0f) ? 0.0f : 1.0f;
        clamped = true;
    }
    return clamped;
}