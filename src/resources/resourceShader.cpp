#include "resourceShader.h"

using namespace mage;

// shader resource
resourceShader::resourceShader()
{
	shShaderError = false;
}

resourceShader::resourceShader(const resourceShader & copy)
	: resource(copy)
{
	shShaderError = false;
}

resourceShader::resourceShader(std::string singleShader, sf::Shader::Type type)
	: shShaderError(false)
{
	setTypePath(type, singleShader);
}

resourceShader::resourceShader(std::string vfComboV, std::string vfComboF)
	: shShaderError(false)
{
	setTypePath(sf::Shader::Vertex, vfComboV);
	setTypePath(sf::Shader::Fragment, vfComboF);
}

resourceShader::resourceShader(std::string vgfComboV, std::string vgfComboG, std::string vgfComboF)
	: shShaderError(false)
{
	setTypePath(sf::Shader::Vertex, vgfComboV);
	setTypePath(sf::Shader::Geometry, vgfComboF);
	setTypePath(sf::Shader::Fragment, vgfComboF);
}

resourceShader& mage::resourceShader::operator=(const resourceShader & copy)
{
	resource::operator=(copy);

	return *this;
}

bool resourceShader::load()
{
	bool anythingFailed = false;

	if (isType(sf::Shader::Vertex)) {
		p::log("\tVertex Shader: " + vertexPath);
		if (!m_data.loadFromFile(vertexPath, sf::Shader::Vertex)) anythingFailed = true;
	}

	if (isType(sf::Shader::Geometry)) {
		p::log("\tGeometry Shader: " + geometryPath);
		if (!m_data.loadFromFile(geometryPath, sf::Shader::Geometry)) anythingFailed = true;
	}

	if (isType(sf::Shader::Fragment)) {
		p::log("\tFragment Shader: " + fragmentPath);
		if (!m_data.loadFromFile(fragmentPath, sf::Shader::Fragment)) anythingFailed = true;
	}

	if (anythingFailed)
		return false;

	return resource::load();
}

sf::Shader* resourceShader::get()
{
	return &m_data;
}

bool resourceShader::isType(sf::Shader::Type t)
{
	switch (t) {
	case sf::Shader::Fragment:
		return fragmentPath != "";
	case sf::Shader::Geometry:
		return geometryPath != "";
	case sf::Shader::Vertex:
		return vertexPath != "";
	}

	return false;
}

void resourceShader::setTypePath(sf::Shader::Type t, std::string path)
{
	auto rpath = p::realPath(path);

	if (!sf::Shader::isAvailable()) {
		showShaderErrorOnce("This machine does not support shaders.");
	}

	switch (t) {
	case sf::Shader::Fragment:
		fragmentPath = rpath;
		break;
	case sf::Shader::Geometry:
		geometryPath = rpath;

		if (!sf::Shader::isGeometryAvailable()) {
			showShaderErrorOnce("This machine does not support geometry shaders.");
		}

		break;
	case sf::Shader::Vertex:
		vertexPath = rpath;
		break;
	}
}

void resourceShader::showShaderErrorOnce(std::string err)
{
	if (!shShaderError) {
		p::warn(err);
		shShaderError = true;
	}
}

// sfShaderResource
#include "scriptingEngine.h"

MAGE_DeclareScriptingResource(resourceShader);
MAGE_DeclareScriptingConstructor(resourceShader(std::string, sf::Shader::Type), "resourceShader");
MAGE_DeclareScriptingConstructor(resourceShader(std::string, std::string), "resourceShader");
MAGE_DeclareScriptingConstructor(resourceShader(std::string, std::string, std::string), "resourceShader");
MAGE_DeclareScriptingFunction(&resourceShader::get, "get");
MAGE_DeclareScriptingFunction(&resourceShader::vertexPath, "vertexPath");
MAGE_DeclareScriptingFunction(&resourceShader::geometryPath, "geometryPath");
MAGE_DeclareScriptingFunction(&resourceShader::fragmentPath, "fragmentPath");
MAGE_DeclareScriptingFunction(&resourceShader::setTypePath, "setTypePath");