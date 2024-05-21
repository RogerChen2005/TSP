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

bmp::bmp(int width,int height)
{
    this->width = width;
    this->height = height;
    this->_data = new rgb[width*height];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++){
            int start = i*width+j;
            this->_data[start].change(255,255,255);
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
            double c = std::sqrt(radius*radius - dx * dx);
            int bottom = std::floor(_pos.y- c), top = std::ceil(_pos.y + c);
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

void bmp::dot(point& _p,double radius){
    int left = std::floor(_p.x - radius), right = std::ceil(_p.x + radius);
    for (int x = left; x <= right; x++)
    {
        double dx = x - _p.x;
        double c = std::sqrt(radius*radius - dx * dx);
        int bottom = std::floor(_p.y- c), top = std::ceil(_p.y + c);
        for (int y = bottom; y <= top; y++)
        {
            if (x > 0 && x < this->width && y > 0 && y < this->height)
            {
                this->_data[clac_pos(x,y)].change(0,0,0);
            }
        }
    }
}

void bmp::circle(point& _p,double radius){
    int left = std::floor(_p.x - radius), right = std::ceil(_p.x + radius);
    for (int x = left; x <= right; x++)
    {
        double dx = x - _p.x;
        double c = std::sqrt(radius*radius - dx * dx);
        int bottom = std::floor(_p.y- c), top = std::ceil(_p.y + c);
        if (x > 0 && x < this->width){
            if(bottom > 0){
                this->_data[clac_pos(x,bottom)].change(0,0,0);
            }
            if(top < this->height){
                this->_data[clac_pos(x,top)].change(0,0,0);
            }
        }
            
    }
}

void bmp::lineTo(point& _p1,point& _p2,double width,rgb& color){
    this->dot(_p1,width);
    this->dot(_p2,width);
    double x1,x2,y1,y2;
    if(_p1.x < _p2.x) {
        x1=_p1.x,x2=_p2.x,y1=_p1.y,y2=_p2.y;
    }
    else{
        x1=_p2.x,x2=_p1.x,y1=_p2.y,y2=_p1.y;
    }
    double theta = std::atan2(x2-x1,y2-y1);
    for (int x = std::floor(x1); x <= std::ceil(x2); x++)
    {
        // double abs_sin = std::fabs(std::sin(theta));
        // int start = std::floor(x - width * abs_sin);
        // int end = std::ceil(x + width * abs_sin);
        int y = y1 + (x - x1) * std::tan(theta);
        // for(int mx = start;mx < end;mx++){
        //     double my = y + (x - mx) / std::tan(theta);
        //     this->_data[clac_pos(mx,my)].change(0,0,0);
        // }
        point tmp = {(double)x,(double)y};
        this->dot(tmp,width);
    }
}

void bmp::exports(const char * filename){
    if(writeBMP(filename,this->_data,this->width,this->height)){
        std::cout << "Output Successful!" << std::endl;
    }
}