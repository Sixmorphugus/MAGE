#pragma once
#include "resources.h"

namespace mage {

// SFML Shader Resource
class MAGEDLL resourceShader : public resource
{
public:
	resourceShader();
	resourceShader(const resourceShader& copy);
	resourceShader(std::string singleShader, sf::Shader::Type type);
	resourceShader(std::string vfComboV, std::string vfComboF);
	resourceShader(std::string vgfComboV, std::string vgfComboG, std::string vgfComboF);

	resourceShader& operator=(const resourceShader& copy);

	bool load();

	sf::Shader* get();

	bool isType(sf::Shader::Type t);
	void setTypePath(sf::Shader::Type t, std::string path);

	// SFML proxy functions

public:
	std::string vertexPath;
	std::string geometryPath;
	std::string fragmentPath;

private:
	void showShaderErrorOnce(std::string err);

	sf::Shader m_data;
	bool shShaderError;
};

} // namespace mage