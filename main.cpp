#include "Draw.hpp"
#include "GL.hpp"

#include <SDL.h>
#include <glm/glm.hpp>

#include <chrono>
#include <iostream>
#include <ctime>

int main(int argc, char **argv) {
	//Configuration:
	struct {
		std::string title = "Game0: Shepherd Dog";
		glm::uvec2 size = glm::uvec2(550, 550);
	} config;

	//------------  initialization ------------

	//Initialize SDL library:
	SDL_Init(SDL_INIT_VIDEO);

	//Ask for an OpenGL context version 3.3, core profile, enable debug:
	SDL_GL_ResetAttributes();
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//create window:
	SDL_Window *window = SDL_CreateWindow(
		config.title.c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		config.size.x, config.size.y,
		SDL_WINDOW_OPENGL /*| SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI*/
	);

	if (!window) {
		std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
		return 1;
	}

	//Create OpenGL context:
	SDL_GLContext context = SDL_GL_CreateContext(window);

	if (!context) {
		SDL_DestroyWindow(window);
		std::cerr << "Error creating OpenGL context: " << SDL_GetError() << std::endl;
		return 1;
	}

	#ifdef _WIN32
	//On windows, load OpenGL extensions:
	if (!init_gl_shims()) {
		std::cerr << "ERROR: failed to initialize shims." << std::endl;
		return 1;
	}
	#endif

	//Set VSYNC + Late Swap (prevents crazy FPS):
	if (SDL_GL_SetSwapInterval(-1) != 0) {
		std::cerr << "NOTE: couldn't set vsync + late swap tearing (" << SDL_GetError() << ")." << std::endl;
		if (SDL_GL_SetSwapInterval(1) != 0) {
			std::cerr << "NOTE: couldn't set vsync (" << SDL_GetError() << ")." << std::endl;
		}
	}

	//Hide mouse cursor (note: showing can be useful for debugging):
	SDL_ShowCursor(SDL_DISABLE);

	//------------  game state ------------
	#define NONE 0
	#define SHEEP 1
	#define SHEPHERD 2
	#define RIGHT 1
	#define UP 2
	#define LEFT 3
	#define DOWN 4
	
	class Sheep {
		private:
			glm::vec2 init_location;
			glm::vec2 location;
			glm::vec2 direction;
			float speed;
			int collide_type;
			int collide_direction;
			void turn() {
				if(collide_type = SHEEP)
				{
					switch(collide_direction) {
						case RIGHT: {
							direction.x = -1;
							direction.y = 0;
							break;
						}
						case UP: {
							direction.x = 0;
							direction.y = -1;
							break;
						}
						case LEFT: {
							direction.x = 1;
							direction.y = 0;
							break;
						}
						case DOWN: {
							direction.x = 0;
							direction.y = 1;
							break;
						}
					}
				}
				else if(collide_type = SHEPHERD) {
					direction.x *= -1;
					direction.y *= -1;
				}
			}
		public:
		Sheep() {};
		Sheep(float x, float y) {
			init_location = glm::vec2(x, y);
			//std::srand((unsigned)std::time(0));
			switch(std::rand() % 4) {
				case 0: {
					direction.x = 1;
					direction.y = 0;
					break;
				}
				case 1: {
					direction.x = 0;
					direction.y = 1;
					break;
				}
				case 2: {
					direction.x = -1;
					direction.y = 0;
					break;
				}
				case 3: {
					direction.x = 0;
					direction.y = -1;
					break;
				}
			}
			initSheep();
		}
		void initSheep() {
			location.x = init_location.x;
			location.y = init_location.y;
			speed = 0.001f;
			collide_type = NONE;
			collide_direction = NONE;
		}
		void move() {
			location.x += speed * direction.x;
			location.y += speed * direction.y;
		}
		glm::vec2 getLocation() {
			return location;
		}
		bool OutOfArea() {
			if(location.x>=0.85f || location.x<=-0.85f || location.y>=0.85f || location.y<=-0.85f) {
				return true;
			}
			else {
				return false;
			}
		}
		void detectCollide() {};
	};
	glm::vec2 mouse = glm::vec2(0.0f, 0.0f);
	//glm::vec2 ball = glm::vec2(0.0f, 0.0f);
	//glm::vec2 ball_velocity = glm::vec2(0.5f, 0.5f);

	//------------  game loop ------------

	auto previous_time = std::chrono::high_resolution_clock::now();
	bool should_quit = false;
	bool play = false;
	bool alive = true;
	Sheep sheep1(0.0f, 0.5f), sheep2(-0.5f, 0.1f), sheep3(0.5f, 0.1f), sheep4(0.3f, -0.4f), sheep5(-0.3f, -0.4f);
	while (true) {
		static SDL_Event evt;
		while (SDL_PollEvent(&evt) == 1) {
			//handle input:
			if (evt.type == SDL_MOUSEMOTION) {
				mouse.x = (evt.motion.x + 0.5f) / float(config.size.x) * 2.0f - 1.0f;
				mouse.y = (evt.motion.y + 0.5f) / float(config.size.y) *-2.0f + 1.0f;
			} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
				play = true;
				alive = true;
				//ball = mouse;
				//ball_velocity = glm::vec2(0.5f, 0.5f);
			} else if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE) {
				should_quit = true;
			} else if (evt.type == SDL_QUIT) {
				should_quit = true;
				break;
			}
		}
		if (should_quit) break;
		
		if(alive) {
			auto current_time = std::chrono::high_resolution_clock::now();
			float elapsed = std::chrono::duration< float >(current_time - previous_time).count();
			previous_time = current_time;

			{ //update game state:
				sheep1.move();
				sheep2.move();
				sheep3.move();
				sheep4.move();
				sheep5.move();
				if(sheep1.OutOfArea() || sheep2.OutOfArea() || sheep3.OutOfArea() || sheep4.OutOfArea() || sheep5.OutOfArea()) {
					alive = false;
				}
				
				/*ball += elapsed * ball_velocity;
				if (ball.x < -1.0f) ball_velocity.x = std::abs(ball_velocity.x);
				if (ball.x >  1.0f) ball_velocity.x =-std::abs(ball_velocity.x);
				if (ball.y < -1.0f) ball_velocity.y = std::abs(ball_velocity.y);
				if (ball.y >  1.0f) ball_velocity.y =-std::abs(ball_velocity.y);*/
			}

			//draw output:
			glClearColor(0.2f, 0.7f, 0.3f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			{ //draw game state:
				Draw draw;
				draw.add_rectangle(glm::vec2(-0.95f, 0.95f), glm::vec2(0.95f, 0.90f), glm::u8vec4(0xB9, 0x7A, 0x57, 0xff));
				draw.add_rectangle(glm::vec2(-0.95f, -0.90f), glm::vec2(0.95f, -0.95f), glm::u8vec4(0xB9, 0x7A, 0x57, 0xff));
				draw.add_rectangle(glm::vec2(-0.95f, 0.90f), glm::vec2(-0.90f, -0.90f), glm::u8vec4(0xB9, 0x7A, 0x57, 0xff));
				draw.add_rectangle(glm::vec2(0.90f, 0.90f), glm::vec2(0.95f, -0.95f), glm::u8vec4(0xB9, 0x7A, 0x57, 0xff));
				draw.add_rectangle(sheep1.getLocation() + glm::vec2(-0.05f,-0.05f), sheep1.getLocation() + glm::vec2(0.05f, 0.05f), glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				draw.add_rectangle(sheep2.getLocation() + glm::vec2(-0.05f,-0.05f), sheep2.getLocation() + glm::vec2(0.05f, 0.05f), glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				draw.add_rectangle(sheep3.getLocation() + glm::vec2(-0.05f,-0.05f), sheep3.getLocation() + glm::vec2(0.05f, 0.05f), glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				draw.add_rectangle(sheep4.getLocation() + glm::vec2(-0.05f,-0.05f), sheep4.getLocation() + glm::vec2(0.05f, 0.05f), glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				draw.add_rectangle(sheep5.getLocation() + glm::vec2(-0.05f,-0.05f), sheep5.getLocation() + glm::vec2(0.05f, 0.05f), glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				draw.add_rectangle(mouse + glm::vec2(-0.05f,-0.05f), mouse + glm::vec2(0.05f, 0.05f), glm::u8vec4(0x00, 0x00, 0x00, 0xff));
				draw.draw();
			}


			SDL_GL_SwapWindow(window);
		}
		else {
			sheep1.initSheep();
			sheep2.initSheep();
			sheep3.initSheep();
			sheep4.initSheep();
			sheep5.initSheep();
		}
	}


	//------------  teardown ------------

	SDL_GL_DeleteContext(context);
	context = 0;

	SDL_DestroyWindow(window);
	window = NULL;

	return 0;
}
