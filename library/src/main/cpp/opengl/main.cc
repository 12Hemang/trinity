//
//  main.cc
//  opengl
//
//  Created by wlanjie on 2019/8/26.
//  Copyright © 2019 com.wlanjie.opengl. All rights reserved.
//

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <stdio.h>
#include <glfw3.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <time.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "opengl.h"
#include "gl.h"
#include "gaussian_blur.h"
#include "blur_split_screen.h"

using namespace trinity;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(512, 512, "OpenGL", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    
    trinity::OpenGL render_screen(512, 512, DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
    
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    GLuint texture_id;
    
    unsigned char *data = stbi_load("test_image/lenna.png", &width, &height, &nrChannels, 0);
    printf("width = %d height = %d\n", width, height);
    
    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    
    clock_t start = clock();
    
//    GaussianBlur gaussian_blur(512, 512, 1.0f);
    BlurSplitScreen blur_split_screen(512, 512);
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        uint64_t current_time = (uint64_t) ((double) (clock() - start) / CLOCKS_PER_SEC * 1000);
//        printf("time: %lld\n", current_time);
        int process_id = blur_split_screen.OnDrawFrame(texture_id);
        render_screen.ProcessImage(process_id);
        glfwSwapBuffers(window);
        usleep(30 * 1000);
    }
    glfwTerminate();
    return 0;
}
