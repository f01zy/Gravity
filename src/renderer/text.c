#include <glad/gl.h>
#include <stdio.h>
#include <string.h>

#include "renderer/text.h"
#include "resources/font.h"
#include "resources/resource_manager.h"
#include "resources/shader.h"

void render_text(const ResourceManager *resource_manager, TextResources resources, TextProperties properties) {
  const ShaderPipeline *shader_pipeline = res_get_shader_pipeline(resource_manager, resources.shader_pipeline_id);
  const Mesh *mesh = res_get_mesh(resource_manager, resources.mesh_id);
  const Font *font = res_get_font(resource_manager, resources.font_id);

  if (!shader_pipeline || !mesh || !font) {
    printf("[ERROR] Failed to render the text, because the resources weren't found\n");
    return;
  }

  glUseProgram(shader_pipeline->shader_program);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(mesh->VAO);

  // TODO: передать размеры окна извне
  mat4 projection;
  glm_ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 100.0f, projection);
  uniform_set_mat4(shader_pipeline->shader_program, "projection", projection);
  uniform_set_vec3(shader_pipeline->shader_program, "text_color", properties.color);

  const char *text = properties.text;
  float scale = properties.scale;
  float x = properties.position[0];
  float y = properties.position[1];

  for (int i = 0; i < strlen(text); i++) {
    const Character *character = &font->characters[text[i]];
    float xpos = x + character->bearing[0] * scale;
    float ypos = y - (character->size[1] - character->bearing[1]) * scale;
    float w = character->size[0] * scale;
    float h = character->size[1] * scale;
    vec4 vertices[4] = {
      {xpos, ypos + h, 0.0f, 1.0f},
      {xpos + w, ypos + h, 1.0f, 1.0f},
      {xpos + w, ypos, 1.0f, 0.0f},
      {xpos, ypos, 0.0f, 0.0f},
    };
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, character->texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    x += (character->advance >> 6) * scale;
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
