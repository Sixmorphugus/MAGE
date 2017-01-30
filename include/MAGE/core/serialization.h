#pragma once

// Serialization
// -------------
// Helper functions for serializing things.
// -------------
// Category: Core
// Has Hooks: Yes
// Script-Accessable: Mostly (see comments)
// -------------

#include "CerealAfx.h"
#include "StdAfx.h"

namespace mage {

namespace serialization {
	template<class Archive, class T>
	serializeFile(const std::string& filePath, const T& toSave) {
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

	template<class T>
	saveBinaryFile(const std::string& filePath, const T& toSave) {
		serializeFile<cereal::PortableBinaryOutputArchive>(filePath, toSave);
	}

	template<class T>
	saveJsonFile(const std::string& filePath, const T& toSave) {
		serializeFile<cereal::JSONOutputArchive>(filePath, toSave);
	}

	template<class T>
	saveXmlFile(const std::string& filePath, const T& toSave) {
		serializeFile<cereal::XMLOutputArchive>(filePath, toSave);
	}

	template<class T>
	loadBinaryFile(const std::string& filePath, const T& toLoad) {
		serializeFile<cereal::PortableBinaryInputArchive>(filePath, toLoad);
	}

	template<class T>
	loadJsonFile(const std::string& filePath, const T& toLoad) {
		serializeFile<cereal::JSONInputArchive>(filePath, toLoad);
	}

	template<class T>
	loadXmlFile(const std::string& filePath, const T& toLoad) {
		serializeFile<cereal::XMLInputArchive>(filePath, toLoad);
	}
}

namespace s = serialization;

}