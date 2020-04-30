#pragma once

#include "graphlet/primitive.hpp"
#include "paint.hpp"
#include "brushes.hxx"
namespace WarGrey::SCADA {
	private class SwingWidthlet : public WarGrey::SCADA::IRangelet<double> {
	public:
		SwingWidthlet(double range, float width, float height = 0.0F,
			unsigned int step = 0U, unsigned int precision = 2U,
			Microsoft::Graphics::Canvas::Brushes::ICanvasBrush^ color = nullptr,
			Microsoft::Graphics::Canvas::Brushes::ICanvasBrush^ liquid_color = nullptr,
			Microsoft::Graphics::Canvas::Brushes::ICanvasBrush^ target_color = nullptr,
			Microsoft::Graphics::Canvas::Brushes::ICanvasBrush^ border_color = nullptr,
			Microsoft::Graphics::Canvas::Brushes::CanvasSolidColorBrush^ hatch_color = nullptr);

	public:
		void construct() override;
		void fill_extent(float x, float y, float* w = nullptr, float* h = nullptr) override;
		void draw(Microsoft::Graphics::Canvas::CanvasDrawingSession^ ds, float x, float y, float Width, float Height) override;

	public:
		void set_liquid_height(double h, bool force_update = false);
		void set_target_height(double h, bool force_update = false);

	protected:
		void on_value_changed(double t) override;
		virtual void on_liquid_height_changed(double h);
		virtual void on_target_height_changed(double h);

	private:
		float get_outlet_height(double percentage);

	private:
		Microsoft::Graphics::Canvas::Geometry::CanvasCachedGeometry^ liquid;
		Microsoft::Graphics::Canvas::Geometry::CanvasCachedGeometry^ target;
		Microsoft::Graphics::Canvas::Geometry::CanvasGeometry^ hatchmark;
		Microsoft::Graphics::Canvas::Geometry::CanvasGeometry^ body;
		Microsoft::Graphics::Canvas::Brushes::ICanvasBrush^ color;
		Microsoft::Graphics::Canvas::Brushes::ICanvasBrush^ liquid_color;
		Microsoft::Graphics::Canvas::Brushes::ICanvasBrush^ target_color;
		Microsoft::Graphics::Canvas::Brushes::ICanvasBrush^ border_color;
		Microsoft::Graphics::Canvas::Brushes::ICanvasBrush^ hatch_color;

	private:
		Microsoft::Graphics::Canvas::Text::CanvasTextFormat^ target_font;
		Microsoft::Graphics::Canvas::Geometry::CanvasStrokeStyle^ target_style;

	private:
		double liquid_height;
		double target_height;

	private:
		float ofbbase;
		float ofubase;
		float em;

	private:
		float width;
		float height;
		float thickness;

	private:
		unsigned int step;
		unsigned int precision;
		bool leftward;
	};
}