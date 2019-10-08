//
// Created by aekstrand7207 on 10/8/2019.
//

#include "Headers/Engine/Terrain/Terrain.h"

Terrain::Terrain(Texture &texture, int gridX, int gridY) {
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;
    for(int i = 0; i < VERTEX_COUNT; ++i){
        for(int j = 0; i < VERTEX_COUNT; ++i){
            vertices.push_back(glm::vec3((float)j/(float)(VERTEX_COUNT - 1) * SIZE, 0, (float)i/(float)(VERTEX_COUNT - 1) * SIZE));
            normals.push_back(glm::vec3(0,1,0));
            texCoords.push_back(glm::vec2((float)j/(float)(VERTEX_COUNT - 1), (float)i/(float)(VERTEX_COUNT - 1)));
        }
    }
    mesh = Mesh(vertices, normals, texCoords);
}

Terrain::~Terrain() = default;

void Terrain::render(Camera camera, Shader &shader) {
    shader.use();
    texture.bind();
    mesh.bindVAO();
    camera.setMatrices(shader);
    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(createModelMatrix()));
    glDrawArrays(GL_TRIANGLES, 0, mesh.getNumOfVertices());
    texture.unbind();
    mesh.unbindVAO();
}

glm::mat4 Terrain::createModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}