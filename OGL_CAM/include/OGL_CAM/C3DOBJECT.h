#ifndef C3DOBJECT_H
#define C3DOBJECT_H

#include <vector>
#include "glut.h"		
#include "WORLDCOORD.h"

#define NB_MAX_MATERIAL 256

namespace OGLCAM
{
	struct sample_MATERIAL
	{
		 GLfloat ambient[3];
		 GLfloat diffuse[3];
		 GLfloat specular[3];
		 GLfloat emission[3];
		 GLfloat alpha;
		 GLfloat phExp;
		 int   texture;
	};

	class C3DOBJECT
	{
	public:
		typedef struct sample_MATERIAL TMATERIAL;
		typedef enum DrawStyle {WIRED = 0, SHADED, WIRESHADED} TDRAWSTYLE;

	protected :

		// Oblect Properties
		TDRAWSTYLE	m_DrawStyle;
		GLfloat		m_ScaleFactor [3];

		// Material management
		unsigned int m_nbMaterial;
		TMATERIAL m_Materials [NB_MAX_MATERIAL];
		void SetMaterialValues (GLenum mode,GLfloat *f,GLfloat alpha);
		void SelectMaterial(unsigned int i);

		// Face Management
		typedef struct FACEINDICIES
		{
			unsigned int Vert [3];
			unsigned int Norm [3];
			unsigned int Text [3];
		} TFACEINDICIES;

		typedef struct VERTEX
		{
			GLfloat x;
			GLfloat y; 
			GLfloat z;
		} TVERTEX;

		typedef struct NORMAL
		{
			GLfloat nx;
			GLfloat ny; 
			GLfloat nz;
		} TNORMAL;

		typedef struct MAPPING
		{
			GLfloat s;
			GLfloat t;
		} TMAPPING;

		typedef struct MATERIALREF
		{
			unsigned int matindex;
			unsigned int facecount;
		} TMATERIALREF;

		typedef std::vector<TFACEINDICIES> TFACEINDICIESs;
		typedef TFACEINDICIESs::const_iterator TFACEINDICIESsCI;
		TFACEINDICIESs m_FaceIndicies;

		typedef std::vector<TVERTEX> TVERTICEs;
		typedef TVERTICEs::const_iterator TVERTICEsCI;
		TVERTICEs m_Verticies;

		typedef std::vector<TNORMAL> TNORMALs;
		typedef TNORMALs::const_iterator TNORMALsCI;
		TNORMALs m_Normals;

		typedef std::vector<TMAPPING> TMAPPINGs;
		typedef TMAPPINGs::const_iterator TMAPPINGsCI;
		TMAPPINGs m_Mappings;

		typedef std::vector<TMATERIALREF> TMATERIALREFs;
		typedef TMATERIALREFs::const_iterator TMATERIALREFsCI;
		TMATERIALREFs m_MaterialRefs;

	public:

		CWORLDCOORD m_Position;

		C3DOBJECT(void);
		~C3DOBJECT(void);
		void addMaterials		(TMATERIAL* Mat, unsigned int numMat);
		void addFaceIndicies	(unsigned int* Indicies, unsigned int numFace);
		void addVerticies		(GLfloat* Verticies, unsigned int numVerticies);
		void addNormals			(GLfloat* Normals, unsigned int numNormals);
		void addMappings		(GLfloat* Mappings, unsigned int numMappings);
		void addMatIndicies		(int* matIndicies, unsigned int numIndicies);
		void setScale1F			(GLfloat factor) {m_ScaleFactor [0] = m_ScaleFactor [1] = m_ScaleFactor [2] = factor;}
		void setDrawStyle		(TDRAWSTYLE style) {m_DrawStyle = style;}
		void draw ();
	};

} // Namespace
#endif // _3DOBJECT_H
