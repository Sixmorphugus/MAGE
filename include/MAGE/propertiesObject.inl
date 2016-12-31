namespace mage {
	template<typename T>
	inline propertiesObject::prop<T>::prop(std::function<T(propertiesObject*)> getFunction, 
		std::function<void(propertiesObject*, T)> setFunction, 
		std::function<T(std::string&)> strConvFunction1, 
		std::function<std::string(T&)> strConvFunction2)
	{
		m_getFunction = getFunction;
		m_setFunction = setFunction;
		m_strConv1 = strConvFunction1;
		m_strConv2 = strConvFunction2;
	}

	template<typename T>
	inline T propertiesObject::prop<T>::get()
	{
		if (!m_whatIsThis)
			return;

		if(m_getFunction)
			return m_getFunction(m_whatIsThis);

		return 0;
	}

	template<typename T>
	inline void propertiesObject::prop<T>::set(T newVal)
	{
		if (!m_whatIsThis)
			return;

		if (m_setFunction) {
			m_setFunction(m_whatIsThis, newVal);
			onSet(this);
		}
	}

	template<typename T>
	inline std::type_info & propertiesObject::prop<T>::getTypeInfo()
	{
		return typeid(T);
	}

	template<typename T>
	inline std::string propertiesObject::prop<T>::getString()
	{
		if (m_strConv2) { // use a conversion if we have one
			return m_strConv2(get());
		}

		// if we don't, stick it in a string stream + hope shit works out
		std::stringstream ss;
		ss < get();

		return ss.str();
	}

	template<typename T>
	inline void propertiesObject::prop<T>::setString(std::string str)
	{
		// again, use a conversion if we have one
		if (m_strConv1) {
			set(m_strConv1(str));
			return;
		}

		// if we don't, we have two options
		// if it's actually a string:
		if (getTypeInfo() == typeid(std::string)) {
			set((T)str); // (T) is neccesary for compilation - we know it's std::string though
		}
		// if it's not:
		else {
			set((T)atof(str.c_str())); // a double conversion covers all numeric cases (IN THEORY.)
		}
	}

	template<typename T>
	inline bool propertiesObject::prop<T>::isReadOnly()
	{
		return m_setFunction;
	}

	template<typename T>
	inline std::function<T(std::string&)> propertiesObject::prop<T>::getStringFromConverter()
	{
		return m_strConv1;
	}

	template<typename T>
	inline std::function<std::string(T&)> propertiesObject::prop<T>::getStringToConverter()
	{
		return m_strConv2;
	}
}