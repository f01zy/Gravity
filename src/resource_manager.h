#ifndef RESOURCE_MANAGER_INCLUDED
#define RESOURCE_MANAGER_INCLUDED

#include <stddef.h>
#include <stdint.h>

#include "defines.h"
#include "mesh.h"
#include "shader.h"
#include "sphere.h"

typedef struct {
  ShaderPipeline buf[MAX_RESOURCES_COUNT];
  size_t len;
} ShaderPipelineResources;

typedef struct {
  Mesh buf[MAX_RESOURCES_COUNT];
  size_t len;
} MeshResources;

typedef struct {
  Sphere buf[MAX_RESOURCES_COUNT];
  size_t len;
} SphereResources;

typedef struct {
  unsigned buf[MAX_RESOURCES_COUNT];
  size_t len;
} CommonResources;

typedef struct {
  CommonResources textures;
  MeshResources meshes;
  ShaderPipelineResources shader_pipelines;
  SphereResources spheres;
} ResourceManager;

uint32_t res_create_mesh(ResourceManager *resource_manager, Vertice *vertices, size_t vertices_size, ivec3 *indices, size_t indices_size);
uint32_t res_create_texture(ResourceManager *resource_manager, const char *path);
uint32_t res_create_shader_pipeline(ResourceManager *resource_manager, const char *vertex_shader_path, const char *fragment_shader_path);
uint32_t res_create_sphere(ResourceManager *resource_manager, vec3 position, vec3 velocity, float weight, float radius, int sectors, int stacks,
                           const char *texture_path);

Mesh *res_get_mesh(ResourceManager *resource_manager, uint32_t mesh_id);
uint32_t res_get_texture(ResourceManager *resource_manager, uint32_t texture_id);
ShaderPipeline *res_get_shader_pipeline(ResourceManager *resource_manager, uint32_t shader_pipeline_id);
Sphere *res_get_sphere(ResourceManager *resource_manager, uint32_t sphere_id);

#endif
