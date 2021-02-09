
// *****************************************************************************
//
//                                  Class SkipMap
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	MB, KKr
//
// SG compatible
//
// *****************************************************************************


#if !defined SKIPMAP_HPP
#define SKIPMAP_HPP

#pragma once

// --------------------------------- Includes ----------------------------------

#include "AssociativeContainer.hpp"
#include "Pair.hpp"
#include "FixArray.hpp"
#include "PagedArray.hpp"

#include "ClassInfo.hpp"
#include "RandomGenerator.hpp"
#include "ForwardContainerIterator.hpp"

#include <new>
#include <utility>
#include <functional>

#include "BaseAlgorithms.hpp"
#include "Optional.hpp"
#include "TypeTraits.hpp"
#include "SkipContainerCommon.hpp"

#include "InitializerRefList.hpp"


// ------------------------------ Predeclarations ------------------------------

namespace GS {

template <class Type>
GSErrCode	PrintType (OChannel& oc, const Type& arg);

}


// ================================ Class SkipMap ================================

namespace GS {

class GSROOT_DLL_EXPORT SkipMapPrivate {
public:
	static GS::ClassInfo	classInfo;	// SkipMap's private classInfo
};


template <class Key, class Value, class Comparator = DefaultComparator<Key>>
class SkipMap final : public AssociativeContainer<Key, Value> {
private:
	static const USize MaxLevel = 32;
	struct SkipValueNode;

	struct SkipNode {
		UInt8			capacity = static_cast<UInt8> (ArraySize (fixBuffer));
		UInt8			size = 0;
		SkipValueNode**	forward = fixBuffer;
		SkipValueNode*	fixBuffer[4];
		
		SkipNode (UInt8 level);
		virtual ~SkipNode ();

		SkipNode (const SkipNode& source) = delete;
		SkipNode&	operator= (const SkipNode& source) = delete;

		UInt8	GetForwardSize () const;
		void	EnsureForwardSize (UInt8 newSize);

		void	Move (SkipNode& source);
		void	Swap (SkipNode& rightOp);

		USize	GetNodeLevel () const;
	};

	struct SkipValueNode : public SkipNode {
		Key			key;
		Value		value;

		SkipValueNode ();
		SkipValueNode (const Key& key, const Value& value, UInt8 level);
		SkipValueNode (const Key& key, Value&& value, UInt8 level);
		virtual ~SkipValueNode ();

		SkipValueNode (const SkipValueNode& source) = delete;
		SkipValueNode&	operator= (const SkipValueNode& source) = delete;

		void	Move (SkipValueNode& source);
		void	Swap (SkipValueNode& rightOp);
	};

	SkipNode				  head;
	UInt8					  currentLevel = 1;
	PagedArray<SkipValueNode> nodes;
	RandomGenerator			  rndGen;

	static void		CheckComparator (void);

	UInt8			GetRandomLevel ();

	SkipValueNode*			FindNode (const Key& key);
	const SkipValueNode*	FindNode (const Key& key) const;

	SkipValueNode*			FindNodeOrNext (const Key& key);
	const SkipValueNode*	FindNodeOrNext (const Key& key) const;

	void			CollectPredecessors (const Key& key, FixArray<SkipNode*, MaxLevel>& preds);

	USize			FindNodeDebug (const Key& key) const;

		// CurrentPair, ConstCurrentPair

	struct CurrentPair {
		const Key*		key;
		Value*			value;

		CurrentPair ():								key (nullptr), value (nullptr) {}
		CurrentPair (const Key* key, Value* value):	key (key),     value (value)   {}
	};

	struct ConstCurrentPair {
		const Key*		key;
		const Value*	value;

		ConstCurrentPair ():								   key (nullptr), value (nullptr) {}
		ConstCurrentPair (const Key* key, const Value* value): key (key),     value (value)   {}
	};

		// IteratorState

	struct IteratorState {
		SkipValueNode*	current;	// current entry

		explicit IteratorState (SkipValueNode* initNode) : current (initNode) {}
	};
	void				Increment (IteratorState& state) const;
	bool				IsEqual (const IteratorState& state1, const IteratorState& state2) const;
	bool				IsEnd (const IteratorState& state) const;

	struct ConstIteratorState {
		const SkipValueNode*	current;	// current entry

		explicit ConstIteratorState (const SkipValueNode* initNode) : current (initNode) {}
		ConstIteratorState (const IteratorState& source) :			  current (source.current) {}
		void operator= (const IteratorState& source)				  { current = source.current; }
	};
	void				Increment (ConstIteratorState& state) const;
	bool				IsEqual (const ConstIteratorState& state1, const ConstIteratorState& state2) const;
	bool				IsEnd (const ConstIteratorState& state) const;

		// KeyIteratorState

	struct KeyIteratorState : public IteratorState {
		explicit KeyIteratorState (SkipValueNode* initNode) : IteratorState (initNode) {}
	};
	Key&				GetCurrent (const KeyIteratorState& state);
	const Key&			GetCurrent (const KeyIteratorState& state) const;

		// ValueIteratorState

	struct ValueIteratorState : public IteratorState {
		explicit ValueIteratorState (SkipValueNode* initNode) : IteratorState (initNode) {}
	};
	Value&				GetCurrent (const ValueIteratorState& state);
	const Value&		GetCurrent (const ValueIteratorState& state) const;

		// PairIteratorState

	struct PairIteratorState : public IteratorState {
		mutable CurrentPair			currentPair;	// stores the current key-value pair

		explicit PairIteratorState (SkipValueNode* initNode) : IteratorState (initNode) {}
	};
	CurrentPair&			GetCurrent (const PairIteratorState& state);
	const CurrentPair&		GetCurrent (const PairIteratorState& state) const;

		// ConstPairIteratorState

	struct ConstPairIteratorState : public ConstIteratorState {
		mutable ConstCurrentPair	currentPair;	// stores the current key-value pair

		explicit ConstPairIteratorState (const SkipValueNode* initNode) : ConstIteratorState (initNode) {}
		ConstPairIteratorState (const PairIteratorState& source) :		  ConstIteratorState (source) {}
		void operator= (const PairIteratorState& source)				  { ConstIteratorState::operator= (source); }
	};
	ConstCurrentPair&		GetCurrent (const ConstPairIteratorState& state);
	const ConstCurrentPair&	GetCurrent (const ConstPairIteratorState& state) const;

		// Friend classes

	friend class ConstForwardContainerIterator<SkipMap, KeyIteratorState, Key>;
	friend class ForwardContainerIterator<SkipMap, ValueIteratorState, Value>;
	friend class ConstForwardContainerIterator<SkipMap, ValueIteratorState, Value>;
	friend class ConstForwardContainerIterator<SkipMap, PairIteratorState, CurrentPair>;
	friend class ConstForwardContainerIterator<SkipMap, ConstPairIteratorState, ConstCurrentPair>;
	friend class ConstGeneralForwardContainerIterator<SkipMap, KeyIteratorState, Key>;
	friend class GeneralForwardContainerIterator<SkipMap, ValueIteratorState, Value>;
	friend class ConstGeneralForwardContainerIterator<SkipMap, ValueIteratorState, Value>;
	friend class ConstGeneralForwardContainerIterator<SkipMap, PairIteratorState, CurrentPair>;
	friend class ConstGeneralForwardContainerIterator<SkipMap, ConstPairIteratorState, ConstCurrentPair>;

public:
		// Types

	typedef ConstForwardContainerIterator<SkipMap, KeyIteratorState, Key> 							ConstKeyIterator;
	typedef ForwardContainerIterator<SkipMap, ValueIteratorState, Value> 							ValueIterator;
	typedef ConstForwardContainerIterator<SkipMap, ValueIteratorState, Value> 						ConstValueIterator;
	typedef ConstForwardContainerIterator<SkipMap, PairIteratorState, CurrentPair> 					Iterator;
	typedef ConstForwardContainerIterator<SkipMap, ConstPairIteratorState, ConstCurrentPair>		ConstIterator;

	typedef ConstGeneralForwardContainerIterator<SkipMap, KeyIteratorState, Key> 					ConstGeneralKeyIterator;
	typedef GeneralForwardContainerIterator<SkipMap, ValueIteratorState, Value> 					GeneralValueIterator;
	typedef ConstGeneralForwardContainerIterator<SkipMap, ValueIteratorState, Value> 				ConstGeneralValueIterator;
	typedef ConstGeneralForwardContainerIterator<SkipMap, PairIteratorState, CurrentPair> 			GeneralIterator;
	typedef ConstGeneralForwardContainerIterator<SkipMap, ConstPairIteratorState, ConstCurrentPair>	ConstGeneralIterator;

	class KeyConversionEnumerator;
	class ValueConversionEnumerator;

private:
	
		// Ranges

	class SkipMapAsKeyRange {
	public:
		ConstKeyIterator	begin (void) const		{ return reinterpret_cast<const SkipMap*> (this)->BeginKeys (); }
		ConstKeyIterator	end   (void) const		{ return reinterpret_cast<const SkipMap*> (this)->EndKeys (); }
	};

	class SkipMapAsValueRange {
	public:
		ValueIterator		begin (void)			{ return reinterpret_cast<SkipMap*> (this)->BeginValues (); }
		ValueIterator		end   (void)			{ return reinterpret_cast<SkipMap*> (this)->EndValues (); }
		ConstValueIterator	begin (void) const		{ return reinterpret_cast<const SkipMap*> (this)->BeginValues (); }
		ConstValueIterator	end   (void) const		{ return reinterpret_cast<const SkipMap*> (this)->EndValues (); }
	};

	struct SkipMapRange {
		SkipMap*			container;
		SkipValueNode*      rangeBegin;
		SkipValueNode*      rangeEnd;

		SkipMapRange (SkipMap* container, SkipValueNode* rangeBegin, SkipValueNode* rangeEnd) : container (container), rangeBegin (rangeBegin), rangeEnd (rangeEnd) {}

		auto begin (void)							{ return Iterator (*container, PairIteratorState (rangeBegin)); }
		auto end   (void)							{ return Iterator (*container, PairIteratorState (rangeEnd)); }
	};

	struct ConstSkipMapRange {
		const SkipMap*			container;
		const SkipValueNode*	rangeBegin;
		const SkipValueNode*	rangeEnd;

		ConstSkipMapRange (const SkipMap* container, const SkipValueNode* rangeBegin, const SkipValueNode* rangeEnd) : container (container), rangeBegin (rangeBegin), rangeEnd (rangeEnd) {}

		auto begin (void) const						{ return ConstIterator (*container, ConstPairIteratorState (rangeBegin)); }
		auto end   (void) const						{ return ConstIterator (*container, ConstPairIteratorState (rangeEnd)); }
	};

public:
		// Constructors
		
	SkipMap ();
	SkipMap (std::initializer_list<Pair<Key, Value>> source);
	SkipMap (const SkipMap& source);
	SkipMap (SkipMap&& source);
	~SkipMap ();

	SkipMap&			operator= (const SkipMap& source);
	SkipMap&			operator= (SkipMap&& source);

	inline void			Move (SkipMap& source);
	void				Move (const std::function<void (Value&&)>& processor);

		// Content management

	bool				Add (const Key& key, const Value& value);
	bool				Add (const Key& key, Value&& value);

	void				Put (const Key& key, const Value& value);
	void				Put (const Key& key, Value&& value);

	bool				Delete (const Key& key);
	void				Clear  ();

	void				Swap   (SkipMap& rightOp);

	USize				GetSize () const;
	inline	bool		IsEmpty () const;

		// Content query

	Value&				operator[] (const Key& key);
	const Value&		operator[] (const Key& key) const;

	Value&				Get (const Key& key);
	const Value&		Get (const Key& key) const;
	bool				Get (const Key& key, Value* value) const;
	bool				Get (const Key& key, Value** value);
	bool				Get (const Key& key, const Value** value) const;

	Value&				Retrieve (const Key& key);
	Value&				Retrieve (const Key& key, const Value& defaultValue);

	bool				ContainsKey (const Key& key) const;

	bool				ContainsValue (const Value& value) const;
	bool				ContainsValue (const std::function<bool (const Value&)>& condition) const;

	Optional<Key>		FindValue (const Value& value) const;
	Optional<Key>		FindValue (const std::function<bool (const Value&)>& condition) const;

	USize				Count (const Value& value) const;
	USize				Count (const std::function<bool (const Value&)>& condition) const;

	bool				operator== (const SkipMap& rightOp) const;
	bool				operator!= (const SkipMap& rightOp) const;

	SkipMapRange		operator() (const Key& from, const Key& to);
	ConstSkipMapRange	operator() (const Key& from, const Key& to) const;

		// Iteration

	ConstKeyIterator				EnumerateKeys (void) const;
	ConstKeyIterator				BeginKeys (void) const;
	ConstKeyIterator				EndKeys (void) const;

	ValueIterator					EnumerateValues (void);
	ConstValueIterator				EnumerateValues (void) const;
	ValueIterator					BeginValues (void);
	ConstValueIterator				BeginValues (void) const;
	ValueIterator					EndValues (void);
	ConstValueIterator				EndValues (void) const;

	Iterator						Enumerate (void);
	ConstIterator					Enumerate (void) const;
	Iterator						Begin (void);
	ConstIterator					Begin (void) const;
	Iterator						End (void);
	ConstIterator					End (void) const;

	Iterator						EnumerateFrom (const Key& from);
	ConstIterator					EnumerateFrom (const Key& from) const;

		// General iteration

	ConstGeneralKeyIterator			EnumerateKeysGeneral (void) const;
	ConstGeneralKeyIterator			BeginKeysGeneral (void) const;
	ConstGeneralKeyIterator			EndKeysGeneral (void) const;

	GeneralValueIterator			EnumerateValuesGeneral (void);
	ConstGeneralValueIterator		EnumerateValuesGeneral (void) const;
	GeneralValueIterator			BeginValuesGeneral (void);
	ConstGeneralValueIterator		BeginValuesGeneral (void) const;
	GeneralValueIterator			EndValuesGeneral (void);
	ConstGeneralValueIterator		EndValuesGeneral (void) const;

	GeneralIterator					EnumerateGeneral (void);
	ConstGeneralIterator			EnumerateGeneral (void) const;
	GeneralIterator					BeginGeneral (void);
	ConstGeneralIterator			BeginGeneral (void) const;
	GeneralIterator					EndGeneral (void);
	ConstGeneralIterator			EndGeneral (void) const;

		// Enumeration

	template <class Function>
	void							EnumerateKeys (Function&& processor) const;

	template <class Function>
	void							EnumerateValues (Function&& processor);
	template <class Function>
	void							EnumerateValues (Function&& processor) const;
	template <class Function>
	void							EnumerateValuesConst (Function&& processor) const;

	template <class Function>
	void							Enumerate (Function&& processor);
	template <class Function>
	void							Enumerate (Function&& processor) const;
	template <class Function>
	void							EnumerateConst (Function&& processor) const;

		// Range-based for loop support

	const SkipMapAsKeyRange&	Keys (void) const;
	SkipMapAsValueRange&		Values (void);
	const SkipMapAsValueRange&	Values (void) const;
	auto&						ConstValues (void) const;

	auto&						AsConst (void) const;

		// Conversion

	KeyConversionEnumerator		ConvertKeys (void) const;
	ValueConversionEnumerator	ConvertValues (void) const;

		// I/O

	GSErrCode	Read (IChannel& ic);
	GSErrCode	Write (OChannel& oc) const;

		// Performance tuning

	double		GetAverageSeekFactor () const;
	double		GetAverageNodeLevel () const;
	USize		GetReallocationCount () const;

		// Debug

	void		Print (OChannel& oc) const;
	void		Dump (OChannel& oc) const;

		// Types

	class KeyConversionEnumerator : public GS::ConversionEnumerator<Key> {
	private:
		const SkipMap* source;

	public:
		KeyConversionEnumerator (const SkipMap& source);

		virtual void	Enumerate (const std::function<void (const Key&)>& processor) const override;
	};

	class ValueConversionEnumerator : public GS::ConversionEnumerator<Value> {
	private:
		const SkipMap* source;

	public:
		ValueConversionEnumerator (const SkipMap& source);

		virtual void	Enumerate (const std::function<void (const Value&)>& processor) const override;
	};
};


// === SkipMap methods =================================================================================================

template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::SkipNode::SkipNode (UInt8 level)
{
	EnsureForwardSize (level);
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::SkipNode::~SkipNode ()
{
	if (forward != fixBuffer)
		delete [] forward;
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::SkipNode::EnsureForwardSize (UInt8 newSize)
{
	if (newSize <= size)
		return;

	if (newSize > capacity) {
		UInt8 newCapacity = GS::Max (UInt8 (capacity + 4), newSize);

		if ((newCapacity & 3) != 0)
			newCapacity = (newCapacity + 4) & ~3;

		SkipValueNode** newForward = new SkipValueNode*[newCapacity];

		BNMoveMemory (newForward, forward, size * sizeof (SkipValueNode*));

		if (forward != fixBuffer)
			delete[] forward;

		forward = newForward;
		capacity = newCapacity;
	}

	for (UInt8 i = size; i < newSize; ++i)
		forward[i] = nullptr;

	size = newSize;
}


template <class Key, class Value, class Comparator>
UInt8	SkipMap<Key, Value, Comparator>::SkipNode::GetForwardSize () const
{
	return size;
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::SkipNode::Move (SkipNode& source)
{
	EnsureForwardSize (source.size);
	size = source.size;
	for (UInt8 i = 0; i < size; ++i)
		forward[i] = source.forward[i];
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::SkipNode::Swap (SkipNode& rightOp)
{
	SkipNode tempNode (1);
	tempNode.Move (rightOp);
	rightOp.Move (*this);
	Move (tempNode);
}

template <class Key, class Value, class Comparator>
USize	SkipMap<Key, Value, Comparator>::SkipNode::GetNodeLevel () const
{
	return size;
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::SkipValueNode::SkipValueNode (): 
	SkipNode (1)
{
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::SkipValueNode::SkipValueNode (const Key& key, const Value& value, UInt8 level): 
	key (key), 
	value (value),
	SkipNode (level)
{
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::SkipValueNode::SkipValueNode (const Key& key, Value&& value, UInt8 level): 
	key (key), 
	value (std::move (value)),
	SkipNode (level)
{
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::SkipValueNode::~SkipValueNode ()
{
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::SkipValueNode::Move (SkipValueNode& source)
{
	key   = std::move (source.key);
	value = std::move (source.value);

	SkipNode::Move (source);
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::SkipValueNode::Swap (SkipValueNode& rightOp)
{
	SkipValueNode tempNode;
	tempNode.Move (rightOp);
	rightOp.Move (*this);
	Move (tempNode);
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::CheckComparator (void)
{
	typedef bool (*TestSignature) (const Key&, const Key&);
	static_assert (GS::IsSame<TestSignature, decltype (&Comparator::IsEqual)>, "Invalid signature for Comparator::IsEqual");
	static_assert (GS::IsSame<TestSignature, decltype (&Comparator::IsLess)>, "Invalid signature for Comparator::IsLess");
}


template <class Key, class Value, class Comparator>
UInt8	SkipMap<Key, Value, Comparator>::GetRandomLevel ()
{
	return UInt8 (rndGen.GetLog2 () + 1);
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::SkipMap (): 
	head (MaxLevel), 
	rndGen (0xBADCAFE)
{
	CheckComparator ();
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::SkipMap (std::initializer_list<Pair<Key, Value>> source):
	SkipMap ()
{
	for (const auto& pair : source)
		Put (pair.first, pair.second);
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::SkipMap (const SkipMap& source):
	SkipMap ()
{
	for (auto& node : source)
		Put (*node.key, *node.value);
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::SkipMap (SkipMap&& source):
	SkipMap ()
{
	Move (source);
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::~SkipMap ()
{
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>&	SkipMap<Key, Value, Comparator>::SkipMap::operator= (const SkipMap& source)
{
	NULL_REF_CHECK (source);

	if (&source != this) {
		Clear ();

		for (SkipValueNode* node = source.head.forward[0]; node != nullptr; node = node->forward[0])
			Add (node->key, node->value);
	}

	return *this;
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>&	SkipMap<Key, Value, Comparator>::SkipMap::operator= (SkipMap&& source)
{
	NULL_REF_CHECK (source);

	Move (source);

	return *this;
}


template <class Key, class Value, class Comparator>
inline void		SkipMap<Key, Value, Comparator>::Move (SkipMap& source)
{
	if (DBERROR (&source == this))
		return;

	head.Move (source.head);
	nodes.Move (source.nodes);
	currentLevel = std::move (source.currentLevel);

	source.Clear ();
}


template <class Key, class Value, class Comparator>
void			SkipMap<Key, Value, Comparator>::Move (const std::function<void (Value&&)>& processor)
{
	for (USize i = 0; i < nodes.GetSize (); ++i)
		processor (std::move (nodes[i].value));

	Clear ();
}


template <class Key, class Value, class Comparator>
const typename SkipMap<Key, Value, Comparator>::SkipValueNode*	SkipMap<Key, Value, Comparator>::FindNode (const Key& key) const
{
	const SkipNode* node = &head;
	const SkipValueNode* nextNode = nullptr;

	for (UIndex i = currentLevel; i-- > 0; ) {
		for (nextNode = node->forward[i]; nextNode != nullptr && Comparator::IsLess (nextNode->key, key); nextNode = nextNode->forward[i])
			node = nextNode;
	}

	if (nextNode != nullptr && Comparator::IsEqual (nextNode->key, key))
		return nextNode;
	else
		return nullptr;
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::SkipValueNode*		SkipMap<Key, Value, Comparator>::FindNode (const Key& key)
{
	SkipNode* node = &head;
	SkipValueNode* nextNode = nullptr;

	for (UIndex i = currentLevel; i-- > 0; ) {
		for (nextNode = node->forward[i]; nextNode != nullptr && Comparator::IsLess (nextNode->key, key); nextNode = nextNode->forward[i])
			node = nextNode;
	}

	if (nextNode != nullptr && Comparator::IsEqual (nextNode->key, key))
		return nextNode;
	else
		return nullptr;
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::SkipValueNode*		SkipMap<Key, Value, Comparator>::FindNodeOrNext (const Key& key)
{
	SkipNode* node = &head;
	SkipValueNode* nextNode = nullptr;

	for (UIndex i = currentLevel; i-- > 0; ) {
		for (nextNode = node->forward[i]; nextNode != nullptr && Comparator::IsLess (nextNode->key, key); nextNode = nextNode->forward[i])
			node = nextNode;
	}

	return nextNode;
}


template <class Key, class Value, class Comparator>
const typename SkipMap<Key, Value, Comparator>::SkipValueNode*	SkipMap<Key, Value, Comparator>::FindNodeOrNext (const Key& key) const
{
	const SkipNode* node = &head;
	const SkipValueNode* nextNode = nullptr;

	for (UIndex i = currentLevel; i-- > 0; ) {
		for (nextNode = node->forward[i]; nextNode != nullptr && Comparator::IsLess (nextNode->key, key); nextNode = nextNode->forward[i])
			node = nextNode;
	}

	return nextNode;
}


template <class Key, class Value, class Comparator>
USize	SkipMap<Key, Value, Comparator>::FindNodeDebug (const Key& key) const
{
	USize jumps = 0;

	const SkipNode* node = &head;

	for (UIndex i = currentLevel; i-- > 0; ) {
		while (node->forward[i] != nullptr && Comparator::IsLess (node->forward[i]->key, key)) {
			node = node->forward[i];
			++jumps;
		}
	}
	node = node->forward[0];

	if (node != nullptr && Comparator::IsEqual (static_cast<const SkipValueNode*> (node)->key, key))
		return jumps + 1;
	else
		return 0;
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::CollectPredecessors (const Key& key, FixArray<SkipNode*, MaxLevel>& preds)
{
	SkipNode* node = &head;
	SkipValueNode* nextNode = nullptr;

	for (UIndex i = currentLevel; i-- > 0; ) {
		for (nextNode = node->forward[i]; nextNode != nullptr && Comparator::IsLess (nextNode->key, key); nextNode = nextNode->forward[i])
			node = nextNode;

		preds[i] = node;
	}
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::Add (const Key& key, const Value& value)
{
	NULL_REF_CHECK (key);
	NULL_REF_CHECK (value);

	FixArray<SkipNode*, MaxLevel> preds;
	CollectPredecessors (key, preds);

	SkipValueNode* node = preds[0]->forward[0];

	if (node != nullptr && Comparator::IsEqual (node->key, key))
		return false;

	const UInt8 newNodeLevel = GetRandomLevel ();

	if (currentLevel < newNodeLevel) {
		for (UIndex i = currentLevel; i < newNodeLevel; ++i)
			preds[i] = &head;

		currentLevel = newNodeLevel;
	}

	nodes.PushNew (key, value, newNodeLevel);
	SkipValueNode* newNode = &nodes.GetLast ();

	for (UIndex i = 0; i < newNodeLevel; ++i) {
		if (preds[i] != nullptr) {
			newNode->forward[i] = preds[i]->forward[i];
			preds[i]->forward[i] = newNode;
		}
	}

	return true;
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::Add (const Key& key, Value&& value)
{
	NULL_REF_CHECK (key);
	NULL_REF_CHECK (value);

	FixArray<SkipNode*, MaxLevel> preds;
	CollectPredecessors (key, preds);

	SkipValueNode* node = preds[0]->forward[0];

	if (node != nullptr && Comparator::IsEqual (node->key, key))
		return false;

	const UInt8 newNodeLevel = GetRandomLevel ();

	if (currentLevel < newNodeLevel) {
		for (UIndex i = currentLevel; i < newNodeLevel; ++i)
			preds[i] = &head;

		currentLevel = newNodeLevel;
	}

	nodes.PushNew (key, std::move (value), newNodeLevel);
	SkipValueNode* newNode = &nodes.GetLast ();

	for (UIndex i = 0; i < newNodeLevel; ++i) {
		if (preds[i] != nullptr) {
			newNode->forward[i] = preds[i]->forward[i];
			preds[i]->forward[i] = newNode;
		}
	}

	return true;
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::Put (const Key& key, const Value& value)
{
	NULL_REF_CHECK (key);
	NULL_REF_CHECK (value);

	FixArray<SkipNode*, MaxLevel> preds;
	CollectPredecessors (key, preds);

	SkipValueNode* node = preds[0]->forward[0];

	if (node != nullptr && Comparator::IsEqual (node->key, key)) {
		node->value = value;
		return;
	}

	const UInt8 newNodeLevel = GetRandomLevel ();

	if (currentLevel < newNodeLevel) {
		for (UIndex i = currentLevel; i < newNodeLevel; ++i)
			preds[i] = &head;

		currentLevel = newNodeLevel;
	}

	nodes.PushNew (key, value, newNodeLevel);
	SkipValueNode* newNode = &nodes.GetLast ();

	for (UIndex i = 0; i < newNodeLevel; ++i) {
		if (preds[i] != nullptr) {
			newNode->forward[i] = preds[i]->forward[i];
			preds[i]->forward[i] = newNode;
		}
	}
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::Put (const Key& key, Value&& value)
{
	NULL_REF_CHECK (key);
	NULL_REF_CHECK (value);

	FixArray<SkipNode*, MaxLevel> preds;
	CollectPredecessors (key, preds);

	SkipValueNode* node = preds[0]->forward[0];

	if (node != nullptr && Comparator::IsEqual (node->key, key)) {
		node->value = std::move (value);
		return;
	}

	const UInt8 newNodeLevel = GetRandomLevel ();

	if (currentLevel < newNodeLevel) {
		for (UIndex i = currentLevel; i < newNodeLevel; ++i)
			preds[i] = &head;

		currentLevel = newNodeLevel;
	}

	nodes.PushNew (key, std::move (value), newNodeLevel);
	SkipValueNode* newNode = &nodes.GetLast ();

	for (UIndex i = 0; i < newNodeLevel; ++i) {
		if (preds[i] != nullptr) {
			newNode->forward[i] = preds[i]->forward[i];
			preds[i]->forward[i] = newNode;
		}
	}
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::Delete (const Key& key)
{
	NULL_REF_CHECK (key);

	FixArray<SkipNode*, MaxLevel> preds;
	CollectPredecessors (key, preds);
	
	SkipValueNode* node = preds[0]->forward[0];

	if (node == nullptr || !Comparator::IsEqual (node->key, key))
		return false;

	for (UIndex i = 0; i < node->GetNodeLevel (); ++i)
		preds[i]->forward[i] = node->forward[i];

	SkipValueNode* lastEntry = &nodes.GetLast ();
	if (node != lastEntry) {
		CollectPredecessors (lastEntry->key, preds);
	
		for (UIndex i = 0; i < lastEntry->GetNodeLevel (); ++i)
			preds[i]->forward[i] = node;
	
		node->Move (*lastEntry);
	}
	
	nodes.DeleteLast ();

	return true;
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::Clear ()
{
	nodes.Clear ();

	for (UInt8 i = 0; i < head.GetForwardSize (); ++i)
		head.forward[i] = nullptr;
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::Swap (SkipMap& rightOp)
{
	NULL_REF_CHECK (rightOp);

	if (this == &rightOp)
		return;

	head.Swap (rightOp.head);
	nodes.Swap (rightOp.nodes);
	GS::Swap (currentLevel, rightOp.currentLevel);
}


template <class Key, class Value, class Comparator>
USize	SkipMap<Key, Value, Comparator>::GetSize () const
{
	return nodes.GetSize ();
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::IsEmpty () const
{
	return nodes.IsEmpty ();
}


template <class Key, class Value, class Comparator>
Value&		SkipMap<Key, Value, Comparator>::operator[] (const Key& key)
{
	NULL_REF_CHECK (key);

	SkipValueNode* node = FindNode (key);
	REF_CHECK_R (node != nullptr);

	return node->value;
}


template <class Key, class Value, class Comparator>
const Value&	SkipMap<Key, Value, Comparator>::operator[] (const Key& key) const
{
	NULL_REF_CHECK (key);

	const SkipValueNode* node = FindNode (key);
	REF_CHECK_R (node != nullptr);

	return node->value;
}


template <class Key, class Value, class Comparator>
Value&		SkipMap<Key, Value, Comparator>::Get (const Key& key)
{
	NULL_REF_CHECK (key);

	SkipValueNode* node = FindNode (key);
	REF_CHECK_R (node != nullptr);

	return node->value;
}


template <class Key, class Value, class Comparator>
const Value&	SkipMap<Key, Value, Comparator>::Get (const Key& key) const
{
	NULL_REF_CHECK (key);

	const SkipValueNode* node = FindNode (key);
	REF_CHECK_R (node != nullptr);

	return node->value;
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::Get (const Key& key, Value* value) const
{
	NULL_REF_CHECK (key);
	NULL_PTR_CHECK (value);

	const SkipValueNode* node = FindNode (key);
	if (node == nullptr)
		return false;

	*value = node->value;

	return true;
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::Get (const Key& key, Value** value)
{
	NULL_REF_CHECK (key);
	NULL_PTR_CHECK (value);

	SkipValueNode* node = FindNode (key);
	if (node == nullptr)
		return false;

	*value = &node->value;

	return true;
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::Get (const Key& key, const Value** value) const
{
	NULL_REF_CHECK (key);
	NULL_PTR_CHECK (value);

	const SkipValueNode* node = FindNode (key);
	if (node == nullptr)
		return false;

	*value = &node->value;

	return true;
}


template <class Key, class Value, class Comparator>
Value&	SkipMap<Key, Value, Comparator>::Retrieve (const Key& key)
{
	NULL_REF_CHECK (key);

	FixArray<SkipNode*, MaxLevel> preds;
	CollectPredecessors (key, preds);

	SkipValueNode* node = preds[0]->forward[0];

	if (node != nullptr && Comparator::IsEqual (node->key, key))
		return node->value;

	const UInt8 newNodeLevel = GetRandomLevel ();

	if (currentLevel < newNodeLevel) {
		for (UIndex i = currentLevel; i < newNodeLevel; ++i)
			preds[i] = &head;

		currentLevel = newNodeLevel;
	}

	nodes.PushNew (key, Value (), newNodeLevel);
	SkipValueNode* newNode = &nodes.GetLast ();

	for (UIndex i = 0; i < newNodeLevel; ++i) {
		if (preds[i] != nullptr) {
			newNode->forward[i] = preds[i]->forward[i];
			preds[i]->forward[i] = newNode;
		}
	}

	return newNode->value;
}


template <class Key, class Value, class Comparator>
Value&	SkipMap<Key, Value, Comparator>::Retrieve (const Key& key, const Value& defaultValue)
{
	NULL_REF_CHECK (key);

	FixArray<SkipNode*, MaxLevel> preds;
	CollectPredecessors (key, preds);

	SkipValueNode* node = preds[0]->forward[0];

	if (node != nullptr && Comparator::IsEqual (node->key, key))
		return node->value;

	const UInt8 newNodeLevel = GetRandomLevel ();

	if (currentLevel < newNodeLevel) {
		for (UIndex i = currentLevel; i < newNodeLevel; ++i)
			preds[i] = &head;

		currentLevel = newNodeLevel;
	}

	nodes.PushNew (key, defaultValue, newNodeLevel);
	SkipValueNode* newNode = &nodes.GetLast ();

	for (UIndex i = 0; i < newNodeLevel; ++i) {
		if (preds[i] != nullptr) {
			newNode->forward[i] = preds[i]->forward[i];
			preds[i]->forward[i] = newNode;
		}
	}

	return newNode->value;
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::ContainsKey (const Key& key) const
{
	NULL_REF_CHECK (key);

	return (FindNode (key) != nullptr);
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::ContainsValue (const Value& value) const
{
	NULL_REF_CHECK (value);

	for (auto& it : Values ()) {
		if (it == value)
			return true;
	}

	return false;
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::ContainsValue (const std::function<bool (const Value&)>& condition) const
{
	for (auto& it : Values ()) {
		if (condition (it))
			return true;
	}

	return false;
}


template <class Key, class Value, class Comparator>
Optional<Key>	SkipMap<Key, Value, Comparator>::FindValue (const Value& value) const
{
	NULL_REF_CHECK (value);

	for (auto& it : *this) {
		if (*it.value == value)
			return *it.key;
	}

	return NoValue;
}


template <class Key, class Value, class Comparator>
Optional<Key>	SkipMap<Key, Value, Comparator>::FindValue (const std::function<bool (const Value&)>& condition) const
{
	for (auto& it : *this) {
		if (condition (*it.value))
			return *it.key;
	}

	return NoValue;
}


template <class Key, class Value, class Comparator>
USize	SkipMap<Key, Value, Comparator>::Count (const Value& value) const
{
	NULL_REF_CHECK (value);

	USize count = 0;

	for (auto& it : Values ()) {
		if (it == value)
			++count;
	}

	return count;
}


template <class Key, class Value, class Comparator>
USize	SkipMap<Key, Value, Comparator>::Count (const std::function<bool (const Value&)>& condition) const
{
	USize count = 0;

	for (auto& it : Values ()) {
		if (condition (it))
			++count;
	}

	return count;
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::operator== (const SkipMap& rightOp) const
{
	NULL_REF_CHECK (rightOp);

	if (GetSize () != rightOp.GetSize ())
		return false;

	const SkipValueNode* leftIt = head.forward[0];
	const SkipValueNode* rightIt = rightOp.head.forward[0];

	while (leftIt != nullptr && rightIt != nullptr) {
		if (leftIt->key != rightIt->key)
			return false;

		if (leftIt->value != rightIt->value)
			return false;

		leftIt = leftIt->forward[0];
		rightIt = rightIt->forward[0];
	}

	return true;
}

template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::operator!= (const SkipMap& rightOp) const
{
	NULL_REF_CHECK (rightOp);

	return !(*this == rightOp);
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::SkipMapRange		SkipMap<Key, Value, Comparator>::operator() (const Key& from, const Key& to)
{
	NULL_REF_CHECK (from);
	NULL_REF_CHECK (to);

	SkipValueNode* begRange = FindNodeOrNext (from);
	SkipValueNode* endRange = FindNodeOrNext (to);

	return SkipMapRange (this, begRange, endRange);
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstSkipMapRange		SkipMap<Key, Value, Comparator>::operator() (const Key& from, const Key& to) const
{
	NULL_REF_CHECK (from);
	NULL_REF_CHECK (to);

	const SkipValueNode* begRange = FindNodeOrNext (from);
	const SkipValueNode* endRange = FindNodeOrNext (to);

	return ConstSkipMapRange (this, begRange, endRange);
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::Increment (IteratorState& state) const
{
	state.current = state.current->forward[0];
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::IsEqual (const IteratorState& state1, const IteratorState& state2) const
{
	return state1.current == state2.current;
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::IsEnd (const IteratorState& state) const
{
	return (state.current == nullptr);
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::Increment (ConstIteratorState& state) const
{
	state.current = state.current->forward[0];
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::IsEqual (const ConstIteratorState& state1, const ConstIteratorState& state2) const
{
	return state1.current == state2.current;
}


template <class Key, class Value, class Comparator>
bool	SkipMap<Key, Value, Comparator>::IsEnd (const ConstIteratorState& state) const
{
	return (state.current == nullptr);
}


template <class Key, class Value, class Comparator>
Key&	SkipMap<Key, Value, Comparator>::GetCurrent (const KeyIteratorState& state)
{
	return state.current->key;
}


template <class Key, class Value, class Comparator>
const Key&  SkipMap<Key, Value, Comparator>::GetCurrent (const KeyIteratorState& state) const
{
	return state.current->key;
}


template <class Key, class Value, class Comparator>
Value&	SkipMap<Key, Value, Comparator>::GetCurrent (const ValueIteratorState& state)
{
	return state.current->value;
}


template <class Key, class Value, class Comparator>
const Value&  SkipMap<Key, Value, Comparator>::GetCurrent (const ValueIteratorState& state) const
{
	return state.current->value;
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::CurrentPair&		SkipMap<Key, Value, Comparator>::GetCurrent (const PairIteratorState& state)
{
	state.currentPair.key = &state.current->key;
	state.currentPair.value = &state.current->value;

	return state.currentPair;
}


template <class Key, class Value, class Comparator>
const typename SkipMap<Key, Value, Comparator>::CurrentPair&	SkipMap<Key, Value, Comparator>::GetCurrent (const PairIteratorState& state) const
{
	state.currentPair.key = &state.current->key;
	state.currentPair.value = const_cast<Value*> (&state.current->value);

	return state.currentPair;
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstCurrentPair&		SkipMap<Key, Value, Comparator>::GetCurrent (const ConstPairIteratorState& state)
{
	state.currentPair.key = &state.current->key;
	state.currentPair.value = &state.current->value;

	return state.currentPair;
}


template <class Key, class Value, class Comparator>
const typename SkipMap<Key, Value, Comparator>::ConstCurrentPair&	SkipMap<Key, Value, Comparator>::GetCurrent (const ConstPairIteratorState& state) const
{
	state.currentPair.key = &state.current->key;
	state.currentPair.value = &state.current->value;

	return state.currentPair;
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::KeyConversionEnumerator::KeyConversionEnumerator (const SkipMap& source) :
	source (&source)
{
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::KeyConversionEnumerator::Enumerate (const std::function<void (const Key&)>& processor) const
{
	source->EnumerateKeys (processor);
}


template <class Key, class Value, class Comparator>
SkipMap<Key, Value, Comparator>::ValueConversionEnumerator::ValueConversionEnumerator (const SkipMap& source) :
	source (&source)
{
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::ValueConversionEnumerator::Enumerate (const std::function<void (const Value&)>& processor) const
{
	source->EnumerateValues (processor);
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstKeyIterator		SkipMap<Key, Value, Comparator>::EnumerateKeys (void) const
{
	return ConstKeyIterator (*this, KeyIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstKeyIterator		SkipMap<Key, Value, Comparator>::BeginKeys (void) const
{
	return ConstKeyIterator (*this, KeyIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstKeyIterator		SkipMap<Key, Value, Comparator>::EndKeys (void) const
{
	return ConstKeyIterator (*this, KeyIteratorState (nullptr));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ValueIterator		SkipMap<Key, Value, Comparator>::EnumerateValues (void)
{
	return ValueIterator (*this, ValueIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstValueIterator	SkipMap<Key, Value, Comparator>::EnumerateValues (void) const
{
	return ConstValueIterator (*this, ValueIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ValueIterator		SkipMap<Key, Value, Comparator>::BeginValues (void)
{
	return ValueIterator (*this, ValueIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstValueIterator	SkipMap<Key, Value, Comparator>::BeginValues (void) const
{
	return ConstValueIterator (*this, ValueIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ValueIterator		SkipMap<Key, Value, Comparator>::EndValues (void)
{
	return ValueIterator (*this, ValueIteratorState ( nullptr));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstValueIterator	SkipMap<Key, Value, Comparator>::EndValues (void) const
{
	return ConstValueIterator (*this, ValueIteratorState (nullptr));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::Iterator		SkipMap<Key, Value, Comparator>::Enumerate (void)
{
	return Iterator (*this, PairIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstIterator		SkipMap<Key, Value, Comparator>::Enumerate (void) const
{
	return ConstIterator (*this, ConstPairIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::Iterator		SkipMap<Key, Value, Comparator>::Begin (void)
{
	return Iterator (*this, PairIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstIterator		SkipMap<Key, Value, Comparator>::Begin (void) const
{
	return ConstIterator (*this, ConstPairIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::Iterator		SkipMap<Key, Value, Comparator>::End (void)
{
	return Iterator (*this, PairIteratorState (nullptr));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstIterator		SkipMap<Key, Value, Comparator>::End (void) const
{
	return ConstIterator (*this, ConstPairIteratorState (nullptr));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::Iterator		SkipMap<Key, Value, Comparator>::EnumerateFrom (const Key& from)
{
	return Iterator (*this, PairIteratorState (FindNodeOrNext (from)));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstIterator		SkipMap<Key, Value, Comparator>::EnumerateFrom (const Key& from) const
{
	return ConstIterator (*this, ConstPairIteratorState (FindNodeOrNext (from)));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstGeneralKeyIterator		SkipMap<Key, Value, Comparator>::EnumerateKeysGeneral (void) const
{
	return ConstGeneralKeyIterator (*this, KeyIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstGeneralKeyIterator		SkipMap<Key, Value, Comparator>::BeginKeysGeneral (void) const
{
	return ConstGeneralKeyIterator (*this, KeyIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstGeneralKeyIterator		SkipMap<Key, Value, Comparator>::EndKeysGeneral (void) const
{
	return ConstGeneralKeyIterator (*this, KeyIteratorState (nullptr));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::GeneralValueIterator		SkipMap<Key, Value, Comparator>::EnumerateValuesGeneral (void)
{
	return GeneralValueIterator (*this, ValueIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstGeneralValueIterator		SkipMap<Key, Value, Comparator>::EnumerateValuesGeneral (void) const
{
	return ConstGeneralValueIterator (*this, ValueIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::GeneralValueIterator		SkipMap<Key, Value, Comparator>::BeginValuesGeneral (void)
{
	return GeneralValueIterator (*this, ValueIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstGeneralValueIterator		SkipMap<Key, Value, Comparator>::BeginValuesGeneral (void) const
{
	return ConstGeneralValueIterator (*this, ValueIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::GeneralValueIterator		SkipMap<Key, Value, Comparator>::EndValuesGeneral (void)
{
	return GeneralValueIterator (*this, ValueIteratorState (nullptr));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstGeneralValueIterator		SkipMap<Key, Value, Comparator>::EndValuesGeneral (void) const
{
	return ConstGeneralValueIterator (*this, ValueIteratorState (nullptr));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::GeneralIterator		SkipMap<Key, Value, Comparator>::EnumerateGeneral (void)
{
	return GeneralIterator (*this, PairIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstGeneralIterator		SkipMap<Key, Value, Comparator>::EnumerateGeneral (void) const
{
	return ConstGeneralIterator (*this, ConstPairIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::GeneralIterator		SkipMap<Key, Value, Comparator>::BeginGeneral (void)
{
	return GeneralIterator (*this, PairIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstGeneralIterator		SkipMap<Key, Value, Comparator>::BeginGeneral (void) const
{
	return ConstGeneralIterator (*this, ConstPairIteratorState (head.forward[0]));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::GeneralIterator		SkipMap<Key, Value, Comparator>::EndGeneral (void)
{
	return GeneralIterator (*this, PairIteratorState (nullptr));
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ConstGeneralIterator		SkipMap<Key, Value, Comparator>::EndGeneral (void) const
{
	return ConstGeneralIterator (*this, ConstPairIteratorState (nullptr));
}


template <class Key, class Value, class Comparator>
template <class Function>
void	SkipMap<Key, Value, Comparator>::EnumerateKeys (Function&& processor) const
{
	for (auto& it : Keys ())
		processor (it);
}


template <class Key, class Value, class Comparator>
template <class Function>
void	SkipMap<Key, Value, Comparator>::EnumerateValues (Function&& processor)
{
	for (auto& it : Values ())
		processor (it);
}


template <class Key, class Value, class Comparator>
template <class Function>
void	SkipMap<Key, Value, Comparator>::EnumerateValues (Function&& processor) const
{
	for (auto& it : ConstValues ())
		processor (it);
}


template <class Key, class Value, class Comparator>
template <class Function>
void	SkipMap<Key, Value, Comparator>::EnumerateValuesConst (Function&& processor) const
{
	EnumerateValues (processor);
}


template <class Key, class Value, class Comparator>
template <class Function>
void	SkipMap<Key, Value, Comparator>::Enumerate (Function&& processor)
{
	for (auto& it : *this)
		processor (const_cast<const Key&> (*it.key), *it.value);
}


template <class Key, class Value, class Comparator>
template <class Function>
void	SkipMap<Key, Value, Comparator>::Enumerate (Function&& processor) const
{
	for (auto& it : *this)
		processor (const_cast<const Key&> (*it.key), const_cast<const Value&> (*it.value));
}


template <class Key, class Value, class Comparator>
template <class Function>
void	SkipMap<Key, Value, Comparator>::EnumerateConst (Function&& processor) const
{
	Enumerate (processor);
}


template <class Key, class Value, class Comparator>
const typename SkipMap<Key, Value, Comparator>::SkipMapAsKeyRange&		SkipMap<Key, Value, Comparator>::Keys (void) const
{
	return reinterpret_cast<const SkipMapAsKeyRange&> (*this);
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::SkipMapAsValueRange&		SkipMap<Key, Value, Comparator>::Values (void)
{
	return reinterpret_cast<SkipMapAsValueRange&> (*this);
}


template <class Key, class Value, class Comparator>
const typename SkipMap<Key, Value, Comparator>::SkipMapAsValueRange&	SkipMap<Key, Value, Comparator>::Values (void) const
{
	return reinterpret_cast<const SkipMapAsValueRange&> (*this);
}


template <class Key, class Value, class Comparator>
auto&	SkipMap<Key, Value, Comparator>::ConstValues (void) const
{
	return reinterpret_cast<const SkipMap<Key, typename Container<Value>::template MakeConstPtr<Value>::ConstType>&> (*this).Values ();
}


template <class Key, class Value, class Comparator>
auto&	SkipMap<Key, Value, Comparator>::AsConst (void) const
{
	return reinterpret_cast<const SkipMap<Key, typename Container<Value>::template MakeConstPtr<Value>::ConstType>&> (*this);
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::KeyConversionEnumerator	SkipMap<Key, Value, Comparator>::ConvertKeys (void) const
{
	return KeyConversionEnumerator (*this);
}


template <class Key, class Value, class Comparator>
typename SkipMap<Key, Value, Comparator>::ValueConversionEnumerator		SkipMap<Key, Value, Comparator>::ConvertValues (void) const
{
	return ValueConversionEnumerator (*this);
}


template <class Key, class Value, class Comparator>
GSErrCode	SkipMap<Key, Value, Comparator>::Read (IChannel& ic)
{
	NULL_REF_CHECK (ic);

	Clear ();

	GS::InputFrame frame (ic, SkipMapPrivate::classInfo);
	if (frame.GetStatus () != NoError)
		return frame.GetStatus ();

	Key key = Key ();
	Value value = Value ();

	GSErrCode errorCode = NoError;
	while (errorCode != GS::IChannel::EndOfInput) {
		errorCode = GS::ReadType (ic, key);
		if (errorCode == NoError)
			errorCode = GS::ReadType (ic, value);

		if (errorCode == NoError)
			Put (key, value);
	}

	return errorCode;
}


template <class Key, class Value, class Comparator>
GSErrCode	SkipMap<Key, Value, Comparator>::Write (OChannel& oc) const
{
	NULL_REF_CHECK (oc);

	GS::OutputFrame frame (oc, SkipMapPrivate::classInfo);

	for (const auto& it : *this) {
		GSErrCode errorCode = GS::WriteType (oc, *it.key);
		if (errorCode != NoError)
			return errorCode;

		errorCode = GS::WriteType (oc, *it.value);
		if (errorCode != NoError)
			return errorCode;
	}

	return NoError;
}


template <class Key, class Value, class Comparator>
double		SkipMap<Key, Value, Comparator>::GetAverageSeekFactor (void) const
{
	UInt64 sum = 0;
	for (const auto& it : *this)
		sum += FindNodeDebug (*it.key);

	return static_cast<double> (sum) / static_cast<double> (GetSize ());
}


template <class Key, class Value, class Comparator>
double		SkipMap<Key, Value, Comparator>::GetAverageNodeLevel (void) const
{
	UInt64 sum = 0;

	for (const SkipNode* node = &head; node != nullptr; node = node->forward[0])
		sum += node->GetNodeLevel ();

	return static_cast<double> (sum) / static_cast<double> (GetSize ());
}


template <class Key, class Value, class Comparator>
USize		SkipMap<Key, Value, Comparator>::GetReallocationCount (void) const
{
	USize counter = 0;

	for (const SkipNode* node = &head; node != nullptr; node = node->forward[0]) {
		if (node->GetForwardSize () > 4)
			++counter;
	}

	return counter;
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::Print (OChannel& oc) const
{
	NULL_REF_CHECK (oc);

	oc << "SkipMap size: " << GetSize () << '\n';
	oc << "---------------------\n";

	UIndex i = 0;
	for (auto& it : *this) {
		char index[32];
		::sprintf (index, "[%04lu]  ", (GS::ULongForStdio) i);
		oc << index;

		GS::PrintType (oc, *it.key);
		oc << "  =>  ";
		GS::PrintType (oc, *it.value);
		oc << '\n';

		i++;
	}

	oc << '\n';

	oc << "Average seek factor: " << GetAverageSeekFactor () << '\n';
}


template <class Key, class Value, class Comparator>
void	SkipMap<Key, Value, Comparator>::Dump (OChannel& oc) const
{
	NULL_REF_CHECK (oc);

	oc << "head:          "		   << &head << '\n';
	oc << "nodes:          "	   << &nodes << '\n';
	oc << "currentLevel:         " << currentLevel << '\n';
}


// === class SkipMapInserter ===========================================================================================
template <class Key, class Value>
class SkipMapInserter {
	SkipMap<Key, Value>* skipMap;

public:
	explicit SkipMapInserter (SkipMap<Key, Value>* skipMap) :
		skipMap (skipMap)
	{
	}

	void operator() (const Pair<Key, Value>& pair) const { skipMap->Add (pair.first, pair.second); }
	void operator() (Pair<Key, Value>&& pair)	   const { skipMap->Add (pair.first, std::move (pair.second)); }

	typedef Pair<Key, Value> DecayedFirstArgumentType;
};


// === Global functions ================================================================================================

template <class Key, class Value, class Comparator>
auto	begin (SkipMap<Key, Value, Comparator>& skipMap)
{
	return skipMap.Begin ();
}


template <class Key, class Value, class Comparator>
auto	end (SkipMap<Key, Value, Comparator>& skipMap)
{
	return skipMap.End ();
}


template <class Key, class Value, class Comparator>
auto	begin (const SkipMap<Key, Value, Comparator>& skipMap)
{
	return skipMap.Begin ();
}


template <class Key, class Value, class Comparator>
auto	end (const SkipMap<Key, Value, Comparator>& skipMap)
{
	return skipMap.End ();
}


template <class Key, class Value>
inline auto		GetInserter (SkipMap<Key, Value>& skipMap)
{
	return SkipMapInserter<Key, Value> (&skipMap);
}


template <class Key, class Value, class Comparator>
GSErrCode	Read (IChannel& oc, SkipMap<Key, Value, Comparator>& skipMap)
{
	return skipMap.Read (oc);
}


template <class Key, class Value, class Comparator>
GSErrCode	Write (OChannel& oc, const SkipMap<Key, Value, Comparator>& skipMap)
{
	return skipMap.Write (oc);
}


template <class Key, class Value, class Comparator>
void	Print (OChannel& oc, const SkipMap<Key, Value, Comparator>& skipMap)
{
	skipMap.Print (oc);
}


template <class Key, class Value, class Comparator>
void	Dump (OChannel& oc, const SkipMap<Key, Value, Comparator>& skipMap)
{
	skipMap.Dump (oc);
}

}	// namespace GS
// _______________________________ Class SkipMap _______________________________


#endif
