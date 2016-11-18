//draw_order.h

#pragma once

//#include <boost/signals2.hpp>
#include <array>
#include <vector>
#include <functional>
#include <utility>

#ifndef GP_FALSE_VALUE
#define GP_FALSE_VALUE 0

namespace gp
{
	namespace Imple
	{
		class DrawOrder
		{
		public:
			static constexpr size_t MAX_GROUP_NUMBER = 30;
			using GrHandleType = int;
		private:
			using Order = void(void);
			using Group = std::vector<std::function<Order>>;
			using OrderList = std::array<Group, MAX_GROUP_NUMBER>;
			using GraphList = std::vector<GrHandleType>;

			DrawOrder(void);

			static OrderList& orders(void) { static OrderList o; return o; }
			static GraphList& graphs(void) { static GraphList o; return o; }

		public:
			//first: level, second: index
			using Reference = std::pair<unsigned int, unsigned int>;

			class Level
			{
			private:
				unsigned int level;
			public:
				Level(unsigned int _level);
				unsigned int value(void) const;
			};

			class ScopedGraph
			{
			private:
				GrHandleType value;
			public:
				ScopedGraph(int XSize, int YSize, bool UseAlphaChannel) noexcept;

				//Move only
				ScopedGraph(ScopedGraph&&) noexcept;
				ScopedGraph& operator=(ScopedGraph&&) noexcept;

				//Non-copyable
				ScopedGraph(const ScopedGraph&) = delete;
				ScopedGraph& operator=(const ScopedGraph&) = delete;

				int clear(void) noexcept;

				operator GrHandleType() const noexcept;

				~ScopedGraph(void) noexcept;
			};

			static Level level(unsigned int level);

			static void clear(void);
			static void draw(void);

			static Reference DrawCircle(int x, int y, int r, int Color, int FillFlag = 1, int LineThickness = 1);
			static Reference DrawCircle(Level level, int x, int y, int r, int color, int FillFlag = 1, int LineThickness = 1);
			static Reference DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = GP_FALSE_VALUE);
			static Reference DrawRotaGraphF(Level level, float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = GP_FALSE_VALUE);
			static Reference DrawRotaGraph(int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = GP_FALSE_VALUE);
			static Reference DrawRotaGraph(Level level, int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = GP_FALSE_VALUE);
			static Reference DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Grhandle, int TransFlag = GP_FALSE_VALUE);
			static Reference DrawModiGraph(Level level, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Grhandle, int TransFlag = GP_FALSE_VALUE);
			static Reference DrawLine(int x1, int y1, int x2, int y2, unsigned int Color, int Thickness = 1);
			static Reference DrawLine(Level level, int x1, int y1, int x2, int y2, unsigned int Color, int Thickness = 1);
			static Reference DrawGraph(int x, int y, int GrHandle, int TransFlag = GP_FALSE_VALUE);
			static Reference DrawGraph(Level level, int x, int y, int GrHandle, int TransFlag = GP_FALSE_VALUE);

			static Reference SetDrawBlendModeOf(Reference Drawer, int BlendMode, int Pal);
			static Reference SetDrawScreenOf(Reference Drawer, int GrHandle);


			static GrHandleType MakeScreen(int XSize, int YSize, int UseAlphaChannel = 0);
			static GrHandleType DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHendle);

			friend Reference operator>>(Reference Bottom, Reference Top);
		};

		DrawOrder::Reference operator>>(DrawOrder::Reference Bottom, DrawOrder::Reference Top);
	}

	void SetDefaultScreen(int _screen);

	using Level = Imple::DrawOrder::Level;
	Level level(unsigned int level);

	using Reference = Imple::DrawOrder::Reference;
	using GrHandle = Imple::DrawOrder::GrHandleType;
	using ScopedGraph = Imple::DrawOrder::ScopedGraph;

	void ApplyDrawer(void);
	void ClearDrawer(void);

	Reference DrawCircle(int x, int y, int r, int Color, int FillFlag = 1, int LineThickness = 1);
	Reference DrawCircle(Level level, int x, int y, int r, int color, int FillFlag = 1, int LineThickness = 1);
	Reference DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = GP_FALSE_VALUE);
	Reference DrawRotaGraphF(Level level, float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = GP_FALSE_VALUE);
	Reference DrawRotaGraph(int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = GP_FALSE_VALUE);
	Reference DrawRotaGraph(Level level, int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = GP_FALSE_VALUE);
	Reference DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Grhandle, int TransFlag = GP_FALSE_VALUE);
	Reference DrawModiGraph(Level level, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Grhandle, int TransFlag = GP_FALSE_VALUE);
	Reference DrawLine(int x1, int y1, int x2, int y2, unsigned int Color, int Thickness = 1);
	Reference DrawLine(Level level, int x1, int y1, int x2, int y2, unsigned int Color, int Thickness = 1);
	Reference DrawGraph(int x, int y, int GrHandle, int TransFlag = GP_FALSE_VALUE);
	Reference DrawGraph(Level level, int x, int y, int GrHandle, int TransFlag = GP_FALSE_VALUE);

	Reference operator>>(Reference Bottom, Reference Top);

	Reference SetDrawBlendModeOf(Reference Drawer, int BlendMode, int Pal);
	Reference SetDrawScreenOf(Reference Drawer, int GrHandle);

	GrHandle MakeScreen(int XSize, int YSize, int UseAlphaChannel = 0);
	GrHandle DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHendle);
}

#undef GP_FALSE_VALUE

#endif