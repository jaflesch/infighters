#ifndef HOHEX_HOGL_H
#define HOHEX_HOGL_H
#include "../common.h"
#undef internal

//#include <GL/gl.h>
#include <GL/glew.h>
#if 0
typedef char GLchar;
typedef int GLint;
typedef float GLfloat;
typedef __int64 GLsizeiptr;
typedef int* GLintptr;

#define GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB 0x8242

#define GL_NUM_EXTENSIONS 0x821D

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
#define GL_CLAMP_TO_EDGE 0x812F

#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_TEXTURE_LOD_BIAS	0x8501

#endif
#define HOGL_API extern "C"
#ifndef HOGL_IMPLEMENT
#define HOGL_END_API
#else
#define HOGL_END_API = 0
#endif

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_FLAGS_ARB                   0x2094

HOGL_API bool(__stdcall* wglSwapIntervalEXT_)(int interval) HOGL_END_API;
//HOGL_API HGLRC(WINAPI* wglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext, int *attribList) HOGL_END_API;

#if 0
HOGL_API void(__stdcall* glDebugMessageCallbackARB)(void(__stdcall *)(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void *), const void*) HOGL_END_API;

HOGL_API GLuint(__stdcall* glCreateProgram)() HOGL_END_API;
HOGL_API GLuint(__stdcall* glCreateShader)(GLenum shaderType) HOGL_END_API;
HOGL_API void(__stdcall* glShaderSource)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) HOGL_END_API;
HOGL_API void(__stdcall* glCompileShader)(GLuint shader) HOGL_END_API;
HOGL_API void(__stdcall* glGetShaderiv)(GLuint shader, GLenum pname, GLint* params) HOGL_END_API;
HOGL_API void(__stdcall* glGetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) HOGL_END_API;
HOGL_API void(__stdcall* glGetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog) HOGL_END_API;
HOGL_API void(__stdcall* glAttachShader)(GLuint program, GLuint shader) HOGL_END_API;
HOGL_API void(__stdcall* glLinkProgram)(GLuint program) HOGL_END_API;
HOGL_API void(__stdcall* glGetProgramiv)(GLuint program, GLenum pname, GLint* params) HOGL_END_API;
HOGL_API void(__stdcall* glValidateProgram)(GLuint program) HOGL_END_API;
HOGL_API void(__stdcall* glUseProgram)(GLuint program) HOGL_END_API;

HOGL_API void(__stdcall* glDeleteProgram)(GLuint program) HOGL_END_API;
HOGL_API void(__stdcall* glDeleteShader)(GLuint shader) HOGL_END_API;
HOGL_API void(__stdcall* glDetachShader)(GLuint program, GLuint shader) HOGL_END_API;

HOGL_API GLint(_stdcall* glGetUniformLocation)(GLuint program, const GLchar* name) HOGL_END_API;
HOGL_API void(_stdcall* glUniform1f)(GLint location, GLfloat v0) HOGL_END_API;
HOGL_API void(_stdcall* glUniform2f)(GLint location, GLfloat v0, GLfloat v1) HOGL_END_API;
HOGL_API void(_stdcall* glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) HOGL_END_API;
HOGL_API void(_stdcall* glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) HOGL_END_API;
HOGL_API void(_stdcall* glUniform1i)(GLint location, GLint v0) HOGL_END_API;
HOGL_API void(_stdcall* glUniform2i)(GLint location, GLint v0, GLint v1) HOGL_END_API;
HOGL_API void(_stdcall* glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2) HOGL_END_API;
HOGL_API void(_stdcall* glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) HOGL_END_API;

HOGL_API void(_stdcall* glUniform1fv)(GLint location, GLsizei count, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniform2fv)(GLint location, GLsizei count, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniform3fv)(GLint location, GLsizei count, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniform4fv)(GLint location, GLsizei count, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniform1iv)(GLint location, GLsizei count, const GLint* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniform2iv)(GLint location, GLsizei count, const GLint* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniform3iv)(GLint location, GLsizei count, const GLint* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniform4iv)(GLint location, GLsizei count, const GLint* value) HOGL_END_API;

HOGL_API void(_stdcall* glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) HOGL_END_API;
HOGL_API void(_stdcall* glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) HOGL_END_API;

HOGL_API void(__stdcall *glGenBuffers)(GLsizei n, GLuint* buffers) HOGL_END_API;
HOGL_API void(__stdcall* glBindBuffer)(GLenum target, GLuint buffer) HOGL_END_API;
HOGL_API void(__stdcall* glBufferData)(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) HOGL_END_API;
HOGL_API void(__stdcall* glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data) HOGL_END_API;
HOGL_API void(__stdcall* glDeleteBuffers)(GLsizei n, const GLuint* buffers) HOGL_END_API;
HOGL_API void(__stdcall* glEnableVertexAttribArray)(GLuint index) HOGL_END_API;
HOGL_API void(__stdcall* glDisableVertexAttribArray)(GLuint index) HOGL_END_API;
HOGL_API void(__stdcall* glGetBufferPointerv)(GLenum target, GLenum pname, GLvoid** params) HOGL_END_API;
HOGL_API void*(__stdcall* glMapBuffer)(GLenum target, GLenum access) HOGL_END_API;
HOGL_API GLboolean(__stdcall* glUnmapBuffer)(GLenum target) HOGL_END_API;
HOGL_API void(__stdcall* glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) HOGL_END_API;
HOGL_API GLint(__stdcall* glGetAttribLocation)(GLuint program, const GLchar* name) HOGL_END_API;

HOGL_API void(__stdcall* glBlendEquation)(GLenum mode) HOGL_END_API;
HOGL_API void(__stdcall* glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha) HOGL_END_API;
HOGL_API void(__stdcall* glBlendFuncSeparate)(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) HOGL_END_API;

HOGL_API void(__stdcall* glBindFramebuffer)(GLenum target, GLuint framebuffer) HOGL_END_API;
HOGL_API void(__stdcall* glBindRenderbuffer)(GLenum target, GLuint renderbuffer) HOGL_END_API;
HOGL_API void(__stdcall* glCreateFramebuffers)(GLsizei n, GLuint *ids) HOGL_END_API;
HOGL_API void(__stdcall* glCreateRenderbuffers)(GLsizei n, GLuint *renderbuffers) HOGL_END_API;
HOGL_API void(__stdcall* glDeleteFramebuffers)(GLsizei n, GLuint *framebuffers) HOGL_END_API;
HOGL_API void(__stdcall* glDeleteRenderbuffers)(GLsizei n, GLuint *renderbuffers) HOGL_END_API;
HOGL_API void(__stdcall* glGenFramebuffers)(GLsizei n, GLuint *ids) HOGL_END_API;
HOGL_API void(__stdcall* glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers) HOGL_END_API;

HOGL_API const GLubyte*(__stdcall* glGetStringi)(GLenum name, GLuint index) HOGL_END_API;

HOGL_API void(__stdcall* glBindVertexArray)(GLuint array) HOGL_END_API;
HOGL_API void(__stdcall* glDeleteVertexArrays)(GLsizei n, const GLuint* arrays) HOGL_END_API;
HOGL_API void(__stdcall* glGenVertexArrays)(GLsizei n, GLuint *arrays) HOGL_END_API;
HOGL_API void(__stdcall* glActiveTexture)(GLenum texture) HOGL_END_API;
HOGL_API void(__stdcall* glGenerateMipmap)(GLenum target) HOGL_END_API;

#endif
#ifdef HOGL_IMPLEMENT

#define HOGL_PIXELFORMAT_ERROR 1

static char* error_codes[] = { 
	"", 
	"Error creating context pixel descriptor.\n",  
};
/*
void init_gl_extensions()
{
	wglSwapIntervalEXT = (bool(__stdcall*)(int)) wglGetProcAddress("wglSwapIntervalEXT");

	glCreateProgram = (GLuint(__stdcall*)()) wglGetProcAddress("glCreateProgram");
	glCreateShader = (GLuint(__stdcall*)(GLenum)) wglGetProcAddress("glCreateShader");
	glShaderSource = (void(__stdcall*) (GLuint, GLsizei, const GLchar**, const GLint*)) wglGetProcAddress("glShaderSource");
	glCompileShader = (void(__stdcall*)(GLuint)) wglGetProcAddress("glCompileShader");
	glGetShaderiv = (void(__stdcall*)(GLuint, GLenum, GLint*)) wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (void(__stdcall*)(GLuint, GLsizei, GLsizei*, GLchar*)) wglGetProcAddress("glGetShaderInfoLog");
	glGetProgramInfoLog = (void(__stdcall*)(GLuint, GLsizei, GLsizei*, GLchar*)) wglGetProcAddress("glGetProgramInfoLog");
	glAttachShader = (void(__stdcall*)(GLuint, GLuint)) wglGetProcAddress("glAttachShader");
	glLinkProgram = (void(__stdcall*)(GLuint)) wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (void(__stdcall*)(GLuint, GLenum, GLint*)) wglGetProcAddress("glGetProgramiv");
	glValidateProgram = (void(__stdcall*)(GLuint)) wglGetProcAddress("glValidateProgram");
	glUseProgram = (void(__stdcall*)(GLuint)) wglGetProcAddress("glUseProgram");

	glDeleteProgram = (void(__stdcall*)(GLuint)) wglGetProcAddress("glDeleteProgram");
	glDeleteShader = (void(__stdcall*)(GLuint)) wglGetProcAddress("glDeleteShader");
	glDetachShader = (void(__stdcall*)(GLuint, GLuint)) wglGetProcAddress("glDetachShader");

	glGetUniformLocation = (GLint(_stdcall*)(GLuint program, const GLchar* name)) wglGetProcAddress("glGetUniformLocation");
	glUniform1f = (void(_stdcall*)(GLint, GLfloat)) wglGetProcAddress("glUniform1f");
	glUniform2f = (void(_stdcall*)(GLint, GLfloat, GLfloat)) wglGetProcAddress("glUniform2f");
	glUniform3f = (void(_stdcall*)(GLint, GLfloat, GLfloat, GLfloat)) wglGetProcAddress("glUniform3f");
	glUniform4f = (void(_stdcall*)(GLint, GLfloat, GLfloat, GLfloat, GLfloat)) wglGetProcAddress("glUniform4f");
	glUniform1i = (void(_stdcall*)(GLint, GLint)) wglGetProcAddress("glUniform1i");
	glUniform2i = (void(_stdcall*)(GLint, GLint, GLint)) wglGetProcAddress("glUniform2i");
	glUniform3i = (void(_stdcall*)(GLint, GLint, GLint, GLint)) wglGetProcAddress("glUniform3i");
	glUniform4i = (void(_stdcall*)(GLint, GLint, GLint, GLint, GLint)) wglGetProcAddress("glUniform4i");

	glUniform1fv = (void(_stdcall*)(GLint, GLsizei, const GLfloat*)) wglGetProcAddress("glUniform1fv");
	glUniform2fv = (void(_stdcall*)(GLint, GLsizei, const GLfloat*)) wglGetProcAddress("glUniform2fv");
	glUniform3fv = (void(_stdcall*)(GLint, GLsizei, const GLfloat*)) wglGetProcAddress("glUniform3fv");
	glUniform4fv = (void(_stdcall*)(GLint, GLsizei, const GLfloat*)) wglGetProcAddress("glUniform4fv");
	glUniform1iv = (void(_stdcall*)(GLint, GLsizei, const GLint*)) wglGetProcAddress("glUniform1iv");
	glUniform2iv = (void(_stdcall*)(GLint, GLsizei, const GLint*)) wglGetProcAddress("glUniform2iv");
	glUniform3iv = (void(_stdcall*)(GLint, GLsizei, const GLint*)) wglGetProcAddress("glUniform3iv");
	glUniform4iv = (void(_stdcall*)(GLint, GLsizei, const GLint*)) wglGetProcAddress("glUniform4iv");

	glUniformMatrix2fv = (void(_stdcall*)(GLint, GLsizei, GLboolean, const GLfloat*)) wglGetProcAddress("glUniformMatrix2fv");
	glUniformMatrix3fv = (void(_stdcall*)(GLint, GLsizei, GLboolean, const GLfloat*)) wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv = (void(_stdcall*)(GLint, GLsizei, GLboolean, const GLfloat*)) wglGetProcAddress("glUniformMatrix4fv");
	glUniformMatrix2x3fv = (void(_stdcall*)(GLint, GLsizei, GLboolean, const GLfloat*)) wglGetProcAddress("glUniformMatrix2x3fv");
	glUniformMatrix3x2fv = (void(_stdcall*)(GLint, GLsizei, GLboolean, const GLfloat*)) wglGetProcAddress("glUniformMatrix3x2fv");
	glUniformMatrix2x4fv = (void(_stdcall*)(GLint, GLsizei, GLboolean, const GLfloat*)) wglGetProcAddress("glUniformMatrix2x4fv");
	glUniformMatrix4x2fv = (void(_stdcall*)(GLint, GLsizei, GLboolean, const GLfloat*)) wglGetProcAddress("glUniformMatrix4x2fv");
	glUniformMatrix3x4fv = (void(_stdcall*)(GLint, GLsizei, GLboolean, const GLfloat*)) wglGetProcAddress("glUniformMatrix3x4fv");
	glUniformMatrix4x3fv = (void(_stdcall*)(GLint, GLsizei, GLboolean, const GLfloat*)) wglGetProcAddress("glUniformMatrix4x3fv");

	glGenBuffers = (void(__stdcall *)(GLsizei, GLuint*)) wglGetProcAddress("glGenBuffers");
	glBindBuffer = (void(__stdcall*)(GLenum, GLuint)) wglGetProcAddress("glBindBuffer");
	glBufferData = (void(__stdcall*)(GLenum, GLsizeiptr, const GLvoid*, GLenum)) wglGetProcAddress("glBufferData");
	glBufferSubData = (void(__stdcall*)(GLenum, GLintptr, GLsizeiptr, const GLvoid*)) wglGetProcAddress("glBufferSubData");
	glDeleteBuffers = (void(__stdcall*)(GLsizei, const GLuint*)) wglGetProcAddress("glDeleteBuffers");
	glEnableVertexAttribArray = (void(__stdcall*)(GLuint)) wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (void(__stdcall*)(GLuint)) wglGetProcAddress("glDisableVertexAttribArray");
	glGetBufferPointerv = (void(__stdcall*)(GLenum, GLenum, GLvoid**)) wglGetProcAddress("glGetBufferPointerv");
	glMapBuffer = (void*(__stdcall*)(GLenum, GLenum)) wglGetProcAddress("glMapBuffer");
	glUnmapBuffer = (GLboolean(__stdcall*)(GLenum)) wglGetProcAddress("glUnmapBuffer");
	glDeleteBuffers = (void(__stdcall*)(GLsizei, const GLuint*)) wglGetProcAddress("glDeleteBuffers");
	glVertexAttribPointer = (void(__stdcall*)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*)) wglGetProcAddress("glVertexAttribPointer");
	glGetAttribLocation = (GLint(__stdcall*)(GLuint, const GLchar*)) wglGetProcAddress("glGetAttribLocation");

	glBlendEquation = (void(__stdcall*)(GLenum)) wglGetProcAddress("glBlendEquation");
	glBlendEquationSeparate = (void(__stdcall*)(GLenum, GLenum)) wglGetProcAddress("glBlendEquationSeparate");
	glBlendFuncSeparate = (void(__stdcall*)(GLenum, GLenum, GLenum, GLenum)) wglGetProcAddress("glBlendFuncSeparate");

	glBindFramebuffer = (void(__stdcall*)(GLenum, GLuint)) wglGetProcAddress("glBindFramebuffer");
	glBindRenderbuffer = (void(__stdcall*)(GLenum, GLuint)) wglGetProcAddress("glBindRenderbuffer");
	glCreateFramebuffers = (void(__stdcall*)(GLsizei, GLuint*)) wglGetProcAddress("glCreateFramebuffers");
	glCreateRenderbuffers = (void(__stdcall*)(GLsizei, GLuint*)) wglGetProcAddress("glCreateRenderbuffers");
	glDeleteFramebuffers = (void(__stdcall*)(GLsizei, GLuint*)) wglGetProcAddress("glDeleteFramebuffers");
	glDeleteRenderbuffers = (void(__stdcall*)(GLsizei, GLuint*)) wglGetProcAddress("glDeleteRenderbuffers");
	glGenFramebuffers = (void(__stdcall*)(GLsizei, GLuint*)) wglGetProcAddress("glGenFramebuffers");
	glGenRenderbuffers = (void(__stdcall*)(GLsizei, GLuint*)) wglGetProcAddress("glGenRenderbuffers");

	glGetStringi = (const GLubyte*(__stdcall*)(GLenum, GLuint)) wglGetProcAddress("glGetStringi");

	glBindVertexArray = (void(__stdcall*)(GLuint)) wglGetProcAddress("glBindVertexArray");
	glDeleteVertexArrays = (void(__stdcall*)(GLsizei, const GLuint*)) wglGetProcAddress("glDeleteVertexArrays");
	glGenVertexArrays = (void(__stdcall*)(GLsizei, GLuint *)) wglGetProcAddress("glGenVertexArrays");
	glActiveTexture = (void(__stdcall*)(GLenum texture)) wglGetProcAddress("glActiveTexture");
	glGenerateMipmap = (void(__stdcall*)(GLenum target)) wglGetProcAddress("glGenerateMipmap");

	glDebugMessageCallbackARB = (void(__stdcall*)(void(__stdcall*)(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *, const void*), const void*)) wglGetProcAddress("glDebugMessageCallbackARB");
}
*/
#if 0
int init_opengl(HWND window_handle, HDC* device_context, HGLRC* rendering_context, int major_ver, int minor_ver)
{
	int PixelFormat;
	*device_context = GetDC(window_handle);

	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 24;
	pfd.iPixelType = PFD_TYPE_RGBA;

	PixelFormat = ChoosePixelFormat(*device_context, &pfd);
	if (!SetPixelFormat(*device_context, PixelFormat, &pfd)) {
		return HOGL_PIXELFORMAT_ERROR;
	}

	HGLRC temp_context = wglCreateContext(*device_context);
	BOOL error = wglMakeCurrent(*device_context, temp_context);

	//int attribs[] =
	//{
	//	WGL_CONTEXT_MAJOR_VERSION_ARB, major_ver,
	//	WGL_CONTEXT_MINOR_VERSION_ARB, minor_ver,
	//	WGL_CONTEXT_FLAGS_ARB, 0,
	//	0
	//};
	//wglCreateContextAttribsARB = (HGLRC(WINAPI*)(HDC, HGLRC, int *))wglGetProcAddress("wglCreateContextAttribsARB");
	//wglMakeCurrent(NULL, NULL);
	//wglDeleteContext(temp_context);
	//*rendering_context = wglCreateContextAttribsARB(*device_context, 0, attribs);
	//wglMakeCurrent(*device_context, *rendering_context);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		exit(-1);
	}
	//init_gl_extensions();
	wglSwapIntervalEXT_ = (bool(__stdcall*)(int)) wglGetProcAddress("wglSwapIntervalEXT");

	return 0;
}
#endif

#endif // HOGL_IMPLEMENT
#endif // HOHEX_HOGL_H
