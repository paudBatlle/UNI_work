#include "application.h"
#include "utils.h"
#include "image.h"
#include "mesh.h"


Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);

	zbuffer.resize(w, h);
	framebuffer.resize(w, h);
}

//Here we have already GL working, so we can create meshes and textures
//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;
	
	//here we create a global camera and set a position and projection properties
	camera = new Camera();
	camera->lookAt(Vector3(0,10,20),Vector3(0,10,0),Vector3(0,1,0)); //define eye,center,up
	camera->perspective(60, window_width / (float)window_height, 0.1, 10000); //define fov,aspect,near,far

	//load a mesh
	mesh = new Mesh();
	if( !mesh->loadOBJ("lee.obj") )
		std::cout << "FILE Lee.obj NOT FOUND" << std::endl;

	//load the texture
	texture = new Image();
	texture->loadTGA("color.tga");
}
bool Application::check(int x, int check)
{
	if (x >= check || x <= 0)return false;
	return true;
}
void Application::bresenham1(int x0, int y0, int x1, int y1) {
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;
	for (; ;) {
		if (x0 < table[y0].minx) table[y0].minx = x0;
		if (x0 > table[y0].maxx)table[y0].maxx = x0;
		if(x0 == x1 && y0 == y1) break;
		e2 = err;
		if(e2 > -dx) { err -= dy; x0 += sx; }
		if(e2 < dy) { err += dx; y0 += sy; }
	}
}

//la nostra funció de bresenham però no acab de funcionar tant bé com la proporcionada  a l'annex
void Application::bresenham2(Vector3 start, Vector3 end) {

	int dx, dy, d, x, y, inc_E, inc_NE;

	dx = abs(end.x - start.x);
	dy = abs(end.y - start.y);
	//4th and 5th octant
	if (start.x > end.x) {
		Vector3 aux = end;
		end = start;
		start = aux;
	}
	//3rd and 6th octant
	if (dy > dx && start.y > end.y) {
		Vector3 aux = end;
		end = start;
		start = aux;
	}
	x = start.x;
	y = start.y;

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
			
			//if (check(x, window_width) && check(y, window_height)) {
				if (x < table[y].minx) table[y].minx = x;
				if (x > table[y].maxx)table[y].maxx = x;

			//}	
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
			if (x < table[y].minx) table[y].minx = x;
			if (x > table[y].maxx)table[y].maxx = x;
			
		}
	}
}

Vector3 Application::interpolation(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 pixel)
{
	Vector2 v0, v1, v2;
	float d00, d01, d11, d20, d21, denom, u, v, w;
	float result;
	v0 = p2 - p1;
	v1 = p3 - p1;
	v2 = pixel - p1;

	d00 = v0.dot(v0);
	d01 = v0.dot(v1);
	d11 = v1.dot(v1);
	d20 = v2.dot(v0);
	d21 = v2.dot(v1);

	denom = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / (float)denom;
	float v_clamp = clamp(v, 0.0, 1.0);
	w = (d00 * d21 - d01 * d20) / (float)denom;
	float w_clamp = clamp(w, 0.0, 1.0);
	u = 1.0 - v - w;
	float u_clamp = clamp(u, 0.0, 1.0);
	float sum = v_clamp + w_clamp + u_clamp;
	v = v_clamp/ sum;
	w = w_clamp/ sum;
	u = u_clamp/ sum;
	
	return Vector3(u,v,w);

}

//this function fills the triangle by computing the bounding box of the triangle in screen space and using the barycentric interpolation
//to check which pixels are inside the triangle. It is slow for big triangles, but faster for small triangles 
void Application::fillTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, Vector2& uv0, Vector2& uv1, Vector2& uv2, float d0, float d1, float d2)
{
	//inicialitzem la taula de límits del triangle
	table.resize(this->window_height);
	for (int i = 0; i < table.size(); ++i)
	{
		table[i].minx = 1000000;
		table[i].maxx = -1000000;
	}
	//cridem a bresenham per delimitar el triangle
	bresenham1(p0.x, p0.y, p1.x, p1.y);
	bresenham1(p1.x, p1.y, p2.x, p2.y);
	bresenham1(p0.x, p0.y, p2.x, p2.y);
	/*
	bresenham(p0, p1);
	bresenham(p1, p2);
	bresenham(p0, p2);*/
	Vector2 pixel;
	
	//Recorrem tota la taula que hem delimitat abans
	for (int y = 0; y < window_height; ++y)
	{
		for (int x = table[y].minx; x <= table[y].maxx; ++x) {				
				pixel.x = x;
				pixel.y = y;
				Vector3 pesos = interpolation(Vector2(p0.x,p0.y),Vector2(p1.x, p1.y), Vector2(p2.x,p2.y), pixel);
				//Calculem les coordenades de textura per el pixel interpolant
				Vector2 tex_coord = uv0 * pesos.x + uv1 * pesos.y + uv2 * pesos.z;
				//comprovem que les coordenades estàn dins la imatge
				if (!check(tex_coord.x, texture->width)) {
					if (tex_coord.x < 0)tex_coord.x = 0;
					else tex_coord.x = (texture->width) - 1;
				}
				else if (!check(tex_coord.y, texture->height)) {
					if (tex_coord.y < 0)tex_coord.y = 0;
					else tex_coord.y= (texture->height)-1;
				}
				Color c = texture->getPixel(tex_coord.x,tex_coord.y);
				//interpolem la distancia del pixel a la camara
				float dist = d0 * pesos.x + d1 * pesos.y + d2 * pesos.z;
				//mirem si la coordenada x sigui valida
				if (x > 0 && x < window_width) {
					//mirem si la distancia és més petita que la del z buffer i si ho és dibuixem el pixel i actualitzem el z buffer
					if (zbuffer.getPixel(x, y) > dist) {
						framebuffer.setPixel(x, y, c);
						zbuffer.setPixel(x, y, dist);
						
					}
				}
		}
	}
}

//render one frame
void Application::render(Image& framebuffer)
{
	framebuffer.fill(Color(40, 45, 60)); //clear
	//inicialitzem els valors del z buffer a un valor molt gran
	zbuffer.fill(1000000);

	//for every point of the mesh (to draw triangles take three points each time and connect the points between them (1,2,3,   4,5,6,   ...)
	for (int i = 0; i < mesh->vertices.size(); ++i)
	{
		Vector3 vertex1 = mesh->vertices[i]; //extract vertex from mesh
		Vector2 uv0 = mesh->uvs[i];			
		++i;
		Vector3 vertex2 = mesh->vertices[i];
		Vector2 uv1 = mesh->uvs[i];
		++i;
		Vector3 vertex3 = mesh->vertices[i];
		Vector2 uv2 = mesh->uvs[i]; //texture coordinate of the vertex (they are normalized, from 0,0 to 1,1)

		//passem de coordenades 0,0 1,1 a 0,0 with,height de la imatge de textura
		uv0.x = uv0.x * texture->width;
		uv0.y = uv0.y * texture->height;
		uv1.x = uv1.x * texture->width;
		uv1.y = uv1.y * texture->height;
		uv2.x = uv2.x * texture->width;
		uv2.y = uv2.y * texture->height;

		//distancia a la camera vertex
		float d0 = abs(camera->eye.z - vertex1.z);
		float d1 = abs(camera->eye.z - vertex2.z);
		float d2 = abs(camera->eye.z - vertex3.z);

		//project every point in the mesh to normalized coordinates using the viewprojection_matrix inside camera
		vertex1 = camera->projectVector(vertex1);
		vertex2 = camera->projectVector(vertex2);
		vertex3 = camera->projectVector(vertex3);
		
		//convert from normalized (-1 to +1) to framebuffer coordinates (0,W)
		
		vertex1.x = (vertex1.x + 1) * (this->window_width / 2);
		vertex1.y = (vertex1.y + 1) * (this->window_height / 2);
		vertex2.x = (vertex2.x + 1) * (this->window_width / 2);
		vertex2.y = (vertex2.y + 1) * (this->window_height / 2);
		vertex3.x = (vertex3.x + 1) * (this->window_width / 2);
		vertex3.y = (vertex3.y + 1) * (this->window_height / 2);
		//paint points in framebuffer

		fillTriangle(vertex1, vertex2, vertex3, uv0, uv1, uv2, d0, d1, d2);
	}
}


//called after render
void Application::update(double seconds_elapsed)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (keystate[SDL_SCANCODE_SPACE])
	{
		//...
	}

	if (keystate[SDL_SCANCODE_A])
		camera->center.x += 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_D])
		camera->center.x -= 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_S])
		camera->center.y += 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_W])
		camera->center.y -= 5 * seconds_elapsed;
	if(keystate[SDL_SCANCODE_G])
		camera->fov += 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_F])
		camera->fov -= 5 * seconds_elapsed;
	if (button_up) {
		camera->eye.x += 0.2 * mouse_delta.x;
		camera->eye.y += 0.2 * mouse_delta.y;
	}
	
	
	//if we modify the camera fields, then update matrices
	camera->updateViewMatrix();
	camera->updateProjectionMatrix();
}

void Application::onResize(float width, float height){
	camera->aspect = width / height;
}
//keyboard press event 
void Application::onKeyDown( SDL_KeyboardEvent event )
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch (event.keysym.scancode)
	{
		case SDL_SCANCODE_ESCAPE: exit(0); break; //ESC key, kill the app
	}
}

//keyboard released event 
void Application::onKeyUp(SDL_KeyboardEvent event)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch (event.keysym.scancode)
	{
	}
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
		button_up = true;
	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{
		button_up = false;
	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}
