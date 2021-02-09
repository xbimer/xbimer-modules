
// *****************************************************************************
//
//                                Class UniString
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	MB
//
// SG compatible
//
// *****************************************************************************


#if !defined UNISTRING_HPP
#define UNISTRING_HPP

#pragma once


// --------------------------------- Includes ----------------------------------

// from Standard C++ library
#if defined (_MSC_VER)
#include <stddef.h>
#include <stdarg.h>
namespace std { using ::va_list; }
#else
#include <cstdarg>
#include <cstddef>
#endif

#include "Object.hpp"
#include "UniCHar.hpp"
#include "uchar_t.hpp"
#include "TSRefCounter.hpp"
#include "GSException.hpp"
#include "FastLogger.hpp"

// ============================== Class UniString ==============================

// UniString is reference counted. This raises some problems in multi-threaded environment.
// To avoid them the following rule is obeyed throughout the implementation:
//     No string will operate on a buffer that is shared (it should be detached first).
// Moreover as UniString's external interface is not thread-safe, user should serialize
// concurrent accesses to the same string objects.
// Consequently:
//     - A shared buffer can be read (const operations)
//     - A shared buffer can not be deleted
//     - In the copy constructor the source buffer will not be deleted because at least the
//       source string will hold it.
//     - Inside any operation if the buffer's reference counter is 1 then the buffer will
//       remain unshared until the operation itself modifies it (because there are no more
//       references to the buffer and the same string can not participate in another operation)
// In the implementation:
//     - It is sufficient to serialize accesses to the reference counter
//     - After the AboutToModify method is called the buffer is surely unshared

namespace GS {

class String;

inline const uchar_t*	GetScanfFormatUStrOf (const void* par)		{ static uchar_t formats [] = {'%','T', 0 , 0 }; return par == nullptr ? L ("") : formats; }

class GSROOT_DLL_EXPORT UniString: public Object {
	DECLARE_CLASS_INFO

public:
	class Substring;
	class ConstSubstring;
	class Concatenation;

private:
	struct SharedBuffer;

	SharedBuffer*	content;	// shared buffer storing content of the string

		// Implementation classes and structures

	struct SharedBuffer {
		USize			length;			// length of the string in UniChar::Layout units
		USize			capacity;		// capacity of the allocated buffer in UniChar::Layout units
		TSRefCounter	refCounter;		// stores number of references to this shared buffer
		char			reserved[2];	// padding reserved for the future use
		UniChar::Layout	string[1];		// buffer storing content of the string (extends beyond of the SharedBuffer)

		inline SharedBuffer ();
		inline SharedBuffer (USize initialLength, USize initialCapacity, Int32 initialRefCounter);
	};

	static SharedBuffer&	EmptyBuffer ();	// represents the empty string

	class GSROOT_DLL_EXPORT UniCharRef {
	private:
		UniString*	target;	// target of the reference
		UIndex		idx;	// index of the referred character

	public:
		UniCharRef (UniString* targetString, UIndex targetCharIdx): target (targetString), idx (targetCharIdx) {}

			// UniChar interface delegation

		UniCharRef&		operator= (const UniCharRef& ucr) { target->SetChar (idx, ucr); return *this; }
		UniCharRef&		operator= (const UniChar& uc)	  { target->SetChar (idx, uc); return *this;  }
		UniCharRef&		operator= (UniChar::Layout uc)	  { target->SetChar (idx, uc); return *this;  }
		UniCharRef&		operator= (char c)				  { target->SetChar (idx, c); return *this;   }
		UniCharRef&		operator= (unsigned char c)		  { target->SetChar (idx, c); return *this;   }

		operator UniChar () const		 { return target->GetChar (idx); }
		operator UniChar::Layout () const { return target->GetChar (idx); }
		operator char () const			 { return target->GetChar (idx); }
		operator unsigned char () const	 { return target->GetChar (idx); }

		friend bool	operator== (const UniCharRef& leftOp, const UniCharRef& rightOp) { return (leftOp.target->GetChar (leftOp.idx) == rightOp.target->GetChar (rightOp.idx)); }
		friend bool	operator== (const UniCharRef& leftOp, const UniChar& rightOp)    { return (leftOp.target->GetChar (leftOp.idx) == rightOp);  }
		friend bool	operator== (const UniChar& leftOp, const UniCharRef& rightOp)    { return (leftOp == rightOp.target->GetChar (rightOp.idx)); }
		friend bool	operator== (const UniCharRef& leftOp, UniChar::Layout rightOp)   { return (leftOp.target->GetChar (leftOp.idx) == rightOp);  }
		friend bool	operator== (UniChar::Layout leftOp, const UniCharRef& rightOp)   { return (leftOp == rightOp.target->GetChar (rightOp.idx)); }
		friend bool	operator== (const UniCharRef& leftOp, char rightOp) 			 { return (leftOp.target->GetChar (leftOp.idx) == rightOp);  }
		friend bool	operator== (char leftOp, const UniCharRef& rightOp) 			 { return (leftOp == rightOp.target->GetChar (rightOp.idx)); }
		friend bool	operator== (const UniCharRef& leftOp, unsigned char rightOp) 	 { return (leftOp.target->GetChar (leftOp.idx) == rightOp);  }
		friend bool	operator== (unsigned char leftOp, const UniCharRef& rightOp) 	 { return (leftOp == rightOp.target->GetChar (rightOp.idx)); }

		friend bool	operator!= (const UniCharRef& leftOp, const UniCharRef& rightOp) { return (leftOp.target->GetChar (leftOp.idx) != rightOp.target->GetChar (rightOp.idx)); }
		friend bool	operator!= (const UniCharRef& leftOp, const UniChar& rightOp)    { return (leftOp.target->GetChar (leftOp.idx) != rightOp);  }
		friend bool	operator!= (const UniChar& leftOp, const UniCharRef& rightOp)    { return (leftOp != rightOp.target->GetChar (rightOp.idx)); }
		friend bool	operator!= (const UniCharRef& leftOp, UniChar::Layout rightOp)   { return (leftOp.target->GetChar (leftOp.idx) != rightOp);  }
		friend bool	operator!= (UniChar::Layout leftOp, const UniCharRef& rightOp)   { return (leftOp != rightOp.target->GetChar (rightOp.idx)); }
		friend bool	operator!= (const UniCharRef& leftOp, char rightOp) 			 { return (leftOp.target->GetChar (leftOp.idx) != rightOp);  }
		friend bool	operator!= (char leftOp, const UniCharRef& rightOp) 			 { return (leftOp != rightOp.target->GetChar (rightOp.idx)); }
		friend bool	operator!= (const UniCharRef& leftOp, unsigned char rightOp) 	 { return (leftOp.target->GetChar (leftOp.idx) != rightOp);  }
		friend bool	operator!= (unsigned char leftOp, const UniCharRef& rightOp) 	 { return (leftOp != rightOp.target->GetChar (rightOp.idx)); }

		bool		IsDigit				(void) const { return target->GetChar (idx).IsDigit ();				}
		bool		IsASCIIHexaDigit	(void) const { return target->GetChar (idx).IsASCIIHexaDigit ();	}
		bool		IsAlpha				(void) const { return target->GetChar (idx).IsAlpha ();				}
		bool		IsAlphaNumeric		(void) const { return target->GetChar (idx).IsAlphaNumeric ();		}
		bool		IsWhiteSpace		(void) const { return target->GetChar (idx).IsWhiteSpace ();		}
		bool		IsPunctuation		(void) const { return target->GetChar (idx).IsPunctuation ();		}
		bool		IsControl			(void) const { return target->GetChar (idx).IsControl ();			}
		bool		IsLower				(void) const { return target->GetChar (idx).IsLower ();				}
		bool		IsASCIILower		(void) const { return target->GetChar (idx).IsASCIILower ();		}
		bool		IsUpper				(void) const { return target->GetChar (idx).IsUpper ();				}
		bool		IsASCIIUpper		(void) const { return target->GetChar (idx).IsASCIIUpper ();		}
	};

	class GSROOT_DLL_EXPORT CStr {
	private:
		enum { BufferSize = 256 };

		char*	content;			// stores content of the C string
		USize	length;				// length of the C string
		char	buffer[BufferSize];	// buffer for smaller C strings

		CStr&	operator= (const CStr& source);	// disabled

		friend class UniString;

	public:
		explicit CStr (USize strLength);
		// destructive copy constructor (has non-const source)
		CStr (CStr& source);	//lint !e1724: Parameter to copy constructor for class 'GS::UniString::CStr' should be a const reference
	   ~CStr ();

		inline operator const char* () const { return content; }
		inline const char*	Get (void) const { return content; }
	};

	class GSROOT_DLL_EXPORT PStr {
	private:
		enum { BufferSize = 256 };

		unsigned char	content[BufferSize];	// stores content of the Pascal string

		PStr&	operator= (const PStr& source);	// disabled

		friend class UniString;

	public:
		PStr () {content[0] = 0; }
		PStr (const PStr& source);

		inline operator const unsigned char* () const { return content; }
		inline const unsigned char*	 Get (void) const { return content; }
	};

	class GSROOT_DLL_EXPORT UStr {
	private:
		UniChar::Layout*	content;		// stores content of the plain Unicode string
		bool				ownsContent;	// indicates whether the UStr owns its content

		UStr&	operator= (const UStr& source);	// disabled

	public:
		explicit UStr (UniChar::Layout* content, bool ownsContent);
		// destructive copy constructor (has non-const source)
		UStr (UStr& source);	//lint !e1724: Parameter to copy constructor for class 'GS::UniString::UStr' should be a const reference
	   ~UStr ();

		inline operator const UniChar::Layout* () const { return content; }
		inline const UniChar::Layout*  Get (void) const { return content; }
	};

	static bool GSCALL UniStringHandleTextForPrintf (void* pointer, GS::uchar_t *&s, int& s_len);
	static bool GSCALL UniStringHandleTextForScanf  (void* pointer, int status, const GS::uchar_t *s, int s_len);

	static GS::UniString	PrintfImpl (const uchar_t* format, std::va_list arg);
	
	static GS::UniString	PrintfFwd (const char* format, ...);
	static GS::UniString	PrintfFwd (const uchar_t* format, ...);

	int		CheckedScanf (const UniChar::Layout* format, const UniChar::Layout** formatStrs, ...) const;
	int		CheckedScanf (const GS::UniString& format, const UniChar::Layout** formatStrs, ...) const;
	int		CheckedScanf (const char* format, const UniChar::Layout** formatStrs, ...) const;
	static UniString	CheckedPrintf (const UniChar::Layout* format, const UniChar::Layout** formatStrs, ...);
	static UniString	CheckedPrintf (const GS::UniString& format, const UniChar::Layout** formatStrs, ...);
	static UniString	CheckedPrintf (const char* format, const UniChar::Layout** formatStrs, ...);

	void	Init (const char* chars, USize logicalLength, USize physicalLength, GSCharCode charCode);

	void	Set (const char* chars, USize logicalLength, USize physicalLength, GSCharCode charCode);
	void	Set (const UniString& source);

	static UIndex	FindFirstIn	   (const UniChar::Layout* source, USize sourceLength, const UniChar::Layout* charSet, USize charCount);
	static UIndex	FindLastIn	   (const UniChar::Layout* source, USize sourceLength, const UniChar::Layout* charSet, USize charCount);
	static UIndex	FindFirstNotIn (const UniChar::Layout* source, USize sourceLength, const UniChar::Layout* charSet, USize charCount);
	static UIndex	FindLastNotIn  (const UniChar::Layout* source, USize sourceLength, const UniChar::Layout* charSet, USize charCount);

	static USize		CountLogicalLength  (const char* cStr, GSCharCode charCode);
	static USize		CountLogicalLength  (const char* chars, USize charCount, GSCharCode charCode);
	inline static USize	CountPhysicalLength (const char* cStr);
	static void			CountLengths        (const char* cStr, USize* logicalLength, USize* physicalLength, GSCharCode charCode);

	inline static USize	CountLength (const UniChar::Layout* uStr);

	inline static Concatenation		Concatenate (void) { return Concatenation (); }

	inline static bool	IsEqual (const UniString& leftOp,      const UniString&       rightOp);
	inline static bool	IsEqual (const UniString& leftOp,      const char*            rightOp);
	inline static bool	IsEqual (const UniString& leftOp,      const UniChar::Layout* rightOp);
	inline static bool	IsEqual (const UniString& leftOp,      const Substring&       rightOp);
	inline static bool	IsEqual (const UniString& leftOp,      const ConstSubstring&  rightOp);
	inline static bool	IsEqual (const Substring& leftOp,      const char*            rightOp);
	inline static bool	IsEqual (const Substring& leftOp,      const UniChar::Layout* rightOp);
	inline static bool	IsEqual (const Substring& leftOp,      const Substring&       rightOp);
	inline static bool	IsEqual (const Substring& leftOp,      const ConstSubstring&  rightOp);
	inline static bool	IsEqual (const ConstSubstring& leftOp, const char*            rightOp);
	inline static bool	IsEqual (const ConstSubstring& leftOp, const UniChar::Layout* rightOp);
	inline static bool	IsEqual (const ConstSubstring& leftOp, const ConstSubstring&  rightOp);

		// Modification signaling

	inline void		AboutToModify (void);
	inline void		AboutToModify (USize minCapacity);
	inline void		AboutToInsert (USize charCount);
	inline void		AboutToDelete (void);

		// Capacity management (can be called only when the buffer is not shared)

	void            IncreaseCapacity (USize minCapacity);
	void            DecreaseCapacity (USize minCapacity);
	void            AdjustCapacity   (USize minCapacity);
	void			Reallocate	     (USize newCapacity);

		// SharedBuffer management

	inline void		SetBuffer     (SharedBuffer* newContent);
	inline void		DetachBuffer  (void);
	inline void		DetachBuffer  (USize newCapacity);
	inline void		ReleaseBuffer (void);

	static SharedBuffer*	GSCALL AllocateBuffer	(USize capacity);
	static SharedBuffer*	GSCALL ShareEmptyBuffer	(void);
	static SharedBuffer*	GSCALL CopyBuffer		(const SharedBuffer* source);
	static SharedBuffer*	GSCALL CopyBuffer		(const SharedBuffer* source, USize newCapacity);
	static void				GSCALL DeleteBuffer		(SharedBuffer* buffer);

	inline static USize		CapacityToBufferSize (USize capacity);
	inline static USize		BufferSizeToCapacity (USize bufferSize);

		// Utility

	inline static void	CopyUnicode    (UniChar::Layout* target, const UniChar::Layout* source, USize length);
	inline static void	MoveUnicode    (UniChar::Layout* target, const UniChar::Layout* source, USize length);
	inline static bool	CompareUnicode (const UniChar::Layout* source1, const UniChar::Layout* source2, USize length);

	class Type1 {};		// for the documentation generator
	class Type2 {};		// for the documentation generator

public:
		// Implementation classes and structures

	class GSROOT_DLL_EXPORT Substring {
	private:
		UniString*	base;	// base string of the substring
		UIndex		from;	// offset of the substring in the base string
		USize		range;	// length of the substring

		Substring (UniString& base, UIndex from, USize range);

		friend class UniString;

	public:
		Substring&	operator= (const Substring& source);
		Substring&	operator= (const ConstSubstring& source);
		Substring&	operator= (UniChar uc);
		Substring&	operator= (const UniString& str);
		Substring&	operator= (const UniChar::Layout* uStr);
		Substring&	operator= (const char* cStr);
		Substring&	operator= (const unsigned char* pStr);
	};

	class GSROOT_DLL_EXPORT ConstSubstring {
	private:
		const UniString*	base;	// base string of the constant substring
		UIndex				from;	// offset of the constant substring in the base string
		USize				range;	// length of the constant substring

		ConstSubstring (const UniString& base, UIndex from, USize range);

		friend class UniString;
		friend class Substring;
	};

	class GSROOT_DLL_EXPORT Concatenation {
	private:
		enum OperandType { TUniChar, TUniString, TUStr, TCStr, TSubstring, TConstSubstring };	// possible operand types of the concatenation

		struct Operand {	// stores an operand of the concatenation
			OperandType		type;
			union {
				UniChar::Layout			uc;
				const UniString*		str;
				const UniChar::Layout*	uStr;
				const char*				cStr;
				const Substring*		substr;
				const ConstSubstring*	constSubstr;
			};
		};

		enum { MaxOperands = 10 };

		UniString*	subResult;				// subresult of long (with more than MaxOperands operands) concatenations
		Operand		operands[MaxOperands];	// operands of the concatenation
		USize		operandCount;			// number of operands

		Concatenation ();

		friend class UniString;

	public:
		Concatenation (const Concatenation& source);
		Concatenation&	operator= (const Concatenation& source);
	   ~Concatenation ();

		Concatenation&	operator+ (UniChar                rightOp);
		Concatenation&	operator+ (const UniString&       rightOp);
		Concatenation&	operator+ (const UniChar::Layout* rightOp);
		Concatenation&	operator+ (const char*            rightOp);
		Concatenation&	operator+ (const Substring&       rightOp);
		Concatenation&	operator+ (const ConstSubstring&  rightOp);
	};

		// Constructors

	inline UniString ();
	UniString (const UniChar::Layout* uStr);
	UniString (const UniChar::Layout* uChars, USize charCount);
	UniString (const char* cStr, GSCharCode charCode = CC_Default);
	UniString (const char* chars, USize charCount, GSCharCode charCode = CC_Default);
	UniString (const unsigned char* pStr, GSCharCode charCode = CC_Default);
	UniString (const String& string);
	UniString (const UniString& source, UIndex sourceFrom = 0, USize sourceRange = MaxUSize);
	inline UniString (UniString&& source);
	UniString (const Substring& source);
	UniString (const ConstSubstring& source);
	UniString (const Concatenation& source);

	template<typename... Args>
	static UniString	Printf (const char* formatString, Args... args);
	template<typename... Args>
	static UniString	Printf (const UniChar::Layout* formatString, Args... args);
	template<typename... Args>
	static UniString	Printf (const UniString& formatString, Args... args);

	UniString&	operator= (const UniChar::Layout* uStr);
	UniString&	operator= (const char* cStr);
	UniString&	operator= (const unsigned char* pStr);
	UniString&	operator= (const String& source);
	UniString&	operator= (const UniString& source);
	UniString&	operator= (UniString&& source);
	UniString&	operator= (const Substring& source);
	UniString&	operator= (const ConstSubstring& source);
	UniString&	operator= (const Concatenation& source);
	UniString&	Assign (const UniChar::Layout* uChars, USize charCount);
	UniString&	Assign (const char* cStr, GSCharCode charCode);
	UniString&	Assign (const char* chars, USize charCount, GSCharCode charCode = CC_Default);
	UniString&	Assign (const unsigned char* pStr, GSCharCode charCode);
	UniString&	Assign (const UniString& source, UIndex sourceFrom = 0, USize sourceRange = MaxUSize);

	void		Swap (UniString& source);

	inline ~UniString ();

	virtual	UniString*		Clone (void) const override;

		// Character access

	inline UniCharRef	operator[] (UIndex idx);
	inline UniChar		operator[] (UIndex idx) const;

	inline UniChar	GetChar (UIndex idx) const;
	inline void		SetChar (UIndex idx, UniChar uc);

	inline UniChar	GetLast (void) const;

		// Substring access

	Substring		operator() (UIndex from, USize range);
	ConstSubstring	operator() (UIndex from, USize range) const;
	Substring		operator() (UniChar leftSeparator, UniChar rightSeparator, UIndex from = 0, USize range = MaxUIndex);
	ConstSubstring	operator() (UniChar leftSeparator, UniChar rightSeparator, UIndex from = 0, USize range = MaxUIndex) const;

	Substring		GetSubstring (UIndex from, USize range);
	ConstSubstring	GetSubstring (UIndex from, USize range) const;
	Substring		GetSubstring (UniChar leftSeparator, UniChar rightSeparator, UIndex from = 0, USize range = MaxUIndex);
	ConstSubstring	GetSubstring (UniChar leftSeparator, UniChar rightSeparator, UIndex from = 0, USize range = MaxUIndex) const;

	Substring		GetPrefix (USize range);
	ConstSubstring	GetPrefix (USize range) const;

	Substring		GetSuffix (USize range);
	ConstSubstring	GetSuffix (USize range) const;

	enum EmptyPartProcessing {
		KeepEmptyParts,
		SkipEmptyParts
	};
	UInt32	Split (const GS::UniString& separatorString, GS::Array<GS::UniString>* parts) const;
	UInt32	Split (const GS::UniString& separatorCharSet, EmptyPartProcessing emptyPartProcessing, GS::Array<GS::UniString>* parts) const;

	void	SetSubstring (UIndex from, USize range, UniChar uc);
	void	SetSubstring (UIndex from, USize range, const UniString& str, UIndex strFrom = 0, USize strRange = MaxUSize);
	void	SetSubstring (UIndex from, USize range, const UniChar::Layout* uStr);
	void	SetSubstring (UIndex from, USize range, const UniChar::Layout* uChars, USize charCount);
	void	SetSubstring (UIndex from, USize range, const char* cStr, GSCharCode charCode = CC_Default);
	void	SetSubstring (UIndex from, USize range, const char* chars, USize charCount, GSCharCode charCode = CC_Default);
	void	SetSubstring (UIndex from, USize range, const unsigned char* pStr, GSCharCode charCode = CC_Default);

		// Inserting, appending and filling

	void	Insert (UIndex to, UniChar uc);
	void	Insert (UIndex to, const UniString& str, UIndex strFrom = 0, USize strRange = MaxUSize);

	void	Append (UniChar uc);
	void	Append (const UniString& str, UIndex strFrom = 0, USize strRange = MaxUSize);
	void	Append (const UniChar::Layout* uStr);
	void	Append (const UniChar::Layout* uChars, USize charCount);
	void	Append (const char* cStr, GSCharCode charCode = CC_Default);
	void	Append (const char* chars, USize charCount, GSCharCode charCode = CC_Default);
	void	Append (const unsigned char* pStr, GSCharCode charCode = CC_Default);

	inline UniString&	operator+= (UniChar uc);
	inline UniString&	operator+= (const UniString& str);
	inline UniString&	operator+= (const UniChar::Layout* uStr);
	inline UniString&	operator+= (const char* cStr);
	inline UniString&	operator+= (const unsigned char* pStr);

	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (UniChar leftOp, UniChar                rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (UniChar leftOp, const UniString&       rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (UniChar leftOp, const UniChar::Layout* rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (UniChar leftOp, const char*            rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (UniChar leftOp, const Substring&       rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (UniChar leftOp, const ConstSubstring&  rightOp);

	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const UniString& leftOp, UniChar                rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const UniString& leftOp, const UniString&       rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const UniString& leftOp, const UniChar::Layout* rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const UniString& leftOp, const char*            rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const UniString& leftOp, const Substring&       rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const UniString& leftOp, const ConstSubstring&  rightOp);

	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const UniChar::Layout* leftOp, UniChar               rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const UniChar::Layout* leftOp, const UniString&      rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const UniChar::Layout* leftOp, const Substring&      rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const UniChar::Layout* leftOp, const ConstSubstring& rightOp);

	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const char* leftOp, UniChar               rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const char* leftOp, const UniString&      rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const char* leftOp, const Substring&      rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const char* leftOp, const ConstSubstring& rightOp);

	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const Substring& leftOp, UniChar                rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const Substring& leftOp, const UniString&       rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const Substring& leftOp, const UniChar::Layout* rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const Substring& leftOp, const char*            rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const Substring& leftOp, const Substring&       rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const Substring& leftOp, const ConstSubstring&  rightOp);

	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const ConstSubstring& leftOp, UniChar                rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const ConstSubstring& leftOp, const UniString&       rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const ConstSubstring& leftOp, const UniChar::Layout* rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const ConstSubstring& leftOp, const char*            rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const ConstSubstring& leftOp, const Substring&       rightOp);
	GSROOT_DLL_EXPORT friend Concatenation	GSCALL operator+ (const ConstSubstring& leftOp, const ConstSubstring&  rightOp);

	void	Pad	(USize toLength, UniChar filler);

	void	Fill (UniChar uc, UIndex from = 0, USize range = MaxUSize);

		// Deleting

	void	Delete		(UIndex idx);
	void	Delete		(UIndex from, USize range);
	void	DeleteFirst (void);
	UIndex	DeleteFirst (UniChar uc, UIndex from = 0, USize range = MaxUSize);
	UIndex	DeleteFirst	(const UniString& str, UIndex from = 0, USize range = MaxUSize);
	void	DeleteLast  (void);
	UIndex	DeleteLast	(UniChar uc);
	UIndex	DeleteLast	(const UniString& str);
	void	DeleteAll	(UniChar uc, UIndex from = 0, USize range = MaxUSize);
	void	DeleteAll	(const UniString& str, UIndex from = 0, USize range = MaxUSize);

	void	TrimLeft  (void);
	void	TrimLeft  (UniChar uc);
	void	TrimRight (void);
	void	TrimRight (UniChar uc);
	void	Trim      (void);
	void	Trim      (UniChar uc);

	void	Clear (void);
	void	ClearContent (void);

		// Length and capacity management

	void			SetLength (USize newLength);
	void			SetLength (USize newLength, UniChar filler);
	void			Truncate  (USize newLength);
	inline USize	GetLength (void) const;
	inline bool		IsEmpty	  (void) const;

	void	SetCapacity	(USize newCapacity);
	USize	GetCapacity	(void) const;

		// Comparing

	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const UniString& leftOp, const UniString&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const UniString& leftOp, const char*            rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const UniString& leftOp, const UniChar::Layout* rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const UniString& leftOp, const Substring&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const UniString& leftOp, const ConstSubstring&  rightOp);

	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const char* leftOp, const UniString&      rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const char* leftOp, const Substring&      rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const char* leftOp, const ConstSubstring& rightOp);

	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const UniChar::Layout* leftOp, const UniString&      rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const UniChar::Layout* leftOp, const Substring&      rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const UniChar::Layout* leftOp, const ConstSubstring& rightOp);

	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const Substring& leftOp, const UniString&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const Substring& leftOp, const char*            rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const Substring& leftOp, const UniChar::Layout* rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const Substring& leftOp, const Substring&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const Substring& leftOp, const ConstSubstring&  rightOp);

	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const ConstSubstring& leftOp, const UniString&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const ConstSubstring& leftOp, const char*            rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const ConstSubstring& leftOp, const UniChar::Layout* rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const ConstSubstring& leftOp, const Substring&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator== (const ConstSubstring& leftOp, const ConstSubstring&  rightOp);

	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const UniString& leftOp, const UniString&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const UniString& leftOp, const char*            rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const UniString& leftOp, const UniChar::Layout* rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const UniString& leftOp, const Substring&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const UniString& leftOp, const ConstSubstring&  rightOp);

	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const char* leftOp, const UniString&      rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const char* leftOp, const Substring&      rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const char* leftOp, const ConstSubstring& rightOp);

	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const UniChar::Layout* leftOp, const UniString&      rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const UniChar::Layout* leftOp, const Substring&      rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const UniChar::Layout* leftOp, const ConstSubstring& rightOp);

	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const Substring& leftOp, const UniString&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const Substring& leftOp, const char*            rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const Substring& leftOp, const UniChar::Layout* rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const Substring& leftOp, const Substring&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const Substring& leftOp, const ConstSubstring&  rightOp);

	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const ConstSubstring& leftOp, const UniString&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const ConstSubstring& leftOp, const char*            rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const ConstSubstring& leftOp, const UniChar::Layout* rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const ConstSubstring& leftOp, const Substring&       rightOp);
	GSROOT_DLL_EXPORT friend bool	GSCALL operator!= (const ConstSubstring& leftOp, const ConstSubstring&  rightOp);

	bool	operator<  (const UniString& rightOp) const;
	bool	operator<= (const UniString& rightOp) const;
	bool	operator>  (const UniString& rightOp) const;
	bool	operator>= (const UniString& rightOp) const;

	enum CaseComparison { CaseSensitive, CaseInsensitive };
	bool	IsEqual (const UniString& rightOp, CaseComparison caseComparison = CaseSensitive) const;

	enum CompareResult { Less, Equal, Greater };
	CompareResult	Compare (const UniString& rightOp, CaseComparison caseComparison = CaseSensitive) const;

		// Find and replace

	bool	Contains (UniChar uc, UIndex from = 0, USize range = MaxUSize) const;
	bool	Contains (const UniString& str, UIndex from = 0, USize range = MaxUSize) const;

	UIndex	FindFirst (UniChar uc, UIndex from = 0, USize range = MaxUSize) const;
	UIndex	FindFirst (const UniString& str, UIndex from = 0, USize range = MaxUSize) const;
	UIndex	FindLast  (UniChar uc) const;
	UIndex	FindLast  (const UniString& str) const;
	void	Find	  (UniChar uc, const std::function<void (UIndex index)>& processor) const;
	void	Find	  (const UniString& str, const std::function<void (UIndex index)>& processor) const;

	UIndex	FindFirstIn	(const UniString& str, UIndex from = 0, USize range = MaxUSize) const;
	UIndex	FindFirstIn	(const char* cStr, UIndex from = 0, USize range = MaxUSize, GSCharCode charCode = CC_Default) const;
	UIndex	FindLastIn	(const UniString& str) const;
	UIndex	FindLastIn	(const char* cStr, GSCharCode charCode = CC_Default) const;

	UIndex	FindFirstNotIn (const UniString& str, UIndex from = 0, USize range = MaxUSize) const;
	UIndex	FindFirstNotIn (const char* cStr, UIndex from = 0, USize range = MaxUSize, GSCharCode charCode = CC_Default) const;
	UIndex	FindLastNotIn  (const UniString& str ) const;
	UIndex	FindLastNotIn  (const char* cStr, GSCharCode charCode = CC_Default) const;

	inline bool		BeginsWith (UniChar uc) const;
	bool			BeginsWith (const UniString& str) const;
	inline bool		EndsWith   (UniChar uc) const;
	bool			EndsWith   (const UniString& str) const;

	UIndex	ReplaceFirst (UniChar oldChar, UniChar newChar, UIndex from = 0, USize range = MaxUSize);
	UIndex	ReplaceFirst (const UniString& oldStr, const UniString& newStr, UIndex from = 0, USize range = MaxUSize);
	UIndex	ReplaceLast  (UniChar oldChar, UniChar newChar);
	UIndex	ReplaceLast  (const UniString& oldStr, const UniString& newStr);
	USize	ReplaceAll   (UniChar oldChar, UniChar newChar, UIndex from = 0, USize range = MaxUSize);
	USize	ReplaceAll   (const UniString& oldStr, const UniString& newStr, UIndex from = 0, USize range = MaxUSize);

	USize	Count (UniChar uc, UIndex from = 0, USize range = MaxUSize) const;
	USize	Count (const UniString& str, UIndex from = 0, USize range = MaxUSize) const;

		// Character specific manipulation

	void		SetToUpperCase (UIndex from = 0, USize range = MaxUSize);
	void		SetToLowerCase (UIndex from = 0, USize range = MaxUSize);

	UniString	ToUpperCase (UIndex from = 0, USize range = MaxUSize) const;
	UniString   ToLowerCase (UIndex from = 0, USize range = MaxUSize) const;

	void		SetToHalfWidth (UIndex from = 0, USize range = MaxUSize);
	void		SetToFullWidth (UIndex from = 0, USize range = MaxUSize);

		// Parsing

	bool	IsASCII (void) const;

	template<typename FormatType, class Type1>
	USize	SScanf (FormatType format, Type1* par1) const
	{
		const UniChar::Layout* formatStrs[2] = {GetScanfFormatUStrOf(par1), nullptr};
		return CheckedScanf (format, formatStrs, par1);
	}

	template<typename FormatType, class Type1, class... Types>
	USize	SScanf (FormatType format, Type1* par1, Types*... pars) const
	{
		const UniChar::Layout* formatStrs[] = { GetScanfFormatUStrOf (par1), GetScanfFormatUStrOf (pars)..., nullptr };
		return CheckedScanf (format, formatStrs, par1, pars...);
	}

	template<typename FormatType, class Type1>
	static UniString	SPrintf (FormatType format, Type1 par1)
	{
		const UniChar::Layout* formatStrs[2] = {GetPrintfFormatUStrOf(par1), nullptr};
		return CheckedPrintf (format, formatStrs, par1);
	}

	template<typename FormatType, class Type1, class... Types>
	static UniString	SPrintf (FormatType format, Type1 par1, Types... pars)
	{
		const UniChar::Layout* formatStrs[] = { GetPrintfFormatUStrOf (par1), GetPrintfFormatUStrOf (pars)..., nullptr };
		return CheckedPrintf (format, formatStrs, par1, pars...);
	}

		// Conversion

	enum OnConversionError { ReplaceWithFillers, ThrowException };

	UStr	ToUStr (UIndex from = 0, USize range = MaxUSize) const;
	CStr	ToCStr (UIndex from = 0, USize range = MaxUSize, GSCharCode charCode = CC_Default, OnConversionError onConversionError = ReplaceWithFillers) const;
	PStr	ToPStr (UIndex from = 0, USize range = MaxUSize, GSCharCode charCode = CC_Default, OnConversionError onConversionError = ReplaceWithFillers) const;

	UniString    ToDecomposed (void) const;
    UniString    ToComposed (void) const;
	UniString    ToCompatibilityDecomposed (void) const;
	UniString    ToCompatibilityComposed (void) const;

	// Copies string to a newly allocated Layout/UTF8 buffer
	GS::UniChar::Layout*	CopyUStr (UIndex from = 0, USize range = MaxUSize) const;	// Returned pointer should be deleted by the caller -> delete[]
	char*					CopyUTF8 (UIndex from = 0, USize range = MaxUSize, OnConversionError onConversionError = ReplaceWithFillers) const;	// Returned pointer should be deleted by the caller -> delete[]

	const void*		ToPrintf (void) const;
	void*			ToScanf  (void);

		// I/O

	virtual	GSErrCode	Read  (GS::IChannel& ic) override;
	virtual	GSErrCode	Write (GS::OChannel& oc) const override;

		// Other

	ULong	GenerateHashValue (void) const;

		// Debug

	virtual void	Print	   (OChannel& oc) const override;
	virtual void	Dump	   (OChannel& oc) const override;
	virtual bool	CheckState (void) const override;
};

GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (UniChar leftOp, UniChar                rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (UniChar leftOp, const UniString&       rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (UniChar leftOp, const UniChar::Layout* rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (UniChar leftOp, const char*            rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (UniChar leftOp, const UniString::Substring&       rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (UniChar leftOp, const UniString::ConstSubstring&  rightOp);

GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString& leftOp, UniChar                rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString& leftOp, const UniString&       rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString& leftOp, const UniChar::Layout* rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString& leftOp, const char*            rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString& leftOp, const UniString::Substring&       rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString& leftOp, const UniString::ConstSubstring&  rightOp);

GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniChar::Layout* leftOp, UniChar               rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniChar::Layout* leftOp, const UniString&      rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniChar::Layout* leftOp, const UniString::Substring&      rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniChar::Layout* leftOp, const UniString::ConstSubstring& rightOp);

GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const char* leftOp, UniChar               rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const char* leftOp, const UniString&      rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const char* leftOp, const UniString::Substring&      rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const char* leftOp, const UniString::ConstSubstring& rightOp);

GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::Substring& leftOp, UniChar                rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::Substring& leftOp, const UniString&       rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::Substring& leftOp, const UniChar::Layout* rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::Substring& leftOp, const char*            rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::Substring& leftOp, const UniString::Substring&       rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::Substring& leftOp, const UniString::ConstSubstring&  rightOp);

GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::ConstSubstring& leftOp, UniChar                rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::ConstSubstring& leftOp, const UniString&       rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::ConstSubstring& leftOp, const UniChar::Layout* rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::ConstSubstring& leftOp, const char*            rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::ConstSubstring& leftOp, const UniString::Substring&       rightOp);
GSROOT_DLL_EXPORT UniString::Concatenation	GSCALL operator+ (const UniString::ConstSubstring& leftOp, const UniString::ConstSubstring&  rightOp);

GSROOT_DLL_EXPORT extern const UniString EmptyUniString;

inline ULong	GenerateHashValue (const UniString& target);

GS_FORCE_INLINE void	Append (GS::FastLogger::LogComposer& logComposer, const UniString& string);

}	// namespace GS



inline GS::UniString::UniString ()
{
	content = ShareEmptyBuffer ();
}


inline GS::UniString::UniString (UniString&& source):
	content (source.content)
{
	source.content = nullptr;
}


inline GS::UniString::~UniString ()
{
	ReleaseBuffer ();
}


inline GS::UniString::UniCharRef	GS::UniString::operator[] (UIndex idx)
{
	REF_CHECK (idx < content->length);

	return UniCharRef (this, idx);
}


inline GS::UniChar	GS::UniString::operator[] (UIndex idx) const
{
	REF_CHECK (idx < content->length);

	return content->string[idx];
}


inline GS::UniChar	GS::UniString::GetChar (UIndex idx) const
{
	REF_CHECK (idx < content->length);

	return content->string[idx];
}


inline void		GS::UniString::SetChar (UIndex idx, UniChar uc)
{
	REF_CHECK (idx < content->length);

	AboutToModify ();

	content->string[idx] = uc;
}


inline GS::UniChar	GS::UniString::GetLast (void) const
{
	REF_CHECK (content->length > 0);

	return content->string[content->length - 1];
}


inline GS::UniString&	GS::UniString::operator+= (UniChar uc)
{
	Append (uc);

	return *this;
}


inline GS::UniString&	GS::UniString::operator+= (const UniString& str)
{
	NULL_REF_CHECK (str);

	Append (str);

	return *this;
}


inline GS::UniString&	GS::UniString::operator+= (const UniChar::Layout* uStr)
{
	NULL_PTR_CHECK (uStr);

	Append (uStr);

	return *this;
}


inline GS::UniString&	GS::UniString::operator+= (const char* cStr)
{
	NULL_PTR_CHECK (cStr);

	Append (cStr, CC_Default);

	return *this;
}


inline GS::UniString&	GS::UniString::operator+= (const unsigned char* pStr)
{
	NULL_PTR_CHECK (pStr);

	Append (pStr, CC_Default);

	return *this;
}


inline USize	GS::UniString::GetLength (void) const
{
	return content->length;
}


inline bool		GS::UniString::IsEmpty	(void) const
{
	return (content->length == 0);
}


inline bool		GS::UniString::BeginsWith (UniChar uc) const
{
	if (content->length == 0)
		return false;

	return (content->string[0] == uc);
}


inline bool		GS::UniString::EndsWith (UniChar uc) const
{
	if (content->length == 0)
		return false;

	return (content->string[content->length - 1] == uc);
}


inline void		GS::UniString::AboutToModify (void)
{
	if (content->refCounter > static_cast<Int32>(1))
		DetachBuffer ();
}


inline void		GS::UniString::AboutToModify (USize minCapacity)
{
	if (content->refCounter > static_cast<Int32>(1))
		DetachBuffer (minCapacity);
	else
		AdjustCapacity (minCapacity);
}


inline void		GS::UniString::AboutToInsert (USize charCount)
{
	if (content->refCounter > static_cast<Int32>(1))
		DetachBuffer (content->length + charCount);
	else
		IncreaseCapacity (content->length + charCount);
}


inline void		GS::UniString::AboutToDelete (void)
{
	if (content->refCounter > static_cast<Int32>(1))
		DetachBuffer ();
}


inline void		GS::UniString::SetBuffer (SharedBuffer* newContent)
{
	ReleaseBuffer ();

	content = newContent;
}


inline void		GS::UniString::DetachBuffer (void)
{
	SetBuffer (CopyBuffer (content));
}


inline void		GS::UniString::DetachBuffer (USize newCapacity)
{
	SetBuffer (CopyBuffer (content, newCapacity));
}


inline void		GS::UniString::ReleaseBuffer (void)
{
	if (content != nullptr) {
		if (content->refCounter.UnsafeDecrementIf1 () == 0)
			DeleteBuffer (content);

		content = nullptr;
	}
}


template<typename... Args>
GS::UniString	GS::UniString::Printf (const char* formatString, Args... args)
{
	return PrintfFwd (formatString, args...);
}


template<typename... Args>
GS::UniString	GS::UniString::Printf (const UniString& formatString, Args... args)
{
	return PrintfFwd (formatString.ToUStr (), args...);
}

template<typename... Args>
GS::UniString	GS::UniString::Printf (const UniChar::Layout* formatString, Args... args)
{
	return PrintfFwd (formatString, args...);
}


inline ULong	GS::GenerateHashValue (const UniString& target)
{
	NULL_REF_CHECK (target);

	return target.GenerateHashValue ();
}


GS_FORCE_INLINE void	GS::Append (GS::FastLogger::LogComposer& logComposer, const UniString& string)
{
	logComposer.Append (static_cast<const char*> (string.ToCStr ()), string.GetLength ());
}
// ______________________________ Class UniString ______________________________

// ---------------------------------------------------------------------------------------------------------------------

#define	UniStringToConstCString(str)			(str).ToCStr().Get()
#define	ConstCStringToUniString(cStr)			GS::UniString(cStr)
#define	UniStringToConstUTF8String(str)			(str).ToCStr(0, MaxUSize, CC_UTF8).Get()
#define	ConstUTF8StringToUniString(cStr)		GS::UniString(cStr, CC_UTF8)
#define ConstCStringToUniStringOrEmpty(cStr)	(((cStr) == nullptr) ? GS::UniString() : GS::UniString(cStr))
#define ConstUTF8StringToUniStringOrEmpty(cStr)	(((cStr) == nullptr) ? GS::UniString() : GS::UniString(cStr, CC_UTF8))

#if defined(__OBJC__) && __OBJC__ && !defined (__linux__)
#import <Foundation/Foundation.h>

@interface NSString (UniStringExtensions)
- (void)getGSUniStringTo:(GS::UniString*) uniString;

+ (NSString*)stringFromGSUniString:(const GS::UniString &) uniString;
+ (NSString*)stringFromGSUString:(const GS::uchar_t*) uString;

@end
#endif

#endif
