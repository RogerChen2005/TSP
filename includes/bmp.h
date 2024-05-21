#pragma once
#include <pos.h>
#include <vector>
#include <ref.h>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixelDataOffset;
    uint32_t dibHeaderSize;
    uint32_t width;
    uint32_t height;
    uint16_t colorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod;
    uint32_t imageSize;
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    uint32_t colorsInPalette;
    uint32_t importantColors;
};
#pragma pack(pop)

struct rgb{
    unsigned char r,g,b;
    void change (const unsigned char r,const unsigned char g,const unsigned char b);
};

class bmp
{
private:
    rgb *_data;
    int width,height;
public:
    bmp(char ** data,int width,int height);
    ~bmp();
    void exports(const char * filename);
    void draw(std::vector<pos>& path,double h);
    void draw(pos* path,int length);
    int clac_pos(int x,int y);
};


