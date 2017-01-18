#include "point.h"

namespace mage {
	template<typename T> class point;
	typedef point<float> pointF;
	typedef point<int> pointI;
	typedef point<unsigned int> pointU;

	// prop functions
	template<typename T>
	inline prop<T>::prop(std::string name,
		std::function<T(const propertiesObject*)> getFunction,
		std::function<void(propertiesObject*, T)> setFunction,
		std::function<T(std::string&)> strConvFunction1,
		std::function<std::string(T&)> strConvFunction2)
		: propBase(name)
	{
		m_getFunction = getFunction;
		m_setFunction = setFunction;
		m_strConv1 = strConvFunction1;
		m_strConv2 = strConvFunction2;
	}

	template<typename T>
	inline T prop<T>::get() const
	{
		if (!m_whatIsThis)
			return 0;

		if(m_getFunction)
			return m_getFunction(m_whatIsThis);

		return 0;
	}

	template<typename T>
	inline void prop<T>::set(T newVal)
	{
		if (!m_whatIsThis)
			return;

		if (m_setFunction) {
			m_setFunction(m_whatIsThis, newVal);
			onSet.notify(this);
		}
	}

	template<typename T>
	inline const std::type_info & prop<T>::getTypeInfo() const
	{
		return typeid(T);
	}

	template<typename T>
	inline std::string prop<T>::getString() const
	{
		if (m_strConv2) { // use a conversion if we have one
			T obj = get();
			return m_strConv2(obj);
		}

		// if we don't, stick it in a string stream + hope shit works out
		std::stringstream ss;
		ss << get();

		return ss.str();
	}
	template<>
	inline std::string prop<pointF>::getString() const
	{
		if (m_strConv2) { // use a conversion if we have one
			return m_strConv2(get());
		}

		// if we don't, stick it in a string stream + hope it works out
		std::stringstream ss;
		ss << get().x << " " << get().y << " " << get().z;

		return ss.str();
	}
	template<>
	inline std::string prop<pointI>::getString() const
	{
		if (m_strConv2) { // use a conversion if we have one
			return m_strConv2(get());
		}

		// if we don't, stick it in a string stream + hope it works out
		std::stringstream ss;
		ss << get().x << " " << get().y << " " << get().z;

		return ss.str();
	}
	template<>
	inline std::string prop<pointU>::getString() const
	{
		if (m_strConv2) { // use a conversion if we have one
			return m_strConv2(get());
		}

		// if we don't, stick it in a string stream + hope it works out
		std::stringstream ss;
		ss << get().x << " " << get().y << " " << get().z;

		return ss.str();
	}

	template<typename T>
	inline void prop<T>::setString(std::string str)
	{
		// again, use a conversion if we have one
		if (m_strConv1) {
			set(m_strConv1(str));
			return;
		}

		set((T)atof(str.c_str())); // a double conversion covers all numeric cases (IN THEORY.)
	}
	template<>
	inline void prop<std::string>::setString(std::string str)
	{
		// again, use a conversion if we have one
		if (m_strConv1) {
			set(m_strConv1(str));
			return;
		}

		set(str);
	}
	template<>
	inline void prop<pointF>::setString(std::string str)
	{
		// again, use a conversion if we have one
		if (m_strConv1) {
			set(m_strConv1(str));
			return;
		}

		set(pointF(str));
	}
	template<>
	inline void prop<pointI>::setString(std::string str)
	{
		// again, use a conversion if we have one
		if (m_strConv1) {
			set(m_strConv1(str));
			return;
		}

		set(pointI(str));
	}
	inline void prop<pointU>::setString(std::string str)
	{
		// again, use a conversion if we have one
		if (m_strConv1) {
			set(m_strConv1(str));
			return;
		}

		set(pointU(str));
	}

	template<typename T>
	inline bool prop<T>::isReadOnly() const
	{
		return (m_setFunction == nullptr);
	}

	template<typename T>
	inline std::function<T(std::string&)> prop<T>::getStringFromConverter() const
	{
		return m_strConv1;
	}

	template<typename T>
	inline std::function<std::string(T&)> prop<T>::getStringToConverter() const
	{
		return m_strConv2;
	}

	template<typename T>
	inline std::string prop<T>::instPropStringGet(propertiesObject& inst)
	{
		// a hack to get a property as a string on a specific instance.
		auto oldThis = m_whatIsThis;

		m_whatIsThis = &inst;
		auto str = getString();
		m_whatIsThis = oldThis;

		return str;
	}

	template<typename T>
	inline void prop<T>::instPropStringSet(propertiesObject& inst, std::string setTo)
	{
		// a hack to get a property as a string on a specific instance.
		auto oldThis = m_whatIsThis;

		m_whatIsThis = &inst;
		setString(setTo);
		m_whatIsThis = oldThis;
	}

	// actual object functions
	template<typename T>
	inline std::shared_ptr<prop<T>> propertiesObject::createPropertyProxy(unsigned int pr)
	{
		// clean the safety list up when we call this
		safetyListCleanup();

		// get the actual property object
		auto pData = getPropertyData(pr);

		// does it convert to the right property type
		auto propConv = std::dynamic_pointer_cast<prop<T>>(pData);

		if (!propConv)
			return nullptr;

		// make a copy.
		std::shared_ptr<prop<T>> propConvCpy = std::make_shared<prop<T>>(*propConv.get());
		propConvCpy->m_whatIsThis = this;
		propConvCpy->m_dataInstance = propConv.get();

		// add the copy to our safety list.
		// the safety list makes sure that any remote properties that still exist for this object know when we are destroyed and can't crash the game.
		m_safetyList.push_back(std::weak_ptr<prop<T>>(propConvCpy));

		// return the copy.
		return propConvCpy;
	}
	template<typename T>
	inline std::shared_ptr<prop<T>> propertiesObject::createPropertyProxy(std::string prop)
	{
		for (unsigned int i = 0; i < m_props.size(); i++) {
			if (m_props[i]->getName() == prop) {
				return createPropertyProxy<T>(i);
			}
		}

		return nullptr;
	}
}