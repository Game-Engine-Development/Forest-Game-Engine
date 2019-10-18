#include "Headers/Engine/Terrain/Terrain.h"

Terrain::Terrain(Texture &texture, int gridX, int gridZ) {
    x = gridX * SIZE;
    z = gridZ * SIZE;
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    for(int i = 0; i < VERTEX_COUNT; ++i){
        for(int j = 0; j < VERTEX_COUNT; ++j){
            vertices.push_back(glm::vec3((float)j/(float)(VERTEX_COUNT - 1) * SIZE, 0, (float)i/(float)(VERTEX_COUNT - 1) * SIZE));
            normals.push_back(glm::vec3(0,1,0));
            texCoords.push_back(glm::vec2((float)j/(float)(VERTEX_COUNT - 1), (float)i/(float)(VERTEX_COUNT - 1)));
        }
    }
    for(int gz = 0; gz < VERTEX_COUNT - 1; ++gz){
        for(int gx = 0; gx < VERTEX_COUNT - 1; ++gx){
            int topLeft = (gz*VERTEX_COUNT) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz+1)*VERTEX_COUNT)+gx;
            int bottomRight = bottomLeft + 1;
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    terrainMesh = TerrainMesh(vertices, normals, texCoords, indices);
    this->texture = texture;
    position = glm::vec3(x, 0, z);
}

Terrain::~Terrain() = default;

void Terrain::render(Camera &camera, Shader &shader) {
    shader.use();
    terrainMesh.bindVAO();
    texture.bind();
    camera.setMatrices(shader);
    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, terrainMesh.getNumOfVertices(), GL_UNSIGNED_INT, 0);
    texture.unbind();
    terrainMesh.unbindVAO();
}

glm::mat4 Terrain::createModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    return modelMatrix;
}