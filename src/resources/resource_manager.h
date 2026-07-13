#ifndef RESOURCE_MANAGER_INCLUDED
#define RESOURCE_MANAGER_INCLUDED

#include <stddef.h>
#include <stdint.h>

#include "resources/font.h"
#include "resources/mesh.h"
#include "resources/shader.h"
#include "resources/sphere.h"

typedef struct {
  ShaderPipeline *buf;
  size_t len;
} ShaderPipelineResources;

typedef struct {
  Mesh *buf;
  size_t len;
} MeshResources;

typedef struct {
  Font *buf;
  size_t len;
} FontResources;

typedef struct {
  Sphere *buf;
  size_t len;
} SphereResources;

typedef struct {
  uint32_t *buf;
  size_t len;
} CommonResources;

typedef struct {
  CommonResources textures;
  MeshResources meshes;
  ShaderPipelineResources shader_pipelines;
  FontResources fonts;
  SphereResources spheres;
} ResourceManager;

uint32_t res_create_text_mesh(ResourceManager *resource_manager);
uint32_t res_create_mesh(ResourceManager *resource_manager, MeshProperties properties);
uint32_t res_create_texture(ResourceManager *resource_manager, const char *path);
uint32_t res_create_shader_pipeline(ResourceManager *resource_manager, const char *vertex_shader_path, const char *fragment_shader_path);
uint32_t res_create_font(ResourceManager *resource_manager, const char *path, int size);
uint32_t res_create_sphere(ResourceManager *resource_manager, const char *texture_path, SphereProperties properties);
Mesh *res_get_mesh(const ResourceManager *resource_manager, uint32_t mesh_id);
uint32_t res_get_texture(const ResourceManager *resource_manager, uint32_t texture_id);
ShaderPipeline *res_get_shader_pipeline(const ResourceManager *resource_manager, uint32_t shader_pipeline_id);
Font *res_get_font(const ResourceManager *resource_manager, uint32_t font_id);
Sphere *res_get_sphere(const ResourceManager *resource_manager, uint32_t sphere_id);
void res_remove_all(ResourceManager *resource_manager);

#endif
