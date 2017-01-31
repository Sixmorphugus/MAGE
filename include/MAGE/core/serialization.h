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
	void serializeFile(const std::string& filePath, const T& toSave) {
		std::ofstream writeOut(filePath);

		if (writeOut.bad()) {
			p::warn("Serialization failed, bad file.");
			return;
		}

		{
			Archive arch(writeOut);
			arch(toSave);
		}
			
		writeOut.close();

		p::info("Saved " + filePath);
	}

	template<class Archive, class T>
	void deserializeFile(const std::string& filePath, T& toLoad) {
		std::ifstream writeOut(filePath);

		if (writeOut.bad()) {
			p::warn("Deserialization failed, bad file.");
			return;
		}

		{
			Archive arch(writeOut);
			arch(toLoad);
		}

		writeOut.close();

		p::info("Loaded " + filePath);
	}

	template<class T>
	void saveBinaryFile(const T& toSave, const std::string& filePath) {
		serializeFile<cereal::PortableBinaryOutputArchive>(filePath, toSave);
	}

	template<class T>
	void saveJsonFile(const T& toSave, const std::string& filePath) {
		serializeFile<cereal::JSONOutputArchive>(filePath, toSave);
	}

	template<class T>
	void saveXmlFile(const T& toSave, const std::string& filePath) {
		serializeFile<cereal::XMLOutputArchive>(filePath, toSave);
	}

	template<class T>
	void loadBinaryFile(T& toLoad, const std::string& filePath) {
		deserializeFile<cereal::PortableBinaryInputArchive>(filePath, toLoad);
	}

	template<class T>
	void loadJsonFile(T& toLoad, const std::string& filePath) {
		deserializeFile<cereal::JSONInputArchive>(filePath, toLoad);
	}

	template<class T>
	void loadXmlFile(T& toLoad, const std::string& filePath) {
		deserializeFile<cereal::XMLInputArchive>(filePath, toLoad);
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