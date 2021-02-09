// =====================================================================================================================
// DGEventNotifier.hpp
// =====================================================================================================================

#ifndef DGEVENTNOTIFIER_HPP
#define DGEVENTNOTIFIER_HPP

#include <functional>
#include "DGUtility.hpp"


template<typename S, typename E, typename T = void, bool B = true>
class DGEventNotifier
{
public:
	typedef S Source;
	typedef E EventArg;

	typedef std::function<T (const Source&, const EventArg&)> Runnable;

private:
	GS::Array<Runnable> runnables;
	std::function<void (bool)> notifierStateHandler;

public:
	DGEventNotifier ();

	DGEventNotifier&	operator+= (const Runnable& runnable);

	template <typename Function, typename Target>
	DGEventNotifier&	Add (Function function, Target target);

	void SetNotifierStateHandler (std::function <void (bool)> stateHandler);

	T	NotifyAll (const Source&, const EventArg&);
};


// DGEventNotifier partial template specialization: NotifyAll: void ----------------------------------------------------

template<typename S, typename E>
class DGEventNotifier<S, E, void, true>
{
public:
	typedef S Source;
	typedef E EventArg;

	typedef std::function<void (const Source&, const EventArg&)> Runnable;

private:
	GS::Array<Runnable> runnables;
	std::function<void (bool)> notifierStateHandler;

public:
	DGEventNotifier ();

	DGEventNotifier&	operator+= (const Runnable& runnable);

	template<typename Function, typename Target>
	DGEventNotifier&	Add (Function function, Target target);

	void	SetNotifierStateHandler (std::function <void (bool)> stateHandler);

	void	NotifyAll (const Source&, const EventArg&);
};


template<typename S, typename E>
DGEventNotifier<S, E, void, true>::DGEventNotifier ()
{
}


template<typename S, typename E>
void	DGEventNotifier<S, E, void, true>::NotifyAll (const Source& s, const EventArg& e)
{
	for (auto& r : runnables) {
		r (s, e);
	}
}

template<typename S, typename E>
void	DGEventNotifier<S, E, void, true>::SetNotifierStateHandler (std::function <void (bool)> stateHandler)
{
	notifierStateHandler = stateHandler;
}

template<typename S, typename E>
DGEventNotifier<S, E, void, true>&	DGEventNotifier<S, E, void, true>::operator+= (const Runnable& runnable)
{
	if (notifierStateHandler != nullptr) {
		notifierStateHandler (true);
	}
	runnables.Push (runnable);
	return *this;
}


template<typename S, typename E>
template<typename Function, typename Target>
DGEventNotifier<S, E, void, true>&	DGEventNotifier<S, E, void, true>::Add (Function function, Target target)
{
	if (notifierStateHandler != nullptr) {
		notifierStateHandler (true);
	}
	runnables.Push (std::bind (function, target, std::placeholders::_1, std::placeholders::_2));
	return *this;
}


// DGEventNotifier partial template specialization: NotifyAll: true ----------------------------------------------------

template<typename S, typename E>
class DGEventNotifier<S, E, bool, true>
{
public:
	typedef S Source;
	typedef E EventArg;

	typedef std::function<bool (const Source&, const EventArg&)> Runnable;

private:
	GS::Array<Runnable> runnables;
	std::function<void (bool)> notifierStateHandler;

public:
	DGEventNotifier ();

	DGEventNotifier&	operator+= (const Runnable& runnable);

	template<typename Function, typename Target>
	DGEventNotifier&	Add (Function function, Target target);

	void	SetNotifierStateHandler (std::function <void (bool)> stateHandler);

	bool	NotifyAll (const Source&, const EventArg&);
};


template<typename S, typename E>
DGEventNotifier<S, E, bool, true>::DGEventNotifier ()
{
}


template<typename S, typename E>
bool DGEventNotifier<S, E, bool, true>::NotifyAll (const Source& s, const EventArg& e)
{
	bool ret = true;
	for (auto& r : runnables) {
		ret &= r (s, e);
	}
	return ret;
}

template<typename S, typename E>
void	DGEventNotifier<S, E, bool, true>::SetNotifierStateHandler (std::function <void (bool)> stateHandler)
{
	notifierStateHandler = stateHandler;
}


template<typename S, typename E>
DGEventNotifier<S, E, bool, true>&	DGEventNotifier<S, E, bool, true>::operator+= (const Runnable& runnable)
{
	if (notifierStateHandler != nullptr) {
		notifierStateHandler (true);
	}
	runnables.Push (runnable);
	return *this;
}


template<typename S, typename E>
template<typename Function, typename Target>
DGEventNotifier<S, E, bool, true>&	DGEventNotifier<S, E, bool, true>::Add (Function function, Target target)
{
	if (notifierStateHandler != nullptr) {
		notifierStateHandler (true);
	}
	runnables.Push (std::bind (function, target, std::placeholders::_1, std::placeholders::_2));
	return *this;
}


// DGEventNotifier partial template specialization: NotifyAll: false ---------------------------------------------------

template<typename S, typename E>
class DGEventNotifier<S, E, bool, false>
{
public:
	typedef S Source;
	typedef E EventArg;

	typedef std::function<bool (const Source&, const EventArg&)> Runnable;

private:
	GS::Array<Runnable> runnables;
	std::function<void (bool)> notifierStateHandler;

public:
	DGEventNotifier ();

	DGEventNotifier&	operator+= (const Runnable& runnable);

	template<typename Function, typename Target>
	DGEventNotifier&	Add (Function function, Target target);

	void	SetNotifierStateHandler (std::function <void (bool)> stateHandler);

	bool	NotifyAll (const Source&, const EventArg&);
};


template<typename S, typename E>
DGEventNotifier<S, E, bool, false>::DGEventNotifier ()
{
}


template<typename S, typename E>
bool	DGEventNotifier<S, E, bool, false>::NotifyAll (const Source& s, const EventArg& e)
{
	bool ret = false;
	for (auto& r : runnables) {
		ret |= r (s, e);
	}
	return ret;
}


template<typename S, typename E>
void	DGEventNotifier<S, E, bool, false>::SetNotifierStateHandler (std::function <void (bool)> stateHandler)
{
	notifierStateHandler = stateHandler;
}


template<typename S, typename E>
DGEventNotifier<S, E, bool, false>&		DGEventNotifier<S, E, bool, false>::operator+= (const Runnable& runnable)
{
	if (notifierStateHandler != nullptr) {
		notifierStateHandler (true);
	}
	runnables.Push (runnable);
	return *this;
}


template<typename S, typename E>
template<typename Function, typename Target>
DGEventNotifier<S, E, bool, false>& DGEventNotifier<S, E, bool, false>::Add (Function function, Target target)
{
	if (notifierStateHandler != nullptr) {
		notifierStateHandler (true);
	}
	runnables.Push (std::bind (function, target, std::placeholders::_1, std::placeholders::_2));
	return *this;
}

#endif // DGEVENTNOTIFIER_HPP
