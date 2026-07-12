#include <stdint.h>
#include <stdio.h>

#include "defines.h"
#include "mesh.h"
#include "resource_manager.h"
#include "shader.h"
#include "sphere.h"
#include "texture.h"

// TODO: счетчики увеличиваются до проверки на ошибку
// TODO: выделить буферы под ресурсы динамически

uint32_t res_create_mesh(ResourceManager *resource_manager, Vertice *vertices, size_t vertices_size, ivec3 *indices, size_t indices_size) {
  if (!resource_manager) return INVALID_RESOURCE;
  size_t meshes_size = sizeof(resource_manager->meshes.buf) / sizeof(resource_manager->meshes.buf[0]);
  if (resource_manager->meshes.len >= meshes_size) {
    printf("[WARNING] The limit of meshes has been reached\n");
    return INVALID_RESOURCE;
  }

  Mesh *mesh = &resource_manager->meshes.buf[resource_manager->meshes.len++];
  MeshInitStatus status = mesh_initialize(mesh, vertices, vertices_size, indices, indices_size);
  if (status != MESH_INIT_SUCCESS) {
    const char *err;
    switch (status) {
    case MESH_INIT_MISSING_DATA:
      err = "The mesh data is missing";
      break;
    default:
      err = "Failed to initialize the mesh";
      break;
    }
    printf("[ERROR] %s\n", err);
    return INVALID_RESOURCE;
  }

  return resource_manager->meshes.len - 1;
}

uint32_t res_create_texture(ResourceManager *resource_manager, const char *path) {
  if (!resource_manager) return INVALID_RESOURCE;
  size_t textures_size = sizeof(resource_manager->textures.buf) / sizeof(resource_manager->textures.buf[0]);
  if (resource_manager->textures.len >= textures_size) {
    printf("[WARNING] The limit of textures has been reached\n");
    return INVALID_RESOURCE;
  }

  uint32_t *texture = &resource_manager->textures.buf[resource_manager->textures.len++];
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

  return resource_manager->textures.len - 1;
}

uint32_t res_create_shader_pipeline(ResourceManager *resource_manager, const char *vertex_shader_path, const char *fragment_shader_path) {
  if (!resource_manager) return INVALID_RESOURCE;
  size_t pipelines_size = sizeof(resource_manager->shader_pipelines.buf) / sizeof(resource_manager->shader_pipelines.buf[0]);
  if (resource_manager->shader_pipelines.len >= pipelines_size) {
    printf("[WARNING] The limit of shader pipelines has been reached\n");
    return INVALID_RESOURCE;
  }

  ShaderPipeline *pipeline = &resource_manager->shader_pipelines.buf[resource_manager->shader_pipelines.len++];
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

  return resource_manager->shader_pipelines.len - 1;
}

uint32_t res_create_sphere(ResourceManager *resource_manager, vec3 position, vec3 velocity, float weight, float radius, int sectors, int stacks,
                           const char *texture_path) {
  if (!resource_manager) return INVALID_RESOURCE;
  size_t spheres_size = sizeof(resource_manager->spheres.buf) / sizeof(resource_manager->spheres.buf[0]);
  if (resource_manager->spheres.len >= spheres_size) {
    printf("[WARNING] The limit of spheres has been reached\n");
    return INVALID_RESOURCE;
  }

  Sphere *sphere = &resource_manager->spheres.buf[resource_manager->spheres.len++];
  SphereInitStatus status = sphere_initialize(sphere, position, velocity, weight, radius, sectors, stacks);
  if (status != SPHERE_INIT_SUCCESS) {
    const char *err;
    switch (status) {
    case SPHERE_INIT_MISSING_DATA:
      err = "The sphere data is missing";
      break;
    default:
      err = "Failed to initialize the sphere";
    }
    printf("[ERROR] %s\n", err);
    return INVALID_RESOURCE;
  }

  if (texture_path) {
    uint32_t texture_id = res_create_texture(resource_manager, texture_path);
    sphere->texture = texture_id == INVALID_RESOURCE ? 0 : texture_id;
  }

  size_t vertices_size = get_sphere_vertices_size(sphere);
  size_t indices_size = get_sphere_indices_size(sphere);
  uint32_t mesh_id = res_create_mesh(resource_manager, sphere->vertices.buf, vertices_size, sphere->indices.buf, indices_size);
  sphere->mesh = mesh_id;

  return resource_manager->spheres.len - 1;
}

Mesh *res_get_mesh(ResourceManager *resource_manager, uint32_t mesh_id) {
  if (!resource_manager) return NULL;
  size_t meshes_size = sizeof(resource_manager->meshes.buf) / sizeof(resource_manager->meshes.buf[0]);
  if (mesh_id < 0 || mesh_id >= meshes_size) return NULL;
  return &resource_manager->meshes.buf[mesh_id];
}

uint32_t res_get_texture(ResourceManager *resource_manager, uint32_t texture_id) {
  if (!resource_manager) return INVALID_RESOURCE;
  size_t textures_size = sizeof(resource_manager->textures.buf) / sizeof(resource_manager->textures.buf[0]);
  if (texture_id < 0 || texture_id >= textures_size) return INVALID_RESOURCE;
  return resource_manager->textures.buf[texture_id];
}

ShaderPipeline *res_get_shader_pipeline(ResourceManager *resource_manager, uint32_t shader_pipeline_id) {
  if (!resource_manager) return NULL;
  size_t pipelines_size = sizeof(resource_manager->shader_pipelines.buf) / sizeof(resource_manager->shader_pipelines.buf[0]);
  if (shader_pipeline_id < 0 || shader_pipeline_id >= pipelines_size) return NULL;
  return &resource_manager->shader_pipelines.buf[shader_pipeline_id];
}

Sphere *res_get_sphere(ResourceManager *resource_manager, uint32_t sphere_id) {
  if (!resource_manager) return NULL;
  size_t spheres_size = sizeof(resource_manager->spheres.buf) / sizeof(resource_manager->spheres.buf[0]);
  if (sphere_id < 0 || sphere_id >= spheres_size) return NULL;
  return &resource_manager->spheres.buf[sphere_id];
}
