#include "application.h"
#include "utils.h"
#include "image.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "light.h"
#include "material.h"

Camera* camera = NULL;
Mesh* mesh = NULL;
Matrix44 model_matrix;
Shader* shader = NULL;
Shader* phong_shader = NULL;
Shader* phong_specular = NULL;
Shader* shader_a = NULL;
Light* light = NULL;
Material* material0 = NULL;
Material* material1 = NULL;
Material* material2 = NULL;


Texture* texture1 = NULL;
Texture* texture2 = NULL;

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
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;
	
	//here we create a global camera and set a position and projection properties
	camera = new Camera();
	camera->lookAt(Vector3(0,20,20),Vector3(0,10,0),Vector3(0,1,0));
	camera->setPerspective(60,window_width / window_height,0.1,10000);

	//then we load a mesh
	mesh = new Mesh();
	mesh->loadOBJ("../res/meshes/lee.obj");

	//load the texture
	texture1 = new Texture();
	if(!texture1->load("../res/textures/lee_color_specular.tga"))
	{
		std::cout << "Texture lee_color_specular not found" << std::endl;
		exit(1);
	}
	texture2 = new Texture();
	if (!texture2->load("../res/textures/lee_normal.tga"))
	{
		std::cout << "Texture lee_normal not found" << std::endl;
		exit(1);
	}

	//we load a shader
	shader = Shader::Get("../res/shaders/texture.vs","../res/shaders/texture.fs");
	phong_shader = Shader::Get("../res/shaders/phong_D.vs", "../res/shaders/phong_D.fs");
	phong_specular = Shader::Get("../res/shaders/phong_S.vs", "../res/shaders/phong_S.fs");
	material0 = new Material();
	material1 = new Material();
	material2 = new Material();
	material1->diffuse.set(1, 0, 0);
	material1->specular.set(1, 0, 0);
	material1->ambient.set(0.6, 0.2, 0.2);
	material1->shininess = 70;
	material2->diffuse.set(0, 1, 0);
	material2->specular.set(0, 1, 0);
	material2->ambient.set(0.1, 0.5, 0.2);
	material2->shininess = 100;
	
	light = new Light();
}

//render one frame
void Application::render(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable( GL_DEPTH_TEST );

	//Get the viewprojection
	camera->aspect = window_width / window_height;
	Matrix44 viewprojection = camera->getViewProjectionMatrix();
	if (render_mode == PHONG_D)shader_a = phong_shader;
	else if (render_mode == PHONG_S)shader_a = phong_specular;
	else shader_a = shader;
	//enable the shader
	shader_a->enable();
	shader_a->setMatrix44("model", model_matrix); //upload info to the shader
	shader_a->setMatrix44("viewprojection", viewprojection); //upload info to the shader

	shader_a->setTexture("color_texture", texture1, 0 ); //set texture in slot 0
	//EX 1 I 2
	if (render_mode == PHONG_D || render_mode == PHONG_S) {
		shader_a->setVector3("camera", camera->eye);
		shader_a->setUniform3("Ka", material0->ambient);
		shader_a->setVector3("light_position", light->position);
		shader_a->setUniform3("Kd", material0->diffuse);
		shader_a->setUniform3("Ks", material0->specular);
		shader_a->setUniform3("Ia", light->diffuse_color);
		shader_a->setUniform3("Id", light->diffuse_color);
		shader_a->setUniform3("Is", light->specular_color);
		shader_a->setFloat("alpha", material0->shininess);
		shader_a->setTexture("normal_texture", texture2, 1);
		mesh->render(GL_TRIANGLES);
		if (ex4 == true) {
				model_matrix.translate(20, 0, 0);
				shader_a->setMatrix44("model", model_matrix); //upload info to the shader
				shader_a->setFloat("alpha", material1->shininess);
				shader_a->setUniform3("Ka", material1->ambient);

				shader_a->setUniform3("Kd", material1->diffuse);
				shader_a->setUniform3("Ks", material1->specular);
				mesh->render(GL_TRIANGLES);
				model_matrix.translate(-20, 0, 0);
				model_matrix.translate(-20, 0, 0);
				shader_a->setMatrix44("model", model_matrix); //upload info to the shader
				shader_a->setFloat("alpha", material2->shininess);
				shader_a->setUniform3("Ka", material2->ambient);

				shader_a->setUniform3("Kd", material2->diffuse);
				shader_a->setUniform3("Ks", material2->specular);
				mesh->render(GL_TRIANGLES);
				model_matrix.translate(20, 0, 0);

		}
		
	
	}
	else {
		//render the data
		mesh->render(GL_TRIANGLES);
	}
	if (normalmap == true) {
		shader_a->setFloat("normalmap", 1.0);
	}
	else {
		shader_a->setFloat("normalmap", 0.0);
	}

	

	//disable shader
	shader_a->disable();

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

//called after render
void Application::update(double seconds_elapsed)
{
	if (keystate[SDL_SCANCODE_SPACE])
	{
		model_matrix.rotateLocal(seconds_elapsed,Vector3(0,1,0));
	}

	if (keystate[SDL_SCANCODE_RIGHT])
		camera->eye = camera->eye + Vector3(1, 0, 0) * seconds_elapsed * 10.0;
	else if (keystate[SDL_SCANCODE_LEFT])
		camera->eye = camera->eye + Vector3(-1, 0, 0) * seconds_elapsed * 10.0;
	if (keystate[SDL_SCANCODE_UP])
		camera->eye = camera->eye + Vector3(0, 1, 0) * seconds_elapsed * 10.0;
	else if (keystate[SDL_SCANCODE_DOWN])
		camera->eye = camera->eye + Vector3(0, -1, 0) * seconds_elapsed * 10.0;
}

//keyboard press event 
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch (event.keysym.scancode){
		case SDL_SCANCODE_R: Shader::ReloadAll(); break;
        case SDL_SCANCODE_ESCAPE: exit(0); break; //ESC key, kill the app
		case SDL_SCANCODE_1:render_mode = SIMPLE; break;
		case SDL_SCANCODE_2:render_mode = PHONG_D; break;
		case SDL_SCANCODE_3:render_mode = PHONG_S; break;
		case SDL_SCANCODE_N:normalmap = !normalmap; break;
		case SDL_SCANCODE_4:ex4 = !ex4; break;


	}

}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{

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
