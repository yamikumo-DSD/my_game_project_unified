//draw_order.cpp

#include "draw_order.h"
#include "dxlib.h"

namespace gp
{
	namespace Imple
	{
		static int default_screen{static_cast<int>(DX_SCREEN_BACK)};

		DrawOrder::Level::Level(unsigned int _level) :level(_level) {}
		unsigned int DrawOrder::Level::value(void) const { return level; }
		DrawOrder::Level DrawOrder::level(unsigned int _level) { return Level(_level); }

		void DrawOrder::clear(void) { for (auto& slot : orders()) { slot.resize(0); slot.reserve(10000); } }
		void DrawOrder::draw(void)
		{
			for (const auto& group : orders())
			{
				for (const auto& member : group)
				{
					member();
				}
			}
			for (const auto& graph : graphs())
			{
				DxLib::DeleteGraph(graph);
			}
			graphs().resize(0);
		}

		DrawOrder::Reference DrawOrder::DrawCircle(int x, int y, int r, int color, int FillFlag, int LineThickness)
		{
			orders()[0].emplace_back([=](void) {::DrawCircle(x, y, r, color, FillFlag, LineThickness); });
			return std::make_pair(0, orders()[0].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawCircle(Level level, int x, int y, int r, int color, int FillFlag, int LineThickness)
		{
			orders()[level.value()].emplace_back([=](void) {::DrawCircle(x, y, r, color, FillFlag, LineThickness); });
			return std::make_pair(level.value(), orders()[level.value()].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag)
		{
			orders()[0].emplace_back([=](void) {::DrawRotaGraphF(xf, yf, ExRate, Angle, GrHandle, TransFlag, TurnFlag); });
			return std::make_pair(0, orders()[0].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawRotaGraphF(Level level, float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag)
		{
			orders()[level.value()].emplace_back([=](void) {::DrawRotaGraphF(xf, yf, ExRate, Angle, GrHandle, TransFlag, TurnFlag); });
			return std::make_pair(level.value(), orders()[level.value()].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawRotaGraph(int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag)
		{
			orders()[0].emplace_back([=](void) {::DrawRotaGraph(x, y, ExRate, Angle, GrHandle, TransFlag, TurnFlag); });
			return std::make_pair(0, orders()[0].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawRotaGraph(Level level, int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag)
		{
			orders()[level.value()].emplace_back([=](void) {::DrawRotaGraph(x, y, ExRate, Angle, GrHandle, TransFlag, TurnFlag); });
			return std::make_pair(level.value(), orders()[level.value()].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Grhandle, int TransFlag)
		{
			orders()[0].emplace_back([=](void) {::DrawModiGraph(x1, y1, x2, y2, x3, y3, x4, y4, Grhandle, TransFlag); });
			return std::make_pair(0, orders()[0].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawModiGraph(Level level, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Grhandle, int TransFlag)
		{
			orders()[level.value()].emplace_back([=](void) {::DrawModiGraph(x1, y1, x2, y2, x3, y3, x4, y4, Grhandle, TransFlag); });
			return std::make_pair(level.value(), orders()[level.value()].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawLine(int x1, int y1, int x2, int y2, unsigned int Color, int Thickness)
		{
			orders()[0].emplace_back([=](void) {::DrawLine(x1, y1, x2, y2, Color, Thickness); });
			return std::make_pair(0, orders()[0].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawLine(Level level, int x1, int y1, int x2, int y2, unsigned int Color, int Thickness)
		{
			orders()[level.value()].emplace_back([=](void) {::DrawLine(x1, y1, x2, y2, Color, Thickness); });
			return std::make_pair(level.value(), orders()[level.value()].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawGraph(int x, int y, int GrHandle, int TransFlag)
		{
			orders()[0].emplace_back([=](void) {::DrawGraph(x, y, GrHandle, TransFlag); });
			return std::make_pair(0, orders()[0].size() - 1);
		}
		DrawOrder::Reference DrawOrder::DrawGraph(Level level, int x, int y, int GrHandle, int TransFlag)
		{
			orders()[level.value()].emplace_back([=](void) {::DrawGraph(x, y, GrHandle, TransFlag); });
			return std::make_pair(level.value(), orders()[level.value()].size() - 1);
		}

		DrawOrder::Reference operator>>(DrawOrder::Reference Bottom, DrawOrder::Reference Top)
		{
			auto top = std::move(DrawOrder::orders()[Top.first][Top.second]);
			auto bottom = std::move(DrawOrder::orders()[Bottom.first][Bottom.second]);
			DrawOrder::orders()[Top.first][Top.second] = [bottom = std::move(bottom), top = std::move(top)](void)
			{
				bottom();
				top();
			};
			DrawOrder::orders()[Bottom.first][Bottom.second] = [](void) {}; //Bottom -> do notiong.
			return Top;
		}

		DrawOrder::Reference DrawOrder::SetDrawBlendModeOf(Reference Drawer, int BlendMode, int Pal)
		{
			auto temp = std::move(orders()[Drawer.first][Drawer.second]);
			orders()[Drawer.first][Drawer.second] = [BlendMode, Pal, temp = std::move(temp)](void) 
			{
				::SetDrawBlendMode(BlendMode, Pal);
				temp();
				::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			};
			return Drawer;
		}

		DrawOrder::Reference DrawOrder::SetDrawScreenOf(Reference Drawer, int GrHandle)
		{
			auto temp = std::move(orders()[Drawer.first][Drawer.second]);
			orders()[Drawer.first][Drawer.second] = [GrHandle, temp = std::move(temp)](void) 
			{
				::SetDrawScreen(GrHandle);
				temp();
				::SetDrawScreen(default_screen);
			};
			return Drawer;
		}

		DrawOrder::GrHandleType DrawOrder::MakeScreen(int XSize, int YSize, int UseAlphaChannel)
		{
			const auto gr = DxLib::MakeScreen(XSize, YSize, UseAlphaChannel);
			graphs().emplace_back(gr);
			return gr;
		}
		DrawOrder::GrHandleType DrawOrder::DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHendle)
		{
			const auto gr = DxLib::DerivationGraph( SrcX,  SrcY,  Width,  Height,  SrcGraphHendle);
			graphs().emplace_back(gr);
			return gr;
		}

		DrawOrder::ScopedGraph::ScopedGraph(int XSize, int YSize, bool UseAlphaChannel) noexcept
			: value(DxLib::MakeScreen(XSize, YSize, static_cast<int>(UseAlphaChannel)))
		{}

		DrawOrder::ScopedGraph::ScopedGraph(ScopedGraph&& _graph) noexcept
			: value(_graph.value)
		{
			_graph.value = 0;
		}

		DrawOrder::ScopedGraph& DrawOrder::ScopedGraph::operator=(ScopedGraph&& _rhs) noexcept
		{
			value = _rhs.value;
			_rhs.value = 0;
		}

		int DrawOrder::ScopedGraph::clear(void) noexcept
		{
			int result{0};
			result += DxLib::SetDrawScreen(value);
			result += DxLib::ClearDrawScreen();
			result += DxLib::SetDrawScreen(default_screen);
			return result < 0 ? -1 : 0;
		}

		DrawOrder::ScopedGraph::operator DrawOrder::GrHandleType() const noexcept
		{
			return value;
		}

		DrawOrder::ScopedGraph::~ScopedGraph(void) noexcept { DxLib::DeleteGraph(value); }
	}

	void SetDefaultScreen(int _screen)
	{
		Imple::default_screen = _screen;
	}

	Level level(unsigned int level) { return Imple::DrawOrder::level(level); }

	void ApplyDrawer(void) { Imple::DrawOrder::draw(); }
	void ClearDrawer(void) { Imple::DrawOrder::clear(); }

	Reference DrawCircle(int x, int y, int r, int Color, int FillFlag, int LineThickness)
	{
		return Imple::DrawOrder::DrawCircle( x,  y,  r,  Color,  FillFlag,  LineThickness);
	}
	Reference DrawCircle(Level level, int x, int y, int r, int color, int FillFlag, int LineThickness)
	{
		return Imple::DrawOrder::DrawCircle( level,  x,  y,  r,  color,  FillFlag,  LineThickness);
	}
	Reference DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag)
	{
		return Imple::DrawOrder::DrawRotaGraphF( xf,  yf,  ExRate,  Angle,  GrHandle,  TransFlag,  TurnFlag);
	}
	Reference DrawRotaGraphF(Level level, float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag)
	{
		return Imple::DrawOrder::DrawRotaGraphF(level,  xf,  yf,  ExRate,  Angle,  GrHandle,  TransFlag,  TurnFlag);
	}
	Reference DrawRotaGraph(int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag)
	{
		return Imple::DrawOrder::DrawRotaGraph( x,  y,  ExRate,  Angle,  GrHandle,  TransFlag,  TurnFlag);
	}
	Reference DrawRotaGraph(Level level, int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag)
	{
		return Imple::DrawOrder::DrawRotaGraph( level,  x,  y,  ExRate,  Angle,  GrHandle,  TransFlag,  TurnFlag);
	}
	Reference DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Grhandle, int TransFlag)
	{
		return Imple::DrawOrder::DrawModiGraph( x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4,  Grhandle,  TransFlag);
	}
	Reference DrawModiGraph(Level level, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Grhandle, int TransFlag)
	{
		return Imple::DrawOrder::DrawModiGraph( level,  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4,  Grhandle,  TransFlag);
	}
	Reference DrawLine(int x1, int y1, int x2, int y2, unsigned int Color, int Thickness)
	{
		return Imple::DrawOrder::DrawLine( x1,  y1,  x2,  y2,  Color,  Thickness);
	}
	Reference DrawLine(Level level, int x1, int y1, int x2, int y2, unsigned int Color, int Thickness)
	{
		return Imple::DrawOrder::DrawLine(level,  x1,  y1,  x2,  y2,  Color,  Thickness);
	}
	Reference DrawGraph(int x, int y, int GrHandle, int TransFlag)
	{
		return Imple::DrawOrder::DrawGraph(x, y, GrHandle, TransFlag);
	}
	Reference DrawGraph(Level level, int x, int y, int GrHandle, int TransFlag)
	{
		return Imple::DrawOrder::DrawGraph(level, x, y, GrHandle, TransFlag);
	}

	Reference SetDrawBlendModeOf(Reference drawer, int BlendMode, int Pal)
	{
		return Imple::DrawOrder::SetDrawBlendModeOf(drawer, BlendMode, Pal);
	}
	Reference SetDrawScreenOf(Reference Drawer, int GrHandle)
	{
		return Imple::DrawOrder::SetDrawScreenOf(Drawer, GrHandle);
	}
	Reference operator>>(Reference Bottom, Reference Top)
	{
		return Imple::operator>>(Bottom, Top);
	}
	GrHandle MakeScreen(int XSize, int YSize, int UseAlphaChannel)
	{
		return Imple::DrawOrder::MakeScreen(XSize, YSize, UseAlphaChannel);
	}
	GrHandle DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHendle)
	{
		return Imple::DrawOrder::DerivationGraph( SrcX,  SrcY,  Width,  Height,  SrcGraphHendle);
	}
}