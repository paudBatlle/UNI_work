#include "application.h"
#include "utils.h"
#include "image.h"

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

	framebuffer.resize(w, h);
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;


	//here add your init stuff
}

//render one frame
void Application::render(Image& framebuffer)
{
	//clear framebuffer if we want to start from scratch
	framebuffer.fill(Color::BLACK);
	if (render_mode == EX_1)	framebuffer.drawLineDDA(startPosition, endPosition, Color::RED);

	else if (render_mode == EX_2)framebuffer.drawLineBresenham(startPosition, endPosition, Color::GREEN);

	else if (render_mode == EX_3)framebuffer.drawCircle(startPosition, endPosition, Color::CYAN, fill);
	else if (render_mode == EX_4)framebuffer.drawTriangle(p1, p2, p3, Color::BLUE, fill);
	else if (render_mode == EX_5)framebuffer.drawTriangleInterpolate(p1, p2, p3, Color::YELLOW, Color::CYAN, Color::PURPLE, fill);
	//here you can add your code to fill the framebuffer

	//fill every pixel of the image with some random data
	/*
	for (unsigned int x = 0; x < framebuffer.width; x++)
	{
		for (unsigned int y = 0; y < framebuffer.height; y++)
		{
			framebuffer.setPixel(x, y, Color(randomValue() * 255, randomValue() * 255, randomValue() * 255)); //random color
		}
	}*/
}

//called after render
void Application::update(double seconds_elapsed)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (keystate[SDL_SCANCODE_SPACE]) //if key space is pressed
	{
		//...
	}

	//to read mouse position use mouse_position
}

//keyboard press event 
void Application::onKeyDown( SDL_KeyboardEvent event )
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch(event.keysym.scancode)
	{
		case SDL_SCANCODE_1:
			render_mode = EX_1;
			break;
		case SDL_SCANCODE_2:
			render_mode = EX_2;
			break;
		case SDL_SCANCODE_3:
			render_mode = EX_3;
			break;
		case SDL_SCANCODE_4:
			render_mode = EX_4;
			break;
		case SDL_SCANCODE_5:
			render_mode = EX_5;
			break;
		case SDL_SCANCODE_F:
			if (fill)fill = false;
			else fill = true;
			break;
		case SDL_SCANCODE_ESCAPE:
			exit(0); 
			break; //ESC key, kill the app
		}
}

//keyboard key up event 
void Application::onKeyUp(SDL_KeyboardEvent event)
{
	//...
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{

	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
		if (render_mode == EX_1 || EX_2 || EX_3) {
			startPosition = mouse_position;	
		}
		if (render_mode == EX_4||EX_5) {
			if (count == 0) {
				p1 = mouse_position;
				count++;
			}
			else if (count == 1) 
			{
				p2 = mouse_position;
				count++;
			}
			else if (count == 2) {
				p3 = mouse_position;
				count = 0;
			}
			
		}
		//if you read mouse position from the event, careful, Y is reversed, use mouse_position instead
	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{
		if (render_mode == EX_1 || EX_2 || EX_3) {
			 endPosition = mouse_position;
			
		}
	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}
