#include "render.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../clog.h"

#include <stdlib.h>


struct texture {
    int width;
    int height;
    int channels;

    uint id;
    uint bind_id;
};

uint default_texture = 0;
int tex_len = 1;

void init_textures() {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &default_texture);
    glBindTexture(GL_TEXTURE_2D, default_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    uint size = 2;
    unsigned char *data = malloc(size * size * 4 * sizeof(unsigned char));
    clog_assert_m(data != NULL, "Failed to allocate NULL texture");

    memset(data, 0, size * size * 4 * sizeof(unsigned char));
    for (uint i = 0; i < size; i++) {
        for (uint j = 0; j < size; j++) {
            if ((i % 2 == 0) != (j % 2 == 0)) {
                data[(j*size+i) * 4 + 0] = (unsigned char)255;
                data[(j*size+i) * 4 + 1] = (unsigned char)0;
                data[(j*size+i) * 4 + 2] = (unsigned char)255;
                data[(j*size+i) * 4 + 3] = (unsigned char)255;
            }
            else {
                data[(j*size+i) * 4 + 3] = (unsigned char)255;
            }
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(data);
}

texture_t *create_texture(const char *path) {
    stbi_set_flip_vertically_on_load(true);
    int w, h, n;
    unsigned char *data = stbi_load(path, &w, &h, &n, 0);

    if (!data) {
        clog(CLOG_ERROR, "Failed to load image %s", path);
        texture_t *ret = malloc(sizeof(struct texture));
        memset(ret, 0, sizeof(struct texture));
    }

    texture_t *ret = create_texture_d(data, w, h, n);

    free(data);
    return ret;
}

texture_t *create_texture_d(const unsigned char *data, int w, int h, int n) {
    texture_t *ret = malloc(sizeof(struct texture));
    
    glGenTextures(1, &ret->id);
    glActiveTexture(GL_TEXTURE0+(tex_len));
    glBindTexture(GL_TEXTURE_2D, ret->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    switch (n) {
        case 3: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data); break;
        case 4: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); break;
        default: 
            clog(CLOG_FATAL, "Unrecognized texture data format.");
            exit(1);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    ret->bind_id = tex_len;
    tex_len++;
    return ret;
}

void use_texture(render_params_t *params, texture_t *tex) {
    glUniform1i(glGetUniformLocation(get_render_params_shader(params), "texture0"), tex->bind_id);
}

double get_time() {return glfwGetTime();}

