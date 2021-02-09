
// *****************************************************************************
//
//                              Class EventSource
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	MB, AZS
//
// SG compatible
//
// *****************************************************************************


#if !defined EVENTSOURCE_HPP
#define EVENTSOURCE_HPP

#pragma once


// --------------------------------- Includes ----------------------------------

#include "GSRootExport.hpp"
#include "Definitions.hpp"

#include "Array.hpp"

// ------------------------------ Predeclarations ------------------------------

namespace GS { class EventObserver; }



// ============================= Class EventSource =============================

namespace GS {

class GSROOT_DLL_EXPORT EventSource {
private:
	friend class EventObserver;

	struct ObserverEntry {
		EventObserver*	observer;
		UInt64			stamp;

		ObserverEntry (EventObserver* observer, UInt64 stamp) : observer (observer), stamp (stamp) {}
	};

	GS::Array<ObserverEntry>	observers;
	UInt64						nextStamp;

protected:
	class ObserverEnumerator {
	private:
		friend class EventSource;

		const EventSource*	eventSource;
		UInt64				actStamp;
		UInt32				act;
		
		inline ObserverEnumerator (const EventSource* eventSource);
	public:
		inline EventObserver&			operator* () const;

		inline ObserverEnumerator&		operator++ ();

		inline bool		operator== (std::nullptr_t) const;
		inline bool		operator!= (std::nullptr_t) const;
	};


	EventSource ();
	EventSource (const EventSource& source);
	virtual	~EventSource ();
	EventSource&	operator= (const EventSource& source);

	void	Attach (EventObserver& observer);
	void	Detach (EventObserver& observer);

	bool	HasObserver (void) const;

	ObserverEnumerator	EnumerateObservers (void) const;
};

}	// namespace GS


GS::EventSource::ObserverEnumerator::ObserverEnumerator (const EventSource* eventSource) : 
	eventSource (eventSource), 
	act (eventSource->observers.GetSize ()), 
	actStamp (eventSource->observers.IsEmpty () ? 0 : eventSource->observers.GetLast ().stamp)
{
}


GS::EventObserver&	GS::EventSource::ObserverEnumerator::operator* () const
{
	DBASSERT (act != 0 && act <= eventSource->observers.GetSize () && eventSource->observers [act - 1].stamp == actStamp);
	return *eventSource->observers [act - 1].observer;
}


GS::EventSource::ObserverEnumerator&		GS::EventSource::ObserverEnumerator::operator++ ()
{
	DBASSERT (act != 0);

	act = GS::Min (act, eventSource->observers.GetSize ());
	while (act != 0) {
		if (eventSource->observers [act - 1].stamp >= actStamp) {
			--act;
		} else {
			actStamp = eventSource->observers [act - 1].stamp;
			break;
		}
	}
	return *this;
}


bool		GS::EventSource::ObserverEnumerator::operator== (std::nullptr_t) const
{
	return act == 0;
}


bool		GS::EventSource::ObserverEnumerator::operator!= (std::nullptr_t) const
{
	return act != 0;
}


// _____________________________ Class EventSource _____________________________


#endif
