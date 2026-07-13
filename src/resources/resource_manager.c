#include <glad/gl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/defines.h"
#include "resources/font.h"
#include "resources/mesh.h"
#include "resources/resource_manager.h"
#include "resources/shader.h"
#include "resources/sphere.h"
#include "resources/texture.h"

uint32_t res_create_text_mesh(ResourceManager *resource_manager) {
  Attribute attributes[] = {(Attribute){.size = 4, .type = GL_FLOAT}};
  ivec3 indices[] = {{0, 1, 3}, {1, 2, 3}};
  return res_create_mesh(resource_manager, (MeshProperties){
                                             .vertices = NULL,
                                             .vertices_size = sizeof(vec4) * 4,
                                             .indices = indices,
                                             .indices_size = sizeof(indices),
                                             .attributes = attributes,
                                             .attributes_size = 1,
                                             .render_mode = GL_DYNAMIC_DRAW,
                                           });
}

uint32_t res_create_mesh(ResourceManager *resource_manager, MeshProperties properties) {
  resource_manager->meshes.buf = (Mesh *)realloc(resource_manager->meshes.buf, sizeof(Mesh) * (resource_manager->meshes.len + 1));
  Mesh *mesh = &resource_manager->meshes.buf[resource_manager->meshes.len];
  MeshInitStatus status = mesh_initialize(mesh, properties);
  if (status != MESH_INIT_SUCCESS) {
    const char *err;
    switch (status) {
    case MESH_INIT_MISSING_DATA:
      err = "The mesh data is missing";
      break;
    case MESH_INIT_RENDER_MODE_ERROR:
      err = "The mesh render mode received an invalid value";
      break;
    case MESH_INIT_ATTRIBUTE_TYPE_ERROR:
      err = "The attribute type received an invalid value";
      break;
    default:
      err = "Failed to initialize the mesh";
      break;
    }
    printf("[ERROR] %s\n", err);
    return INVALID_RESOURCE;
  }
  return resource_manager->meshes.len++;
}

uint32_t res_create_texture(ResourceManager *resource_manager, const char *path) {
  resource_manager->textures.buf = (uint32_t *)realloc(resource_manager->textures.buf, sizeof(uint32_t) * (resource_manager->textures.len + 1));
  uint32_t *texture = &resource_manager->textures.buf[resource_manager->textures.len];
  TextureInitStatus status = texture_initialize(texture, path);
  if (status != TEXTURE_INIT_SUCCESS) {
    const char *err;
    switch (status) {
    case TEXTURE_INIT_MISSING_DATA:
      err = "The texture data is missing";
      break;
    case TEXTURE_INIT_FAILED_LOAD:
      err = "Failed to load the texture";
      break;
    default:
      err = "Failed to initialize the texture";
      break;
    }
    printf("[ERROR] %s\n", err);
    return INVALID_RESOURCE;
  }
  printf("[LOG] The texture was successfully created: %s\n", path);
  return resource_manager->textures.len++;
}

uint32_t res_create_shader_pipeline(ResourceManager *resource_manager, const char *vertex_shader_path, const char *fragment_shader_path) {
  size_t new_size = sizeof(ShaderPipeline) * (resource_manager->shader_pipelines.len + 1);
  resource_manager->shader_pipelines.buf = (ShaderPipeline *)realloc(resource_manager->shader_pipelines.buf, new_size);
  ShaderPipeline *pipeline = &resource_manager->shader_pipelines.buf[resource_manager->shader_pipelines.len];
  PipelineInitStatus status = shader_pipeline_initialize(pipeline, vertex_shader_path, fragment_shader_path);
  if (status != PIPELINE_INIT_SUCCESS) {
    const char *err;
    switch (status) {
    case PIPELINE_INIT_SHADER_ERROR:
      err = "Failed to compile the vertex or fragment shader";
      break;
    case PIPELINE_INIT_MISSING_DATA:
      err = "The shader pipeline data is missing";
      break;
    case PIPELINE_INIT_FAILED_LINK:
      err = "Failed to link the shader program";
      break;
    default:
      err = "Failed to initialize the shader pipeline";
      break;
    }
    printf("[ERROR] %s\n", err);
    return INVALID_RESOURCE;
  }
  printf("[LOG] The shader pipeline was successfully created: %s\n", vertex_shader_path);
  return resource_manager->shader_pipelines.len++;
}

uint32_t res_create_font(ResourceManager *resource_manager, const char *path, int size) {
  resource_manager->fonts.buf = (Font *)realloc(resource_manager->fonts.buf, sizeof(Font) * (resource_manager->spheres.len + 1));
  Font *font = &resource_manager->fonts.buf[resource_manager->fonts.len];
  FontInitStatus status = font_initialize(font, path, size);
  if (status != FONT_INIT_SUCCESS) {
    const char *err;
    switch (status) {
    case FONT_INIT_MISSING_DATA:
      err = "The font data is missing";
      break;
    case FONT_INIT_FAILED_INIT_FT:
      err = "Failed to initialize freetype";
      break;
    case FONT_INIT_FAILED_LOAD_FONT:
      err = "Failed to load font";
      break;
    default:
      err = "Failed to initialize font";
      break;
    }
    printf("[ERROR] %s\n", err);
    return INVALID_RESOURCE;
  }
  printf("[LOG] The font was successfully created: %s\n", path);
  return resource_manager->fonts.len++;
}

uint32_t res_create_sphere(ResourceManager *resource_manager, const char *texture_path, SphereProperties properties) {
  resource_manager->spheres.buf = (Sphere *)realloc(resource_manager->spheres.buf, sizeof(Sphere) * (resource_manager->spheres.len + 1));
  Sphere *sphere = &resource_manager->spheres.buf[resource_manager->spheres.len];
  SphereInitStatus status = sphere_initialize(sphere, properties);
  if (status != SPHERE_INIT_SUCCESS) {
    const char *err;
    switch (status) {
    case SPHERE_INIT_MISSING_DATA:
      err = "The sphere data is missing";
      break;
    default:
      err = "Failed to initialize the sphere";
      break;
    }
    printf("[ERROR] %s\n", err);
    return INVALID_RESOURCE;
  }
  if (texture_path) {
    uint32_t texture_id = res_create_texture(resource_manager, texture_path);
    if (texture_id == INVALID_RESOURCE) {
      // TODO: удалить выделенные ресурсы
      return INVALID_RESOURCE;
    }
    sphere->texture_id = texture_id;
  }
  Attribute attributes[] = {
    (Attribute){
      .size = 3,
      .type = GL_FLOAT,
    },
    (Attribute){
      .size = 3,
      .type = GL_FLOAT,
    },
    (Attribute){
      .size = 2,
      .type = GL_FLOAT,
    },
  };
  uint32_t mesh_id = res_create_mesh(resource_manager, (MeshProperties){
                                                         .vertices = sphere->vertices.buf,
                                                         .vertices_size = get_sphere_vertices_size(sphere),
                                                         .indices = sphere->indices.buf,
                                                         .indices_size = get_sphere_indices_size(sphere),
                                                         .attributes = attributes,
                                                         .attributes_size = 3,
                                                         .render_mode = GL_STATIC_DRAW,
                                                       });
  if (mesh_id == INVALID_RESOURCE) {
    // TODO: удалить выделенные ресурсы
    return INVALID_RESOURCE;
  }
  sphere->mesh_id = mesh_id;
  return resource_manager->spheres.len++;
}

Mesh *res_get_mesh(const ResourceManager *resource_manager, uint32_t mesh_id) {
  if (mesh_id < 0 || mesh_id >= resource_manager->meshes.len) return NULL;
  return &resource_manager->meshes.buf[mesh_id];
}

uint32_t res_get_texture(const ResourceManager *resource_manager, uint32_t texture_id) {
  if (texture_id < 0 || texture_id >= resource_manager->textures.len) return INVALID_RESOURCE;
  return resource_manager->textures.buf[texture_id];
}

ShaderPipeline *res_get_shader_pipeline(const ResourceManager *resource_manager, uint32_t shader_pipeline_id) {
  if (shader_pipeline_id < 0 || shader_pipeline_id >= resource_manager->shader_pipelines.len) return NULL;
  return &resource_manager->shader_pipelines.buf[shader_pipeline_id];
}

Font *res_get_font(const ResourceManager *resource_manager, uint32_t font_id) {
  if (font_id < 0 || font_id >= resource_manager->shader_pipelines.len) return NULL;
  return &resource_manager->fonts.buf[font_id];
}

Sphere *res_get_sphere(const ResourceManager *resource_manager, uint32_t sphere_id) {
  if (sphere_id < 0 || sphere_id >= resource_manager->spheres.len) return NULL;
  return &resource_manager->spheres.buf[sphere_id];
}

void res_remove_all(ResourceManager *resource_manager) {
  for (int i = 0; i < resource_manager->meshes.len; i++) {
    mesh_remove(&resource_manager->meshes.buf[i]);
  }
  for (int i = 0; i < resource_manager->textures.len; i++) {
    texture_remove(resource_manager->textures.buf[i]);
  }
  for (int i = 0; i < resource_manager->shader_pipelines.len; i++) {
    shader_pipeline_remove(&resource_manager->shader_pipelines.buf[i]);
  }
  for (int i = 0; i < resource_manager->spheres.len; i++) {
    sphere_remove(&resource_manager->spheres.buf[i]);
  }
  for (int i = 0; i < resource_manager->fonts.len; i++) {
    font_remove(&resource_manager->fonts.buf[i]);
  }
  free(resource_manager->meshes.buf);
  free(resource_manager->textures.buf);
  free(resource_manager->shader_pipelines.buf);
  free(resource_manager->fonts.buf);
  free(resource_manager->spheres.buf);
}
