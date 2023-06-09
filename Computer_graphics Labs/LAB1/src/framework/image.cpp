#include "image.h"


Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

//copy constructor
Image::Image(const Image& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
}

//assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height*sizeof(Color)];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}



//change image size (the old one will remain in the top-left corner)
void Image::resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = getPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

//change image size and scale the content
void Image::scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = getPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::getArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.setPixel( x, y, getPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::flipX()
{
	for(unsigned int x = 0; x < width * 0.5; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			Color temp = getPixel(width - x - 1, y);
			setPixel( width - x - 1, y, getPixel(x,y));
			setPixel( x, y, temp );
		}
}

void Image::flipY()
{
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height * 0.5; ++y)
		{
			Color temp = getPixel(x, height - y - 1);
			setPixel( x, height - y - 1, getPixel(x,y) );
			setPixel( x, y, temp );
		}
}


//Loads an image from a TGA file
bool Image::loadTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int bytesPerPixel;
	unsigned int imageSize;

	FILE * file = fopen(filename, "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << filename << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		std::cerr << "TGA file seems to have errors or it is compressed, only uncompressed TGAs supported" << std::endl;
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	//save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	//convert to float all pixels
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			this->setPixel(x , height - y - 1, Color( tgainfo->data[pos+2], tgainfo->data[pos+1], tgainfo->data[pos]) );
		}

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::saveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	FILE *file = fopen(filename, "wb");
	if ( file == NULL )
	{
		fclose(file);
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	//tgainfo->width = header[1] * 256 + header[0];
	//tgainfo->height = header[3] * 256 + header[2];

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	//convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[(height-y-1)*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);
	return true;
}

#ifndef IGNORE_LAMBDAS

//you can apply and algorithm for two images and store the result in the first one
//forEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void forEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

void Image::drawRectangle(int x, int y, unsigned int w, unsigned int h, Color color, bool fill)
{
	int end1 = x + w;
	int end2 = y + h;
	if ((end1) >= width) { end1 = width - 1; }
	if ((end2) >= height) { end2 = height - 1; }
	
	if (fill) {
		for (int i = x; i < end1; ++i) {
			for (int j = y; j < end2; ++j) {
				setPixel(i, j, color);
			}
		}
	}
	else
		for (int i = x; i < end1; ++i){
			setPixel(i, y, color);
			setPixel(i, end2 - 1, color);
		}
		for (int j = y+1; j < end2-1; ++j){
			setPixel(x, j, color);
			setPixel(end1 - 1, j, color);
		}
	
	

}
void Image::drawLine( unsigned int x,unsigned int y, unsigned int x2, unsigned int y2, Color color)
{	
	int middle_x = (x+x2)/2;
	int middle_y = (y+y2)/2;

	if (middle_x == x && middle_y == y || middle_x == x2 && middle_y == y2) { // cas base: els pixels son consecutius => no hi ha mig
		setPixelSafe(middle_x, middle_y, color);
	}
	else {
		setPixelSafe(middle_x, middle_y, color);//pintar middle pixel
		drawLine(x, y, middle_x, middle_y, color); //crida recursiva amb la primera meitat de la linia
		drawLine(middle_x, middle_y, x2, y2, color); //crida recursiva amb la segona meitat de la linia
	}
}
void Image::drawCircle(int x, int y, int r, Color color, bool fill)
{
	int end1 = x + r;
	int end2 = y + r;
	int s1 = x - r;
	int s2 = y - r;
	if (s1 < 0) s1 = 0;
	if (s2 < 0) s2 = 0;
	if ((end1) >= width) end1 = width - 1; 
	if ((end2) >= height) end2 = height - 1; 
	

	for (int i = s1; i < end1; ++i){
		for (int j = s2; j < end2; ++j){				
			unsigned int distx = x - i;
			unsigned int disty = y - j;
			if (fill){
				//si el punt esta a distancia<= que el radi: es del cercle
				if (sqrt(distx*distx + disty*disty) <= r){
					setPixel(i, j, color);
				}
			}
			else {
				if ((sqrt(distx*distx + disty*disty)) <= r && (sqrt(distx*distx + disty*disty) > r - 1))
				{
					setPixel(i, j, color);
				}
			}
		}
	}
}
void Image::gradient_ex2()
{
	for (int i = 0; i < width; ++i) 
	{
		for (int j = 0; j < height; ++j)
		{
			//comen�a tot blau i 0 vermell i va incrementant la quantitat de vermell i disminuint la de blau
			float r = i / (float)width;
			r = 255 * r;
			float b = i / (float)width;
			b = 255 - b * 255;
			setPixel(i, j, Color(r, 0, b));
		}
	}
}
void Image::checkeredBoard_ex2()
{
	int y = 1; //contador de quadrats y
	int x = 1; //contador de quadrats x
	for (int i = 0; i < width; ++i){
		for (int j = 0; j < height; ++j){
			
			if (j == (y * (height / 20))) y = y+1;	
			
			if(fmod(x+y,2) == 0) setPixel(i, j, Color::WHITE);
			else setPixel(i, j, Color::BLACK);
			
		}
		if (i == (x * (width / 20))) x++;
		y = 1;

	}
}
void Image::filtre_invert_ex3(Image img)
{
	img.scale(width, height);
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++) {

			Color pixel = img.getPixelSafe(i, j);
			setPixel(i, j, Color(255-pixel.r, 255-pixel.g, 255-pixel.b));
		}
			
	}
}
void Image::filtre_threshold_ex3(Image img) {

	img.scale(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Color c;
			Color pixel = img.getPixelSafe(i, j);
			if (pixel.g > 100) c = Color::BLACK; //threshold a green 100
			else c = Color::WHITE;
			setPixel(i, j, c);
		}

	}

}
void Image::rotate_ex4(int angle, Image img)
{
	img.scale(width, height);
	int x2, y2;
	float radians = angle * DEG2RAD ;
	int centre_x = width / 2;
	int centre_y = height / 2;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

			int xtoc = i - centre_x;
			int ytoc = j - centre_y;

			x2 =(xtoc * cos(radians)) - (ytoc * sin(radians))+centre_x;
			y2 =(xtoc * sin(radians)) + (ytoc * cos(radians))+centre_y;
			Color pixel = img.getPixelSafe(x2, y2);

			setPixelSafe(i, j, pixel);
		}
	}

}
void Image::scale_ex4(Image img) {
	img.scale(width, height);
	float x2, y2;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

			x2 = i*0.1;
			y2 = j*0.1;
			Color pixel = img.getPixelSafe(x2, y2);

			setPixelSafe(i, j, pixel);
		}
	}

}

#endif