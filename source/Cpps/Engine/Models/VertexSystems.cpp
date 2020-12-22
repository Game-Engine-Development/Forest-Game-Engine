#include "Headers/Engine/Models/VertexSystems.h"

#include "Headers/Engine/Scene/Components.h"

unsigned int generateMesh(const std::string &path, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Component::TextureComponent> &textures) {
    unsigned int VAO{}, VBO{}, EBO{};

    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);

    LookupTables::ModelCache.at(path).first.meshes.push_back(Component::Mesh{VAO, indices.size(), vertices, indices, textures});
    LookupTables::ModelCache.at(path).first.meshIDs.emplace_back(VAO, VBO, EBO, indices.size());

    return VAO;
}

std::vector<Component::TextureComponent> loadMaterialTextures(Component::Model &model, aiMaterial *mat, aiTextureType type, const std::string &typeName) {
    std::vector<Component::TextureComponent> textures;
    std::int32_t textureUnit = 0;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::cout << "texturePath: " << (model.path.substr(0, model.path.find_last_of('/')) + '/' + std::string(str.C_Str())) << '\n';
        Component::TextureComponent texture(model.path.substr(0, model.path.find_last_of('/')) + '/' + std::string(str.C_Str()), textureUnit++, type, typeName);
        textures.push_back(texture);
    }
    return textures;
}

Component::Mesh processMesh(Component::Model &model, aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Component::TextureComponent> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if(mesh->mTextureCoords[0]) //check is mesh contains texture coordinates
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process material
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Component::TextureComponent> diffuseMaps = loadMaterialTextures(model, material,
                                                           aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(std::end(textures), std::begin(diffuseMaps), std::end(diffuseMaps));
        std::vector<Component::TextureComponent> specularMaps = loadMaterialTextures(model, material,
                                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(std::end(textures), std::begin(specularMaps), std::end(specularMaps));
    }

    return Component::Mesh{generateMesh(model.path, vertices, indices, textures), indices.size(), vertices, indices, textures};
}

void processNode(Component::Model &model, aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        model.meshes.push_back(processMesh(model, mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(model, node->mChildren[i], scene);
    }
}

std::optional<Component::Model> loadModel(const std::string &path)
{
    if(LookupTables::ModelCache.count(path)) {
        LookupTables::ModelCache.at(path).second += 1u;
        return Component::Model(LookupTables::ModelCache.at(path).first.meshes, path);
    } else {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << '\n';
            return std::nullopt;
        }

        Component::Model model;

        model.path = path;

        //the loading system will just push_back into the meshes and meshIDs std::vectors
        LookupTables::ModelCache.emplace(path, std::pair(ModelMeshesResourceContainer{{}, {}}, 1u));

        processNode(model, scene->mRootNode, scene);

        return model;
    }
}
