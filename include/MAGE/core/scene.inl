#include "scene.h"
namespace mage {
template<class T2>
inline std::vector<std::shared_ptr<T2>> scene::list() const
{
	std::vector<std::shared_ptr<T2>> retList;

	for (unsigned int i = 0; i < m_objectList.size(); i++) {
		std::shared_ptr<T2> cast = std::dynamic_pointer_cast<T2>(m_objectList[i]);

		if (cast) {
			retList.push_back(cast);
		}
	}

	return retList;
}


template<class Filter>
inline void scene::combine(const scene *gIn)
{
	if (ignoreIncorporation) return;

	gIn->onCombined.notify(gIn, this);
	incorporate<Filter>(gIn);
}


template<class Filter>
inline void scene::set(const scene *gIn)
{
	if (ignoreIncorporation) return;

	gIn->onSet.notify(gIn, this);
	clearObjects<Filter>();
	incorporate<Filter>(gIn);
}


template<class Filter>
inline void scene::incorporate(const scene* gIn)
{
	if (gIn == nullptr)
		return;

	gIn->onIncorporated.notify(gIn, this);

	// add everything from the scene<T2> class that matches the type Filter
	std::vector<std::shared_ptr<Filter>> oList = gIn->list<Filter>();

	for (unsigned int i = 0; i < oList.size(); i++) {
		if (!oList[i]->isCloneable())
			p::fatal("Object was not cloneable");

		auto clone = oList[i]->clone();
		attach(clone); // the template will not compile if the Filter isn't a compatible type so this is fine
	}
}


template<class T>
inline std::shared_ptr<T> scene::getAs(unsigned int index) const
{
	return std::dynamic_pointer_cast<T>(get(index));
}

template<class T2>
inline void scene::clearObjects()
{
	for (int i = 0; i < (int)m_objectList.size(); i++) {
		auto cast = std::dynamic_pointer_cast<T2>(m_objectList[i]);

		if (cast) {
			detach(i);
			i--;
		}
	}
}
}