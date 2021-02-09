// *********************************************************************************************************************
// File:			DGEditControl.hpp
//
// Description:		EditControl classes
//
// Project:			GRAPHISOFT Dialog Manager (DGLib)
//
// Namespace:		DG
//
// Contact person:	AZS, BM
//
// SG compatible
// *********************************************************************************************************************

#ifndef DGEDITCONTROL_HPP
#define DGEDITCONTROL_HPP

#pragma once


// --- Includes --------------------------------------------------------------------------------------------------------

#include "DGItemProperty.hpp"
#include "DGUtility.hpp"
#include "DGPanel.hpp"


// --- Predeclarations -------------------------------------------------------------------------------------------------

namespace DG {
	class EditControl;
	class PosIntEdit;
	class IntEdit;
	class RealEdit;
	class TextEditBase;
	class TextEdit;
	class ShortcutEdit;
	class PosIntEditObserver;
	class IntEditObserver;
	class RealEditObserver;
	class TextEditBaseObserver;
	class ShortcutEditObserver;
}


// --- Class declarations ----------------------------------------------------------------------------------------------

namespace DG {

// --- EditDragSourceEvent ---------------------------------------------------------------------------------------------

class DG_DLL_EXPORT EditDragSourceEvent: public ItemDragSourceEvent
{
friend class EditDragSourceObserver;		// To access protected constructor
friend class PosIntEditObserver;			// To access protected constructor
friend class IntEditObserver;				// To access protected constructor
friend class RealEditObserver;				// To access protected constructor
friend class TextEditBaseObserver;			// To access protected constructor

protected:
	explicit EditDragSourceEvent (const ItemDragSourceEvent& ev);

public:
	~EditDragSourceEvent ();

	EditControl*	GetSource (void) const;
};


// --- EditDragSourceObserver ------------------------------------------------------------------------------------------

class DG_DLL_EXPORT EditDragSourceObserver
{
protected:
	EditDragSourceObserver ();

public:
	virtual ~EditDragSourceObserver ();

	virtual void	EditDragStarting (const EditDragSourceEvent& ev, bool* canStart);
	virtual void	EditDragStarted (const EditDragSourceEvent& ev, UShort* effect);
	virtual void	EditDragStarted (const EditDragSourceEvent& ev, UShort* effect, bool* rightDragMenu);
	virtual void	EditDragEnded (const EditDragSourceEvent& ev, DragDrop::Effect effect);
	virtual void	EditDragSetDelayedData (const EditDragSourceEvent& ev);
};


// --- EditDropTargetEvent ---------------------------------------------------------------------------------------------

class DG_DLL_EXPORT EditDropTargetEvent: public ItemDropTargetEvent
{
friend class PosIntEditObserver;			// To access protected constructor
friend class IntEditObserver;				// To access protected constructor
friend class RealEditObserver;				// To access protected constructor
friend class TextEditBaseObserver;			// To access protected constructor

private:
	ULong	caretPosition;

protected:
	explicit EditDropTargetEvent (const ItemDropTargetEvent& ev);

public:
	~EditDropTargetEvent ();

	EditControl*	GetSource (void) const;

	ULong			GetCaretPosition (void) const;
};


// --- EditDropTargetObserver ------------------------------------------------------------------------------------------

class DG_DLL_EXPORT EditDropTargetObserver
{
protected:
	EditDropTargetObserver ();

public:
	virtual ~EditDropTargetObserver ();

	virtual void	EditDragEntered (const EditDropTargetEvent& ev, DragDrop::Effect* effect, bool* defBehaviour);
	virtual void	EditDragEntered (const EditDropTargetEvent& ev, DragDrop::Effect* effect, bool* defBehaviour,
									 bool* rightDragMenu);
	virtual void	EditDragMoved (const EditDropTargetEvent& ev, DragDrop::Effect* effect, DragDrop::Feedback* denyFeedback);
	virtual void	EditDragLeft (const EditDropTargetEvent& ev, DragDrop::Effect* effect);
	virtual void	EditDropped (const EditDropTargetEvent& ev, DragDrop::Effect* effect);
};


// --- PosIntEditChangeEvent -------------------------------------------------------------------------------------------

class DG_DLL_EXPORT PosIntEditChangeEvent: public ItemChangeEvent
{
friend class PosIntEditObserver;			// To access protected constructor

private:
	UInt32	previousValue;
	Int32	hint;

protected:
	explicit PosIntEditChangeEvent (const ItemChangeEvent& ev);

public:
	~PosIntEditChangeEvent ();

	PosIntEdit*		GetSource (void) const;

	UInt32	GetPreviousValue (void) const;
	bool	WasRelativeInput (void) const;
};


// --- PosIntEditObserver ----------------------------------------------------------------------------------------------

class DG_DLL_EXPORT PosIntEditObserver: public ItemObserver,
										public EditDragSourceObserver,
										public EditDropTargetObserver
{
protected:
	virtual	short	SpecChanged (const ItemChangeEvent& ev) override;
	virtual short	SpecDragStarting (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragStarted (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragEnded (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragSetDelayedData (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragEntered (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDragMoved (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDragLeft (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDropped (const ItemDropTargetEvent& ev) override;

	virtual void	PosIntEditChanged (const PosIntEditChangeEvent& ev);

public:
	PosIntEditObserver ();
   ~PosIntEditObserver ();
};


// --- IntEditChangeEvent ----------------------------------------------------------------------------------------------

class DG_DLL_EXPORT IntEditChangeEvent: public ItemChangeEvent
{
friend class IntEditObserver;				// To access protected constructor

private:
	Int32	previousValue;
	Int32	hint;

protected:
	explicit IntEditChangeEvent (const ItemChangeEvent& ev);

public:
	~IntEditChangeEvent ();

	IntEdit*	GetSource (void) const;

	Int32	GetPreviousValue (void) const;
	bool	WasRelativeInput (void) const;
};


// --- IntEditObserver -------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT IntEditObserver: public ItemObserver,
									 public EditDragSourceObserver,
									 public EditDropTargetObserver
{
protected:
	virtual	short	SpecChanged (const ItemChangeEvent& ev) override;
	virtual short	SpecDragStarting (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragStarted (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragEnded (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragSetDelayedData (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragEntered (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDragMoved (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDragLeft (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDropped (const ItemDropTargetEvent& ev) override;

	virtual void	IntEditChanged (const IntEditChangeEvent& ev);

public:
	IntEditObserver ();
   ~IntEditObserver ();
};


// --- RealEditChangeEvent ---------------------------------------------------------------------------------------------

class DG_DLL_EXPORT RealEditChangeEvent: public ItemChangeEvent
{
friend class RealEditObserver;				// To access protected constructor

private:
	double	previousValue;
	Int32	hint;

protected:
	explicit RealEditChangeEvent (const ItemChangeEvent& ev);

public:
	~RealEditChangeEvent ();

	RealEdit*	GetSource (void) const;

	double	GetPreviousValue (void) const;
	bool	WasRelativeInput (void) const;
};


// --- RealEditObserver ------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT RealEditObserver: public ItemObserver,
									  public EditDragSourceObserver,
									  public EditDropTargetObserver
{
protected:
	virtual	short	SpecChanged (const ItemChangeEvent& ev) override;
	virtual short	SpecDragStarting (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragStarted (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragEnded (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragSetDelayedData (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragEntered (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDragMoved (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDragLeft (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDropped (const ItemDropTargetEvent& ev) override;

	virtual void	RealEditChanged (const RealEditChangeEvent& ev);

public:
	RealEditObserver ();
   ~RealEditObserver ();
};


// --- TextEditChangeEvent ---------------------------------------------------------------------------------------------

class DG_DLL_EXPORT TextEditChangeEvent: public ItemChangeEvent
{
friend class TextEditBaseObserver;				// To access protected constructor

protected:
	explicit TextEditChangeEvent (const ItemChangeEvent& ev);

public:
	~TextEditChangeEvent ();

	TextEditBase*	GetSource (void) const;
};


// --- TextEditBaseObserver --------------------------------------------------------------------------------------------

class DG_DLL_EXPORT TextEditBaseObserver: public ItemObserver,
										  public EditDragSourceObserver,
										  public EditDropTargetObserver
{
protected:
	virtual	short	SpecChanged (const ItemChangeEvent& ev) override;
	virtual short	SpecDragStarting (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragStarted (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragEnded (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragSetDelayedData (const ItemDragSourceEvent& ev) override;
	virtual short	SpecDragEntered (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDragMoved (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDragLeft (const ItemDropTargetEvent& ev) override;
	virtual short	SpecDropped (const ItemDropTargetEvent& ev) override;

	virtual void	TextEditChanged (const TextEditChangeEvent& ev);

public:
	TextEditBaseObserver ();
   ~TextEditBaseObserver ();
};


// --- ShortcutHitEvent ------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT ShortcutHitEvent: public ItemChangeEvent
{
friend class ShortcutEditObserver;				// To access protected constructor

protected:
	explicit ShortcutHitEvent (const ItemChangeEvent& ev);

public:
	~ShortcutHitEvent ();

	Key::Code		GetKey (void) const;
	ULong			GetModifierFlags (void) const;

	ShortcutEdit*	GetSource (void) const;
};


// --- ShortcutEditObserver --------------------------------------------------------------------------------------------

class DG_DLL_EXPORT ShortcutEditObserver: public ItemObserver
{
protected:
	virtual	short	SpecChanged (const ItemChangeEvent& ev) override;

	virtual void	ShortcutHit (const ShortcutHitEvent& ev);

public:
	ShortcutEditObserver ();
   ~ShortcutEditObserver ();
};


// --- EditControl -----------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT EditControl: public Item,
								 public ItemFontProperty,
								 public FocusableProperty
{
protected:
	EditControl ();
	EditControl (const Panel& panel, short item);

public:
	enum FrameType {
		Frame		= DG_ET_FRAME,
		NoFrame		= DG_ET_NOFRAME
	};

	enum UpdateType {
		Update		= DG_ET_UPDATE,
		NoUpdate	= DG_ET_NOUPDATE,
		NoDelay		= DG_ET_NODELAY
	};

	enum ReadOnlyType {
		Editable	= DG_ET_EDITABLE,
		ReadOnly	= DG_ET_READONLY
	};

	~EditControl ();
};


// --- PosIntEdit ------------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT PosIntEdit: public EditControl,
								public ItemVariousProperty
{
public:
	enum AbsRelType {
		Absolute	= DG_ET_ABSOLUTE,
		Relative	= DG_ET_RELATIVE
	};

	PosIntEdit (const Panel& panel, short item);
	PosIntEdit (const Panel& panel, const Rect&	rect, FrameType frame = Frame,
				AbsRelType absRel = Absolute, UpdateType update = Update, ReadOnlyType readOnly = Editable);
   ~PosIntEdit ();

	void	Attach (PosIntEditObserver& observer);
	void	Detach (PosIntEditObserver& observer);

	void	SetMin (ULong min);
	void	SetMax (ULong max);
	void	SetValue (ULong value);

	ULong	GetMin (void) const;
	ULong	GetMax (void) const;
	ULong	GetValue (void) const;
};


// --- IntEdit ---------------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT IntEdit:	public EditControl,
								public ItemVariousProperty
{
public:
	enum AbsRelType {
		Absolute	= DG_ET_ABSOLUTE,
		Relative	= DG_ET_RELATIVE
	};

	IntEdit (const Panel& panel, short item);
	IntEdit (const Panel& panel, const Rect& rect, FrameType frame = Frame,
			 AbsRelType absRel = Absolute, UpdateType update = Update, ReadOnlyType readOnly = Editable);
   ~IntEdit ();

	void	Attach (IntEditObserver& observer);
	void	Detach (IntEditObserver& observer);

	void	SetMin (Int32 min);
	void	SetMax (Int32 max);
	void	SetValue (Int32 value);

	Int32	GetMin (void) const;
	Int32	GetMax (void) const;
	Int32	GetValue (void) const;
};


// --- RealEdit --------------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT RealEdit:	public EditControl,
								public ItemVariousProperty
{
protected:
	RealEdit ();
	RealEdit (const Panel& panel, short item, short subType);

public:
	enum AbsRelType {
		Absolute	= DG_ET_ABSOLUTE,
		Relative	= DG_ET_RELATIVE
	};

	RealEdit (const Panel& panel, short item);
	RealEdit (const Panel& panel, const Rect& rect, FrameType frame = Frame,
			  AbsRelType absRel = Absolute, UpdateType update = Update, ReadOnlyType readOnly = Editable);
   ~RealEdit ();

	void	Attach (RealEditObserver& observer);
	void	Detach (RealEditObserver& observer);

	void	SetMin (double min);
	void	SetMax (double max);
	void	SetValue (double value);

	double	GetMin (void) const;
	double	GetMax (void) const;
	double	GetValue (void) const;
};


// --- LengthEdit ------------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT LengthEdit: public RealEdit
{
public:
	enum ChangeFontType {
		ChangeFont		= DG_ET_CHANGEFONT,
		NoChangeFont	= DG_ET_NOCHANGEFONT
	};

	LengthEdit (const Panel& panel, short item);
	LengthEdit (const Panel& panel, const Rect& rect, ChangeFontType chgFont = ChangeFont,
				FrameType frame = Frame, AbsRelType absRel = Absolute, UpdateType update = Update,
				ReadOnlyType readOnly = Editable);
   ~LengthEdit ();
};


// --- AreaEdit --------------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT AreaEdit : public RealEdit
{
public:
	AreaEdit (const Panel& panel, short item);
	AreaEdit (const Panel& panel, const Rect& rect, FrameType frame = Frame,
			  AbsRelType absRel = Absolute, UpdateType update = Update, ReadOnlyType readOnly = Editable);
	~AreaEdit ();
};


// --- VolumeEdit ------------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT VolumeEdit : public RealEdit
{
public:
	VolumeEdit (const Panel& panel, short item);
	VolumeEdit (const Panel& panel, const Rect& rect, FrameType frame = Frame,
				AbsRelType absRel = Absolute, UpdateType update = Update, ReadOnlyType readOnly = Editable);
	~VolumeEdit ();
};


// --- AngleEdit -------------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT AngleEdit: public RealEdit
{
public:
	AngleEdit (const Panel& panel, short item);
	AngleEdit (const Panel& panel, const Rect& rect, FrameType frame = Frame,
			   AbsRelType absRel = Absolute, UpdateType update = Update, ReadOnlyType readOnly = Editable);
   ~AngleEdit ();
};


// --- PolarAngleEdit --------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT PolarAngleEdit: public RealEdit
{
public:
	PolarAngleEdit (const Panel& panel, short item);
	PolarAngleEdit (const Panel& panel, const Rect& rect, FrameType frame = Frame,
					AbsRelType absRel = Absolute, UpdateType update = Update, ReadOnlyType readOnly = Editable);
   ~PolarAngleEdit ();
};


// --- MMPointEdit -----------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT MMPointEdit: public RealEdit
{
public:
	MMPointEdit (const Panel& panel, short item);
	MMPointEdit (const Panel& panel, const Rect& rect, FrameType frame = Frame,
				 AbsRelType absRel = Absolute, UpdateType update = Update, ReadOnlyType readOnly = Editable);
   ~MMPointEdit ();
};


// --- TextEditBase ----------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT TextEditBase: public EditControl,
								  public ItemTextProperty
{
protected:
	TextEditBase ();
	TextEditBase (const Panel& panel, short item);
	TextEditBase (const Panel& panel, short item, short subType);
	TextEditBase (const Panel& panel, const Rect& rect, short lengthLimit = 0,
				  FrameType frame = Frame, UpdateType update = Update, ReadOnlyType readOnly = Editable);

public:
   ~TextEditBase ();

	void	Attach (TextEditBaseObserver& observer);
	void	Detach (TextEditBaseObserver& observer);

	void		SetSelection (const CharRange& selRange);
	CharRange	GetSelection (void) const;
};


// --- TextEdit --------------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT TextEdit: public TextEditBase,
							  public ItemVariousProperty,
							  public ItemTextLengthLimitProperty
{
protected:
	TextEdit ();
	TextEdit (const Panel& panel, short item, short subType);

public:
	TextEdit (const Panel& panel, short item);
	TextEdit (const Panel& panel, const Rect& rect, short lengthLimit = 0,
			  FrameType frame = Frame, UpdateType update = Update, ReadOnlyType readOnly = Editable);
   ~TextEdit ();
};


// --- PasswordEdit ----------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT PasswordEdit: public TextEditBase,
								  public ItemTextLengthLimitProperty
{
public:
	PasswordEdit (const Panel& panel, short item);
	PasswordEdit (const Panel& panel, const Rect& rect, short lengthLimit = 0,
				  FrameType frame = Frame, UpdateType update = Update, ReadOnlyType readOnly = Editable);
   ~PasswordEdit ();
};


// --- ShortcutEdit ----------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT ShortcutEdit: public TextEditBase,
								  public ItemTextLengthLimitProperty
{
public:
	ShortcutEdit (const Panel& panel, short item);
	ShortcutEdit (const Panel& panel, const Rect& rect, short lengthLimit = 0,
				  FrameType frame = Frame, UpdateType update = Update);
   ~ShortcutEdit ();

	void	Attach (ShortcutEditObserver& observer);
	void	Detach (ShortcutEditObserver& observer);
};


// --- MultiLineEdit ---------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT MultiLineEdit: public TextEditBase
{
public:
	enum ScrollType {
		NoScroll	= DG_ET_NOSCROLL,
		HScroll		= DG_ET_HSCROLL,
		VScroll		= DG_ET_VSCROLL,
		HVScroll	= DG_ET_HSCROLL | DG_ET_VSCROLL
	};

	MultiLineEdit (const Panel& panel, short item);
	MultiLineEdit (const Panel& panel, const Rect& rect, ScrollType scroll = NoScroll,
				   FrameType frame = Frame, UpdateType update = Update, ReadOnlyType readOnly = Editable);
   ~MultiLineEdit ();

   GS::UniString	GetText (EOL::Type eolType = DG::EOL::Default) const;
};

}	// namespace DG

#endif
