#pragma once

// Serialization
// -------------
// Helper functions and Macros for serializing things.
// Mainly just pipe over cereal's functionality.
// -------------
// Category: Core
// Has Hooks: Yes
// Script-Accessable: Mostly (see comments)
// -------------

#include "CerealAfx.h"
#include "platform.h"

namespace mage {

namespace serialization {

	template<class Archive, class T>
	bool serializeFile(const std::string& filePath, const T& toSave) {
		std::ofstream writeOut(filePath);

		if (writeOut.bad()) {
			p::warn("Serialization failed, bad file.");
			return false;
		}

		{
			Archive arch(writeOut);
			arch(toSave);
		}
			
		writeOut.close();

		return true;
	}

	template<class Archive, class T>
	bool deserializeFile(const std::string& filePath, T& toLoad) {
		std::ifstream writeOut(filePath);

		if (writeOut.bad()) {
			p::warn("Deserialization failed, bad file.");
			return false;
		}

		{
			Archive arch(writeOut);
			arch(toLoad);
		}

		writeOut.close();

		return true;
	}

	template<class T>
	bool saveBinaryFile(const T& toSave, const std::string& filePath) {
		return serializeFile<cereal::PortableBinaryOutputArchive>(filePath, toSave);
	}

	template<class T>
	bool saveJsonFile(const T& toSave, const std::string& filePath) {
		return serializeFile<cereal::JSONOutputArchive>(filePath, toSave);
	}

	template<class T>
	bool saveXmlFile(const T& toSave, const std::string& filePath) {
		return serializeFile<cereal::XMLOutputArchive>(filePath, toSave);
	}

	template<class T>
	bool loadBinaryFile(T& toLoad, const std::string& filePath) {
		return deserializeFile<cereal::PortableBinaryInputArchive>(filePath, toLoad);
	}

	template<class T>
	bool loadJsonFile(T& toLoad, const std::string& filePath) {
		return deserializeFile<cereal::JSONInputArchive>(filePath, toLoad);
	}

	template<class T>
	bool loadXmlFile(T& toLoad, const std::string& filePath) {
		return deserializeFile<cereal::XMLInputArchive>(filePath, toLoad);
	}

	template<class T>
	std::string getXml(const T& toConvert) {
		std::ostringstream str;
		
		{
			cereal::XMLOutputArchive a(str);
			a(toConvert);
		}
		
		return str.str();
	}

	template<class T>
	void setFromXml(T& toConvert, std::string input) {
		std::istringstream str(input);

		{
			cereal::XMLInputArchive a(str);
			a(toConvert);
		}
	}

	template<class T>
	std::string getJson(const T& toConvert) {
		std::ostringstream str;

		{
			cereal::JSONOutputArchive a(str);
			a(toConvert);
		}

		return str.str();
	}
	
	template<class T>
	void setFromJson(T& toConvert, std::string input) {
		std::istringstream str;

		{
			cereal::JSONInputArchive a(str);
			a(toConvert);
		}
	}

}

namespace s = serialization;

}

#define MAGE_DeclareSerializationBase(type) CEREAL_REGISTER_TYPE(type)

#define MAGE_DeclareSerializationList(...)\
template<class Archive>\
void serialize(Archive& archive) {\
	archive(__VA_ARGS__);\
}

#define MAGE_SerializedBase(baseClass) cereal::base_class<baseClass>(this)
#define MAGE_SerializedNVP cereal::make_nvp
#define MAGE_SerializedN CEREAL_NVP