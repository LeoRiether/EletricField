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

// Linked list for all particles
// because why not
typedef struct PNode {
	PNode* next;
	Particle* self;	
} PNode;

// Declaring here, but initializing real values in main()
// (because the position is tied to SCREEN_W and SCREEN_H, 
//  which are not initialized here)
//Particle a, b, c;
PNode* pnodeHead;

// Eletric field function
Vector fn(int j, int i, float& d, float& theta, float& val) {
	Vector p = Vector(j, i);
//	return p;

	Vector r;

	PNode* node;
	node = pnodeHead;
	while (true) {
		Vector a = node->self->getFieldAt(p, d, theta, val);
		r = r + a;
		if (node->next == NULL) {
			break;
		} else {
			node = node->next;
		}
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
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 500, 500, 0, 0);
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    
    set_close_button_callback(on_close_callback);
    
    // Initializing particles
    Particle a(Vector(SMX-100.0f, SMY), 5e4);
    Particle b(Vector(SMX+100.0f, SMY), 5e4);
    Particle c(Vector(SMX, SMY-75.0f), -5e4);
    
    // Initializing nodes
    PNode cnode; cnode.self = &c; cnode.next = NULL;
    PNode bnode; bnode.self = &b; bnode.next = &cnode;
    PNode anode; anode.self = &a; anode.next = &bnode;
    pnodeHead = &anode;
    
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