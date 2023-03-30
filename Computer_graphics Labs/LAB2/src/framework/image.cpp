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


int Image::sgn(int n)
{
	if (n < 0)return -1;
	else return 1;
}

bool Image::check(int x, int check)
{
	if (x >= check || x <= 0)return false;
	return true;
}

void Image::drawLineDDA(Vector2 start, Vector2 end, Color c) {

	
	
	float d, x, y;
	float dx = (end.x - start.x);
	float dy = (end.y - start.y);
	if (fabs(dx) >= fabs(dy))
		d = fabs(dx);
	else
		d = fabs(dy);
	float vx = dx / d;
	float vy = dy / d;

	x = start.x + sgn(start.x) * 0.5;
	y = start.y + sgn(start.y) * 0.5;
	for (int i = 0; i <= d; i++) {
		if(check(x,width)&&check(y,height))setPixel(floor(x), floor(y), c);
		x = x + vx;
		y = y + vy;
	}
}

void Image::drawLineBresenham(Vector2 start, Vector2 end, Color c)
{
	int dx, dy, inc_E, inc_NE, d, x, y ;

	//comprovar que les coordenades estan dins el framebuffer


	dx = abs(end.x - start.x);
	dy = abs(end.y - start.y);
	//4th and 5th octant
	if (start.x > end.x) {
		Vector2 aux = end;
		end = start;
		start = aux;
	}
	//3rd and 6th octant
	if (dy > dx && start.y > end.y) {
		Vector2 aux = end;
		end = start;
		start = aux;
	}
	x = start.x;
	y = start.y;
	if(check(x,width) && check(y, height))	setPixel(x, y, c);

	//octants verticals
	if (dy > dx) {
	
		inc_E = 2 * dx;
		inc_NE = 2 * (dx - dy);
		d = 2 * dx - dy;
		while (y < end.y)
		{
			if (d <= 0) {
				d = d + inc_E;
				y = y + 1;

			}
			else {
				d = d + inc_NE;
				if (start.y > end.y) {
					y = y - 1;
				}
				else {
					y = y + 1;
					if (start.x > end.x) {
						x = x - 1;
					}
					else x = x + 1;
				}
			}
			if (check(x, width) && check(y, height))setPixel(x, y, c);
		}
	}
	//octants horitzontals
	else {

		inc_E = 2 * dy;
		inc_NE = 2 * (dy - dx);
		d = 2 * dy - dx;
		while (x < end.x)
		{
			if (d <= 0) {
				d = d + inc_E;
				x = x + 1;

			}
			else {
				d = d + inc_NE;
				if (start.x > end.x) {
					x = x - 1;
				}
				else {
					x = x + 1;
					if (start.y > end.y) {
						y = y - 1;
					}
					else y = y + 1;
				}
			}
			if (check(x, width) && check(y, height))setPixel(x, y, c);
		}
	}
}

void Image::drawCircle(Vector2 centre, Vector2 fi, Color c, bool fill)
{
	int d, x, y, v, dx, dy, x_a, x_b, x_c, x_d, y_a, y_b, y_c, y_d;
	dy = fi.y - centre.y;
	dx = fi.x - centre.x;
	int r = sqrt(dy * dy + dx * dx);
	v = 1-r;
	x = 0;
	y = r;

	x_a = centre.x + x;
	y_a = centre.y + y;
	x_b = centre.x - x;
	y_b = centre.y - y;
	x_c = centre.x + y;
	y_c = centre.y + x;
	x_d = centre.x - y;
	y_d = centre.y - x;
	if (fill) {
		for (int i = x_d; i < x_c; i++) {
			if (check(i, width) && check(centre.y, height))	setPixel(i, centre.y, c);
		}
	}
	if (check(x_a, width) && check(y_a, height))setPixel(x_a, y_a, c);
	if (check(x_b, width) && check(y_a, height))setPixel(x_b, y_a, c);
	if (check(x_a, width) && check(y_b, height))setPixel(x_a, y_b, c);
	if (check(x_b, width) && check(y_b, height))setPixel(x_b, y_b, c);
	if (check(x_c, width) && check(y_c, height))setPixel(x_c, y_c, c);
	if (check(x_d, width) && check(y_c, height))setPixel(x_d, y_c, c);
	if (check(x_c, width) && check(y_d, height))setPixel(x_c, y_d, c);
	if (check(x_d, width) && check(y_d, height))setPixel(x_d, y_d, c);
	while (y > x) {
		if (v < 0) {
			v = v + 2 * x + 3;
			x++;
		}
		else {
			v = v + 2*(x - y) + 5;
			x++;
			y--;
		}
		x_a = centre.x + x;
		y_a = centre.y + y;
		x_b = centre.x - x;
		y_b = centre.y - y;
		x_c = centre.x + y;
		y_c = centre.y + x;
		x_d = centre.x - y;
		y_d = centre.y - x;
		if (check(x_a, width) && check(y_a, height))setPixel(x_a, y_a, c);
		if (check(x_b, width) && check(y_a, height))setPixel(x_b, y_a, c);
		if (check(x_a, width) && check(y_b, height))setPixel(x_a, y_b, c);
		if (check(x_b, width) && check(y_b, height))setPixel(x_b, y_b, c);
		if (check(x_c, width) && check(y_c, height))setPixel(x_c, y_c, c);
		if (check(x_d, width) && check(y_c, height))setPixel(x_d, y_c, c);
		if (check(x_c, width) && check(y_d, height))setPixel(x_c, y_d, c);
		if (check(x_d, width) && check(y_d, height))setPixel(x_d, y_d, c);
		if (fill) {
			for (int i = x_b; i < x_a; i++) {
				if (check(i, width) && check(y_a, height))setPixel(i,y_a, c);
				if (check(i, width) && check(y_b, height))setPixel(i, y_b, c);
			}
			for (int j = x_d; j < x_c; j++) {
				if (check(j, width) && check(y_c, height))setPixel(j, y_c, c);
				if (check(j, width) && check(y_d, height))setPixel(j, y_d, c);

			}

		}
	}
	
}

void Image::drawTriangle(Vector2 p1, Vector2 p2, Vector2 p3, Color c, bool fill)
{
	std::vector<taula> table;
	table.resize(this->height);
	//init table
	for (int i = 0; i < table.size(); i++)
	{
		table[i].minx = 1000000;
		table[i].maxx = -1000000;
	}
	
	
	int dx, dy, d,x,y,inc_E,inc_NE;
	Vector2 start, end;
	start = p1;
	end = p2;
	for (int i = 0; i < 3; i++)
	{
		dx = abs(end.x - start.x);
		dy = abs(end.y - start.y);
		//4th and 5th octant
		if (start.x > end.x) {
			Vector2 aux = end;
			end = start;
			start = aux;
		}
		//3rd and 6th octant
		if (dy > dx && start.y > end.y) {
			Vector2 aux = end;
			end = start;
			start = aux;
		}
		x = start.x;
		y = start.y;
		setPixel(x, y, c);
		if (x < table[y].minx) table[y].minx = x;
		if (x > table[y].maxx)table[y].maxx = x;
		//octants verticals
		if (dy > dx) {

			inc_E = 2 * dx;
			inc_NE = 2 * (dx - dy);
			d = 2 * dx - dy;
			while (y < end.y)
			{
				if (d <= 0) {
					d = d + inc_E;
					y = y + 1;

				}
				else {
					d = d + inc_NE;
					if (start.y > end.y) {
						y = y - 1;
					}
					else {
						y = y + 1;
						if (start.x > end.x) {
							x = x - 1;
						}
						else x = x + 1;
					}
				}
				setPixel(x, y, c);
				if (x < table[y].minx) table[y].minx = x;
				if (x > table[y].maxx)table[y].maxx = x;
			}
		}
		//octants horitzontals
		else {

			inc_E = 2 * dy;
			inc_NE = 2 * (dy - dx);
			d = 2 * dy - dx;
			while (x < end.x)
			{
				if (d <= 0) {
					d = d + inc_E;
					x = x + 1;

				}
				else {
					d = d + inc_NE;
					if (start.x > end.x) {
						x = x - 1;
					}
					else {
						x = x + 1;
						if (start.y > end.y) {
							y = y - 1;
						}
						else y = y + 1;
					}
				}
				setPixel(x, y, c);
				if (x < table[y].minx) table[y].minx = x;
				if (x > table[y].maxx)table[y].maxx = x;
			}
		}
		if (i == 0) {
			start = p2;
			end = p3;
		}
		else if (i == 1) {
			start = p3;
			end = p1;
		}
	}
	if (fill) {
		for (int i = 0; i < height; i++)
		{
			for (int j = table[i].minx; j < table[i].maxx; j++) {
				if (table[i].minx > 0 && table[i].maxx < width) setPixel(j, i, c);
			}
		}
	}
	
}

Color Image::interpolation(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 pixel, Color c1, Color c2, Color c3)
{
	Vector2 v0, v1, v2;
	float d00, d01, d11, d20, d21, denom, u, v, w;
	Color c;
	v0 = p2-p1;
	v1 = p3 - p1;
	v2 = pixel - p1;
	
	d00 = v0.dot(v0);
	d01 = v0.dot(v1);
	d11 = v1.dot(v1);
	d20 = v2.dot(v0);
	d21 = v2.dot(v1);
	denom = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / denom;
	w = (d00 * d21 - d01 * d20) / denom;
	u = 1.0 - v - w;
	c = c1 * u + c2 * v + c3 * w;
	return c;
}

void Image::drawTriangleInterpolate(Vector2 p1, Vector2 p2, Vector2 p3, Color c1, Color c2, Color c3, bool fill)
{
	std::vector<taula> table;
	table.resize(this->height);
	//init table
	for (int i = 0; i < table.size(); i++)
	{
		table[i].minx = 1000000;
		table[i].maxx = -1000000;
	}


	int dx, dy, d, x, y, inc_E, inc_NE;
	Vector2 start, end, pixel;
	Color c;
	start = p1;
	end = p2;
	for (int i = 0; i < 3; i++)
	{
		dx = abs(end.x - start.x);
		dy = abs(end.y - start.y);
		//4th and 5th octant
		if (start.x > end.x) {
			Vector2 aux = end;
			end = start;
			start = aux;
		}
		//3rd and 6th octant
		if (dy > dx && start.y > end.y) {
			Vector2 aux = end;
			end = start;
			start = aux;
		}
		x = start.x;
		y = start.y;
		pixel.x = x;
		pixel.y = y;
		c = interpolation(p1, p2, p3, pixel, c1, c2, c3);
		if(check(x,width)&&check(y,height)) setPixel(x, y, c);

		if (x < table[y].minx) table[y].minx = x;
		if (x > table[y].maxx)table[y].maxx = x;
		//octants verticals
		if (dy > dx) {

			inc_E = 2 * dx;
			inc_NE = 2 * (dx - dy);
			d = 2 * dx - dy;
			while (y < end.y)
			{
				if (d <= 0) {
					d = d + inc_E;
					y = y + 1;

				}
				else {
					d = d + inc_NE;
					if (start.y > end.y) {
						y = y - 1;
					}
					else {
						y = y + 1;
						if (start.x > end.x) {
							x = x - 1;
						}
						else x = x + 1;
					}
				}
				pixel.x = x;
				pixel.y = y;
				c = interpolation(p1, p2, p3, pixel, c1, c2, c3);
				if (check(x, width) && check(y, height)) setPixel(x, y, c);
				if (x < table[y].minx) table[y].minx = x;
				if (x > table[y].maxx)table[y].maxx = x;
			}
		}
		//octants horitzontals
		else {

			inc_E = 2 * dy;
			inc_NE = 2 * (dy - dx);
			d = 2 * dy - dx;
			while (x < end.x)
			{
				if (d <= 0) {
					d = d + inc_E;
					x = x + 1;

				}
				else {
					d = d + inc_NE;
					if (start.x > end.x) {
						x = x - 1;
					}
					else {
						x = x + 1;
						if (start.y > end.y) {
							y = y - 1;
						}
						else y = y + 1;
					}
				}
				pixel.x = x;
				pixel.y = y;
				c = interpolation(p1, p2, p3, pixel, c1, c2, c3);
				if (check(x, width) && check(y, height)) setPixel(x, y, c);
				if (x < table[y].minx) table[y].minx = x;
				if (x > table[y].maxx)table[y].maxx = x;
			}
		}
		if (i == 0) {
			start = p2;
			end = p3;
		}
		else if (i == 1) {
			start = p3;
			end = p1;
		}
	}
	if (fill) {
		for (int i = 0; i < height; i++)
		{
			for (int j = table[i].minx; j < table[i].maxx; j++) {
				if (table[i].minx > 0 && table[i].maxx < width) {
					pixel.x = j;
					pixel.y = i;
					c = interpolation(p1, p2, p3, pixel, c1, c2, c3);
					if (check(j, width) && check(i, height)) setPixel(j, i, c);
					
				}
			}
		}
	}
}


#endif