
// *****************************************************************************
//
// Declaration of HeaderField class
//
// Module:			HTTP
// Namespace:		HTTP::MessageHeader
// Contact person:	KOVA
//
// *****************************************************************************

#ifndef HTTP_MESSAGEHEADER_HEADERFIELD_HPP
#define HTTP_MESSAGEHEADER_HEADERFIELD_HPP

#pragma once

// --------------------------------- Includes ----------------------------------

#include "HTTP/HTTPExport.hpp"
#include "HTTP/HTTPException.hpp"

#include "UniString.hpp"


namespace HTTP {
namespace MessageHeader {

class HTTP_API HeaderField {

private:
	GS::UniString m_name;
	GS::UniString m_values;

public:
	HeaderField ();
	HeaderField (const GS::UniString& key, const GS::UniString& values);
	virtual ~HeaderField ();

	inline const GS::UniString& GetName () const;
	inline void					SetName (const GS::UniString& name);
	inline const GS::UniString& GetValues () const;
	inline void					SetValues (const GS::UniString& values);
	inline GS::UniString		ToString () const;
	inline void					Clear ();
	inline bool					NameIsEqual (const HeaderField& headerField) const;
	inline bool					NameIsEqual (const GS::UniString& name) const;
	inline static bool			NameIsEqual (const GS::UniString& name1, const GS::UniString& name2);

	inline bool	operator == (const HeaderField& rhs) const;
	inline bool	operator != (const HeaderField& rhs) const;

};


// -----------------------------------------------------------------------------
// GetName
// -----------------------------------------------------------------------------

inline const GS::UniString& HeaderField::GetName () const
{
	return m_name;
}

// -----------------------------------------------------------------------------
// SetName
// -----------------------------------------------------------------------------

inline void HeaderField::SetName (const GS::UniString& name)
{
	m_name = name;
}

// -----------------------------------------------------------------------------
// GetValues
// -----------------------------------------------------------------------------

inline const GS::UniString& HeaderField::GetValues () const
{
	return m_values;
}

// -----------------------------------------------------------------------------
// SetValues
// -----------------------------------------------------------------------------

inline void HeaderField::SetValues (const GS::UniString& value)
{
	m_values = value;
}

// -----------------------------------------------------------------------------
// ToString
// -----------------------------------------------------------------------------

inline GS::UniString HeaderField::ToString () const
{
	return GetName () + ": " + GetValues ();
}

// -----------------------------------------------------------------------------
// Clear
// -----------------------------------------------------------------------------

inline void HeaderField::Clear ()
{
	SetName ("");
	SetValues ("");
}

// -----------------------------------------------------------------------------
// operator ==
// -----------------------------------------------------------------------------

inline bool	HeaderField::operator == (const HeaderField& rhs) const
{
	return (NameIsEqual(rhs) && (rhs.GetValues () == this->GetValues ()));
}

// -----------------------------------------------------------------------------
// operator !=
// -----------------------------------------------------------------------------

inline bool	HeaderField::operator != (const HeaderField& rhs) const
{
	return !(*this == rhs);
}

// -----------------------------------------------------------------------------
// NameIsEqual
// -----------------------------------------------------------------------------

inline bool HeaderField::NameIsEqual (const HeaderField& headerField) const
{
	return NameIsEqual (headerField.GetName ());
}

// -----------------------------------------------------------------------------
// NameIsEqual
// -----------------------------------------------------------------------------

inline bool HeaderField::NameIsEqual (const GS::UniString& name) const
{
	return NameIsEqual (name, this->GetName ());
}

// -----------------------------------------------------------------------------
// NameIsEqual
// -----------------------------------------------------------------------------

inline bool HeaderField::NameIsEqual (const GS::UniString& name1, const GS::UniString& name2)
{
	return name1.IsEqual (name2, GS::UniString::CaseInsensitive);
}

}
}

#endif
