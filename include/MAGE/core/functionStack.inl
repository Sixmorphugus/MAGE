namespace mage {
	template<typename _Returns, typename ..._Args>
	inline functionStack<_Returns, ..._Args>::functionStack()
	{
		m_stackOffset = 0;
	}
	template<typename _Returns, typename ..._Args>
	inline functionStack<_Returns, ..._Args>::functionStack(std::function<_Returns(functionStack&, _Args...)> bottom)
	{
		m_stackOffset = 0;
		pushFunction(bottom);
	}
	template<typename _Returns, typename ..._Args>
	inline _Returns functionStack<_Returns, ..._Args>::call(unsigned int stackOffset, _Args ... a) const
	{
		if (getNumFunctions() == 0) {
			p::fatal("Empty function stack was called"); // currently unrecoverable
		}

		if (stackOffset >= getNumFunctions()) {
			p::fatal("Call called on/from beyond bottom of function stack"); // could be a "warn" but would result in annoying behavior
			stackOffset = getNumFunctions() - 1;
		}

		unsigned int psov = m_stackOffset;

		m_stackOffset = stackOffset + 1; // stack offset for next execution should be one below this one
		_Returns ret = m_stack[m_stack.size() - 1 - stackOffset](this, a...);
		m_stackOffset = psov;

		return ret;
	}
	template<typename _Returns, typename ..._Args>
	inline unsigned int functionStack<_Returns, ..._Args>::getNumFunctions() const
	{
		return m_stack.size();
	}
	template<typename _Returns, typename ..._Args>
	inline unsigned int functionStack<_Returns, ..._Args>::pushFunction(std::function<_Returns(functionStack&, _Args...)> func)
	{
		m_stack.push_back(func);
		return getNumFunctions()-1;
	}
	template<typename _Returns, typename ..._Args>
	inline unsigned int functionStack<_Returns, ..._Args>::popFunction()
	{
		m_stack.pop_back();
		return getNumFunctions();
	}
	template<typename _Returns, typename ..._Args>
	inline void functionStack<_Returns, ..._Args>::clear()
	{
		m_stack.clear();
	}
	template<typename _Returns, typename ..._Args>
	inline _Returns functionStack<_Returns, ..._Args>::operator()(_Args ... a) const
	{
		return call(m_stackOffset, a...);
	}
	template<typename _Returns, typename ..._Args>
	inline unsigned int functionStack<_Returns, ..._Args>::operator+=(std::function<_Returns(functionStack&, _Args...)> func)
	{
		return pushFunction(func);
	}
	template<typename _Returns, typename ..._Args>
	inline unsigned int functionStack<_Returns, ..._Args>::operator--()
	{
		return popFunction();
	}
}