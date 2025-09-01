#include <iostream>
#include <vector>
#include "raylib.h"


const int screenWidth = 1280;
const int screenHeight = 720;
const float diagonal = sqrtf((screenWidth * screenWidth) + ((screenHeight / 2) * (screenHeight / 2)));
const int FPS = 60;

class Ripple {
private:
	float growRate = 0.0f;
	Vector2 position = { -100.0f, -100.0f };
	float energy = 10.0f;
	float radius = 0.0f;

	bool alive = true;
	bool reflected = false;
public:

	Ripple(Vector2 pos, float rate) {
		position = pos;
		growRate = rate;
	}
	// reflected ripple constructor
	Ripple(Vector2 pos, float rate, float radius, float energy) {
		position = pos;
		growRate = rate;
		this->radius = radius;
		this->energy = energy;
	}

	void Update() {
		radius += growRate;
		//printf("Thickness: %f \n", energy);
		energy -= 0.01f;
		//if (radius > screenWidth*8) {
		if ((position.x + radius - growRate > diagonal && position.x - radius + growRate < -diagonal) &&
			(position.y + radius - growRate > diagonal && position.y - radius + growRate < -diagonal)) {
			alive = false;
			return;
		}
	}

	bool IsDead() {
		if (energy <= 1.5f) return true;
		return !alive;
	}

	void Draw() {
		DrawRing(position, radius - energy, radius, 0.0f, 360.0f, 256, BLACK);
	}

	bool TouchedEdge() {

		if ((position.x + radius - growRate < screenWidth) && (position.x + radius >= screenWidth)) return true;
			/*std::cout << "ring with centre: " << position.x << " " << position.y << " has collided with the right edge" << "\n";
			return true;
		}*/
		if ((position.y + radius - growRate < screenHeight) && (position.y + radius >= screenHeight)) return true;

		if ((position.x - radius + growRate > 0.0f) && (position.x - radius <= 0.0f)) return true;
		if ((position.y - radius + growRate > 0.0f) && (position.y - radius <= 0.0f)) return true;


		return false;
	}

	Ripple ReflectedRipple() {
		if ((position.x + radius - growRate < screenWidth) && (position.x + radius >= screenWidth)) return Ripple({ screenWidth + radius, position.y }, growRate, radius, energy-2.0f);
		if ((position.y + radius - growRate < screenHeight) && (position.y + radius >= screenHeight)) return Ripple({ position.x, screenHeight + radius }, growRate, radius, energy-2.0f);

		if ((position.x - radius + growRate > 0.0f) && (position.x - radius <= 0.0f)) return Ripple({ -radius, position.y }, growRate, radius, energy-2.0f);
		if ((position.y - radius + growRate > 0.0f) && (position.y - radius <= 0.0f)) return Ripple({ position.x, -radius }, growRate, radius, energy-2.0f);
	}

};


int main() {
	

	InitWindow(screenWidth, screenHeight, "Ripple");
	SetTargetFPS(FPS);

	std::vector<Ripple> ripples;

	float defGrowRate = 5.0f;

	// Main Loop
    while (!WindowShouldClose()) {

		// Update

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			ripples.push_back(Ripple(GetMousePosition(), defGrowRate));
		}

		for (auto& ripple : ripples) { 
			ripple.Update();
			if (ripple.TouchedEdge()) {
				ripples.push_back(ripple.ReflectedRipple());
			}
		}
		
		for (auto it = ripples.begin(); it != ripples.end(); ) {
			if (it->IsDead()) {
				it = ripples.erase(it);
			}
			else {
				++it;
			}
		}


		// Draw

        BeginDrawing();

		ClearBackground(RAYWHITE);

		for (auto& ripple : ripples) ripple.Draw();


        EndDrawing();
    }

	// Close window and OpenGL
	CloseWindow();
	return 0;
}