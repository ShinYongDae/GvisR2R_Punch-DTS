// **************************************************************************
// safelock.h
//
// Description:
//	An auto releasing synchronization object using MFC.
// **************************************************************************

#ifndef _SAFELOCK_H
#define _SAFELOCK_H

// **************************************************************************
class CSafeLock
	{
	public:
		CSafeLock (CSyncObject *pso)
			{
			ASSERT (pso);
			pso->Lock ();
			m_pso = pso;
			}

		~CSafeLock ()
			{
			m_pso->Unlock ();
			}
	
	private:
		CSyncObject *m_pso;
	};

class CAutoBool
	{
	public:
		CAutoBool (BOOL *pBool)
			{
				ASSERT (pBool);
				*pBool=TRUE;
				m_pBool = pBool;
			}

		~CAutoBool ()
			{
				*m_pBool=FALSE;
			}
	
	private:
		BOOL *m_pBool;
	};

#endif	// _SAFELOCK_H
