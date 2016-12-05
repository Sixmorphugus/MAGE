#pragma once
#include "resources.h"

namespace mage {

// SFML Shader Resource
class MAGEDLL resourceShader : public resource
{
public:
	resourceShader();
	resourceShader(std::string singleShader, sf::Shader::Type type);
	resourceShader(std::string vfComboV, std::string vfComboF);
	resourceShader(std::string vgfComboV, std::string vgfComboG, std::string vgfComboF);

	bool load();
	void unload();

	std::shared_ptr<sf::Shader> get();

	bool isType(sf::Shader::Type t);
	void setTypePath(sf::Shader::Type t, std::string path);

public:
	std::string vertexPath;
	std::string geometryPath;
	std::string fragmentPath;

private:
	void showShaderErrorOnce(std::string err);

	std::shared_ptr<sf::Shader> data;
	bool shShaderError;
};

} // namespace mage