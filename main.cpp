#include "Draw.hpp"
#include "GL.hpp"

#include <SDL.h>
#include <glm/glm.hpp>

#include <chrono>
#include <iostream>
#include <ctime>

#define NONE 0
#define SHEEP 1
#define SHEPHERD 2
#define SHEPHERD_WAIT 3
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
public:
	Sheep() {};
	Sheep(float x, float y) {
		init_location = glm::vec2(x, y);
		initSheep();
	}
	void initSheep() {
		location.x = init_location.x;
		location.y = init_location.y;
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
		speed = 0.001f;
		collide_type = NONE;
		collide_direction = NONE;
		return;
	}
	void move() {
		location.x += speed * direction.x;
		location.y += speed * direction.y;
		return;
	}
	glm::vec2 getLocation() {
		return location;
	}
	int getDirection() {
		if(direction.x==1) {
			return RIGHT;
		}
		else if(direction.y==1) {
			return UP;
		}
		else if(direction.x==-1) {
			return LEFT;
		}
		else {
			return DOWN;
		}
	}
	bool outOfArea() {
		if(location.x>=0.85f || location.x<=-0.85f || location.y>=0.85f || location.y<=-0.85f) {
			return true;
		}
		else {
			return false;
		}
	}
	void detectSheepCollide(Sheep anotherSheep) {
		if(direction.x==1) {
			if(abs(location.x-anotherSheep.getLocation().x)<=0.1 && abs(location.y-anotherSheep.getLocation().y)<0.1 && abs(location.x-anotherSheep.getLocation().x)>abs(location.y-anotherSheep.getLocation().y)) {
				collide_type = SHEEP;
				collide_direction = RIGHT;
			}
			else if(abs(location.x-anotherSheep.getLocation().x)<0.1 && abs(location.y-anotherSheep.getLocation().y)<=0.1 && abs(location.x-anotherSheep.getLocation().x)<abs(location.y-anotherSheep.getLocation().y)) {
				collide_type = SHEEP;
				if(anotherSheep.getDirection()==UP) {
					collide_direction = DOWN;
				}
				else {
					collide_direction = UP;
				}
			}
		}
		else if(direction.y==1) {
			if(abs(location.x-anotherSheep.getLocation().x)<0.1 && abs(location.y-anotherSheep.getLocation().y)<=0.1 && abs(location.x-anotherSheep.getLocation().x)<abs(location.y-anotherSheep.getLocation().y)) {
				collide_type = SHEEP;
				collide_direction = UP;
			}
			else if(abs(location.x-anotherSheep.getLocation().x)<=0.1 && abs(location.y-anotherSheep.getLocation().y)<0.1 && abs(location.x-anotherSheep.getLocation().x)>abs(location.y-anotherSheep.getLocation().y)) {
				collide_type = SHEEP;
				if(anotherSheep.getDirection()==RIGHT) {
					collide_direction = LEFT;
				}
				else {
					collide_direction = RIGHT;
				}
			}
		}
		else if(direction.x==-1) {
			if(abs(location.x-anotherSheep.getLocation().x)<=0.1 && abs(location.y-anotherSheep.getLocation().y)<0.1 && abs(location.x-anotherSheep.getLocation().x)>abs(location.y-anotherSheep.getLocation().y)) {
				collide_type = SHEEP;
				collide_direction = LEFT;
			}
			else if(abs(location.x-anotherSheep.getLocation().x)<0.1 && abs(location.y-anotherSheep.getLocation().y)<=0.1 && abs(location.x-anotherSheep.getLocation().x)<abs(location.y-anotherSheep.getLocation().y)) {
				collide_type = SHEEP;
				if(anotherSheep.getDirection()==UP) {
					collide_direction = DOWN;
				}
				else {
					collide_direction = UP;
				}
			}
		}
		else if(direction.y==-1) {
			if(abs(location.x-anotherSheep.getLocation().x)<0.1 && abs(location.y-anotherSheep.getLocation().y)<=0.1 && abs(location.x-anotherSheep.getLocation().x)<abs(location.y-anotherSheep.getLocation().y)) {
				collide_type = SHEEP;
				collide_direction = DOWN;
			}
			else if(abs(location.x-anotherSheep.getLocation().x)<=0.1 && abs(location.y-anotherSheep.getLocation().y)<0.1 && abs(location.x-anotherSheep.getLocation().x)>abs(location.y-anotherSheep.getLocation().y)) {
				collide_type = SHEEP;
				if(anotherSheep.getDirection()==RIGHT) {
					collide_direction = LEFT;
				}
				else {
					collide_direction = RIGHT;
				}
			}
		}
		return;
	}
	void detectShepherdCollide(glm::vec2 mouse) {
		if(abs(location.x-mouse.x)<=0.1 && abs(location.y-mouse.y)<=0.1 && collide_type==NONE) {
			collide_type = SHEPHERD;
		}
		else if((abs(location.x-mouse.x)>=0.1 || abs(location.y-mouse.y)>=0.1) && collide_type==SHEPHERD_WAIT) {
			collide_type = NONE;
		}
		return;
	}
	void turn() {
		if(collide_type == SHEEP)
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
			collide_type = NONE;
			collide_direction = NONE;
		}
		else if(collide_type == SHEPHERD) {
			direction.x *= -1;
			direction.y *= -1;
			collide_type = SHEPHERD_WAIT;
		}
		return;
	}
	void speedUp() {
		speed *= 1.06f;
		return;
	}
};

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
	std::srand((unsigned)std::time(0));
	glm::vec2 mouse = glm::vec2(0.0f, 0.0f);

	//------------  game loop ------------

	auto previous_time = std::chrono::high_resolution_clock::now();
	bool should_quit = false;
	bool alive = true;
	float passed_time = 0;
	float level_time = 0;
	Sheep sheep1(0.0f, 0.5f);
	Sheep sheep2(-0.5f, 0.1f);
	Sheep sheep3(0.5f, 0.1f);
	Sheep sheep4(0.3f, -0.4f);
	Sheep sheep5(-0.3f, -0.4f);
	while (true) {
		static SDL_Event evt;
		while (SDL_PollEvent(&evt) == 1) {
			//handle input:
			if (evt.type == SDL_MOUSEMOTION) {
				mouse.x = (evt.motion.x + 0.5f) / float(config.size.x) * 2.0f - 1.0f;
				mouse.y = (evt.motion.y + 0.5f) / float(config.size.y) *-2.0f + 1.0f;
			} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
				if(!alive) {
					alive = true;
					passed_time = 0;
					level_time = 0;
					sheep1.initSheep();
					sheep2.initSheep();
					sheep3.initSheep();
					sheep4.initSheep();
					sheep5.initSheep();
				}
			} else if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE) {
				should_quit = true;
			} else if (evt.type == SDL_QUIT) {
				should_quit = true;
				break;
			}
		}
		if (should_quit) break;
		
		//when a round is still going
		if(alive) {
			auto current_time = std::chrono::high_resolution_clock::now();
			float elapsed = std::chrono::duration< float >(current_time - previous_time).count();
			previous_time = current_time;
			passed_time += elapsed;
			//adjust the speed of the sheep
			level_time += elapsed;
			if(level_time>0.7) {
				level_time -= 0.7f;
				sheep1.speedUp();
				sheep2.speedUp();
				sheep3.speedUp();
				sheep4.speedUp();
				sheep5.speedUp();
			}
			{ //update game state:
				//move to the next location
				sheep1.move();
				sheep2.move();
				sheep3.move();
				sheep4.move();
				sheep5.move();
				//check if any sheep is out of the area
				if(sheep1.outOfArea() || sheep2.outOfArea() || sheep3.outOfArea() || sheep4.outOfArea() || sheep5.outOfArea()) {
					alive = false;
					printf("\nSurvival Time: %f\n", passed_time);
				}
				//check collision with the shepherd
				sheep1.detectShepherdCollide(mouse);
				sheep2.detectShepherdCollide(mouse);
				sheep3.detectShepherdCollide(mouse);
				sheep4.detectShepherdCollide(mouse);
				sheep5.detectShepherdCollide(mouse);
				//check the collisions between sheep
				sheep1.detectSheepCollide(sheep2);
				sheep1.detectSheepCollide(sheep3);
				sheep1.detectSheepCollide(sheep4);
				sheep1.detectSheepCollide(sheep5);
				sheep2.detectSheepCollide(sheep1);
				sheep2.detectSheepCollide(sheep3);
				sheep2.detectSheepCollide(sheep4);
				sheep2.detectSheepCollide(sheep5);
				sheep3.detectSheepCollide(sheep1);
				sheep3.detectSheepCollide(sheep2);
				sheep3.detectSheepCollide(sheep4);
				sheep3.detectSheepCollide(sheep5);
				sheep4.detectSheepCollide(sheep1);
				sheep4.detectSheepCollide(sheep2);
				sheep4.detectSheepCollide(sheep3);
				sheep4.detectSheepCollide(sheep5);
				sheep5.detectSheepCollide(sheep1);
				sheep5.detectSheepCollide(sheep2);
				sheep5.detectSheepCollide(sheep3);
				sheep5.detectSheepCollide(sheep4);
				//sheep make the turns
				sheep1.turn();
				sheep2.turn();
				sheep3.turn();
				sheep4.turn();
				sheep5.turn();
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
		}
		SDL_GL_SwapWindow(window);
	}

	//------------  teardown ------------

	SDL_GL_DeleteContext(context);
	context = 0;

	SDL_DestroyWindow(window);
	window = NULL;

	return 0;
}
