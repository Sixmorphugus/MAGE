#pragma once
#include "StdAfx.h"
#include "platform.h"

#include <chaiscript/chaiscript.hpp>

namespace mage {
// very simple implementation of the observer design pattern.
class basic;

class MAGEDLL hookBase {
public:
	virtual void forgetObserver(unsigned int ob) = 0;
	virtual void clearObservers() = 0;
	virtual unsigned int numObservers() const = 0;

protected:
	void hEE(const chaiscript::exception::eval_error &e) const;

public:
	bool enabled;
};

template<typename ... Args>
class hook : public hookBase
{
private:
	struct Observer {
		Observer(std::function<void(Args...)> of)
		{
			observerFunctor = of;
			workingDirectory = p::getWorkDir();
		}

		void notify(Args ... a) const {
			p::setWorkDir(workingDirectory);
			observerFunctor(a...);
			p::resetWorkDir();
		}

		std::function<void(Args...)> observerFunctor;
		std::string workingDirectory;
	};

public:
	hook() { enabled = true; }
	hook(const hook& h) { observers = h.observers; enabled = true; }
	hook& operator=(const hook& a) { observers = a.observers; enabled = a.enabled; return *this; }

	unsigned int registerObserver(std::function<void(Args...)> callback)
	{
		observers.push_back(callback);
		return observers.size() - 1;
	}

	void forgetObserver(unsigned int ob) {
		observers.erase(observers.begin() + ob);
	}

	void clearObservers()
	{
		observers.clear();
	}

	unsigned int numObservers() const {
		return observers.size();
	}

	void notify(Args ... a) const {
		if (!enabled)
			return;

		for (unsigned int i = 0; i < observers.size(); i++) {
			try {
				observers[i].notify(a...);
			}
			catch (chaiscript::exception::eval_error &e) {
				hEE(e); // idk if this WOULD cause confusion but bsts
			}
			catch (std::bad_function_call &e) {
				p::warn("bad_function_call: " + std::string(e.what()));
			}
			catch (std::runtime_error &e) {
				p::warn(std::string(e.what()));
			}
		}
	}

private:
	std::vector<Observer> observers;
};

} // namespace mage

#define DeclareScriptingHook(name, ...) \
DeclareScriptingCustom(user_type< mage::hook<__VA_ARGS__> >(), name); \
DeclareScriptingCustom(constructor<mage::hook<__VA_ARGS__>()>(), name); \
DeclareScriptingBaseClass(hookBase, mage::hook<__VA_ARGS__>); \
DeclareScriptingCustom(fun(&mage::hook<__VA_ARGS__>::registerObserver), "registerObserver"); \
DeclareScriptingCustom(fun(&mage::hook<__VA_ARGS__>::notify), "notify");
