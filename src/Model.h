#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Mesh.h"
#include "Shader.h"

uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model {
	std::vector<Mesh> meshes;
	std::string meshDirectory;
	std::vector<Texture> loadedTextures;
public:
	Model(const char* path) {
		loadModel(path);
	}
	void Draw(Shader& shader);
private:
	

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, TextureType typeName);
	
};


void Model::Draw(Shader& shader) {
	for (uint32_t i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

std::vector<Texture> Model::loadMaterialTexture(aiMaterial* mat, aiTextureType type, TextureType typeName) {
	std::vector<Texture> textures;
	for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (uint32_t j = 0; j < loadedTextures.size(); j++) {
			if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), meshDirectory);
			texture.textureType = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
		
	}
	return textures;
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;
	bool hasTexture;
	ModelMaterial mat;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex{
			.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
			.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
			.texCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f, 0.0f),
			.tangents = mesh->HasTangentsAndBitangents() ? glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z) : glm::vec3(0.0f, 0.0f, 0.0f),
			.bitTangents = mesh->HasTangentsAndBitangents() ? glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z) : glm::vec3(0.0f, 0.0f, 0.0f)
		};
		vertices.push_back(vertex);
		
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		aiColor3D color(0.0f, 0.0f, 0.0f);
		float shininess = 0.0f;

		if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) != AI_SUCCESS) {
			color = aiColor3D(0.2f, 0.2f, 0.2f);
			
		}
		mat.ambient = glm::vec3(color.r, color.g, color.b);

		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) != AI_SUCCESS) {
			color = aiColor3D(0.5f, 0.5f, 0.5f);

		}
		mat.diffuse = glm::vec3(color.r, color.g, color.b);

		if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) != AI_SUCCESS) {
			color = aiColor3D(0.5f, 0.5f, 0.5f);

		}
		mat.specular = glm::vec3(color.r, color.g, color.b);

		if (material->Get(AI_MATKEY_SHININESS, shininess) != AI_SUCCESS) {
			shininess = 32.0f;

		}
		mat.shininess = 32.0f;

	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, TextureType::Specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<Texture> normalMaps = loadMaterialTexture(material, aiTextureType_HEIGHT, TextureType::Normal);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		std::vector<Texture> baseColorMaps = loadMaterialTexture(material, aiTextureType_BASE_COLOR, TextureType::BaseColor);
		textures.insert(textures.end(), baseColorMaps.begin(), baseColorMaps.end());
		std::vector<Texture> metallicMaps = loadMaterialTexture(material, aiTextureType_REFLECTION, TextureType::Metallic);
		textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());

		if (diffuseMaps.size() == 0) {
			hasTexture = false;
		}
		else {
			hasTexture = true;
		}		
		
	}

	return Mesh(vertices, indices, textures, hasTexture, mat);

}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (uint32_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

void Model::loadModel(std::string path) {
	Assimp::Importer importer;
	Assimp::DefaultLogger::create("assimp_log.txt", Assimp::Logger::VERBOSE);
	std::ifstream file(path);
	if (!file) {
		std::cerr << "File not found!" << std::endl;
	}
	Assimp::DefaultLogger::get()->info("Starting GLB import...");
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	Assimp::DefaultLogger::kill();
	meshDirectory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
	
}

uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma) {
	std::string filename = path;
	filename = directory + "/" + filename;


	uint32_t texture_id;
	glGenTextures(1, &texture_id);

	int32_t width, height, n_channels;
	uint8_t* data = stbi_load(filename.c_str(), &width, &height, &n_channels, 0);

	if (data) {
		GLenum format;
		if (n_channels == 1) {
			format = GL_RED;
		}
		else if (n_channels == 2) {
			format = GL_RG;
		}
		else if (n_channels == 3) {
			format = GL_RGB;
		}
		else if (n_channels == 4) {
			format = GL_RGBA;
		}
		else {
			std::cout << "Unsupported texture format" << std::endl;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);


	}
	else {
		std::cout << "Texture failed to load at path:" << path << std::endl;
	}
	return texture_id;

}


