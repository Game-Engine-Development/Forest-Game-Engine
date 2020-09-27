#include "Headers/Engine/Terrain/TerrainMesh.h"

TerrainMesh::TerrainMesh(const char *const filename) : noise(std::nullopt) {
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    loadTerrain(vertices, normals, texCoords, indices, filename);
    numOfVertices = indices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &texCoordBuffer);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &normalBuffer);
    bindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unbindVAO();
}

TerrainMesh::TerrainMesh(const char *const filename, const long seed) : noise(Noise::OpenSimplex2S(seed)) {
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    loadTerrain(vertices, normals, texCoords, indices, filename);
    numOfVertices = indices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &texCoordBuffer);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &normalBuffer);
    bindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unbindVAO();

    std::cout << "Construction VAO: " << VAO << '\n';
}

TerrainMesh::~TerrainMesh() {
    std::cout << "Destruction VAO: " << VAO << '\n';
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &texCoordBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void TerrainMesh::bindVAO() const {
    glBindVertexArray(VAO);
}

void TerrainMesh::unbindVAO() {
    glBindVertexArray(0);
}

[[nodiscard]] unsigned int TerrainMesh::getNumOfVertices() const {
    return numOfVertices;
}

void TerrainMesh::loadTerrain(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &texCoords, std::vector<unsigned int> &indices, const char *const filename) {
    int width{}, nrchannels{};
    unsigned char *const data = stbi_load(filename, &width, &height, &nrchannels, 1);
    dataContainer = std::make_shared<stb_PointerContainer>(data, width, height, nrchannels);

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < height; ++j) {
            const auto floatI = static_cast<float>(i);
            const auto floatJ = static_cast<float>(j);

            const auto heightAsFloat = static_cast<float>(height - 1);

            const float iOverHeight = floatI/heightAsFloat;
            const float jOverHeight = floatJ/heightAsFloat;

            vertices.emplace_back(jOverHeight * SIZE, getHeight(j, i), iOverHeight * SIZE);
            normals.push_back(calculateNormal(j, i));
            texCoords.emplace_back(jOverHeight, iOverHeight);
        }
    }

    for(int gz = 0; gz < (height - 1); ++gz){
        for(int gx = 0; gx < (height - 1); ++gx){
            const int topLeft = gz * height + gx;
            const int topRight = topLeft + 1;
            const int bottomLeft = (gz + 1) * height + gx;
            const int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

[[nodiscard]] float TerrainMesh::getHeightFromNoise(double x, double z) const {
    assert(x <= height);
    assert(z <= height);

    constexpr float fRadius = 5.f; //modify this to scale noise

    const double fScaleX = x / (height-1); //0 <= x <= fPeriodSize
    const double fPeriodX = fScaleX * 2 * M_PI; //0 <= fPeriod <= 2PI
    x = fRadius * (std::sin(fPeriodX) + 1);

    //apply same to y
    const double fScaleZ = z / (height-1);
    const double fPeriodZ = fScaleZ * 2 * M_PI;
    z = fRadius * (std::cos(fPeriodZ) + 1);

    return static_cast<float>(noise->Noise2(x/1000.0, z/1000.0) + noise->Noise2(x/100.0, z/100.0) + noise->Noise2(x, z)) * multiplier;
}

[[nodiscard]] float TerrainMesh::getHeightFromHeightmap(const int x, const int z) const {
    const int clampedX = std::clamp(x, 0, getWidth());
    const int clampedZ = std::clamp(z, 0, getWidth());
    const int index = clampedZ * height + clampedX;
    const float data = dataContainer->getData()[index];
    const float ret = (data/127.5f - 1.0f) * MAX_HEIGHT;
    return ret;
}

[[nodiscard]] glm::vec3 TerrainMesh::calculateNormal(const int x, const int z) const {
    const float heightL = getHeight(x - 1, z);
    const float heightR = getHeight(x + 1, z);
    const float heightU = getHeight(x, z + 1);
    const float heightD = getHeight(x, z - 1);
    const glm::vec3 normal(heightL - heightR, 2, heightD - heightU);
    return glm::normalize(normal);
}

[[nodiscard]] int TerrainMesh::getWidth() const {
    return height - 1;
}

[[nodiscard]] float TerrainMesh::getHeight(const int x, const int z) const {
    return (!noise.has_value()) ? getHeightFromHeightmap(x, z) : getHeightFromNoise(x, z);
}
