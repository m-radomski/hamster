#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// TODO: As a learning expierience try to implement png reading yourself.
// PNG reading is quite a chore, just because of the zlib compressed IDAT chunks,
// maybe just use zlib and decode the rest yourself.
#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

#include "hamster_math.cpp"
#include "hamster_util.cpp"
#include "hamster_graphics.cpp"

struct Window
{
	GLFWwindow *ptr;
	int width;
	int height;
	
	Window() :
	ptr(NULL), width(800), height(600)
	{ }
};

struct Button
{
	u8 down : 1;
	u8 last : 1;
	u8 pressed : 1;
};

struct ProgramState
{
	Window window;
	
	Button kbuttons[GLFW_KEY_LAST];
	Button mbuttons[GLFW_MOUSE_BUTTON_LAST];
};

void
opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
					  GLsizei length, const GLchar *message, const void *userParam)
{
	(void)source;
	(void)type;
	(void)id;
	(void)severity;
	(void)length;
	(void)userParam;
	
	printf("OpenGL Error: %s\n", message);
	assert(false);
}

static Window
create_opengl_window()
{
	assert(glfwInit());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	Window window;
	window.ptr = glfwCreateWindow(window.width, window.height, "hamster", NULL, NULL);
	glfwMakeContextCurrent(window.ptr);
	assert(glewInit() == 0); // That means no errors
	
	glfwSetInputMode(window.ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	return window;
}

// TODO: I don't know if this keyboard button callback is going to stay because
// it's really unresponsive. There is a delay that you can definitley feel from
// pressing a button and it registering as being down. Might switch to polling
// all the necesseray keys for greater smoothness. -radomski
static void
keyboard_button_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	assert(key != GLFW_KEY_UNKNOWN);
	ProgramState *state = (ProgramState *)glfwGetWindowUserPointer(window);
	(void)mods; // NOTE: I guess we would use it somewhere??
	(void)scancode; // That is unused.
	
	if(action == GLFW_PRESS) {
		state->kbuttons[key].down = true;
	} else if(action == GLFW_RELEASE) {
		state->kbuttons[key].down = false;
	}
}

static void
mouse_button_callback(GLFWwindow *window, int key, int action, int mods)
{
	assert(key != GLFW_KEY_UNKNOWN);
	ProgramState *state = (ProgramState *)glfwGetWindowUserPointer(window);
	(void)mods; // NOTE: I guess we would use it somewhere??
	
	if(action == GLFW_PRESS) {
		state->mbuttons[key].down = true;
	} else {
		state->mbuttons[key].down = false;
	}
}

static void
buttons_update(Button *buttons, u32 length)
{
	for(u32 i = 0; i < length; i++)
	{
		buttons[i].pressed = buttons[i].down && !buttons[i].last;
		buttons[i].last = buttons[i].down;
	}
}

int main()
{
	ProgramState state = {};
	state.window = create_opengl_window();
	glfwSetWindowUserPointer(state.window.ptr, &state);
	glfwSetKeyCallback(state.window.ptr, keyboard_button_callback);
	glfwSetMouseButtonCallback(state.window.ptr, mouse_button_callback);
	
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(opengl_error_callback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	
	Model obj_model = model_create_from_obj("data/model.obj");
	Model floor_model = model_create_debug_floor();
	GLuint basic_program = program_create(main_vertex_shader_src, main_fragment_shader_src);
	GLuint line_program = program_create(main_vertex_shader_src, line_fragment_shader_src);
	UIElement crosshair = ui_element_create(Vec2(0.0f, 0.0f), Vec2(0.1f, 0.1f),
											"data/crosshair.png");
	Entity monkey = {};
	monkey.position = Vec3(0.0f, 0.0f, 0.0f);
	monkey.size = Vec3(1.0f, 1.0f, 1.0f);
	monkey.model = &obj_model;
	
	Entity floor = {};
	floor.position = Vec3(0.0f, -2.0f, 0.0f);
	floor.size = Vec3(3.0f, 1.0f, 3.0f);
	floor.model = &floor_model;
	
	Camera camera = {};
	camera.position = Vec3(0.0f, 0.0f, 3.0f);
	camera.yaw = asinf(-1.0f); // Where we look
	camera_calculate_vectors(&camera);
	
	// const Vec3 world_up = Vec3(0.0f, 1.0f, 0.0f); // Don't repeat yourself
	Mat4 lookat = look_at(camera.front, camera.position, camera.up);
	f32 aspect_ratio = (f32)state.window.width/(f32)state.window.height;
	Mat4 proj = create_perspective(aspect_ratio, 90.0f, 0.1f, 100.0f);
	Mat4 model = Mat4(1.0f);
	
	glEnable(GL_DEPTH_TEST);
	Line ray_line = {};
	GLuint line_vao, line_vbo;
	glGenVertexArrays(1, &line_vao);
	glBindVertexArray(line_vao);
	glGenBuffers(1, &line_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, line_vbo);
	
	glfwSwapInterval(0);
	
	f64 xmouse, ymouse;
	f64 xmouseold, ymouseold;
	glfwGetCursorPos(state.window.ptr, &xmouse, &ymouse);
	while(!glfwWindowShouldClose(state.window.ptr))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		xmouseold = xmouse;
		ymouseold = ymouse;
		glfwGetCursorPos(state.window.ptr, &xmouse, &ymouse);
		camera_mouse_moved(&camera, xmouse - xmouseold, ymouse - ymouseold);
		
		buttons_update(state.kbuttons, ARRAY_LEN(state.kbuttons));
		buttons_update(state.mbuttons, ARRAY_LEN(state.mbuttons));
		
		lookat = look_at(add(camera.front, camera.position), camera.position, camera.up);
		//model = rot_around_vec(Mat4(1.0), glfwGetTime(), Vec3(0.0f, 1.0f, 0.0f));
		model = Mat4(1.0f);
		
		if(state.kbuttons[GLFW_KEY_M].pressed && (obj_model.flags & MODEL_FLAGS_GOURAUD_SHADED))
		{
			model_mesh_normals_shade(&obj_model);
		}
		
		if(state.kbuttons[GLFW_KEY_N].pressed && obj_model.flags & MODEL_FLAGS_MESH_NORMALS_SHADED)
		{
			model_gouraud_shade(&obj_model);
		}
		
		float movement_scalar = 0.01f;
		if(state.kbuttons[GLFW_KEY_W].down) {
			camera.position = add(camera.position, scale(camera.front, movement_scalar));
		} if(state.kbuttons[GLFW_KEY_S].down) {
			camera.position = sub(camera.position, scale(camera.front, movement_scalar));
		} if(state.kbuttons[GLFW_KEY_D].down) {
			camera.position = add(camera.position, scale(camera.right, movement_scalar));
		} if(state.kbuttons[GLFW_KEY_A].down) {
			camera.position = sub(camera.position, scale(camera.right, movement_scalar));
		}
		
		if(state.kbuttons[GLFW_KEY_SPACE].pressed)
		{
			// TODO: Models should get a bounding box
			Vec3 dir = camera.front;
			bool hit = ray_intersect_model(camera.position, dir, &obj_model);
			ray_line = line_from_direction(camera.position, camera.front, 10.0f);
			
			printf("Hit: %s\n", (hit ? "True" : "False"));
		}
		
		glBindVertexArray(line_vao);
		glBindBuffer(GL_ARRAY_BUFFER, line_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ray_line), &ray_line, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glUseProgram(line_program);
		opengl_set_uniform(basic_program, "view", lookat);
		opengl_set_uniform(basic_program, "proj", proj);
		opengl_set_uniform(basic_program, "model", model);
		glDrawArrays(GL_LINES, 0, 2);
		glUseProgram(0);
		
		glUseProgram(basic_program);
		opengl_set_uniform(basic_program, "view", lookat);
		opengl_set_uniform(basic_program, "proj", proj);
		
		opengl_set_uniform(basic_program, "view_pos", camera.position);
		
		opengl_set_uniform(basic_program, "spotlight.position", camera.position);
		opengl_set_uniform(basic_program, "spotlight.direction", camera.front);
		opengl_set_uniform(basic_program, "spotlight.cutoff", cosf(to_radians(12.5f)));
		opengl_set_uniform(basic_program, "spotlight.outer_cutoff", cosf(to_radians(16.5f)));
		opengl_set_uniform(basic_program, "spotlight.ambient_component", Vec3(0.1f, 0.1f, 0.1f));
		opengl_set_uniform(basic_program, "spotlight.diffuse_component", Vec3(0.8f, 0.8f, 0.8f));
		opengl_set_uniform(basic_program, "spotlight.specular_component", Vec3(1.0f, 1.0f, 1.0f));
		opengl_set_uniform(basic_program, "spotlight.atten_const", 1.0f);
		opengl_set_uniform(basic_program, "spotlight.atten_linear", 0.09f);
		opengl_set_uniform(basic_program, "spotlight.atten_quad", 0.032f);
		
		opengl_set_uniform(basic_program, "direct_light.direction", Vec3(0.0f, -1.0f, 0.0f));
		opengl_set_uniform(basic_program, "direct_light.ambient_component", Vec3(0.05f, 0.05f, 0.05f));
		opengl_set_uniform(basic_program, "direct_light.diffuse_component", Vec3(0.2f, 0.2f, 0.2f));
		opengl_set_uniform(basic_program, "direct_light.specular_component", Vec3(0.4f, 0.4f, 0.4f));
		
		entity_draw(monkey, basic_program);
		entity_draw(floor, basic_program);
		
		ui_element_draw(crosshair);
		
		glfwSwapBuffers(state.window.ptr);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	return 0;
}
