#include <bmp.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>
#include <cmath>

void rgb::change (const unsigned char r,const unsigned char g,const unsigned char b){
    this->r = r;
    this->g = g;
    this->b = b;
}

bool writeBMP(const char* filename, rgb* pixels, int width, int height) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "Error: Failed to open file for writing: " << filename << std::endl;
        return false;
    }
    BMPHeader header;
    header.signature = 0x4D42; // BM
    header.fileSize = sizeof(BMPHeader) + width * height * sizeof(char);
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.pixelDataOffset = sizeof(BMPHeader);
    header.dibHeaderSize = 40;
    header.width = width;
    header.height = height;
    header.colorPlanes = 1;
    header.bitsPerPixel = 24; // Change to 32 if you want 32-bit BMP
    header.compressionMethod = 0;
    header.imageSize = width * height * sizeof(char);
    header.horizontalResolution = 2835; // 72 DPI
    header.verticalResolution = 2835; // 72 DPI
    header.colorsInPalette = 0;
    header.importantColors = 0;

    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(pixels), width*height*3);
    file.close();
    return true;
}

bmp::bmp(char ** data,int width,int height)
{
    this->width = width;
    this->height = height;
    this->_data = new rgb[width*height];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++){
            int start = i*width+j;
            unsigned char dat = data[i][j]*255;
            this->_data[start].change(dat,dat,dat);
        }
    }
}

bmp::~bmp(){
    delete[] this->_data;
}

int bmp::clac_pos(int x,int y){
    return x + y * this->width;
}

void bmp::draw(std::vector<pos>& path,double h){
    static const int radius = 2; 
    int length = path.size();
    for(const pos& _pos:path){
        int left = _pos.x - radius, right = _pos.x + radius;
        for (int x = left; x <= right; x++)
        {
            int dx = x - _pos.x;
            int c = std::ceil(std::sqrt(radius*radius - dx * dx));
            int bottom = _pos.y- c, top = _pos.y + c;
            for (int y = bottom; y <= top; y++)
            {
                if (x > 0 && x < this->width && y > 0 && y < this->height)
                {
                    this->_data[clac_pos(x,y)].change(255,255*h,255*(1-h));
                }
            }
        }
    }
}

void bmp::exports(const char * filename){
    if(writeBMP(filename,this->_data,this->width,this->height)){
        std::cout << "Output Successful!" << std::endl;
    }
}