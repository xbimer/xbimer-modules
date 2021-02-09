
// *****************************************************************************
//
//                                   Class Id
//
// Module:		   ObjectDatabase
// Namespace:	   ODB
// Contact person: MB
//
// SG compatible
//
// *****************************************************************************


#if !defined ODBID_HPP
#define ODBID_HPP

#pragma once


// --------------------------------- Includes ----------------------------------

#include "ODBBase.hpp"

#include "GSGuid.hpp"
#include "UniString.hpp"
#include "FastLogger.hpp"
#include "XMLChannel.hpp"

namespace GS { template <class Type> struct ObjectStateTypeConversion; }
namespace GS { class ObjectState; }
namespace MD5 { class FingerPrint; }

#define XMLTag_Id		"guid"


// ================================= Class Id ==================================

namespace ODB {

class ODB_DLL_EXPORT Id {
private:
	GS::Guid	guid;	// global unique identifier

	void	GenerationFailed ();

public:
	enum GenerateIdSelector { GenerateId };

	inline Id ();
	inline Id (const GS::Guid& guid);
		   Id (const MD5::FingerPrint& fingerPrint);
	inline Id (GenerateIdSelector);
		   Id (const char* value);

	inline void		Generate (void);
	inline void		Clear    (void);

	inline bool		operator== (const Id& rightOp) const;
	inline bool		operator!= (const Id& rightOp) const;
	inline bool		operator<  (const Id& rightOp) const;
	inline bool		operator>  (const Id& rightOp) const;
	inline bool		operator<= (const Id& rightOp) const;
	inline bool		operator>= (const Id& rightOp) const;

	inline bool		IsNull (void) const;

	inline const GS::Guid&	ToGuid   (void) const;
	GS::UniString			ToString (void) const;
	void					ToCString (char (&result)[64]) const;

	inline GSErrCode	Read  (GS::IChannel& ic);
	inline GSErrCode	Write (GS::OChannel& oc) const;

	inline GSErrCode	ReadXML  (GS::XMLIChannel& ic);
	inline GSErrCode	WriteXML (GS::XMLOChannel& oc) const;

	ODB_FORCE_INLINE ULong	GenerateHashValue (void) const;
};


ODB_DLL_EXPORT extern const Id	NullId;


inline GSErrCode	Read  (GS::IChannel& ic, Id& id);
inline GSErrCode	Write (GS::OChannel& oc, const Id& id);

inline ULong	GenerateHashValue (const ODB::Id& target);

inline Id	GenerateId (void);

ODB_DLL_EXPORT Id	GenerateId (const Id& id1, const Id& id2);

GS_FORCE_INLINE void	Append (GS::FastLogger::LogComposer& logComposer, const Id& id);

}	// namespace ODB


namespace GS {

template <>
struct ODB_DLL_EXPORT ObjectStateTypeConversion<ODB::Id> {
	static constexpr bool isConvertible = true;

	typedef UniString ConvertedType;

	static GSErrCode	 Convert (const ODB::Id& id, UniString& idString);
	static GSErrCode	 Convert (const UniString& idString, ODB::Id& id);
};

}


inline ODB::Id::Id ()
{
}


inline ODB::Id::Id (const GS::Guid& guid):
	guid (guid)
{
}


inline ODB::Id::Id (GenerateIdSelector)
{
	Generate ();
}


inline void		ODB::Id::Generate (void)
{
	if (guid.Generate () != NoError)
		GenerationFailed ();
}


inline void		ODB::Id::Clear (void)
{
	guid.Clear ();
}


inline bool		ODB::Id::operator== (const Id& rightOp) const
{
	return (guid == rightOp.guid);
}


inline bool		ODB::Id::operator!= (const Id& rightOp) const
{
	return (guid != rightOp.guid);
}


inline bool		ODB::Id::operator< (const Id& rightOp) const
{
	return (guid < rightOp.guid);
}


inline bool		ODB::Id::operator> (const Id& rightOp) const
{
	return (guid > rightOp.guid);
}


inline bool		ODB::Id::operator<= (const Id& rightOp) const
{
	return (guid <= rightOp.guid);
}


inline bool		ODB::Id::operator>= (const Id& rightOp) const
{
	return (guid >= rightOp.guid);
}


inline bool		ODB::Id::IsNull (void) const
{
	return guid.IsNull ();
}


inline const GS::Guid&	ODB::Id::ToGuid (void) const
{
	return guid;
}


inline GSErrCode	ODB::Id::Read (GS::IChannel& ic)
{
	return guid.Read (ic);
}


inline GSErrCode	ODB::Id::ReadXML (GS::XMLIChannel& ic)
{
	return ic.ReadXML (XMLTag_Id, guid);
}


inline GSErrCode	ODB::Id::Write (GS::OChannel& oc) const
{
	return guid.Write (oc);
}


inline GSErrCode	ODB::Id::WriteXML (GS::XMLOChannel& oc) const
{
	return oc.WriteXML (XMLTag_Id, guid);
}


ODB_FORCE_INLINE ULong	ODB::Id::GenerateHashValue (void) const
{
	static_assert (sizeof (GS::Guid) == 4 * sizeof (ULong), "sizeof (GS::Guid) != 4 * sizeof (ULong)");

	const ULong* g = reinterpret_cast<const ULong*> (&guid);

	return (g[0] + g[1] + g[2] + g[3]);		// resulting deviation is not the best, but is fast and can be inlined
}


inline ODB::Id	ODB::GenerateId (void)
{
	return Id (Id::GenerateId);
}


inline GSErrCode	ODB::Read (GS::IChannel& ic, Id& id)
{
	return id.Read (ic);
}


inline GSErrCode	ODB::Write (GS::OChannel& oc, const Id& id)
{
	return id.Write (oc);
}


inline ULong	ODB::GenerateHashValue (const ODB::Id& target)
{
	return target.GenerateHashValue ();
}


GS_FORCE_INLINE void	ODB::Append (GS::FastLogger::LogComposer& logComposer, const Id& id)
{
	logComposer.Append (id.ToGuid ());
}
// _________________________________ Class Id __________________________________


#endif
