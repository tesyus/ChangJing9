﻿#include <map>

#include "page/hopper_doors.hpp"
#include "configuration.hpp"
#include "menu.hpp"

#include "graphlet/symbol/door/hopper_doorlet.hpp"
#include "graphlet/dashboard/alarmlet.hpp"
#include "graphlet/dashboard/cylinderlet.hpp"
#include "graphlet/ui/statuslet.hpp"

#include "iotables/ai_metrics.hpp"
#include "iotables/ai_doors.hpp"
#include "iotables/di_doors.hpp"
#include "iotables/ai_pumps.hpp"
#include "iotables/di_pumps.hpp"
#include "iotables/do_doors.hpp"

#include "decorator/ship.hpp"

#include "module.hpp"
#include "shape.hpp"
#include "geometry.hpp"
#include "transformation.hpp"
#include <iotables\di_devices.hpp>
#include <iotables\do_devices.hpp>

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Numerics;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::Brushes;
using namespace Microsoft::Graphics::Canvas::Geometry;

// WARNING: order matters
private enum class HD : unsigned int {
	//labels
	Port, Starboard,
	
	// Cylinders
	EarthWork, Capacity, HopperHeight, Payload, Displacement,
	
	// Hydraulic pump dimensions, see hydraulics.cpp
	C1, C2, D1, D2,// H,
	OpenDegree,

	// Other Dimensions
	Heel, Trim, BowDraft, SternDraft,

	_
};

private class Doors final : public PLCConfirmation {
public:
	Doors(HopperDoorsPage* master, BottomDoorDecorator* ship) : master(master), decorator(ship) {
		this->label_font = make_bold_text_format(large_font_size);
		this->metrics_style = make_plain_dimension_style(small_metrics_font_size, normal_font_size);
		this->plain_style = make_plain_dimension_style(small_metrics_font_size, 5U, 2);
		this->pump_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::Background);
		this->highlight_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::Green);
		this->percentage_style.precision = 1;
		this->percentage_style.number_font = make_bold_text_format("Cambria Math", large_metrics_font_size);
		this->setting_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::GhostWhite, Colours::RoyalBlue);
		this->button_style.font = make_bold_text_format("Consolas", small_font_size);
		this->button_style.corner_radius = 2.0F;
		this->button_style.thickness = 2.0F;
	}

public:
	void pre_read_data(Syslog* logger) override {
		this->master->enter_critical_section();
		this->master->begin_update_sequence();
	}

	void on_analog_input(long long timepoint_ms, const uint8* DB2, size_t count2, const uint8* DB203, size_t count203, Syslog* logger) override {
		this->set_cylinder(HD::HopperHeight, DBD(DB2, average_hopper_height));
		this->set_cylinder(HD::Displacement, DBD(DB2, displacement_value));
		this->set_cylinder(HD::Payload, DBD(DB2, payload_value));
		this->set_cylinder(HD::EarthWork, DBD(DB2, earthwork_value));
		this->set_cylinder(HD::Capacity, DBD(DB2, vessel_value));

		this->dimensions[HD::BowDraft]->set_value(DBD(DB2, fixed_bow_draught));
		this->dimensions[HD::SternDraft]->set_value(DBD(DB2, fixed_stern_draught));
		this->dimensions[HD::Trim]->set_value(DBD(DB2, trim_degrees));
		this->dimensions[HD::Heel]->set_value(DBD(DB2, heel_degrees));

		{ // door progresses
			this->set_door_progress(Door::PS1, RealData(DB203, bottom_door_PS1_progress));
			this->set_door_progress(Door::PS2, RealData(DB203, bottom_door_PS2_progress));
			this->set_door_progress(Door::PS3, RealData(DB203, bottom_door_PS3_progress));
			this->set_door_progress(Door::PS4, RealData(DB203, bottom_door_PS4_progress));
			this->set_door_progress(Door::PS5, RealData(DB203, bottom_door_PS5_progress));
			this->set_door_progress(Door::PS6, RealData(DB203, bottom_door_PS6_progress));
			this->set_door_progress(Door::PS7, RealData(DB203, bottom_door_PS7_progress));

			this->set_door_progress(Door::SB1, RealData(DB203, bottom_door_SB1_progress));
			this->set_door_progress(Door::SB2, RealData(DB203, bottom_door_SB2_progress));
			this->set_door_progress(Door::SB3, RealData(DB203, bottom_door_SB3_progress));
			this->set_door_progress(Door::SB4, RealData(DB203, bottom_door_SB4_progress));
			this->set_door_progress(Door::SB5, RealData(DB203, bottom_door_SB5_progress));
			this->set_door_progress(Door::SB6, RealData(DB203, bottom_door_SB6_progress));
			this->set_door_progress(Door::SB7, RealData(DB203, bottom_door_SB7_progress));
		}

		{ // pump pressures
			this->dimensions[HD::C1]->set_value(RealData(DB203, pump_C1_pressure), GraphletAnchor::LB);
			this->dimensions[HD::C2]->set_value(RealData(DB203, pump_C2_pressure), GraphletAnchor::LT);

			this->dimensions[HD::D1]->set_value(RealData(DB203, pump_D1_pressure), GraphletAnchor::CC);
			this->dimensions[HD::D2]->set_value(RealData(DB203, pump_D2_pressure), GraphletAnchor::CC);
		}
	}

	void on_digital_input(long long timepoint_ms, const uint8* DB4, size_t count4, const uint8* DB205, size_t count205, Syslog* logger) override {
		DI_hydraulic_pump_dimension(this->dimensions[HD::C1], DB4, pump_C1_feedback);
		DI_hydraulic_pump_dimension(this->dimensions[HD::C2], DB4, pump_C2_feedback);
		DI_hydraulic_pump_dimension(this->dimensions[HD::D1], DB4, pump_D1_feedback);
		DI_hydraulic_pump_dimension(this->dimensions[HD::D2], DB4, pump_D2_feedback);
		//DI_hydraulic_pump_dimension(this->dimensions[HD::H], DB4, pump_H_feedback);

		DI_hopper_door(this->hdoors[Door::PS1], DB4, bottom_door_PS1_closed, DB205, bottom_door_PS1_status);
		DI_hopper_door(this->hdoors[Door::PS2], DB4, bottom_door_PS2_closed, DB205, bottom_door_PS2_status);
		DI_hopper_door(this->hdoors[Door::PS3], DB4, bottom_door_PS3_closed, DB205, bottom_door_PS3_status);
		DI_hopper_door(this->hdoors[Door::PS4], DB4, bottom_door_PS4_closed, DB205, bottom_door_PS4_status);
		DI_hopper_door(this->hdoors[Door::PS5], DB4, bottom_door_PS5_closed, DB205, bottom_door_PS5_status);
		DI_hopper_door(this->hdoors[Door::PS6], DB4, bottom_door_PS6_closed, DB205, bottom_door_PS6_status);
		DI_hopper_door(this->hdoors[Door::PS7], DB4, bottom_door_PS7_closed, DB205, bottom_door_PS7_status);

		DI_hopper_door(this->hdoors[Door::SB1], DB4, bottom_door_SB1_closed, DB205, bottom_door_SB1_status);
		DI_hopper_door(this->hdoors[Door::SB2], DB4, bottom_door_SB2_closed, DB205, bottom_door_SB2_status);
		DI_hopper_door(this->hdoors[Door::SB3], DB4, bottom_door_SB3_closed, DB205, bottom_door_SB3_status);
		DI_hopper_door(this->hdoors[Door::SB4], DB4, bottom_door_SB4_closed, DB205, bottom_door_SB4_status);
		DI_hopper_door(this->hdoors[Door::SB5], DB4, bottom_door_SB5_closed, DB205, bottom_door_SB5_status);
		DI_hopper_door(this->hdoors[Door::SB6], DB4, bottom_door_SB6_closed, DB205, bottom_door_SB6_status);
		DI_hopper_door(this->hdoors[Door::SB7], DB4, bottom_door_SB7_closed, DB205, bottom_door_SB7_status);

		DI_hopper_doors_auto_lock(this->lockers[BottomDoorCommand::AutoLock], DB205);
		DI_hopper_doors_locked(this->lockers[BottomDoorCommand::Locked], DB205);
		DI_opendegree(this->button[HD::OpenDegree],DB205, open_degree);
	}

	void on_forat(long long timepoint_ms, const uint8* DB20, size_t count, Syslog* logger) override {
		this->door_opendegree[HD::OpenDegree]->set_value(DBD(DB20, 218U), GraphletAnchor::LB);//左舱内
	}
	void post_read_data(Syslog* logger) override {
		this->master->end_update_sequence();
		this->master->leave_critical_section();
	}
public:

	bool on_key(VirtualKey key, PLCMaster* plc) {
		bool handled = false;

		if (key == VirtualKey::Enter) {
			auto editor = dynamic_cast<Credit<Dimensionlet, HD>*>(this->master->get_focus_graphlet());

			if (editor != nullptr) {
				float rpm = float(editor->get_input_number());
				if (rpm >= 0.0F) {
					plc->send_setting(218U, rpm);
				}

				this->master->hide_virtual_keyboard();
				this->master->set_caret_owner(nullptr);

				handled = true;
			}
		}

		return handled;
	}
public:
	void load(float width, float height, float vinset) {
		float cell_width, cell_height, radius, cylinder_height;
		
		this->decorator->fill_door_cell_extent(nullptr, nullptr, &cell_width, &cell_height, 1, 0.0F);
		
		radius = std::fminf(cell_width, cell_height) * 0.75F * 0.5F;
		this->load_doors(this->hdoors, this->progresses, this->doors, Door::PS1, Door::PS7, radius);
		this->load_doors(this->hdoors, this->progresses, this->doors, Door::SB1, Door::SB7, radius);

		cylinder_height = cell_height * 1.618F;
		this->load_cylinder(this->cylinders, HD::EarthWork, cylinder_height, earthwork_range, 0U, "meter3");
		this->load_cylinder(this->cylinders, HD::Capacity, cylinder_height, vessel_range, 0U, "meter3");
		this->load_cylinder(this->cylinders, HD::HopperHeight, cylinder_height, hopper_height_range, 2U, "meter");
		this->load_cylinder(this->cylinders, HD::Payload, cylinder_height, payload_range, 0U, "ton");
		this->load_cylinder(this->cylinders, HD::Displacement, cylinder_height, displacement_range, 0U, "ton");

		this->load_dimensions(this->dimensions, HD::C1, HD::D2, "bar");
		this->load_dimensions(this->dimensions, HD::Heel, HD::Trim, "degrees", this->plain_style);
		this->load_dimensions(this->dimensions, HD::BowDraft, HD::SternDraft, "meter", this->plain_style);

		this->load_alarms(this->lockers, BottomDoorCommand::AutoLock, BottomDoorCommand::Locked, vinset * 2.0F);
		//泥门开度设置
		this->load_settings(this->door_opendegree, HD::OpenDegree, "percent", "S");
		this->load_button(this->button, HD::OpenDegree);
		
		{ // load captions
			CanvasTextFormat^ cpt_font = make_bold_text_format("Microsoft YaHei", large_font_size);

			this->load_label(this->labels, HD::Port, Colours::make(default_ps_color), cpt_font);
			this->load_label(this->labels, HD::Starboard, Colours::make(default_sb_color), cpt_font);
		}
	}

	void reflow(float width, float height, float vinset) {
		this->reflow_doors(this->hdoors, this->progresses, this->doors, Door::PS1, Door::PS7, 1.0F, -0.5F);
		this->reflow_doors(this->hdoors, this->progresses, this->doors, Door::SB1, Door::SB7, 3.0F, 0.5F);

		this->reflow_cylinders(this->cylinders, this->dimensions, this->labels, HD::EarthWork, HD::Displacement);

		{ // reflow dimensions
			float x, y, off;

			this->labels[HD::Port]->fill_extent(0.0F, 0.0F, nullptr, &off);
			off *= 0.618F;

			this->decorator->fill_ascent_anchor(0.5F, 1.0F, &x, &y);
			this->master->move_to(this->labels[HD::Port], x, y, GraphletAnchor::CB, 0.0F, -off);
			
			this->decorator->fill_descent_anchor(0.5F, 0.0F, &x, &y);
			this->master->move_to(this->labels[HD::Starboard], x, y, GraphletAnchor::CT, 0.0F, +off);

			this->decorator->fill_ship_anchor(1.0F, 0.5F, &x, &y, false);
			this->master->move_to(this->dimensions[HD::Heel], x, y, GraphletAnchor::LB, 0.0F, -off);
			this->master->move_to(this->dimensions[HD::C2], this->dimensions[HD::Heel], GraphletAnchor::LT, GraphletAnchor::LB, 0.0F, -off);
			this->master->move_to(this->dimensions[HD::C1], this->dimensions[HD::C2], GraphletAnchor::LT, GraphletAnchor::LB, 0.0F, -off);
			this->master->move_to(this->dimensions[HD::Trim], x, y, GraphletAnchor::LT, 0.0F, +off);
			this->master->move_to(this->dimensions[HD::D1], this->dimensions[HD::Trim], GraphletAnchor::LB, GraphletAnchor::LT, 0.0F, +off);
			this->master->move_to(this->dimensions[HD::D2], this->dimensions[HD::D1], GraphletAnchor::LB, GraphletAnchor::LT, 0.0F, +off);
			//this->master->move_to(this->dimensions[HD::H], this->dimensions[HD::E], GraphletAnchor::LB, GraphletAnchor::LT, 0.0F, +off);

			this->decorator->fill_ship_anchor(1.0F, 0.5F, &x, &y, true);
			this->master->move_to(this->dimensions[HD::BowDraft], x, y, GraphletAnchor::RC, -off);

			this->decorator->fill_ship_anchor(0.0F, 0.5F, &x, &y, true);
			this->master->move_to(this->dimensions[HD::SternDraft], x, y, GraphletAnchor::RC, -off);
		}

		{ // reflow alarms
			float alarm_x, alarm_y;

			this->decorator->fill_ascent_anchor(1.0F, 1.0F, &alarm_x, nullptr);
			this->master->fill_graphlet_location(cylinders[HD::Displacement], nullptr, &alarm_y, GraphletAnchor::CC);

			this->master->move_to(this->lockers[BottomDoorCommand::AutoLock], alarm_x, alarm_y, GraphletAnchor::LB, 0.0F, -vinset);
			this->master->move_to(this->lockers[BottomDoorCommand::Locked], alarm_x, alarm_y, GraphletAnchor::LT, 0.0F, +vinset);
			
			for (BottomDoorCommand cmd = BottomDoorCommand::AutoLock; cmd <= BottomDoorCommand::Locked; cmd++) {
				this->master->move_to(this->locker_labels[cmd], this->lockers[cmd], GraphletAnchor::RC, GraphletAnchor::LC, vinset);
				this->master->move_to(this->locker_labels[cmd], this->lockers[cmd], GraphletAnchor::RC, GraphletAnchor::LC, vinset);
			}
		}
		{// 泥门开度
			this->master->move_to(this->door_opendegree[HD::OpenDegree], this->doors[Door::SB3], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, vinset * 3);
			this->master->move_to(this->button[HD::OpenDegree], this->door_opendegree[HD::OpenDegree], GraphletAnchor::LC, GraphletAnchor::RC);
		}
	}

public:
	bool doors_selected(Door ids[], unsigned int count, int tolerance) {
		bool okay = false;
		int ok = 0;

		for (unsigned int idx = 0; idx < count; idx++) {
			if (this->master->is_selected(this->hdoors[ids[idx]])) {
				ok += 1;

				if (ok >= tolerance) {
					okay = true;
					break;
				}
			}
		}

		return okay;
	}

	template<unsigned int N>
	bool doors_selected(Door(&ids)[N], int tolerance) {
		return this->doors_selected(ids, N, tolerance);
	}

private:
	template<typename E>
	void load_dimensions(std::map<E, Credit<Dimensionlet, E>*>& ds, E id0, E idn, Platform::String^ unit, DimensionStyle& s) {
		for (E id = id0; id <= idn; id++) {
			ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(s, unit, _speak(id)), id);
		}
	}

	template<typename E>
	void load_dimensions(std::map<E, Credit<Dimensionlet, E>*>& ds, E id0, E idn, Platform::String^ unit) {
		for (E id = id0; id <= idn; id++) {
			ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(unit, id.ToString()), id);

			ds[id]->set_style(DimensionState::Default, this->pump_style);
			ds[id]->set_style(DimensionState::Highlight, this->highlight_style);
		}
	}

	template<class D, typename E>
	void load_doors(std::map<E, Credit<D, E>*>& ds, std::map<E, Credit<Percentagelet, E>*>& ps
		, std::map<E, Credit<Doorlet, E>*>& ts, E id0, E idn, float radius) {
		float door_height = radius * 2.0F * 1.618F;
		float door_width = door_height * 0.20F;

		for (E id = id0; id <= idn; id++) {
			ds[id] = this->master->insert_one(new Credit<D, E>(radius), id);
			ps[id] = this->master->insert_one(new Credit<Percentagelet, E>(this->percentage_style), id);
			ts[id] = this->master->insert_one(new Credit<Doorlet, E>(door_width, door_height), id);
		}
	}

	template<typename E>
	void load_cylinder(std::map<E, Credit<Cylinderlet, E>*>& cs, E id, float height
		, double range, unsigned int precision, Platform::String^ unit) {
		auto cylinder = new Credit<Cylinderlet, E>(LiquidSurface::Convex, range, height * 0.2718F, height, 3.0F, 0U, precision);

		cs[id] = this->master->insert_one(cylinder, id);

		this->metrics_style.precision = precision;
		this->load_label(this->labels, id, Colours::Silver, this->label_font);
		this->dimensions[id] = this->master->insert_one(new Credit<Dimensionlet, E>(this->metrics_style, unit), id);
	}

	template<typename E>
	void load_alarms(std::map<E, Credit<Alarmlet, E>*>& as, E id0, E idn, float size) {
		for (E id = id0; id <= idn; id++) {
			auto alarm = new Credit<Alarmlet, E>(size);

			as[id] = this->master->insert_one(alarm, id);
			this->load_label(this->locker_labels, id, Colours::Silver, this->label_font);
		}
	}

	template<typename E>
	void load_label(std::map<E, Credit<Labellet, E>*>& ls, E id, ICanvasBrush^ color, CanvasTextFormat^ font = nullptr) {
		ls[id] = this->master->insert_one(new Credit<Labellet, E>(_speak(id), font, color), id);
	}

	template<typename E>
	void load_settings(std::map<E, Credit<Dimensionlet, E>*>& ds, E id, Platform::String^ unit, Platform::String^ label) {
		ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(DimensionState::Input, this->setting_style, unit, label), id);
		ds[id]->set_maximum(double(gland_pump_rpm_range));
	}

	template<typename E>
	void load_button(std::map<E, Credit<Buttonlet, E>*>& bs, E id, float width = 128.0F, float height = 32.0F) {
		bs[id] = this->master->insert_one(new Credit<Buttonlet, E>(_speak(id), width, height), id);
		bs[id]->set_style(this->button_style);
	}
private:
	template<class D, typename E>
	void reflow_doors(std::map<E, Credit<D, E>*>& ds, std::map<E, Credit<Percentagelet, E>*>& ps
		, std::map<E, Credit<Doorlet, E>*>& ts, E id0, E idn, float side_hint, float fy) {
		GraphletAnchor t_anchor = GraphletAnchor::CT;
		GraphletAnchor p_anchor = GraphletAnchor::CB;
		float cell_x, cell_y, cell_width, cell_height, center;
		float tube_width, door_width, center_xoff;

		ds[id0]->fill_extent(0.0F, 0.0F, &door_width);
		ts[id0]->fill_extent(0.0F, 0.0F, &tube_width);
		center_xoff = tube_width + door_width * 0.5F;

		if (fy > 0.0F) { // Starboard
			t_anchor = GraphletAnchor::CB;
			p_anchor = GraphletAnchor::CT;
		}
		
		for (E id = id0; id <= idn; id++) {
			size_t idx = static_cast<size_t>(id) - static_cast<size_t>(id0) + 1;

			this->decorator->fill_door_cell_extent(&cell_x, &cell_y, &cell_width, &cell_height, idx, side_hint);
			center = cell_x + cell_width * 0.64F;
			
			this->master->move_to(ds[id], center, cell_y + cell_height * fy, GraphletAnchor::CC);
			this->master->move_to(ts[id], ds[id], t_anchor, t_anchor, -center_xoff);
			this->master->move_to(ps[id], ts[id], p_anchor, p_anchor, center_xoff);
		}
	}

	template<class C, typename E>
	void reflow_cylinders(std::map<E, Credit<C, E>*>& is, std::map<E, Credit<Dimensionlet, E>*>& ds
		, std::map<E, Credit<Labellet, E>*>& ls, E id0, E idn) {
		unsigned int count = _I(idn) - _I(id0) + 1;
		float flcount = float(count);
		float x, y, xoff, gapsize;
	
		this->decorator->fill_door_cell_extent(nullptr, &y, &xoff, nullptr, (hopper_count - count), 6.0F);
		xoff *= 0.5F;

		for (E id = id0; id <= idn; id++) {
			ls[id]->fill_extent(0.0F, 0.0F, nullptr, &gapsize);

			this->decorator->fill_descent_anchor(float(_I(id) - _I(id0)) / flcount, 0.0F, &x, nullptr);

			this->master->move_to(is[id], x, y, GraphletAnchor::CT, xoff, -gapsize);
			this->master->move_to(ls[id], is[id], GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, -gapsize * 0.5F);
			this->master->move_to(ds[id], is[id], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, +gapsize * 0.5F);
		}
	}

private:
	void set_cylinder(HD id, float value) {
		this->cylinders[id]->set_value(value);
		this->dimensions[id]->set_value(value, GraphletAnchor::CC);
	}

	void set_door_progress(Door id, float value) {
		this->doors[id]->set_value(value / 100.0F);
		this->hdoors[id]->set_value(value / 100.0F);
		this->progresses[id]->set_value(value, GraphletAnchor::CC);

		if (value >= 96.5F) {
			this->hdoors[id]->set_state(DoorState::Open);
		}

		//AI_hopper_door(this->hdoors[id], value, bottom_door_open_threshold, std::nanf("follow signal rather than progress"));
	}

private: // never delete these graphlets manually.
	std::map<HD, Credit<Labellet, HD>*> labels;
	std::map<Door, Credit<HopperDoorlet, Door>*> hdoors;
	std::map<Door, Credit<Percentagelet, Door>*> progresses;
	std::map<Door, Credit<Doorlet, Door>*> doors;
	std::map<HD, Credit<Dimensionlet, HD>*> dimensions;
	std::map<HD, Credit<Cylinderlet, HD>*> cylinders;
	std::map<BottomDoorCommand, Credit<Alarmlet, BottomDoorCommand>*> lockers;
	std::map<BottomDoorCommand, Credit<Labellet, BottomDoorCommand>*> locker_labels;
	std::map<HD, Credit<Dimensionlet, HD>*> door_opendegree;//泵转速设定
	std::map<HD, Credit<Buttonlet, HD>*> button;//按钮

private:
	CanvasTextFormat^ label_font;
	DimensionStyle percentage_style;
	DimensionStyle metrics_style;
	DimensionStyle pump_style;
	DimensionStyle highlight_style;
	DimensionStyle plain_style;
	DimensionStyle setting_style;
	ButtonStyle button_style;

private:
	HopperDoorsPage* master;
	BottomDoorDecorator* decorator;
};

/*************************************************************************************************/
HopperDoorsPage::HopperDoorsPage(PLCMaster* plc) : Planet(__MODULE__), device(plc) {
	BottomDoorDecorator* decorator = new BottomDoorDecorator();
	Doors* dashboard = new Doors(this, decorator);

	this->dashboard = dashboard;
	this->push_decorator(decorator);

	if (this->device != nullptr) {
		this->door_op = make_bottom_door_menu(plc);
		this->gdoors13_op = make_bottom_doors_group_menu(BottomDoorsGroup::HDoor13, plc);
		this->gdoors57_op = make_bottom_doors_group_menu(BottomDoorsGroup::HDoor57, plc);
		this->gdoors246_op = make_bottom_doors_group_menu(BottomDoorsGroup::HDoor246, plc);
		this->gdoors17_op = make_bottom_doors_group_menu(BottomDoorsGroup::HDoor17, plc);

		this->device->push_confirmation_receiver(dashboard);
	}
}

HopperDoorsPage::~HopperDoorsPage() {
	if (this->dashboard != nullptr) {
		delete this->dashboard;
	}
}

void HopperDoorsPage::load(CanvasCreateResourcesReason reason, float width, float height) {
	auto db = dynamic_cast<Doors*>(this->dashboard);

	if (db != nullptr) {
		db->load(width, height, statusbar_height());
	}
}

void HopperDoorsPage::reflow(float width, float height) {
	auto db = dynamic_cast<Doors*>(this->dashboard);
	
	if (db != nullptr) {
		db->reflow(width, height, statusbar_height());
	}
}

void HopperDoorsPage::on_timestream(long long timepoint_ms, size_t addr0, size_t addrn, uint8* data, size_t size, uint64 p_type, size_t p_size, Syslog* logger) {
	auto db = dynamic_cast<Doors*>(this->dashboard);

	if (db != nullptr) {
		db->on_all_signals(timepoint_ms, addr0, addrn, data, size, logger);
	}
}

bool HopperDoorsPage::can_select(IGraphlet* g) {
	return ((this->device != nullptr)
		&& ((dynamic_cast<HopperDoorlet*>(g) != nullptr)
			|| (dynamic_cast<Alarmlet*>(g) != nullptr)
			|| (dynamic_cast<Buttonlet*>(g) != nullptr)));
}

bool HopperDoorsPage::can_select_multiple() {
	return (this->device != nullptr);
}

bool HopperDoorsPage::on_key(VirtualKey key, bool wargrey_keyboard) {
	bool handled = Planet::on_key(key, wargrey_keyboard);

	if ((!handled) && (this->device != nullptr) && (this->device->authorized())) {
		auto db = dynamic_cast<Doors*>(this->dashboard);

		if (db != nullptr) {
			handled = db->on_key(key, this->device);
		}
	}

	return handled;
}
void HopperDoorsPage::on_focus(IGraphlet* g, bool yes) {
	if (yes) {
		auto editor = dynamic_cast<IEditorlet*>(g);

		if (editor != nullptr) {
			if ((this->device != nullptr) && (this->device->authorized())) {
				this->show_virtual_keyboard(ScreenKeyboard::Numpad);
			}
			else {
				this->set_caret_owner(nullptr);
			}
		}
	}
}
void HopperDoorsPage::on_tap_selected(IGraphlet* g, float local_x, float local_y) {
	auto hdoor = dynamic_cast<HopperDoorlet*>(g);
	auto alarm = dynamic_cast<Credit<Alarmlet, BottomDoorCommand>*>(g);
	auto btn = dynamic_cast<Buttonlet*>(g);

	
	if (hdoor != nullptr) {
		menu_popup(this->door_op, hdoor, local_x, local_y);
	} else if (alarm != nullptr && (this->device->get_mode() != TCPMode::User)) {
		this->device->send_command(DO_bottom_doors_special_command(alarm->id));
	}else if (btn != nullptr && (this->device->get_mode() != TCPMode::User)) {
		this->device->send_command(open_degree_command);
	}
}

void HopperDoorsPage::on_gesture(std::list<float2>& anchors, float x, float y) {
	auto dashboard = dynamic_cast<Doors*>(this->dashboard);

	if (dashboard != nullptr) {
		Door hds13[] = { Door::PS1, Door::PS3, Door::SB1, Door::SB3 };
		Door hds57[] = { Door::PS5, Door::SB5, Door::PS7, Door::SB7};
		Door hds246[] = { Door::PS2, Door::PS4,Door::PS6, Door::SB2, Door::SB4, Door::SB6 };
		bool g1_okay = dashboard->doors_selected(hds13, 1);
		bool g2_okay = dashboard->doors_selected(hds57, 1);
		bool g3_okay = dashboard->doors_selected(hds246, 1);

		if (g1_okay && g2_okay && g3_okay) {
			group_menu_popup(this->gdoors17_op, this, x, y);
		}
		else if (dashboard->doors_selected(hds13, 2)) {
			group_menu_popup(this->gdoors13_op, this, x, y);
		}
		else if (dashboard->doors_selected(hds57, 2)) {
			group_menu_popup(this->gdoors57_op, this, x, y);
		}
		else if (dashboard->doors_selected(hds246, 2)) {
			group_menu_popup(this->gdoors246_op, this, x, y);
		}
		/*
		Door hds12[] = { Door::PS1, Door::PS2, Door::SB1, Door::SB2 };
		Door hds35[] = { Door::PS3, Door::PS4, Door::PS5, Door::SB3, Door::SB4, Door::SB5 };
		Door hds67[] = { Door::PS6, Door::PS7, Door::SB6, Door::SB7 };
		bool g1_okay = dashboard->doors_selected(hds12, 1);
		bool g2_okay = dashboard->doors_selected(hds35, 1);
		bool g3_okay = dashboard->doors_selected(hds67, 1);
		
		if (g1_okay && g2_okay && g3_okay) {
			group_menu_popup(this->gdoors17_op, this, x, y);
		} else if (dashboard->doors_selected(hds35, 2)) {
			group_menu_popup(this->gdoors35_op, this, x, y);
		} else if (dashboard->doors_selected(hds12, 2)) {
			group_menu_popup(this->gdoors12_op, this, x, y);
		} else if (dashboard->doors_selected(hds67, 2)) {
			group_menu_popup(this->gdoors67_op, this, x, y);
		}
		*/
	}
}
