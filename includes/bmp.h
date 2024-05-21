#pragma once
#include <pos.h>
#include <vector>
#include <ref.h>

#pragma pack(push, 1)
struct BMPHeader {
    uint16 signature;
    uint32 fileSize;
    uint16 reserved1;
    uint16 reserved2;
    uint32 pixelDataOffset;
    uint32 dibHeaderSize;
    uint32 width;
    uint32 height;
    uint16 colorPlanes;
    uint16 bitsPerPixel;
    uint32 compressionMethod;
    uint32 imageSize;
    uint32 horizontalResolution;
    uint32 verticalResolution;
    uint32 colorsInPalette;
    uint32 importantColors;
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
    bmp(int width,int height);
    ~bmp();
    void exports(const char * filename);
    void draw(std::vector<pos>& path,double h);
    void circle(point& _p,double radius);
    void lineTo(point& _p1,point& _p2,double width,rgb& color);
    void dot(point& _p,double radius);
    int clac_pos(int x,int y);
};


