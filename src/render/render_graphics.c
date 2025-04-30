#include "render.h"
#include "../clog.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#include <stdlib.h>

struct render_params {
    uint vao;
    uint vbo;
    uint ebo;

    uint num_elements;

    uint shader;
};

const char* vertexSource = R"glsl(
    #version 330 core

    layout (location = 0) in vec3 position;
    layout (location = 1) in vec2 tex_coord;

    out vec2 texCoord;

    uniform mat4 transform;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * transform * vec4(position, 1.0);
        texCoord = tex_coord;
    }
)glsl";

const char* fragSource = R"glsl(
    #version 330 core

    out vec4 outColor;

    in vec2 texCoord;

    uniform sampler2D texture0;
    uniform vec3 color;

    void main()
    {
        outColor = vec4(color, 1.0) * texture(texture0, texCoord);
    }
)glsl";

render_params_t *create_params(float *data, uint data_size, int *elements, uint elements_size) {
    render_params_t *ret = malloc(sizeof(struct render_params));
    clog_assert_m(ret!=NULL, "Failed to allocate render_params_t.");

    glGenVertexArrays(1, &ret->vao);
    glGenBuffers(1, &ret->vbo);
    glGenBuffers(1, &ret->ebo);

    glBindVertexArray(ret->vao);
    glBindBuffer(GL_ARRAY_BUFFER, ret->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret->ebo);

    glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_size, elements, GL_STATIC_DRAW);

    uint vert = glCreateShader(GL_VERTEX_SHADER);
    uint frag = glCreateShader(GL_FRAGMENT_SHADER);
    ret->shader = glCreateProgram();

    glAttachShader(ret->shader, vert);
    glAttachShader(ret->shader, frag);

    glShaderSource(vert, 1, &vertexSource, NULL);
    glShaderSource(frag, 1, &fragSource, NULL);

    glCompileShader(vert);
    glCompileShader(frag);

    
    int status;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vert, 512, NULL, buffer);
        printf("%s\n", buffer);
        exit(1);
    }
    
    status = 0;
    glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(frag, 512, NULL, buffer);
        printf("%s\n", buffer);
        exit(1);
    }

    glLinkProgram(ret->shader);

    glDeleteShader(vert);
    glDeleteShader(frag);

    ret->num_elements = elements_size / sizeof(int);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUniform1i(glGetUniformLocation(ret->shader, "texture0"), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return ret;
}

uint get_render_params_shader(render_params_t *params) {return params->shader;}

rect_t create_rect(float w, float h, float x, float y) {
    float verts[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    };

    int elements[] = {
        0, 1, 3,
        1, 2, 3
    };

    return (rect_t) {
        float2(w, h),
        float2(x, y),
        0,
        float2(0.0f, 0.0f),
        float3(1.0f, 1.0f, 1.0f),
        create_params(verts, sizeof(verts), elements, sizeof(elements))
    };
}

void draw_rect(window_t *win, rect_t r) {
    glBindVertexArray(r.params->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.params->ebo);

    glUseProgram(r.params->shader);
 
    mat4 transform = GLM_MAT4_IDENTITY_INIT;
    mat4 proj = GLM_MAT4_ZERO_INIT;

    int2_t res = get_window_size(win);

    glm_ortho_default((float)res.x/(float)res.y, proj);

    glm_translate(transform, (vec3) {r.position.x, r.position.y, 0.0f});
    glm_rotate_at(transform, (vec3) {r.origin.x, r.origin.y, 0.0f}, glm_rad(r.rotation), (vec3) {0.0f, 0.0f, 1.0f});

    glm_scale(transform, (vec3) {r.size.x, r.size.y, 1.0f}); 

    glUniformMatrix4fv(glGetUniformLocation(r.params->shader, "transform"), 1, GL_FALSE, transform[0]);
    glUniformMatrix4fv(glGetUniformLocation(r.params->shader, "projection"), 1, GL_FALSE, proj[0]);
    glUniform3fv(glGetUniformLocation(r.params->shader, "color"), 1, (vec3) {r.color.x, r.color.y, r.color.z});

    glDrawElements(GL_TRIANGLES, r.params->num_elements, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

sprite_t create_sprite(float w, float h, float x, float y, texture_t *tex) {
    return (sprite_t) {
        create_rect(w, h, x, y),
        tex
    };
}
void draw_sprite(window_t *win, sprite_t s) {
    use_texture(s.rect.params, s.tex);
    draw_rect(win, s.rect);
}
