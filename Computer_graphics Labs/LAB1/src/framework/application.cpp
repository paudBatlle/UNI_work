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
	
	if (img->loadTGA("tigerking.tga") == false)
		std::cout << "image not found" << std::endl;
	else {
		img->flipY();
		
	}

	for (int i = 0; i < TOTAL_PARTICLES; i++) {
		part_state[i].x = fmod(rand(),window_width-1);
		part_state[i].y = fmod(rand(),window_height-1);
		part_state[i].size = fmod(rand(),4)+2;
		part_state[i].velocity = fmod(rand(),300) + 1;
	}

	
	
}

//render one frame
void Application::render( Image& framebuffer )
{
	//clear framebuffer if we want to start from scratch
	framebuffer.fill(Color::BLACK);

	//EX 1
	if (render_mode == EX_1)
	{
		framebuffer.drawLine(250, 320, 470, 120, Color::CYAN);
		framebuffer.drawLine(50, 120, 250, 320, Color::CYAN);
		framebuffer.drawLine(400, 620, 400, 470, Color::CYAN);
		framebuffer.drawLine(500, 470, 670, 470, Color::CYAN);
		framebuffer.drawRectangle(450, 300, 778, 150, Color::GREEN, true);
		framebuffer.drawRectangle(10, 450, 250, 100, Color::WHITE, false);
		framebuffer.drawCircle(200, 120, 50, Color::WHITE, false);
		framebuffer.drawCircle(690, 150, 100, Color::WHITE, true);
	}
	//EX 2
	else if (render_mode == EX_2) framebuffer.gradient_ex2();
	else if (render_mode == EX_3) framebuffer.checkeredBoard_ex2();
	//EX 3
	else if (render_mode == EX_4)framebuffer.filtre_invert_ex3(*img);
	else if (render_mode == EX_5)framebuffer.filtre_threshold_ex3(*img);
	//EX 4
	else if (render_mode == EX_6)framebuffer.rotate_ex4(45, *img);
	else if (render_mode == EX_7)framebuffer.scale_ex4(*img);
	//EX 5
	else if (render_mode == EX_8) {
		for (int i = 0; i < TOTAL_PARTICLES; i++) {
			framebuffer.drawCircle(part_state[i].x, part_state[i].y, part_state[i].size, Color::WHITE, true);
		}
	}
}

//called after render
//es fa servir per saber si una tecla ENCARA est� premuda
void Application::update(double seconds_elapsed)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (render_mode == EX_8) {
		for (int i = 0; i < TOTAL_PARTICLES; i++) {
			//formula del movient mru 
			part_state[i].y = part_state[i].y -  part_state[i].velocity * seconds_elapsed;
			//si arriba a baix de tot de la pantalla tornar a dalt de tot
			if (part_state[i].y <= 0)part_state[i].y = (int)(window_height - 1) + part_state[i].y;
			
		}
		//omplir de negre per poder pintar la seguent situacio de les particules
		framebuffer.fill(Color::BLACK);
	}
	
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
		case SDL_SCANCODE_6:
			render_mode = EX_6;
			break;
		case SDL_SCANCODE_7:
			render_mode = EX_7;
			break;
		case SDL_SCANCODE_8:
			render_mode = EX_8;
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
		//if you read mouse position from the event, careful, Y is reversed, use mouse_position instead
	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{

	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}
