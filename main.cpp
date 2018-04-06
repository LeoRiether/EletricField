#include <allegro.h>

#ifndef CMATH_H
#define CMATH_H
#include <cmath>
#endif

#ifndef UTIL_H
#define UTIL_H
#include "util.h"
#endif

#ifndef VECTOR_H
#define VECTOR_H
#include "vector.h"
#endif

// Nice naming, me
#include <vector>

#ifndef PARTICLE_H
#define PARTICLE_H
#include "particle.h"
#endif

#define SMX SCREEN_W / 2.0f
#define SMY SCREEN_H / 2.0f

#define LINE_SPACE 20
#define LINE_COLOR 0xa9a9a9

// Closing logic
bool needs_exit = false;
void on_close_callback() {
	needs_exit = true;
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

// Declaring here, but initializing real values in main()
// (because the position is tied to SCREEN_W and SCREEN_H, 
//  which are not initialized here)
std::vector<Particle> plist;

// Eletric field function
Vector fn(int j, int i, float& d, float& theta, float& val) {
	Vector p = Vector(j, i);
	//p.x -= 250;
	//p.y -= 250;
	//return p;

	Vector r, a;

	std::vector<Particle>::iterator part = plist.begin();
	for( ; part != plist.end(); part++) {
		a = (*part).getFieldAt(p, d, theta, val);
		r = r + a;
	}

	// Returns the sum of all fields	
	return r;
}

// Draw the function
bool lines = false;
void draw(BITMAP* buffer) {
	Vector out(0,0);
	float d, theta, val;
	for(int i = 0; i < SCREEN_H; i++) {
		for(int j = 0; j < SCREEN_W; j++) {
			out = fn(j, i, d, theta, val);
			putpixel(buffer, j, i, get_col(out.x, out.y));
		}
	}
	
	float i1, j1;
	if (lines) {
		for(int i = 0; i < SCREEN_H; i += LINE_SPACE) {
			for(int j = 0; j < SCREEN_W; j += LINE_SPACE) {
				out = fn(j, i, d, theta, val);
				float c = (LINE_SPACE/2.0f) / out.abs();
				i1 = i+out.y*c;
				j1 = j+out.x*c;
				fastline(buffer, j, i, j1, i1, LINE_COLOR);
				rectfill(buffer, j1-.75, i1-.75, j1+.75, i1+.75, LINE_COLOR);
				//circlefill(buffer, j1, i1, 1, 0xa9a9a9);
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
    install_mouse();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 500, 500, 0, 0);
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    
    set_close_button_callback(on_close_callback);
    
    // Initializing particles
    plist.push_back(Particle(Vector(SMX-100.0f, SMY), 10e4));
    plist.push_back(Particle(Vector(SMX+100.0f, SMY), -10e4));
    //plist.push_back(Particle(Vector(SMX, SMY-75.0f), -5e4));
    //plist.push_back(Particle(Vector(SMX, SMY+150.0f), 10e4));
    
    Particle* a = &plist[0];
    Particle* b = &plist[1];
    
    show_os_cursor(1);
    
    bool needs_draw = true;
    while(!needs_exit)
    {
    	
    	if(key[KEY_PLUS_PAD]) {
			a->pos.x += 5;
			b->pos.x -= 5;
			needs_draw = true;
		} else if(key[KEY_MINUS_PAD]) {
			a->pos.x -= 5;
			b->pos.x += 5;
			needs_draw = true;
		} else if(key[KEY_V]) {  // Toggles vectors 
			lines = !lines;
			needs_draw = true;
		} else if(key[KEY_L]) {  // Limpa a lista de partículas
			plist.clear();
			needs_draw = true;
		}
    	
        if (needs_draw) {
			clear(buffer);
			draw(buffer);
	        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);	
			needs_draw = false;	
		}
		
		if (mouse_b & 1) {
			plist.push_back(Particle(Vector(mouse_x, mouse_y), 5e4));
			needs_draw = true;
		} else if (mouse_b & 2) {
			plist.push_back(Particle(Vector(mouse_x, mouse_y), -5e4));
			needs_draw = true;
		}

        rest(10);
    }

    allegro_exit();
    return 0;
}
END_OF_MAIN();