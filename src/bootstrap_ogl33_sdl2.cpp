// MIT LICENSE
//
// Copyright(c) 2019
// Christophe Meneboeuf <christophe@xtof.info>
// Joey De Vries < https://joeydevries.com/#contact>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the �Software�), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.




// This code will gets you started with OpenGL Core + SDL2
// It will draw an orange squared as a test
// HEAVY borrows from: https://learnopengl.com/Getting-started/Hello-Triangle


#include <iostream>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>


static constexpr int SCREEN_WIDTH = 800;
static constexpr int SCREEN_HEIGHT = 600;

static constexpr int OPENGL_MAJOR_VERSION = 3;
static constexpr int OPENGL_MINOR_VERSION = 3;

static constexpr auto OPENGL_PROFILE = SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE;  // OpenGL core profile - deprecated functions are disabled



namespace bootstrap {

  void InitGL();
  int  SetViewport(int width, int height);

}


namespace test {

  // Required element to draw the test square
  struct Square_t {
    unsigned hProgrammShader;
    unsigned hVao;
    unsigned nbIndices;
  };

  Square_t SetUpRectangle();

}



int main(int argc, char *argv[])
{

  // # SDL Init
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "There was an error initing SDL2: " << SDL_GetError() << std::endl;
    return 1;
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, OPENGL_PROFILE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);

  // # SDL Window creation
  auto window = SDL_CreateWindow("Bootstrap OpenGL+SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  if (window == nullptr) {
    std::cerr << "There was an error creating the window: " << SDL_GetError() << std::endl;
    return 1;
  }

  // # OpenGL global context
  auto context_ogl = SDL_GL_CreateContext(window);
  if (context_ogl == nullptr) {
    std::cerr << "There was an error creating OpenGL context: " << SDL_GetError() << std::endl;
    return 1;
  }
 
  // # OpenGL Init
  bootstrap::InitGL();
  bootstrap::SetViewport(SCREEN_WIDTH, SCREEN_HEIGHT);

  // # Test elements
  const auto square = test::SetUpRectangle();
  const struct {
    float r = 0.2f;
    float g = 0.3f;
    float b = 0.3f;
    float a = 1.0f;
  } color_background;



  // # Main loop
  SDL_Event e;
  while(true)
  {
    // check inputs
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT)    {
      break;
    }

    // ## Render

    // ### clear the buffers
    glClearColor(color_background.r, color_background.g, color_background.b, color_background.a);
    glClear(GL_COLOR_BUFFER_BIT);

    // ### draw the square
    glUseProgram(square.hProgrammShader);
    glBindVertexArray(square.hVao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, square.nbIndices, GL_UNSIGNED_INT, 0);
    
    // # Display
    SDL_GL_SwapWindow(window); // swap the buffers work and display buffers
  }



  SDL_GL_DeleteContext(context_ogl);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}



namespace bootstrap
{

  // Inits OpenGL
  // ShadeModel and Hints can be omitted / modified
  void InitGL()
  {
    // init GLEW
    if (glewInit() != GLEW_OK) {
      std::cout << "ERROR: Cannot init GLEW" << std::endl;
      exit(1);
    }
    // select flat or smooth shading
    glShadeModel(GL_SMOOTH);
    // specify implementation-specific hints
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // The most correct, or highest quality, option should be chosen. 
  }

  // Function to reset our viewport after a window resize
  int SetViewport(const int width, const int height)
  {
    // setup the viewport
    glViewport(0, 0, width, height);
    // swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    return 1;
  }

}



namespace test
{

  // Shader sources
  static const char *Src_Shader_Vertex = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
  static const char *Src_Shader_Fragment = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

  Square_t SetUpRectangle()
  {
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    const auto shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_vertex, 1, &Src_Shader_Vertex, NULL);
    glCompileShader(shader_vertex);
    // check for shader compile errors
    int success;
    static char Info_Log[512];
    glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader_vertex, 512, NULL, Info_Log);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << Info_Log << std::endl;
    }
    // fragment shader
    const auto shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_fragment, 1, &Src_Shader_Fragment, NULL);
    glCompileShader(shader_fragment);
    // check for shader compile errors
    glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader_fragment, 512, NULL, Info_Log);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << Info_Log << std::endl;
    }
    // link shaders into the program
    const auto program_shaders = glCreateProgram();
    glAttachShader(program_shaders, shader_vertex);
    glAttachShader(program_shaders, shader_fragment);
    glLinkProgram(program_shaders);
    // check for linking errors
    glGetProgramiv(program_shaders, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(program_shaders, 512, NULL, Info_Log);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << Info_Log << std::endl;
    }
    glDeleteShader(shader_vertex);
    glDeleteShader(shader_fragment);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    // Referencing the vertices to share them between the two triangles
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(vao); // ebo will be hosted in the vao

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configure and enable vertex attribute #0 ("location" 0 in the verter shader)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    return {
      program_shaders,
      vao,
      6
    };
  }
}
