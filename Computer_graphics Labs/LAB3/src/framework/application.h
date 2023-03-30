/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "includes.h"
#include "framework.h"
#include "mesh.h"
#include "camera.h"
#include "image.h"

class Application
{
public:
	//window
	SDL_Window* window;
	float window_width;
	float window_height;

	struct taula {
		int minx;
		int maxx;
	};
	std::vector<taula> table;

	bool button_up = false;
	float time;
	Image framebuffer;
	FloatImage zbuffer;

	Mesh* mesh = NULL;
	Camera* camera = NULL;
	Image* texture = NULL;

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame

	//constructor
	Application(const char* caption, int width, int height);

	//main methods
	void init( void );
	void render( Image& framebuffer );
	void update( double dt );

	//methods for events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp( SDL_MouseButtonEvent event );

	void onResize(float width,float height);

	//other methods to control the app
	void setWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
		zbuffer.resize(width, height);
		framebuffer.resize(width, height);
	}

	Vector2 getWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(w,h);
	}

	void start();
	void fillTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, Vector2& uv0, Vector2& uv1, Vector2& uv2, float d0, float d1, float d2);
	bool check(int x, int check);
	//ens retorna els pesos de la interpolació per un punt
	Vector3 interpolation(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 pixel);
	void bresenham2(Vector3 start, Vector3 end);
	void bresenham1(int x0, int y0, int x1, int y1);
};


#endif 