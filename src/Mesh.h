#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <vector>

#include "Shader.h"


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangents;
	glm::vec3 bitTangents;

};

struct ModelMaterial {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

enum TextureType {
	Specular,
	Diffuse,
	Normal,
	Height,
	BaseColor,
	Emissive,
	Metallic,
	Roughness
};

struct Texture {
	uint32_t id;
	TextureType textureType;
	std::string path;
};

class Mesh {
public:
	std::vector<Vertex> meshVertices;
	std::vector<uint32_t> meshIndices;
	std::vector<Texture> meshTextures;
	bool hasTexture;
	ModelMaterial material;
	

	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures, bool textureExists, ModelMaterial newMaterial) : meshVertices(std::move(vertices)), meshIndices(std::move(indices)), meshTextures(std::move(textures)), hasTexture(textureExists) {
		material.ambient = newMaterial.ambient;
		material.diffuse = newMaterial.diffuse;
		material.specular = newMaterial.specular;
		material.shininess = newMaterial.shininess;
		setupMesh();	
	};
	void Draw(Shader& shader) {
		uint32_t diffuseNx = 1;
		uint32_t specularNx = 1;
		uint32_t normalNx = 1;
		uint32_t heightNx = 1;
		uint32_t metalNx = 1;


		for (uint32_t i = 0; i < meshTextures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			std::string textureName;
			std::string number;
			if (meshTextures[i].textureType == TextureType::Diffuse) {
				textureName = "texture_diffuse";
				number = std::to_string(diffuseNx++);
			}
			else if(meshTextures[i].textureType == TextureType::Specular) {
				textureName = "texture_specular";
				number = std::to_string(specularNx++);
			}
			else if (meshTextures[i].textureType == TextureType::Normal) {
				textureName = "texture_normal";
				number = std::to_string(normalNx++);
			}
			else if (meshTextures[i].textureType == TextureType::Height) {
				textureName == "texture_height";
				number = std::to_string(heightNx++);
			}
			else if (meshTextures[i].textureType == TextureType::Metallic) {
				textureName == "texture_metal";
				number = std::to_string(metalNx++);
			}
			glUniform1i(glGetUniformLocation(shader.ID, (textureName + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, meshTextures[i].id);

		}
		glUniform1i(glGetUniformLocation(shader.ID, "hasTexture"), hasTexture);
		glUniform3fv(glGetUniformLocation(shader.ID, "material.ambient"), 1, glm::value_ptr(material.ambient));
		glUniform3fv(glGetUniformLocation(shader.ID, "material.diffuse"), 1, glm::value_ptr(material.diffuse));
		glUniform3fv(glGetUniformLocation(shader.ID, "material.specular"), 1, glm::value_ptr(material.specular));
		glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), material.shininess);
		
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, meshIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	};

private:
	uint32_t vao;
	uint32_t vbo;
	uint32_t ebo;

	void setupMesh() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);


		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(Vertex), &meshVertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices.size() * sizeof(uint32_t), &meshIndices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tangents)));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, bitTangents)));

		glBindVertexArray(0);

	}
};

