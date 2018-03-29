#include <allegro.h>

#ifndef CMATH_H
#define CMATH_H
#include <cmath>
#endif

#include "vector.h"

#define SMX SCREEN_W / 2.0f
#define SMY SCREEN_H / 2.0f

#define LINE_SPACE 20

// Closing logic
bool needs_exit = false;
void on_close_callback() {
	needs_exit = true;
} 



// Distance between two vectors
float dist(const Vector& a, const Vector& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrt(dx*dx + dy*dy); 
}

// Distance between two vectors, but with different arguments
float ndist(const int& ax, const int& ay, const int& bx, const int& by) {
	int dx = ax - bx;
	int dy = ay - by;
	return sqrt(dx*dx + dy*dy); 
}

// Get output color
// aka which color in the color wheel
int get_col(int j, int i) {
	float h = atan2(i, j) * (180.0f / M_PI); // Angle is hue
	float v = tanh(ndist(j, i, 0, 0)/20.0f); // tanh(Distance/20) is value 
	
	// Converts HSV to RGB
	int r, g, b;
	hsv_to_rgb(h, 0.9f, v, &r, &g, &b);
	return makecol(r, g, b);
}

// Struct for the particles
typedef struct {
	Vector pos;
	float q;
} Particle;

// Declaring here, but initializing real values in main()
Particle a { Vector(0,0), 0 };
Particle b { Vector(0,0), 0 };

// Eletric field function
Vector fn(int j, int i) {
	Vector p = Vector(j, i);
//	return p;
	
	float d, theta, val;
	
	// Field for particle A
	d = dist(a.pos, p);
	theta = atan2(p.y-a.pos.y, p.x-a.pos.x);
	val = a.q / (d*d);  
	Vector field_a(val * cos(theta), val * sin(theta));
	
	// Field for particle B
	d = dist(b.pos, p);
	theta = atan2(p.y-b.pos.y, p.x-b.pos.x);
	val = b.q / (d*d);  
	Vector field_b(val * cos(theta), val * sin(theta));

	// Returns the sum of all fields	
	return field_a + field_b;
}

// Draw the function
bool lines = false;
void draw(BITMAP* buffer) {
	Vector out(0,0);
	for(int i = 0; i < SCREEN_H; i++) {
		for(int j = 0; j < SCREEN_W; j++) {
			out = fn(j, i);
			putpixel(buffer, j, i, get_col(out.x, out.y));
		}
	}
	if (lines) {
		for(int i = 0; i < SCREEN_H; i += LINE_SPACE) {
			for(int j = 0; j < SCREEN_W; j += LINE_SPACE) {
				out = fn(j, i);
				float c = (LINE_SPACE/2.0f) / out.abs();
				fastline(buffer, j, i, j+out.x*c, i+out.y*c, 0xa9a9a9);
			}
		}
	}
}


int main()
{
    BITMAP *buffer = NULL;

	// Initializing stuff
    allegro_init();
    set_color_depth(32);
    install_keyboard();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 500, 500, 0, 0);
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    
    set_close_button_callback(on_close_callback);
    
    a.pos = Vector(SMX-100.0f, SMY-00.0f);
    a.q = 5e4;
    b.pos = Vector(SMX+100.0f, SMY);
    b.q = -5e4;
    
    bool needs_draw = true;
    

    while(!needs_exit)
    {
    	if(key[KEY_PLUS_PAD]) {
			a.pos.x += 5;
			b.pos.x -= 5;
			needs_draw = true;
		} else if(key[KEY_MINUS_PAD]) {
			a.pos.x -= 5;
			b.pos.x += 5;
			needs_draw = true;
		} else if(key[KEY_L]) {  // Toggles lines 
			lines = !lines;
			needs_draw = true;
		}
    	
        if (needs_draw) {
			clear(buffer);
			draw(buffer);
	        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);	
			needs_draw = false;	
		}

        rest(10);
    }

    allegro_exit();
    return 0;
}
END_OF_MAIN();