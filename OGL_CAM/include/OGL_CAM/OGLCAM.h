#ifndef OGLCAM_H
#define OGLCAM_H

#include "3DVector.h"
#include "C3DSCENE.h"
#include "WORLDCOORD.h"
#include "C2DPRIMITIVES.h"
#include "C3DPRIMITIVES.h"
#include "definitions.h"

namespace OGLCAM
{
	class COGLCAM;

	// Tous les angles en degrés
	// Coordonnées dans le monde réel (en mètres)
	// X vers l'est, Y vers le Nord, Z vers le Haut
	extern GLfloat HFov;
	extern GLfloat NCP;
	extern GLfloat FCP;
	extern GLfloat Heading; // Rotation selon l'axe Z Sens trigo : négatif vers la droite positif vers la gauche
	extern GLfloat Pitch;   // Rotation selon l'axe X Sens trigo : négatif à piquer positif à cabrer
	extern GLfloat Roll;    // Rotation selon l'axe Y Sens trigo : négatif vers la gauche positif vers la droite
	extern GLfloat PosX;
	extern GLfloat PosY;
	extern GLfloat PosZ;

	class COGLCAM
	{
	private:
		
		SF3dVector m_ViewDir;
		SF3dVector m_RightVector;	
		SF3dVector m_UpVector;

		CWORLDCOORD m_Position;		
		int m_Width, m_Height;
		GLfloat m_HFov, m_NCP, m_FCP;
		
		C2DPRIMITIVES m_2DPrimitives;
		C3DPRIMITIVES m_3DPrimitives;
		C3DSCENE m_scene;

		void SetPos (SF3dVector pos);
		void SetPos (double x, double y, double z);
		void RotatePitch (GLfloat Angle);
		void RotateYaw (GLfloat Angle);
		void RotateRoll (GLfloat Angle);
		void PreDraw ();
		void Draw ();
		void PostDraw ();
		void ResetAngles ();

	public:
		COGLCAM(void);
		~COGLCAM(void);
		void Update (int Width, int Height, GLfloat HFov, GLfloat NCP, GLfloat FCP);
		void Update (GLfloat x, GLfloat y, GLfloat z, GLfloat h, GLfloat p, GLfloat r, GLfloat HFov);
		void Render () {PreDraw (); Draw (); PostDraw ();}
		
	};

} // Namespace
#endif // OGLCAM_H

