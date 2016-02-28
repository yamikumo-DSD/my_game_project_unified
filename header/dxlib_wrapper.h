//dxlib_wrapper.h

#ifndef __DXLIB_WRAPPER_H
#define __DXLIB_WRAPPER_H

#include <dxlib.h>
#include "drawblendmode.h"

namespace MyGameProject
{
	using gp::ScopedDrawBlendMode;

	namespace Origin
	{
		inline int DrawCircleWrapper(int x, int y, int r, int color,int FillFlag = 1,int LineThickness = 1)
		{
			return DrawCircle(x,y,r,color,FillFlag,LineThickness);
		}
		inline int DrawRotaGraphFWrapper(float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE)
		{
			return DrawRotaGraphF(xf, yf, ExRate, Angle, GrHandle, TransFlag, TurnFlag);
		}
		inline int DrawRotaGraphWrapper(int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE)
		{
			return DrawRotaGraph(x, y, ExRate, Angle, GrHandle, TransFlag, TurnFlag);
		}
		inline int DrawModiGraphWrapper(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4,int Grhandle,int TransFlag = FALSE)
		{
			return DrawModiGraph(x1,y1,x2,y2,x3,y3,x4,y4,Grhandle,TransFlag);
		}
		inline int DrawLineWrapper(int x1, int y1, int x2, int y2, unsigned int Color, int Thickness = 1 )
		{
			return DrawLine(x1, y1, x2, y2, Color, Thickness);
		}
	}
	namespace FunctionWrapper
	{
		template<class CoordType,class RadiusType,class ColorType,class ThicknessType = int>
		inline auto DrawCircle(CoordType x, CoordType y, RadiusType r,ColorType color,bool FillFlag = true,ThicknessType LineThickness = 1)->decltype(Origin::DrawCircleWrapper(int(),int(),int(),int(),int(),int()))
		{
			return Origin::DrawCircleWrapper(static_cast<int>(x),static_cast<int>(y),static_cast<int>(r),static_cast<int>(color),static_cast<int>(FillFlag),static_cast<int>(LineThickness));
		}

		template<class CoordType,class ExRateType,class AngleType>
		inline auto DrawRotaGraph(CoordType xf, CoordType yf, ExRateType ExRate, AngleType Angle, int GrHandle, bool TransFlag, bool TurnFlag = FALSE)->decltype(Origin::DrawRotaGraphFWrapper(float(),float(),double(),double(),int(),int(),int()))
		{
			return Origin::DrawRotaGraphFWrapper(static_cast<float>(xf),static_cast<float>(yf),static_cast<double>(ExRate),static_cast<double>(Angle),GrHandle,static_cast<int>(TransFlag),static_cast<int>(TurnFlag));
		}

		template<class ExRateType, class AngleType>
		inline auto DrawRotaGraph(int x, int y, ExRateType ExRate, AngleType Angle, int GrHandle, bool TransFlag, bool TurnFlag = FALSE)->decltype(Origin::DrawRotaGraphFWrapper(float(), float(), double(), double(), int(), int(), int()))
		{
			return Origin::DrawRotaGraphWrapper(x,y, static_cast<double>(ExRate), static_cast<double>(Angle), GrHandle, static_cast<int>(TransFlag), static_cast<int>(TurnFlag));
		}

		template<class CoordType>
		inline auto DrawModiGraph(CoordType x1, CoordType y1, CoordType x2, CoordType y2, CoordType x3, CoordType y3, CoordType x4, CoordType y4, int Grhandle, bool TransFlag = FALSE)->decltype(Origin::DrawModiGraphWrapper(int(),int(),int(),int(),int(),int(),int(),int(),int(),int()))
		{
			return Origin::DrawModiGraphWrapper(static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2), static_cast<int>(x3), static_cast<int>(y3), static_cast<int>(x4), static_cast<int>(y4),Grhandle,static_cast<int>(TransFlag));
		}

		template<class T1, class T2,class T3,class T4,class T5,class T6 = int>
		inline auto DrawLine(T1 x1, T2 y1, T3 x2, T4 y2, T5 Color, T6 Thickness = 1)
		{
			return Origin::DrawLineWrapper(static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2), static_cast<unsigned int>(Color), static_cast<int>(Thickness));
		}
	}
}

#endif
