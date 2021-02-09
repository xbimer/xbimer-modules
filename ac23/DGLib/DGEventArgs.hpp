
#ifndef DGEVENTARGS_HPP
#define DGEVENTARGS_HPP

#include "DG.h"
#include "DGDefs.h"

class DG_DLL_EXPORT DGEventArg {
public:
	DGEventArg ();
	virtual ~DGEventArg ();
};

/*class DGEventArg_Old : DGEventArg {
private:
	DGMessageData msgData;

public:
	DGEventArg_Old (DGMessageData msgData);
	virtual DGEventArg_Old ();
};*/



#endif // DGEVENTARGS_HPP
