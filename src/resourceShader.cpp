#include "resourceShader.h"

using namespace mage;

// shader resource
resourceShader::resourceShader()
	: shShaderError(false)
{
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

bool resourceShader::load()
{
	data = std::make_shared<sf::Shader>();

	bool anythingFailed = false;

	if (isType(sf::Shader::Vertex)) {
		p::log("\tVertex Shader: " + vertexPath);
		if (!data->loadFromFile(vertexPath, sf::Shader::Vertex)) anythingFailed = true;
	}

	if (isType(sf::Shader::Geometry)) {
		p::log("\tGeometry Shader: " + geometryPath);
		if (!data->loadFromFile(geometryPath, sf::Shader::Geometry)) anythingFailed = true;
	}

	if (isType(sf::Shader::Fragment)) {
		p::log("\tFragment Shader: " + fragmentPath);
		if (!data->loadFromFile(fragmentPath, sf::Shader::Fragment)) anythingFailed = true;
	}

	if (anythingFailed)
		return false;

	return resource::load();
}

void resourceShader::unload()
{
	data = std::shared_ptr<sf::Shader>();

	resource::unload();
}

std::shared_ptr<sf::Shader> resourceShader::get()
{
	return data;
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
		showShaderErrorOnce("This machine does not support shaders!");
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

DeclareScriptingResource(resourceShader);
DeclareScriptingConstructor(resourceShader(std::string, sf::Shader::Type), "resourceShader");
DeclareScriptingConstructor(resourceShader(std::string, std::string), "resourceShader");
DeclareScriptingConstructor(resourceShader(std::string, std::string, std::string), "resourceShader");
DeclareScriptingFunction(&resourceShader::get, "get");
DeclareScriptingFunction(&resourceShader::vertexPath, "vertexPath");
DeclareScriptingFunction(&resourceShader::geometryPath, "geometryPath");
DeclareScriptingFunction(&resourceShader::fragmentPath, "fragmentPath");
DeclareScriptingFunction(&resourceShader::setTypePath, "setTypePath");