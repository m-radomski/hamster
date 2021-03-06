/* date = September 3rd 2020 9:32 pm */

#ifndef HAMSTER_H
#define HAMSTER_H

struct Window
{
	GLFWwindow *ptr;
	int width;
	int height;
	
	Window() :
	ptr(NULL), width(1280), height(720)
	{ }
};

struct Button
{
	u8 down : 1;
	u8 last : 1;
	u8 pressed : 1;
};

struct CursorPosition
{
    f64 x;
    f64 y;
    f64 xold;
    f64 yold;
};

struct ProgramState
{
	Window window;
	Timer timer;
    
    bool in_editor;
	Button kbuttons[GLFW_KEY_LAST];
	Button mbuttons[GLFW_MOUSE_BUTTON_LAST];
    CursorPosition cursor_position;
    RenderContext ctx;
    
    Entity entities[8];
    u32 entities_len;
	EntityInstanced sponge;
	bool divide_sponge;
    EditorPickedEntity edit_picked;
};

void opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                           GLsizei length, const GLchar *message, const void *userParam);

static Window create_opengl_window();
static void window_debug_title(Window *window, Timer timer);

static void keyboard_button_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow *window, int key, int action, int mods);
static void buttons_update(Button *buttons, u32 length);

static Entity *state_push_entity(ProgramState *state);

#endif //HAMSTER_H
