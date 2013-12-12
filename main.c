#include <GL/glew.h>
#include <GL/glfw.h>
#include <string.h>

double view_center[] = {0,0};
double view_size = 2;
double last_time = 0.0;

const GLchar *frag_src = "\
#version 110 \n\
varying vec2 c; \n\
vec2 z; \n\
vec3 color; \n\
void main() { \n\
    int i = 0; \n\
    while( z[0]*z[0] + z[1] * z[1] < 4.0 && i++ < 100 ) { \n\
        z = vec2(c[0] + (z[0] * z[0] - z[1] * z[1]),c[1] + ( 2.0 * z[0] * z[1]) ); \n\
    } \n\
    float shade = (float(i) - log(log(z[0]*z[0]+z[1]*z[1])) / log(4.0)) / 100.0 * 768.0; \n\
    if (shade >= 512.0) { \n\
        color[0] = shade - 512.0; \n\
        color[1] = 255.0 - color[1]; \n\
    } else if (shade >= 256.0) { \n\
        color[2] = shade - 255.0; \n\
        color[1] = 255.0 - color[2]; \n\
    } else { \n\
        color[2] = shade; \n\
    } \n\
    gl_FragColor = vec4(color[0]/255.0,color[1]/255.0,color[2]/255.0,1.0); \n\
}";

const GLchar *vert_src = "\
#version 110 \n\
attribute vec2 position; \n\
varying vec2 c; \n\
void main() { \n\
    gl_Position = vec4(position, 0.0, 1.0); \n\
    c = gl_MultiTexCoord0.xy; \n\
}";

int main(int argc, char** argv) {
    glfwInit();
    glfwOpenWindow(1024, 1024, 8, 8, 8, 0, 0, 0, GLFW_WINDOW);
    glfwSetWindowTitle("Mandelbrot Renderer");
    glewInit();

    const GLchar** vertex_source = &vert_src;
    const GLchar** fragment_source = &frag_src;

    GLuint vertex_shader, fragment_shader, program;
    GLint vert_len, frag_len;

    vert_len = strlen(vert_src);
    frag_len = strlen(frag_src);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_source, &frag_len);
    glCompileShader(fragment_shader);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_source, &vert_len);
    glCompileShader(vertex_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glUseProgram(program);

    do {
        double cur_time = glfwGetTime();
        double dt = cur_time - last_time;
        last_time = cur_time;

        double move_speed = 0.6;

        if(glfwGetKey('I'))
            view_size -= view_size * move_speed * dt;

        if(glfwGetKey('O'))
            view_size += view_size * move_speed * dt;

        if(glfwGetKey('W'))
            view_center[1] -= view_size * move_speed * dt;

        if(glfwGetKey('S'))
            view_center[1] += view_size * move_speed * dt;

        if(glfwGetKey('A'))
            view_center[0] -= view_size * move_speed * dt;

        if(glfwGetKey('D'))
            view_center[0] += view_size * move_speed * dt;

        if(glfwGetKey('R')) {
            view_center[0] = 0;
            view_center[1] = 0;
            view_size = 2;
        }

        glBegin(GL_QUADS);
            glTexCoord2f(view_center[0] - view_size, view_center[1] - view_size); glVertex3f(-1.0f,  1.0f, 0.0f);
            glTexCoord2f(view_center[0] - view_size, view_center[1] + view_size); glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2f(view_center[0] + view_size, view_center[1] + view_size); glVertex3f( 1.0f, -1.0f, 0.0f);
            glTexCoord2f(view_center[0] + view_size, view_center[1] - view_size); glVertex3f( 1.0f,  1.0f, 0.0f);
        glEnd();

        glfwSwapBuffers();
    } while(!glfwGetKey(GLFW_KEY_ESC) && !glfwGetKey('Q') && glfwGetWindowParam(GLFW_OPENED));

    glfwCloseWindow();
    glfwTerminate();
}

