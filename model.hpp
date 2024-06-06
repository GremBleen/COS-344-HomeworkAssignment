#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"

#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "light.hpp"

class Model {
public:
    vector<Mesh> meshes;

    Model(vector<Mesh> meshes) {
        this->meshes = meshes;
    }

    void Draw(Shader& shader, Camera& camera) {
        for (GLuint i = 0; i < meshes.size(); i++) {
            meshes[i].Draw(shader, camera);
        }
    }
};
#endif

#ifndef LOAD_TEXTURES
#define LOAD_TEXTURES

namespace ld_tex {
    static vector<Texture> loadWallTexture() {
        vector<Texture> textures;
        textures.push_back(Texture("Textures/brickwall/wall.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE));
        return textures;
    }

    static vector<Texture> loadPlanksTexture() {
        vector<Texture> textures;
        textures.push_back(Texture("Textures/plank/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE));
        textures.push_back(Texture("Textures/plank/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE));
        return textures;
    }

    static vector<Texture> loadGreyUpholstery() {
        vector<Texture> textures;
        textures.push_back(Texture("Textures/grey_upholstery_bl/grey_upholstery_albedo.png", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE));
        textures.push_back(Texture("Textures/grey_upholstery_bl/grey_upholstery_ao.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE));
        return textures;
    }

    static vector<Texture> loadSmoothTempleBlocks() {
        vector<Texture> textures;
        textures.push_back(Texture("Textures/smooth_temple_blocks_bl/smooth_temple_blocks_albedo.png", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE));
        textures.push_back(Texture("Textures/smooth_temple_blocks_bl/smooth_temple_blocks_ao.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE));
        return textures;
    }

    static vector<Texture> loadConcrete() {
        vector<Texture> textures;
        textures.push_back(Texture("Textures/concrete2_bl/concrete2_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE));
        textures.push_back(Texture("Textures/concrete2_bl/concrete2-ao.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE));
        return textures;
    }

    static vector<Texture> loadSpeckledCounter(){
        vector<Texture> textures;
        textures.push_back(Texture("Textures/speckled_countertop1_bl/speckled_countertop1_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE));
        textures.push_back(Texture("Textures/speckled_countertop1_bl/speckled_countertop1_ao.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE));
        return textures;
    }
}

#endif

#ifndef LOAD_SHAPES 
#define LOAD_SHAPES

namespace ld_shp {
    static vector<Mesh> loadTriangle(Vertex v1, Vertex v2, Vertex v3, vector<Texture> textures) {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        return vector<Mesh>{Mesh(vertices, indices, textures)};
    }

    static vector<Mesh> loadBasicTriangle(vector<Texture>& textures) {

        vector<Vertex> vertices;
        vector<GLuint> indices;

        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f) });
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f) });
        vertices.push_back(Vertex{ vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.5f, 1.0f) });

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        return vector<Mesh>{Mesh(vertices, indices, textures)};
    }

    static vector<Mesh> loadCube(float length, float depth, float height, vector<Texture> textures = {}) {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        float l = length / 2;
        float d = depth / 2;
        float h = height / 2;

        // Front face
        vertices.push_back(Vertex{ vec3(-l, -h, d), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl
        vertices.push_back(Vertex{ vec3(l, -h, d), vec3(0.0f, 0.0f, 1.0f), vec2(5.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(l, h, d), vec3(0.0f,0.0f, 1.0f), vec2(5.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr
        vertices.push_back(Vertex{ vec3(-l, h, d), vec3(0.0f,0.0f, 1.0f), vec2(0.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) });  // ftl

        // Back face
        vertices.push_back(Vertex{ vec3(l, -h, -d), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(-l, -h, -d), vec3(0.0f, 0.0f, -1.0f), vec2(5.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(-l, h, -d), vec3(0.0f, 0.0f, -1.0f), vec2(5.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl
        vertices.push_back(Vertex{ vec3(l, h, -d), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 5.0f) , vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr

        // Left face
        vertices.push_back(Vertex{ vec3(-l, -h, -d), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(-l, -h, d), vec3(-1.0f, 0.0f, 0.0f), vec2(5.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl
        vertices.push_back(Vertex{ vec3(-l, h, d), vec3(-1.0f, 0.0f, 0.0f), vec2(5.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) });  // ftl
        vertices.push_back(Vertex{ vec3(-l, h, -d), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl

        // Right face 
        vertices.push_back(Vertex{ vec3(l, -h, d), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(l, -h, -d), vec3(1.0f, 0.0f, 0.0f), vec2(5.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(l, h, -d), vec3(1.0f, 0.0f, 0.0f), vec2(5.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr
        vertices.push_back(Vertex{ vec3(l, h, d), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr

        // Top face
        vertices.push_back(Vertex{ vec3(-l, h, d), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftl
        vertices.push_back(Vertex{ vec3(l, h, d), vec3(0.0f, 1.0f, 0.0f), vec2(5.0f, 0.0f) , vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr
        vertices.push_back(Vertex{ vec3(l, h, -d), vec3(0.0f, 1.0f, 0.0f), vec2(5.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr
        vertices.push_back(Vertex{ vec3(-l, h, -d), vec3(0.0f, 1.0f,0.0f), vec2(0.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl

        // Bottom face
        vertices.push_back(Vertex{ vec3(-l, -h, -d), vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(l, -h, -d), vec3(0.0f, -1.0f, 0.0f), vec2(5.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(l, -h, d), vec3(0.0f, -1.0f, 0.0f), vec2(5.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(-l, -h, d), vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 5.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl

        // Front face - fbl, fbr, ftr, ftr, ftl, fbl
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        // Back face - bbr, bbl, btl, btl, btr, bbr
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(4);

        // Left face - bbl, fbl, ftl, ftl, btl, bbl
        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(10);
        indices.push_back(11);
        indices.push_back(8);

        // Right face - fbr, bbr, btr, btr, ftr, fbr
        indices.push_back(12);
        indices.push_back(13);
        indices.push_back(14);
        indices.push_back(14);
        indices.push_back(15);
        indices.push_back(12);

        // Top face - ftl, ftr, btr, btr, btl, ftl
        indices.push_back(16);
        indices.push_back(17);
        indices.push_back(18);
        indices.push_back(18);
        indices.push_back(19);
        indices.push_back(16);

        // Bottom face - bbl, bbr, fbr, fbr, fbl, bbl
        indices.push_back(20);
        indices.push_back(21);
        indices.push_back(22);
        indices.push_back(22);
        indices.push_back(23);
        indices.push_back(20);

        return vector<Mesh>{Mesh(vertices, indices, textures)};
    }

    static vector<Mesh> loadSquare(Vertex v1, Vertex v2, Vertex v3, Vertex v4, vector<Texture> textures) {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        vertices.push_back(v4);

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        return vector<Mesh>{Mesh(vertices, indices, textures)};
    }

    static vector<Mesh> loadFloor(float length, float breadth, vector<Texture> textures) {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        Mesh floor = loadCube(length, breadth, 0.1f, textures)[0];
        for (int i = 0; i < floor.vertices.size(); i++) {
            floor.vertices[i].Colour = vec4(0.61f, 0.64f, 0.74f, 1.0f);
        }
        floor.setupMesh();

        return vector<Mesh>{Mesh(floor.vertices, floor.indices, textures)};
    }

    static vector<Mesh> loadBasicSquare(vector<Texture> textures) {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f) });
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f) });
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f) });
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f) });

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        return vector<Mesh>{Mesh(vertices, indices, textures)};
    }

    static vector<Mesh> loadBasicCube(vector<Texture> textures) {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        // Front face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(-1.0f, -1.0f, 1.0f), vec2(0.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // fbl
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, -1.0f, 1.0f), vec2(1.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // ftr
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) });  // ftl

        // Back face
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, -1.0f, -1.0f), vec2(0.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f, -1.0f, -1.0f), vec2(1.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(-1.0f, 1.0f, -1.0f), vec2(1.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // btl
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, -1.0f), vec2(0.0f, 1.0f) , vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // btr

        // Left face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f, -1.0f, -1.0f), vec2(0.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(-1.0f, -1.0f, 1.0f), vec2(1.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // fbl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) });  // ftl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(-1.0f, 1.0f, -1.0f), vec2(0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // btl

        // Right face 
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, -1.0f, 1.0f), vec2(0.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, -1.0f, -1.0f), vec2(1.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, -1.0f), vec2(1.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // btr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // ftr

        // Top face
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // ftl
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f) , vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // ftr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, -1.0f), vec2(1.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // btr
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(-1.0f, 1.0f, -1.0f), vec2(0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // btl

        // Bottom face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f, -1.0f, -1.0f), vec2(0.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, -1.0f, -1.0f), vec2(1.0f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, -1.0f, 1.0f), vec2(1.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(-1.0f, -1.0f, 1.0f), vec2(0.0f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f) }); // fbl

        // Front face - fbl, fbr, ftr, ftr, ftl, fbl
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        // Back face - bbr, bbl, btl, btl, btr, bbr
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(4);

        // Left face - bbl, fbl, ftl, ftl, btl, bbl
        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(10);
        indices.push_back(11);
        indices.push_back(8);

        // Right face - fbr, bbr, btr, btr, ftr, fbr
        indices.push_back(12);
        indices.push_back(13);
        indices.push_back(14);
        indices.push_back(14);
        indices.push_back(15);
        indices.push_back(12);

        // Top face - ftl, ftr, btr, btr, btl, ftl
        indices.push_back(16);
        indices.push_back(17);
        indices.push_back(18);
        indices.push_back(18);
        indices.push_back(19);
        indices.push_back(16);

        // Bottom face - bbl, bbr, fbr, fbr, fbl, bbl
        indices.push_back(20);
        indices.push_back(21);
        indices.push_back(22);
        indices.push_back(22);
        indices.push_back(23);
        indices.push_back(20);

        return vector<Mesh>{Mesh(vertices, indices, textures)};
    }

    static vector<Mesh> loadEastWall() {
        Mesh backWall = loadCube(2.0f, 1.0f, 7.0f, ld_tex::loadSmoothTempleBlocks())[0];
        for (GLuint i = 0; i < backWall.vertices.size(); i++) {
            backWall.vertices[i].Colour = vec4(0.21f, 0.27f, 0.43f, 1.0f);
        }
        backWall.setupMesh();

        return vector<Mesh>{backWall};
    }

    static vector<Mesh> loadEastGlassWall() {
        Mesh sideGlass = loadCube(1.3f, 0.05f, 7.0f)[0];
        for (GLuint i = 0; i < sideGlass.vertices.size(); i++) {
            sideGlass.vertices[i].Colour = vec4(0.2f, 0.2f, 0.2f, 0.1f);
        }
        sideGlass.setupMesh();

        return vector<Mesh>{sideGlass};
    }

    static vector<Mesh> loadLongWall(vector<Texture> textures) {
        Mesh longWall = loadCube(30.0f, 1.0f, 7.0f, textures)[0];

        for (GLuint i = 0; i < longWall.vertices.size(); i++) {
            longWall.vertices[i].Colour = vec4(0.8f, 0.8f, 0.8f, 1.0f);
        }
        longWall.setupMesh();

        return vector<Mesh>{longWall};
    }

    static vector<Mesh> loadWoodPanel(vector<Texture> textures) {
        Mesh woodPanel = loadCube(30.0f, 0.05f, 5.0f, textures)[0];
        for (GLuint i = 0; i < woodPanel.vertices.size(); i++) {
            woodPanel.vertices[i].Colour = vec4(0.4f, 0.4f, 0.4f, 1.0f);
        }
        woodPanel.setupMesh();

        return vector<Mesh>{woodPanel};
    }

    static vector<Mesh> loadGlassWall(float length, float height) {
        Mesh glassWall = loadCube(length, 0.05f, height)[0];
        for (GLuint i = 0; i < glassWall.vertices.size(); i++) {
            glassWall.vertices[i].Colour = vec4(0.2f, 0.2f, 0.2f, 0.1f);
        }
        glassWall.setupMesh();

        return vector<Mesh>{glassWall};
    }

    static vector<Mesh> loadBigGlassPillar(int numSquares, float length, float height) {
        vector<Mesh> glassWall;
        float squareLength = length / numSquares;

        for (int i = 0; i < numSquares; i++) {
            Mesh glassSquare = loadGlassWall(squareLength, squareLength)[0];
            glassWall.push_back(glassSquare);
        }

        float offset = squareLength;
        int k = 1;
        for (int i = 1; i < glassWall.size() - 1; i++) {
            Mesh glassSquare1 = glassWall[i];
            Mesh glassSquare2 = glassWall[i + 1];
            for (GLuint j = 0; j < glassSquare1.vertices.size(); j++) {
                glassSquare1.vertices[j].Colour = vec4(0.2f, 0.2f, 0.2f, 0.1f);

                glassSquare2.vertices[j].Colour = vec4(0.2f, 0.2f, 0.2f, 0.1f);

                glassSquare1.vertices[j].Position += vec3(0.0f, offset * k, 0.0f);
                glassSquare2.vertices[j].Position -= vec3(0.0f, offset * k, 0.0f);
            }
            glassSquare1.setupMesh();
            glassSquare2.setupMesh();
            i++;
            k++;
        }

        return glassWall;
    }

    static vector<Mesh> loadWestWall() {
        Mesh westWall = loadCube(7.0f, 0.05f, 7.0f)[0];
        for (GLuint i = 0; i < westWall.vertices.size(); i++) {
            westWall.vertices[i].Colour = vec4(0.2f, 0.2f, 0.2f, 0.1f);
        }
        westWall.setupMesh();

        return vector<Mesh>{westWall};
    }

    static vector<Mesh> loadGridCross() {
        Mesh horizontal = loadCube(0.5f, 0.05f, 0.05f)[0];
        Mesh vertical = loadCube(0.05f, 0.05f, 0.5f)[0];

        for (GLuint i = 0; i < horizontal.vertices.size(); i++) {
            horizontal.vertices[i].Colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        return vector<Mesh>{horizontal, vertical};
    }

    static vector<Mesh> loadRoof() {
        Mesh roof = loadCube(30.0f, 7.0f, 0.05f)[0];
        for (GLuint i = 0; i < roof.vertices.size(); i++) {
            roof.vertices[i].Colour = vec4(0.2f, 0.2f, 0.2f, 0.1f);
        }
        roof.setupMesh();

        return vector<Mesh>{roof};
    }

    static vector<Mesh> loadTable1(vector<Texture> textures) {
        Mesh tableTop = loadCube(1.0f, 1.0f, 0.01f, textures)[0];
        for (GLuint i = 0; i < tableTop.vertices.size(); i++) {
            tableTop.vertices[i].Colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
        tableTop.setupMesh();

        Mesh tableStand = loadCube(0.05f, 0.05f, 0.5f, textures)[0];
        for (GLuint i = 0; i < tableStand.vertices.size(); i++) {
            tableStand.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
            tableStand.vertices[i].Position -= vec3(0.0f, 0.25f, 0.0f);
        }
        tableStand.setupMesh();

        Mesh tableBottom = loadCube(0.5f, 0.5f, 0.05f, textures)[0];
        for (GLuint i = 0; i < tableBottom.vertices.size(); i++) {
            tableBottom.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
            tableBottom.vertices[i].Position -= vec3(0.0f, 0.525f, 0.0f);
        }
        tableBottom.setupMesh();

        return vector<Mesh>{tableTop, tableStand, tableBottom};
    }

    static vector<Mesh> loadTable2(vector<Texture> textures) {
        Mesh tableTop = loadCube(4.0f, 2.0f, 0.05f, textures)[0];
        for (GLuint i = 0; i < tableTop.vertices.size(); i++) {
            tableTop.vertices[i].Colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
        tableTop.setupMesh();

        Mesh tableLeg1 = loadCube(0.05f, 0.05f, 0.7f, textures)[0];
        Mesh tableLeg2 = loadCube(0.05f, 0.05f, 0.7f, textures)[0];
        Mesh tableLeg3 = loadCube(0.05f, 0.05f, 0.7f, textures)[0];
        Mesh tableLeg4 = loadCube(0.05f, 0.05f, 0.7f, textures)[0];

        for (GLuint i = 0; i < tableLeg1.vertices.size(); i++) {
            tableLeg1.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
            tableLeg2.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
            tableLeg3.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
            tableLeg4.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
        }

        for (GLuint i = 0; i < tableLeg1.vertices.size(); i++) {
            tableLeg1.vertices[i].Position += vec3(-1.94f, -0.375f, -0.95f);
            tableLeg2.vertices[i].Position += vec3(1.94f, -0.375f, -0.95f);
            tableLeg3.vertices[i].Position += vec3(-1.94f, -0.375f, 0.95f);
            tableLeg4.vertices[i].Position += vec3(1.94f, -0.375f, 0.95f); 
        }
        tableLeg1.setupMesh();
        tableLeg2.setupMesh();
        tableLeg3.setupMesh();
        tableLeg4.setupMesh();

        return vector<Mesh>{tableTop, tableLeg1, tableLeg2, tableLeg3, tableLeg4};
    }

    static vector<Mesh> loadChair1(vector<Texture> textures) {
        Mesh chair_seat = loadCube(0.5f, 0.5f, 0.05f, textures)[0];
        for (GLuint i = 0; i < chair_seat.vertices.size(); i++) {
            chair_seat.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
        }
        chair_seat.setupMesh();

        Mesh chair_back = loadCube(0.5f, 0.05f, 0.5f, textures)[0];
        for (GLuint i = 0; i < chair_back.vertices.size(); i++) {
            chair_back.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
            chair_back.vertices[i].Position += vec3(0.0f, 0.25f, 0.25f);
        }
        chair_back.setupMesh();

        Mesh chairLeg1 = loadCube(0.05f, 0.05f, 0.5f, textures)[0];
        Mesh chairLeg2 = loadCube(0.05f, 0.05f, 0.5f, textures)[0];
        Mesh chairLeg3 = loadCube(0.05f, 0.05f, 0.5f, textures)[0];
        Mesh chairLeg4 = loadCube(0.05f, 0.05f, 0.5f, textures)[0];

        for (GLuint i = 0; i < chairLeg1.vertices.size(); i++) {
            chairLeg1.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
            chairLeg2.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
            chairLeg3.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
            chairLeg4.vertices[i].Colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
        }

        for (GLuint i = 0; i < chairLeg1.vertices.size(); i++) {
            chairLeg1.vertices[i].Position += vec3(-0.24f, -0.25f, -0.24f);
            chairLeg2.vertices[i].Position += vec3(0.24f, -0.25f, -0.24f);
            chairLeg3.vertices[i].Position += vec3(-0.24f, -0.25f, 0.24f);
            chairLeg4.vertices[i].Position += vec3(0.24f, -0.25f, 0.24f);
        }
        chairLeg1.setupMesh();
        chairLeg2.setupMesh();
        chairLeg3.setupMesh();
        chairLeg4.setupMesh();
        
        return vector<Mesh>{chair_seat, chair_back, chairLeg1, chairLeg2, chairLeg3, chairLeg4};
    }
    // static vector<Mesh> loadKiosk() {
    //     Mesh floor = loadFloor(10.0f, 10.0f, ld_tex::loadPlanksTexture())[0];
    //     vector<Mesh> eastWall = loadEastWall(ld_tex::loadSmoothTempleBlocks());

    //     return vector<Mesh>{};
    // }
}

#endif

#ifndef LOAD_LIGHTS
#define LOAD_LIGHTS

namespace ld_lights {
    static PointLight loadPointLight() {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        // Front face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) });  // ftl

        // Back face
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr

        // Left face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) });  // ftl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl

        // Right face 
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f) , vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr

        // Top face
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftl
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl

        // Bottom face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl

        // Front face - fbl, fbr, ftr, ftr, ftl, fbl
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        // Back face - bbr, bbl, btl, btl, btr, bbr
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(4);

        // Left face - bbl, fbl, ftl, ftl, btl, bbl
        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(10);
        indices.push_back(11);
        indices.push_back(8);

        // Right face - fbr, bbr, btr, btr, ftr, fbr
        indices.push_back(12);
        indices.push_back(13);
        indices.push_back(14);
        indices.push_back(14);
        indices.push_back(15);
        indices.push_back(12);

        // Top face - ftl, ftr, btr, btr, btl, ftl
        indices.push_back(16);
        indices.push_back(17);
        indices.push_back(18);
        indices.push_back(18);
        indices.push_back(19);
        indices.push_back(16);

        // Bottom face - bbl, bbr, fbr, fbr, fbl, bbl
        indices.push_back(20);
        indices.push_back(21);
        indices.push_back(22);
        indices.push_back(22);
        indices.push_back(23);
        indices.push_back(20);

        vec3 lightPos = vec3(0.0f, 0.0f, 0.0f);
        vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
        vec3 diffuse = vec3(0.7f, 0.7f, 0.7f);
        vec3 specular = vec3(1.0f, 1.0f, 1.0f);

        float constant = 1.0f;
        float linear = 0.2f;
        float quadratic = 0.2f;

        return PointLight(lightPos, ambient, diffuse, specular, constant, linear, quadratic, vertices, indices);
    }

    static DirectionalLight loadDirectionalLight() {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        // Front face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) });  // ftl

        // Back face
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) });
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr

        // Left face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) });  // ftl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl

        // Right face 
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr

        // Top face
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftl
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl

        // Bottom face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f) , vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl

        // Front face - fbl, fbr, ftr, ftr, ftl, fbl
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        // Back face - bbr, bbl, btl, btl, btr, bbr
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(4);

        // Left face - bbl, fbl, ftl, ftl, btl, bbl
        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(10);
        indices.push_back(11);
        indices.push_back(8);

        // Right face - fbr, bbr, btr, btr, ftr, fbr
        indices.push_back(12);
        indices.push_back(13);
        indices.push_back(14);
        indices.push_back(14);
        indices.push_back(15);
        indices.push_back(12);

        // Top face - ftl, ftr, btr, btr, btl, ftl
        indices.push_back(16);
        indices.push_back(17);
        indices.push_back(18);
        indices.push_back(18);
        indices.push_back(19);
        indices.push_back(16);

        // Bottom face - bbl, bbr, fbr, fbr, fbl, bbl
        indices.push_back(20);
        indices.push_back(21);
        indices.push_back(22);
        indices.push_back(22);
        indices.push_back(23);
        indices.push_back(20);

        vec3 position = vec3(-2.0f, 4.0f, -1.0f);
        vec3 direction = vec3(0.0f, -1.0f, 0.0f);
        vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
        vec3 diffuse = vec3(0.4f, 0.4f, 0.4f);
        vec3 specular = vec3(0.5f, 0.5f, 0.5f);

        return DirectionalLight(position, direction, ambient, diffuse, specular, vertices, indices);
    }

    static SpotLight loadSpotLight() {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        // Front face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) });  // ftl

        // Back face
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr

        // Left face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) });  // ftl
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl

        // Right face 
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr

        // Top face
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftl
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // ftr
        vertices.push_back(Vertex{ vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btr
        vertices.push_back(Vertex{ vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // btl

        // Bottom face
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbl
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // bbr
        vertices.push_back(Vertex{ vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbr
        vertices.push_back(Vertex{ vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }); // fbl

        // Front face - fbl, fbr, ftr, ftr, ftl, fbl
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        // Back face - bbr, bbl, btl, btl, btr, bbr
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(4);

        // Left face - bbl, fbl, ftl, ftl, btl, bbl
        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(10);
        indices.push_back(11);
        indices.push_back(8);

        // Right face - fbr, bbr, btr, btr, ftr, fbr
        indices.push_back(12);
        indices.push_back(13);
        indices.push_back(14);
        indices.push_back(14);
        indices.push_back(15);
        indices.push_back(12);

        // Top face - ftl, ftr, btr, btr, btl, ftl
        indices.push_back(16);
        indices.push_back(17);
        indices.push_back(18);
        indices.push_back(18);
        indices.push_back(19);
        indices.push_back(16);

        // Bottom face - bbl, bbr, fbr, fbr, fbl, bbl
        indices.push_back(20);
        indices.push_back(21);
        indices.push_back(22);
        indices.push_back(22);
        indices.push_back(23);
        indices.push_back(20);

        vec3 position = vec3(0.0f, 0.0f, 0.0f);
        vec3 direction = vec3(0.0f, 0.0f, -1.0f);
        vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
        vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);
        vec3 specular = vec3(1.0f, 1.0f, 1.0f);

        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;

        float cutOff = cos(radians(12.5f));
        float outerCutOff = cos(radians(15.0f));

        return SpotLight(position, direction, ambient, diffuse, specular, constant, linear, quadratic, cutOff, outerCutOff, vertices, indices);
    }
}

#endif