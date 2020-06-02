    #include "Headers/Engine/Models/Entity.h"


Entity::Entity() = default;

Entity::Entity(
        const std::shared_ptr<Mesh> &mesh,
        const std::vector<Texture> &textures,
        const glm::vec3 &position = glm::vec3(0, 0, 0),
        const glm::vec3 &rotation = glm::vec3(0,0,0),
        const glm::vec3& scale = glm::vec3(1,1,1)
    ) {
    this->mesh = mesh;
    this->textures = textures;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    createModelMatrix();
    moveEntityPlanes(mesh->getVertices());
}

//takes a shader and a Material instead of an array of textures.
Entity::Entity(
        const std::shared_ptr<Mesh> &mesh,
        const Material &material,
        const Shader &shader,
        const glm::vec3 &position,
        const glm::vec3 &rotation,
        const glm::vec3& scale
) {
    this->mesh = mesh;
    this->shader = shader;
    this->material = material;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    createModelMatrix();
    moveEntityPlanes(mesh->getVertices());
}

Entity::~Entity() = default;

void Entity::render(Camera& camera, Shader& inputShader, glm::vec3& lightPos, glm::vec3& lightColor) {
    inputShader.use();

    mesh->bindVAO();
    for(Texture &texture : textures) {
        texture.bind(inputShader);
    }

    camera.setMatrices(inputShader);

    int modelLoc = glGetUniformLocation(inputShader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int lightLoc = glGetUniformLocation(inputShader.ID, "lightPos");
    glUniform3fv(lightLoc, 1, glm::value_ptr(lightPos));

    int lightCol = glGetUniformLocation(inputShader.ID, "lightColor");
    glUniform3fv(lightCol, 1, glm::value_ptr(lightColor));

    int viewLoc = glGetUniformLocation(inputShader.ID, "viewPos");
    glUniform3fv(viewLoc, 1, glm::value_ptr(camera.getPos()));

    glDrawArrays(GL_TRIANGLES, 0, mesh->getNumOfVertices());

    for (Texture &texture : textures) {
        texture.unbind();
    }
    Mesh::unbindVAO();
}

void Entity::render(Camera& camera, std::vector<PointLight> pointLights) {
    shader.use();
    //glCheckError();

    mesh->bindVAO();

    material.getAlbedo().bind(shader);
    material.getAo().bind(shader);
    material.getMetallic().bind(shader);
    material.getNormal().bind(shader);
    material.getRoughness().bind(shader);

    camera.setMatrices(shader);

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int viewLoc = glGetUniformLocation(shader.ID, "viewPos");
    glUniform3fv(viewLoc, 1, glm::value_ptr(camera.getPos()));

    for(int i = 0; i < pointLights.size(); ++i) {
        shader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
        shader.setVec3("pointLights[" + std::to_string(i) + "].color", pointLights[i].color);
        shader.setFloat("pointLights[" + std::to_string(i) + "].intensity", pointLights[i].intensity);
    }

    shader.setFloat("nLights", pointLights.size());

    //swsw w wglCheckError();
    glDrawArrays(GL_TRIANGLES, 0, mesh->getNumOfVertices());
    //glCheckError();

    material.getAlbedo().unbind();
    material.getAo().unbind();
    material.getMetallic().unbind();
    material.getNormal().unbind();
    material.getRoughness().unbind();
    
    Mesh::unbindVAO();
}

glm::mat4 Entity::createModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0,0,1));
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}

glm::vec3 Entity::getPos(){
    return position;
}

glm::vec3 Entity::getRotation() {
    return rotation;
}

void Entity::setRotation(glm::vec3& newRotation) {
    rotation = newRotation;
    createModelMatrix();
    moveEntityPlanes(mesh->getCollisionCube());
}

void Entity::setPos(glm::vec3& newPos) {
    position = newPos;
    createModelMatrix();
    moveEntityPlanes(mesh->getCollisionCube());
}

void Entity::setPos(glm::vec3&& newPos) {
    position = newPos;
    createModelMatrix();
    moveEntityPlanes(mesh->getCollisionCube());
}

void Entity::rotate(glm::vec3& rotationPar) {
    rotation += rotationPar;
    limitRotation();
    createModelMatrix();
    moveEntityPlanes(mesh->getCollisionCube());
}

void Entity::rotate(float x, float y, float z) {
    rotation += glm::vec3(x,y,z);
    limitRotation();
    createModelMatrix();
    moveEntityPlanes(mesh->getCollisionCube());
}

void Entity::translate(glm::vec3 &translation) {
    position += translation;
    createModelMatrix();
    moveEntityPlanes(mesh->getCollisionCube());
}

void Entity::translate(float x, float y, float z) {
    position += glm::vec3(x,y,z);
    createModelMatrix();
    moveEntityPlanes(mesh->getCollisionCube());
}

void Entity::addScale(glm::vec3 &scalePar) {
    scale += scalePar;
    createModelMatrix();
    moveEntityPlanes(mesh->getCollisionCube());
}

void Entity::addScale(float x, float y, float z) {
    this->scale += glm::vec3(x,y,z);
    createModelMatrix();
    moveEntityPlanes(mesh->getCollisionCube());
}

void Entity::limitRotation() {
    while(rotation.x >= 360) {
        rotation.x -= 360;
    }
    while(rotation.y >= 360) {
        rotation.y -= 360;
    }
    while(rotation.z >= 360) {
        rotation.z -= 360;
    }
    while(rotation.x <= 0) {
        rotation.x += 360;
    }
    while(rotation.y <= 0) {
        rotation.y += 360;
    }
    while(rotation.z <= 0) {
        rotation.z += 360;
    }
}

glm::vec3 Entity::getScale() {
    return scale;
}

void Entity::moveEntityPlanes(std::vector<glm::vec3> &vertices) {
    planes.clear();
    int i = 0;
    glm::vec3 point1, point2, point3;
    while(i < vertices.size()) {
        point1 = modelMatrix * glm::vec4(vertices[i++], 1);
        point2 = modelMatrix * glm::vec4(vertices[i++], 1);
        point3 = modelMatrix * glm::vec4(vertices[i++], 1);
        planes.emplace_back(point1, point2, point3, flipped);
    }
}

void Entity::setAsPlayerEntity() {
    isPlayerEntity = true;
}

bool Entity::checkIfPlayerEntity() {
    return isPlayerEntity;
}

void Entity::setAsBullet() {
    isBullet = true;
}

bool Entity::checkIfBullet() {
    return isBullet;
}

void Entity::setAsAnimal() {
    isAnimal = true;
}

bool Entity::checkIfAnimal() {
    return isAnimal;
}

void Entity::setAsItem() {
    isItem = true;
}

bool Entity::checkIfItem() {
    return isItem;
}

void Entity::setFlipped() {
    flipped = 1;
    moveEntityPlanes(mesh->getVertices());
}

void Entity::create(
        const std::shared_ptr<Mesh> &meshPar,
        const std::vector<Texture>& texturesPar,
        const glm::vec3& positionPar,
        const glm::vec3& rotationPar,
        const glm::vec3& scalePar
    ) {
    mesh = meshPar;
    textures = texturesPar;
    position = positionPar;
    rotation = rotationPar;
    scale = scalePar;

    createModelMatrix();

    moveEntityPlanes(mesh->getVertices());
}