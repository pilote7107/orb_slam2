#ifndef C3DSCENE_H
#define C3DSCENE_H

#include <vector>

#include "glut.h"		// Need to include it here because the GL* types are required
#include "C3DOBJECT.h"

namespace OGLCAM
{
	class C3DSCENE
	{
		typedef std::vector<C3DOBJECT> TOBJECTs;
		typedef TOBJECTs::const_iterator TObjCI;

		TOBJECTs m_Objects;

	public:
		C3DSCENE(void);
		~C3DSCENE(void);

		void InitializeRC (void);
		void load ();
		void draw ();
	};

} // Namespace
#endif // _3DSCENE_H
