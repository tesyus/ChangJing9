#include <map>

#include "page/flushs.hpp"
#include "page/diagnostics/water_pump_dx.hpp"

#include "configuration.hpp"
#include "menu.hpp"

#include "module.hpp"
#include "text.hpp"
#include "paint.hpp"
#include "brushes.hxx"
#include "turtle.hpp"

#include "graphlet/shapelet.hpp"
#include "graphlet/ui/buttonlet.hpp"
#include "graphlet/ui/statuslet.hpp"
#include "graphlet/symbol/door/hatchlet.hpp"
#include "graphlet/symbol/door/hopper_doorlet.hpp"
#include "graphlet/symbol/pump/water_pumplet.hpp"
#include "graphlet/symbol/valve/gate_valvelet.hpp"
#include "graphlet/symbol/valve/manual_valvelet.hpp"

#include "iotables/ai_doors.hpp"
#include "iotables/ai_pumps.hpp"
#include "iotables/ai_water_pumps.hpp"

#include "iotables/di_doors.hpp"
#include "iotables/di_pumps.hpp"
#include "iotables/di_valves.hpp"
#include "iotables/di_water_pumps.hpp"

#include "iotables/do_doors.hpp"
#include "iotables/do_valves.hpp"
#include "iotables/do_water_pumps.hpp"
#include <iotables\ao_pumps.hpp>


using namespace WarGrey::SCADA;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Numerics;
using namespace Windows::System;

using namespace Windows::UI::Xaml::Controls;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::Brushes;
using namespace Microsoft::Graphics::Canvas::Geometry;

static CanvasSolidColorBrush^ water_color = Colours::Green;

private enum class FSFunction { Diagnostics, _ };
WaterPumpConditionAction CurrentWaterWorkingMode= WaterPumpConditionAction::_;
// WARNING: order matters
private enum class FS : unsigned int {
	D1,D2,
	//竖
	HBV1, HBV8, HBV5, HBV6, HBV7,
	HBV9, HBV10, HBV11, HBV12, HBV13, HBV14, HBV15,//泥门阀
	//横
	SBV1, SBV2, SBV3, SBV4, SBV5,
	DHV1, DHV2, HBV4 , HBV3,HBV2,

	// 小泥门
	SB1, SB2, SB3, SB4, SB5, SB6, SB7,
	PS1, PS2, PS3, PS4, PS5, PS6, PS7,

	//左右高压水泵
	PSPump, SBPump,

	//海水
	PSSea, SBSea,
	_,

	//未命名锚点
	tsbv5,
	l2sea, r2sea,
	lt, rt,
	lTower, rTower, water, room,
	h9,h10,h11, h12, h13, h14, h15,
	ulb9, ulb10, ulb11, ulb12, ulb13, ulb14, ulb15, // left-bottom corner of hopper
	dlb9, dlb10, dlb11, dlb12, dlb13, ldb14, dlb15, // left-bottom corner of hopper
	urb9, urb10, urb11, urb12, urb13, urb14, urb15, // right-bottom corner of hopper
	drb9, drb10, drb11, drb12, drb13, drb14, drb15, // right-bottom corner of hopper
	ub9, ub10, ub11, ub12, ub13, ub14, ub15, // left-bottom corner of hopper
	db9, db10, db11, db12, db13, db14, db15, // left-bottom corner of hopper
	lhbv12,lhbv15,

	Port, Starboard,
	tol,tor, toPS,toSB, pspump, hbv1, towermid, btn,

	// anchors used for non-interconnected nodes
	//nic
};

static uint16 DO_butterfly_valve_action(ButterflyValveAction cmd, GateValvelet* valve) {
	uint16 index = 0U;
	auto credit_valve = dynamic_cast<Credit<GateValvelet, FS>*>(valve);

	if (credit_valve != nullptr) {
		index = DO_butterfly_valve_command(cmd, credit_valve->id);
	}

	return index;
}

/*************************************************************************************************/
private class Flush final : public PLCConfirmation {
public:
	Flush(FlushsPage* master, MenuFlyout^ ps_menu = nullptr, MenuFlyout^ sb_menu = nullptr)
		: master(master), ps_menu(ps_menu), sb_menu(sb_menu) {
		this->setting_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::GhostWhite, Colours::RoyalBlue);
	}

public:
	void pre_read_data(Syslog* logger) override {
		this->master->enter_critical_section();
		this->master->begin_update_sequence();

		this->station->clear_subtacks();
	}

	void on_analog_input(long long timepoint_ms, const uint8* DB2, size_t count2, const uint8* DB203, size_t count203, Syslog* logger) override {
		this->pressures[FS::D1]->set_value(RealData(DB203, pump_D1_pressure), GraphletAnchor::CC);
		this->pressures[FS::D2]->set_value(RealData(DB203, pump_D2_pressure), GraphletAnchor::CC);

		//左右排出信号 之后补
		
		this->pressures[FS::PSPump]->set_value(RealData(DB203, sb_water_pump_discharge_pressure), GraphletAnchor::CB);
		this->flows[FS::PSPump]->set_value(RealData(DB203, sb_water_pump_flow), GraphletAnchor::CT);
		this->pressures[FS::SBPump]->set_value(RealData(DB203, ps_water_pump_discharge_pressure), GraphletAnchor::CB);
		this->flows[FS::SBPump]->set_value(RealData(DB203, ps_water_pump_flow), GraphletAnchor::CT);
		

		this->powers[FS::PSPump]->set_value(RealData(DB203, ps_water_pump_power), GraphletAnchor::CB);
		this->rpms[FS::PSPump]->set_value(RealData(DB203, ps_water_pump_rpm), GraphletAnchor::CT);
		this->powers[FS::SBPump]->set_value(RealData(DB203, sb_water_pump_power), GraphletAnchor::CB);
		this->rpms[FS::SBPump]->set_value(RealData(DB203, sb_water_pump_rpm), GraphletAnchor::CT);


		//长鲸9没有小泥门 是抽舱闸阀 这个之后注意一下
		
		this->set_door_progress(Door::PS1, RealData(DB203, upper_door_PS1_progress));
		this->set_door_progress(Door::PS2, RealData(DB203, upper_door_PS2_progress));
		this->set_door_progress(Door::PS3, RealData(DB203, upper_door_PS3_progress));
		this->set_door_progress(Door::PS4, RealData(DB203, upper_door_PS4_progress));
		this->set_door_progress(Door::PS5, RealData(DB203, upper_door_PS5_progress));
		this->set_door_progress(Door::PS6, RealData(DB203, upper_door_PS6_progress));
		this->set_door_progress(Door::PS7, RealData(DB203, upper_door_PS7_progress));

		this->set_door_progress(Door::SB1, RealData(DB203, upper_door_SB1_progress));
		this->set_door_progress(Door::SB2, RealData(DB203, upper_door_SB2_progress));
		this->set_door_progress(Door::SB3, RealData(DB203, upper_door_SB3_progress));
		this->set_door_progress(Door::SB4, RealData(DB203, upper_door_SB4_progress));
		this->set_door_progress(Door::SB5, RealData(DB203, upper_door_SB5_progress));
		this->set_door_progress(Door::SB6, RealData(DB203, upper_door_SB6_progress));
		this->set_door_progress(Door::SB7, RealData(DB203, upper_door_SB7_progress));
		
	}

	void on_digital_input(long long timepoint_ms, const uint8* DB4, size_t count4, const uint8* DB205, size_t count205, Syslog* logger) override {
		DI_water_pump(this->pumps[FS::PSPump], DB4, ps_water_pump_feedback, DB205, ps_water_pump_details);
		DI_water_pump(this->pumps[FS::SBPump], DB4, sb_water_pump_feedback, DB205, sb_water_pump_details);

		DI_hydraulic_pump_dimension(this->pressures[FS::D1], DB4, pump_D1_feedback);
		DI_hydraulic_pump_dimension(this->pressures[FS::D2], DB4, pump_D2_feedback);


		//手动阀没信号
		DI_gate_valve(this->bfvalves[FS::DHV1], DB4, butterfly_valve_DHV01_feedback, DB205, butterfly_valve_DHV01_status);
		DI_gate_valve(this->bfvalves[FS::DHV2], DB4, butterfly_valve_DHV02_feedback, DB205, butterfly_valve_DHV02_status);

		DI_gate_valve(this->bfvalves[FS::HBV1], DB4, butterfly_valve_HBV01_feedback, DB205, butterfly_valve_HBV01_status);
		DI_gate_valve(this->bfvalves[FS::HBV2], DB4, butterfly_valve_HBV02_feedback, DB205, butterfly_valve_HBV02_status);
		DI_gate_valve(this->bfvalves[FS::HBV3], DB4, butterfly_valve_HBV03_feedback, DB205, butterfly_valve_HBV03_status);
		DI_gate_valve(this->bfvalves[FS::HBV4], DB4, butterfly_valve_HBV04_feedback, DB205, butterfly_valve_HBV04_status);
		DI_gate_valve(this->bfvalves[FS::HBV5], DB4, butterfly_valve_HBV05_feedback, DB205, butterfly_valve_HBV05_status);
		DI_gate_valve(this->bfvalves[FS::HBV6], DB4, butterfly_valve_HBV06_feedback, DB205, butterfly_valve_HBV06_status);
		DI_gate_valve(this->bfvalves[FS::HBV7], DB4, butterfly_valve_HBV07_feedback, DB205, butterfly_valve_HBV07_status);
		DI_gate_valve(this->bfvalves[FS::HBV8], DB4, butterfly_valve_HBV08_feedback, DB205, butterfly_valve_HBV08_status);
		DI_gate_valve(this->bfvalves[FS::HBV9], DB4, butterfly_valve_HBV09_feedback, DB205, butterfly_valve_HBV09_status);
		DI_gate_valve(this->bfvalves[FS::HBV10], DB4, butterfly_valve_HBV10_feedback, DB205, butterfly_valve_HBV10_status);
		DI_gate_valve(this->bfvalves[FS::HBV11], DB4, butterfly_valve_HBV11_feedback, DB205, butterfly_valve_HBV11_status);
		DI_gate_valve(this->bfvalves[FS::HBV12], DB4, butterfly_valve_HBV12_feedback, DB205, butterfly_valve_HBV12_status);
		DI_gate_valve(this->bfvalves[FS::HBV13], DB4, butterfly_valve_HBV13_feedback, DB205, butterfly_valve_HBV13_status);
		DI_gate_valve(this->bfvalves[FS::HBV14], DB4, butterfly_valve_HBV14_feedback, DB205, butterfly_valve_HBV14_status);
		DI_gate_valve(this->bfvalves[FS::HBV15], DB4, butterfly_valve_HBV15_feedback, DB205, butterfly_valve_HBV15_status);


		//长鲸9没有小泥门 是抽舱闸阀 这个之后注意一下
		
		DI_hopper_door(this->uhdoors[Door::PS1], DB205, upper_door_PS1_status);
		DI_hopper_door(this->uhdoors[Door::PS2], DB205, upper_door_PS2_status);
		DI_hopper_door(this->uhdoors[Door::PS3], DB205, upper_door_PS3_status);
		DI_hopper_door(this->uhdoors[Door::PS4], DB205, upper_door_PS4_status);
		DI_hopper_door(this->uhdoors[Door::PS5], DB205, upper_door_PS5_status);
		DI_hopper_door(this->uhdoors[Door::PS6], DB205, upper_door_PS6_status);
		DI_hopper_door(this->uhdoors[Door::PS7], DB205, upper_door_PS7_status);

		DI_hopper_door(this->uhdoors[Door::SB1], DB205, upper_door_SB1_status);
		DI_hopper_door(this->uhdoors[Door::SB2], DB205, upper_door_SB2_status);
		DI_hopper_door(this->uhdoors[Door::SB3], DB205, upper_door_SB3_status);
		DI_hopper_door(this->uhdoors[Door::SB4], DB205, upper_door_SB4_status);
		DI_hopper_door(this->uhdoors[Door::SB5], DB205, upper_door_SB5_status);
		DI_hopper_door(this->uhdoors[Door::SB6], DB205, upper_door_SB6_status);
		DI_hopper_door(this->uhdoors[Door::SB7], DB205, upper_door_SB7_status);
		
		DI_shift_button(this->shifts[FlushingCommand::LeftShift], DB205, left_shifting_details);
		DI_shift_button(this->shifts[FlushingCommand::RightShift], DB205, right_shifting_details);
		/*
		if (this->ps_menu != nullptr) {
			DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::PS_PS), DB205, ps_ps_details);
			DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::PS_SB), DB205, ps_sb_details);
			DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::PS_2), DB205, ps_2_details);
			DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::PS_H), DB205, ps_h_details);
			DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::S2_PS), DB205, s2_ps_details);
			DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::S2_SB), DB205, s2_sb_details);
			//DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::S2_2), DB205, s2_2_details);
			DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::S2_H), DB205, s2_h_details);
			//DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::P2_2), DB205, p2_2_details);
			DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::P2_H), DB205, p2_h_details);
			DI_water_condition_menu(this->ps_menu, _I(PSWaterPumpAction::I2_2), DB205, i2_2_details);
		}
		*/
		if (this->sb_menu != nullptr) {
			
			DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::SB_PS), DB205, sb_ps_details);
			/*
			DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::SB_SB), DB205, sb_sb_details);
			DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::SB_2), DB205, sb_2_details);
			DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::SB_H), DB205, sb_h_details);
			DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::S2_PS), DB205, s2_ps_details);
			DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::S2_SB), DB205, s2_sb_details);
			//DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::S2_2), DB205, s2_2_details);
			DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::S2_H), DB205, s2_h_details);
			//DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::P2_2), DB205, p2_2_details);
			DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::P2_H), DB205, p2_h_details);
			DI_water_condition_menu(this->sb_menu, _I(SBWaterPumpAction::I2_2), DB205, i2_2_details);*/
		}
		CurrentWaterWorkingMode = DI_getwaterworkingmode(DB205);
	}

	void on_forat(long long timepoint_ms, const uint8* DB20, size_t count, Syslog* logger) override {
		this->pumps_rpm[FS::PSPump]->set_value(DBD(DB20, 658U), GraphletAnchor::LB);//左
		this->pumps_rpm[FS::SBPump]->set_value(DBD(DB20, 670U), GraphletAnchor::LB);//右
	}
	void post_read_data(Syslog* logger) override {
		{ // flow water
			switch (CurrentWaterWorkingMode) {
				case WarGrey::SCADA::WaterPumpConditionAction::PS_PS: //左高压冲水泵向左耙头冲水
					try_flow_water(8, FS::PSSea, FS::DHV1, FS::pspump, FS::lt, FS::tol, FS::l2sea, FS::toPS, FS::Port);
					break;
				case WaterPumpConditionAction::PS_SB://左高压冲水泵向右耙头冲水
					try_flow_water(8, FS::PSSea, FS::DHV1, FS::pspump, FS::hbv1, FS::lt, FS::tor, FS::toSB, FS::Starboard);
					break;
				case WaterPumpConditionAction::SB_PS: //右高压冲水泵向左耙头冲水
					try_flow_water(8, FS::SBSea, FS::DHV2, FS::SBPump, FS::rTower, FS::rt, FS::tol, FS::toPS, FS::Port);
					break;
				case WaterPumpConditionAction::SB_SB: //右高压冲水泵向右耙头冲水
					try_flow_water(8, FS::SBSea, FS::DHV2, FS::SBPump, FS::rTower, FS::rt, FS::tor, FS::toSB, FS::Starboard);
					break;
				case WaterPumpConditionAction::PS_2://左高压冲水泵向双耙头冲水
					try_flow_water(8, FS::PSSea, FS::DHV1, FS::pspump, FS::lt, FS::tol, FS::l2sea, FS::toPS, FS::Port);
					try_flow_water(8, FS::PSSea, FS::DHV1, FS::pspump, FS::hbv1, FS::lt, FS::tor, FS::toSB, FS::Starboard);
					break;
				case WaterPumpConditionAction::SB_2://右高压冲水泵向双耙头冲水
					try_flow_water(8, FS::SBSea, FS::DHV2, FS::SBPump, FS::rTower, FS::rt, FS::tol, FS::toPS, FS::Port);
					try_flow_water(8, FS::SBSea, FS::DHV2, FS::SBPump, FS::rTower, FS::rt, FS::tor, FS::toSB, FS::Starboard);
					break;
				case WaterPumpConditionAction::S2_PS://左右高压冲水泵串联向左耙头冲水
					try_flow_water(7, FS::lTower, FS::pspump, FS::lt, FS::tol, FS::l2sea, FS::toPS, FS::Port);
					try_flow_water(6, FS::SBSea, FS::DHV2, FS::SBPump, FS::rTower, FS::towermid, FS::lTower);
					break;
				case WaterPumpConditionAction::S2_SB://左右高压冲水泵串联向右耙头冲水
					//try_flow_water(6,  FS::lTower, FS::towermid, FS::rt, FS::tor, FS::r2sea, FS::Starboard);
					try_flow_water(12, FS::SBSea, FS::SBPump, FS::rTower, FS::towermid, FS::lTower, FS::PSPump, FS::hbv1, FS::lt, FS::tor, FS::r2sea,FS::toSB, FS::Starboard);
					break;
				case WaterPumpConditionAction::I2_2://左右高压冲水泵分开向双耙头冲水
					try_flow_water(8, FS::PSSea, FS::DHV1, FS::pspump, FS::lt, FS::tol, FS::l2sea, FS::toPS, FS::Port);
					try_flow_water(8, FS::SBSea, FS::DHV2, FS::SBPump, FS::rTower, FS::rt, FS::tor, FS::toSB, FS::Starboard);
					break;
				case WaterPumpConditionAction::PS_H://左高压冲水泵向泥舱冲水
					try_flow_water(9, FS::PSSea, FS::DHV1, FS::pspump, FS::lt, FS::tol, FS::l2sea, FS::HBV5, FS::SBV5,FS::HBV15);
					break;
				case WaterPumpConditionAction::SB_H://右高压冲水泵向泥舱冲水
					try_flow_water(10, FS::SBSea, FS::DHV2, FS::SBPump, FS::rTower, FS::rt, FS::tor, FS::r2sea, FS::HBV6,FS::SBV5,FS::HBV15);
					break;
				case WaterPumpConditionAction::P2_H://左右高压冲水泵并联向泥舱冲水
					try_flow_water(9, FS::PSSea, FS::DHV1, FS::pspump, FS::lt, FS::tol, FS::l2sea, FS::HBV5, FS::SBV5,FS::HBV15);
					try_flow_water(9, FS::SBSea, FS::DHV2, FS::SBPump, FS::rTower, FS::rt, FS::tor,FS::r2sea, FS::HBV6, FS::SBV5);
					this->station->push_subtrack(FS::rt, FS::lt, water_color);
					break;
				case WaterPumpConditionAction::S2_H://左右高压冲水泵串联向泥舱冲水
					try_flow_water(8,  FS::lTower, FS::pspump, FS::lt, FS::tol, FS::l2sea, FS::HBV5, FS::SBV5, FS::HBV15);
					try_flow_water(6, FS::SBSea, FS::DHV2, FS::SBPump, FS::rTower, FS::towermid, FS::lTower);
					break;
				default:
					break;

			}
			for (FS HBV = FS::HBV9; HBV <= FS::HBV15; HBV++) {
				unsigned int distance = _I(HBV) - _I(FS::HBV9);
				FS ulb = _E(FS, _I(FS::ulb9) + distance);
				FS dlb = _E(FS, _I(FS::dlb9) + distance);
				FS urb = _E(FS, _I(FS::urb9) + distance);
				FS drb = _E(FS, _I(FS::drb9) + distance);
				FS ub = _E(FS, _I(FS::ub9) + distance);
				FS db = _E(FS, _I(FS::db9) + distance);

				this->try_flow_water(HBV, urb, ulb, water_color);
				this->try_flow_water(HBV, drb, dlb, water_color);
				this->try_flow_water(HBV, ub, db, water_color);
			}
		}

		this->master->end_update_sequence();
		this->master->leave_critical_section();
	}

public:
	void construct(float gwidth, float gheight) {
		this->caption_font = make_bold_text_format("Microsoft YaHei", normal_font_size);
		this->label_font = make_bold_text_format("Microsoft YaHei", small_font_size);
		this->pump_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::Background);
		this->highlight_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::Green);

		this->metrics_style.number_font = make_bold_text_format("Cambria Math", large_metrics_font_size);
		this->metrics_style.unit_font = make_bold_text_format("Cambria", normal_font_size);

		this->percentage_style.precision = 1;
		this->percentage_style.number_font = make_bold_text_format("Cambria Math", large_metrics_font_size); 
	}

	bool on_key(VirtualKey key, PLCMaster* plc) {//
		bool handled = false;

		if (key == VirtualKey::Enter) {
			auto editor = dynamic_cast<Credit<Dimensionlet, FS>*>(this->master->get_focus_graphlet());

			if (editor != nullptr) {
				float speed = float(editor->get_input_number());

				if (speed >= 0.0F) {
					plc->send_setting(AO_flush_pump_setting(editor->id), speed);
				}

				this->master->hide_virtual_keyboard();
				this->master->set_caret_owner(nullptr);

				handled = true;
			}
		}

		return handled;
	}
public:
	void load(float width, float height, float gwidth, float gheight) {
		float radius = resolve_gridsize(gwidth, gheight);
		auto hstyle = make_dash_stroke(CanvasDashStyle::Dash);
		Turtle<FS>* pTurtle = new Turtle<FS>(gwidth, gheight, false, FS::SBV5);
		Turtle<FS>* rTurtle = new Turtle<FS>(gwidth, gheight, false);
		Turtle<FS>* wTurtle = new Turtle<FS>(gwidth, gheight, false);
		Turtle<FS>* vTurtle = new Turtle<FS>(gwidth, gheight, false, FS::HBV15);
		float half_width = 2.0F;
		
		//外框
		pTurtle->move_right(2, FS::tsbv5);
		pTurtle->turn_right_up();
		pTurtle->move_up(9, FS::HBV5)->move_up(2)->turn_up_right();
		pTurtle->move_right(2, FS::l2sea)->move_right(20)->turn_right_down(FS::tol);
		pTurtle->move_down(9.5f, FS::lt)->move_down(2, FS::HBV7)->move_down(2, FS::rt)->move_down(9.5f,FS::tor)->turn_down_left();
		pTurtle->move_left(20, FS::r2sea)->move_left(2)->turn_left_up();
		pTurtle->move_up(2, FS::HBV6)->move_up(9)->turn_up_left();

		//左泵
		pTurtle->jump_back(FS::lt)->move_left(4, FS::hbv1)->turn_left_up()->move_up(1, FS::HBV1)->move_up(3.5F, FS::PSPump)->move_up(0.5F, FS::pspump)->move_left(4, FS::lTower)->move_left(4, FS::DHV1)->move_left(4, FS::SBV1)->move_left(4)->turn_left_down()->move_down(2, FS::PSSea);
		//右泵
		pTurtle->jump_back(FS::rt)->move_left(2,FS::HBV2)->move_left(2, FS::rTower)->move_down(3, FS::SBPump)->jump_down(0.5F)->move_left(8.5F, FS::DHV2)->move_left(4, FS::SBV2)->move_left(4)->turn_left_down()->move_down(1, FS::SBSea);
			//pTurtle->jump_back(FS::rt)->move_left(3.5f, FS::HBV7)->move_left(3)->turn_left_down()->move_down(4.5f)->jump_left(0.5f, FS::SBPump);//泵位置
			//pTurtle->move_left(8, FS::DHV8)->move_left(3, FS::SBV2)->move_left(2)->turn_left_down()->move_down(1, FS::SBSea);
		//左右泵串联
		pTurtle->jump_back(FS::lTower)->move_down(4,FS::HBV8)->move_down(5, FS::towermid)->turn_down_right()->move_to(FS::rTower);
			//pTurtle->jump_back(FS::lTower)->move_left(3)->turn_left_down()->move_down(4, FS::HBV6)->move_down(4.5f, FS::rTower);
		//左耙头
		pTurtle->jump_back(FS::l2sea)->move_up(3, FS::toPS)->turn_up_left()->move_left(7, FS::HBV3)->move_left(7, FS::SBV3)->move_left(5, FS::Port);
		//右耙头
		pTurtle->jump_back(FS::r2sea)->move_down(3,FS::toSB)->turn_down_left()->move_left(7, FS::HBV4)->move_left(7, FS::SBV4)->move_left(5, FS::Starboard);
		//按钮位置
		pTurtle->jump_back(FS::HBV5)->jump_left(7, FS::btn);
		//回到泥门部分
		pTurtle->jump_back(FS::SBV5)->move_left(2);

		for (FS HBV = FS::HBV9; HBV <= FS::HBV15; HBV++) {
			unsigned int distance = _I(HBV) - _I(FS::HBV9);
			float half_height = 2.5F;
			float gapsize = 0.618F;
			float room_height = (6.0F + half_height) * 2.0F;
			float water_height = room_height - 5.0F;
			FS hbv = _E(FS, _I(FS::h9) + distance);
			FS ulb = _E(FS, _I(FS::ulb9) + distance);
			FS dlb = _E(FS, _I(FS::dlb9) + distance);
			FS urb = _E(FS, _I(FS::urb9) + distance);
			FS drb = _E(FS, _I(FS::drb9) + distance);
			FS ub = _E(FS, _I(FS::ub9) + distance);
			FS db = _E(FS, _I(FS::db9) + distance);

			pTurtle->move_left(gapsize)->move_left(half_width, HBV);
			pTurtle->move_down(half_height, db)->jump_left(half_width, dlb)->move_right(half_width)->move_right(half_width, drb);
			pTurtle->jump_back(HBV)->move_up(half_height)->jump_left(half_width, ulb)->move_right(half_width, ub)->move_right(half_width, urb);
			pTurtle->jump_back(HBV)->move_left(half_width);

			rTurtle->jump_left(gapsize)->move_left()->jump_left(half_width);
			rTurtle->move_left(hbv)->move_down(room_height);
			rTurtle->move_right()->jump_right(half_width)->move_right();
			rTurtle->move_up(room_height)->jump_back(hbv);

			wTurtle->jump_back()->jump_left(half_width)->jump_left(gapsize)->jump_left(half_width, hbv);
			wTurtle->move_down(gapsize)->move_left(half_width)->move_left(gapsize);
			wTurtle->move_down(water_height)->move_right(gapsize)->move_right(half_width)->move_down(gapsize);

			if (HBV != FS::HBV9) {
				wTurtle->jump_up(gapsize)->move_right(half_width)->jump_up(water_height)->move_left(half_width);
			}
			if (HBV == FS::HBV15) {
				float half_gap = gapsize * 0.5F;
				pTurtle->jump_down()->jump_up(FS::room)->jump_left(gapsize, FS::water);
			}
		}
		
		this->hopper_room = this->master->insert_one(new Tracklet<FS>(rTurtle, default_pipe_thickness, Colours::DimGray, hstyle));
		this->hopper_water = this->master->insert_one(new Tracklet<FS>(wTurtle, default_pipe_thickness, Colours::DimGray, hstyle));
		this->station = this->master->insert_one(new Tracklet<FS>(pTurtle, default_pipe_thickness, default_pipe_color));
		//this->pipeline18 = this->master->insert_one(new Tracklet<FS>(vTurtle, default_pipe_thickness, default_pipe_color));


		{ // load doors
			this->load_doors(this->uhdoors, this->progresses, Door::PS1, Door::PS7, radius);
			this->load_doors(this->uhdoors, this->progresses, Door::SB1, Door::SB7, radius);
		}

		{ // load valves
			this->load_valves(this->mvalves, this->labels, this->captions, FS::SBV1, FS::SBV5, radius * 0.618F, 0.0);
			this->load_valves(this->bfvalves, this->labels, this->captions, FS::HBV1, FS::HBV15, radius, 90.0);//竖
			this->load_valves(this->bfvalves, this->labels, this->captions, FS::DHV1, FS::HBV2, radius, 0.0);//横
			/*
			this->load_valves(this->mvalves, this->labels, this->captions, FS::SBV1, FS::SBV5, radius * 0.618F, 0.0);
			this->load_valves(this->bfvalves, this->labels, this->captions, FS::HBV4, FS::HBV26, radius, 90.0);
			this->load_valves(this->bfvalves, this->labels, this->captions, FS::HBV11, FS::HBV17, radius, 90.0);
			this->load_valves(this->bfvalves, this->labels, this->captions, FS::DHV8, FS::HBV23, radius, 0);
			*/
		}
		this->load_buttons(this->functions);
		this->load_buttons(this->shifts);

		{ // load special nodes
			auto pscolor = Colours::make(default_ps_color);
			auto sbcolor = Colours::make(default_sb_color);
			float dh_radius = gwidth * 2.0F;
			float nic_radius = radius * 0.25F;

			this->load_label(this->captions, FS::PSSea, Colours::Silver);
			this->load_label(this->captions, FS::SBSea, Colours::Silver);

			this->load_pump(this->pumps, this->captions, FS::PSPump, +radius, 0);
			this->load_pump(this->pumps, this->captions, FS::SBPump, -radius, 180.0);
			
			this->ps_sea = this->master->insert_one(new Hatchlet(gwidth, gheight, pscolor));
			this->sb_sea = this->master->insert_one(new Hatchlet(gwidth, gheight, sbcolor));

			this->ps_draghead = this->master->insert_one(
				new Segmentlet(-90.0, 90.0, dh_radius, gheight,
					nullptr, pscolor, default_pipe_thickness));

			this->sb_draghead = this->master->insert_one(
				new Segmentlet(-90.0, 90.0, dh_radius, gheight,
					nullptr, sbcolor, default_pipe_thickness));

			//看一下这个是什么!!!!
			/*
			for (FS id = FS::nic; id <= FS::nic; id++) {
				this->nintercs[id] = this->master->insert_one(
					new Omegalet(180.0, nic_radius, default_pipe_thickness, default_pipe_color));
			}*/
		}

		{ // load other dimensions
			float cylinder_height = gheight * 5.0F;

			
			this->load_dimension(this->pressures, FS::PSPump, "bar", 1);
			this->load_dimension(this->flows, FS::PSPump, "m3ph", 1);
			this->load_dimension(this->pressures, FS::SBPump, "bar", 1);
			this->load_dimension(this->flows, FS::SBPump, "m3ph", 1);

			this->load_dimensions(this->pressures, FS::D1, FS::D2, "bar");
		}
		{

			//转速设定
			this->load_settings(this->pumps_rpm, FS::PSPump, FS::SBPump, "RPM", "S");
		}
	}

public:
	void reflow(float width, float height, float gwidth, float gheight) {
		this->master->move_to(this->station, width * 0.5F, height * 0.5F, GraphletAnchor::CC);
		this->station->map_graphlet_at_anchor(this->hopper_room, FS::room, GraphletAnchor::LC);
		this->station->map_graphlet_at_anchor(this->hopper_water, FS::water, GraphletAnchor::LC);
		//this->station->map_graphlet_at_anchor(this->pipeline18, FS::HBV18, GraphletAnchor::LC);

		this->station->map_graphlet_at_anchor(this->ps_draghead, FS::Port, GraphletAnchor::RC);
		this->station->map_graphlet_at_anchor(this->sb_draghead, FS::Starboard, GraphletAnchor::RC);
		this->station->map_graphlet_at_anchor(this->ps_sea, FS::PSSea, GraphletAnchor::CT);
		this->station->map_graphlet_at_anchor(this->sb_sea, FS::SBSea, GraphletAnchor::CT);
		this->master->move_to(this->captions[FS::PSSea], this->ps_sea, GraphletAnchor::CB, GraphletAnchor::CT);
		this->master->move_to(this->captions[FS::SBSea], this->sb_sea, GraphletAnchor::CB, GraphletAnchor::CT);

		for (auto it = this->nintercs.begin(); it != this->nintercs.end(); it++) {
			/** NOTE
			 * Lines are brush-based shape, they do not have stroke, `Shapelet` does not know how width they are,
			 * thus, we have to do aligning on our own.
			 */
			this->station->map_graphlet_at_anchor(it->second, it->first, GraphletAnchor::CB, 0.0F, default_pipe_thickness * 0.5F);
		}

		this->reflow_doors(this->uhdoors, this->progresses, Door::PS1, Door::PS7, GraphletAnchor::CT);
		this->reflow_doors(this->uhdoors, this->progresses, Door::SB1, Door::SB7, GraphletAnchor::CB);

		{ // reflow buttons
			IGraphlet* shift_target = this->progresses[Door::SB4];
			
			
			this->master->move_to(this->shifts[FlushingCommand::LeftShift], shift_target, GraphletAnchor::LB, GraphletAnchor::RT, 0.0F, gheight);
			this->master->move_to(this->shifts[FlushingCommand::RightShift], shift_target, GraphletAnchor::RB, GraphletAnchor::LT, 0.0F, gheight);

			this->master->move_to(this->functions[FSFunction::Diagnostics], this->station, 0.75F, 0.5F, GraphletAnchor::CC);
		}

		for (auto it = this->pumps.begin(); it != this->pumps.end(); it++) {
			float ox, oy;

			it->second->fill_pump_origin(&ox, &oy);
			this->station->map_credit_graphlet(it->second, GraphletAnchor::CC, -ox, -oy);
			if (it->first == FS::SBPump) {
				this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::CB, GraphletAnchor::CT);
			}
			else {
				this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::CT, GraphletAnchor::CB);
			}
		}
		
		{ // reflow valves
			float gridsize = resolve_gridsize(gwidth, gheight);

			for (auto it = this->bfvalves.begin(); it != this->bfvalves.end(); it++) {
				this->reflow_valve(0.0F, 0.0F, gridsize, it->first, it->second);
			}

			for (auto it = this->mvalves.begin(); it != this->mvalves.end(); it++) {
				this->reflow_valve(0.0F, 0.0F, gridsize, it->first, it->second);
			}
		}

		{ // reflow dimensions
			float xoff = gwidth * 1.5F;
			float ps_oy = this->pumps[FS::PSPump]->get_radiusY() * 0.5F;
			
			this->station->map_credit_graphlet(this->powers[FS::PSPump], GraphletAnchor::LC, xoff, -ps_oy*3);
			this->station->map_credit_graphlet(this->rpms[FS::PSPump], GraphletAnchor::LC, xoff, -ps_oy);
			this->station->map_credit_graphlet(this->pressures[FS::PSPump], GraphletAnchor::LC, xoff , ps_oy);
			this->station->map_credit_graphlet(this->flows[FS::PSPump], GraphletAnchor::LC, xoff, ps_oy*3);

			this->station->map_credit_graphlet(this->powers[FS::SBPump], GraphletAnchor::LT, xoff, -ps_oy * 2);
			this->station->map_credit_graphlet(this->rpms[FS::SBPump], GraphletAnchor::LT, xoff, 0);
			this->station->map_credit_graphlet(this->pressures[FS::SBPump], GraphletAnchor::LT, xoff, ps_oy*2);
			this->station->map_credit_graphlet(this->flows[FS::SBPump], GraphletAnchor::LT, xoff, ps_oy * 4);

			this->master->move_to(this->pressures[FS::D1], this->progresses[Door::PS5], GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, -gheight);
			this->master->move_to(this->pressures[FS::D2], this->progresses[Door::PS3], GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, -gheight);
		}
		{// 泵转速设定
			this->master->move_to(this->pumps_rpm[FS::PSPump], this->pumps[FS::PSPump], GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, -gheight);
			this->master->move_to(this->pumps_rpm[FS::SBPump], this->pumps[FS::SBPump], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, gheight*2);
		}
	}

public:
	bool pump_selected(FS pid) {
		return this->master->is_selected(this->pumps[pid]);
	}
	
	bool valve_selected(FS vid) {
		return this->master->is_selected(this->bfvalves[vid]);
	}

	bool valves_selected(FS vids[], unsigned int count, int tolerance = 0) {
		bool okay = false;
		int ok = 0;

		if (tolerance == 0) {
			tolerance = count;
		}

		for (unsigned int idx = 0; idx < count; idx++) {
			if (this->master->is_selected(this->bfvalves[vids[idx]])) {
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
	bool valves_selected(FS(&vids)[N], int tolerance) {
		return this->valves_selected(vids, N, tolerance);
	}

private:
	template<class G, typename E>
	void load_valves(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, std::map<E, Credit<Labellet, E>*>& cs
		, E id0, E idn, float radius, double degrees) {
		for (E id = id0; id <= idn; id++) {
			this->load_label(ls, id.ToString(), id, Colours::Silver, this->label_font);
			this->load_label(cs, id, Colours::Silver, this->label_font);

			gs[id] = this->master->insert_one(new G(radius, degrees), id);
		}
	}

	template<class D, typename E>
	void load_doors(std::map<E, Credit<D, E>*>& ds, std::map<E, Credit<Percentagelet, E>*>& ps, E id0, E idn, float radius) {
		for (E id = id0; id <= idn; id++) {
			ds[id] = this->master->insert_one(new Credit<D, E>(radius), id);
			ps[id] = this->master->insert_one(new Credit<Percentagelet, E>(this->percentage_style), id);
		}
	}

	template<class G, typename E>
	void load_pump(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, E id, float rx, double degrees) {
		this->load_label(ls, id, Colours::Salmon, this->caption_font);
	
		gs[id] = this->master->insert_one(new G(rx, degrees), id);

		this->load_dimension(this->powers, id, "kwatt", 0);
		this->load_dimension(this->rpms, id, "rpm", 0);
	}

	template<class B, typename CMD>
	void load_buttons(std::map<CMD, Credit<B, CMD>*>& bs, float width = 128.0F, float height = 32.0F) {
		for (CMD cmd = _E(CMD, 0); cmd < CMD::_; cmd++) {
			bs[cmd] = this->master->insert_one(new Credit<B, CMD>(cmd.ToString(), width, height), cmd);
		}
	}

	template<class S, typename E>
	void load_shift(std::map<E, Credit<S, E>*> &ss, E id, float size, double degrees) {
		ss[id] = this->master->insert_one(new Credit<S, E>(size, degrees, Colours::SpringGreen), id);
	}

	template<typename E>
	void load_dimension(std::map<E, Credit<Dimensionlet, E>*>& ds, E id, Platform::String^ unit, int precision) {
		this->metrics_style.precision = precision;
		ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(this->metrics_style, unit), id);
	}

	template<typename E>
	void load_dimensions(std::map<E, Credit<Dimensionlet, E>*>& ds, E id0, E idn, Platform::String^ unit) {
		for (E id = id0; id <= idn; id++) {
			ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(unit, id.ToString()), id);

			ds[id]->set_style(DimensionState::Default, this->pump_style);
			ds[id]->set_style(DimensionState::Highlight, this->highlight_style);
		}
	}

	template<typename E>
	void load_label(std::map<E, Credit<Labellet, E>*>& ls, Platform::String^ caption, E id
		, CanvasSolidColorBrush^ color, CanvasTextFormat^ font = nullptr) {
		ls[id] = this->master->insert_one(new Credit<Labellet, E>(caption, font, color), id);
	}

	template<typename E>
	void load_label(std::map<E, Credit<Labellet, E>*>& ls, E id, CanvasSolidColorBrush^ color, CanvasTextFormat^ font = nullptr) {
		this->load_label(ls, _speak(id), id, color, font);
	}
	template<typename E>
	void load_settings(std::map<E, Credit<Dimensionlet, E>*>& ds, E id0, E idn, Platform::String^ unit, Platform::String^ label) {
		for (E id = id0; id <= idn; id++) {
			ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(DimensionState::Input, this->setting_style, unit, label), id);
			ds[id]->set_maximum(double(gland_pump_rpm_range));
		}
	}

private:
	template<class D, typename E>
	void reflow_doors(std::map<E, Credit<D, E>*>& ds, std::map<E, Credit<Percentagelet, E>*>& ps, E id0, E idn, GraphletAnchor ya) {
		GraphletAnchor d_anchor = GraphletAnchor::CT;
		GraphletAnchor p_anchor = GraphletAnchor::CB;
		unsigned int distance = _I(FS::HBV9) - _I(id0);
		float x, y, py;

		this->master->fill_graphlet_location(this->hopper_room, nullptr, &y, ya);
		this->station->fill_anchor_location(FS::HBV9, &x, &py);
		if (y > py) { // Starboard
			d_anchor = GraphletAnchor::CB;
			p_anchor = GraphletAnchor::CT;
		}

		for (E id = id0; id <= idn; id++) {
			this->station->fill_anchor_location(_E(FS, _I(id) + distance), &x, nullptr);
			
			this->master->move_to(ds[id], x, y, d_anchor);
			this->master->move_to(ps[id], x, y, p_anchor);
		}
	}

	template<class D, typename E>
	void reflow_valve(float x0, float y0, float gridsize, E id, D* valve) {
		GraphletAnchor anchor;
		float label_height, margin, dx, dy, vy, hy;

		switch (id) {

			/*
		case FS::HBV6: {
			valve->fill_margin(x0, y0, nullptr, &margin, nullptr, nullptr);
			dx = x0 + gridsize - margin; dy = y0; anchor = GraphletAnchor::LB;//字在右
		}; break;*/
		case FS::HBV5:case FS::HBV6:case FS::HBV7:case FS::HBV8: case FS::HBV1:{
			valve->fill_margin(x0, y0, nullptr, nullptr, nullptr, &margin);
			dx = x0 - gridsize + margin; dy = y0; anchor = GraphletAnchor::RB;//字在左
		}; break;
			
		case FS::HBV2: {
			valve->fill_margin(x0, y0, nullptr, nullptr, &margin, nullptr);
			dx = x0; dy = y0 + gridsize - margin; anchor = GraphletAnchor::CT;  //字在下
		}; break;
		case FS::SBV1:case FS::SBV2:case FS::SBV3:case FS::SBV4:case FS::SBV5:case FS::DHV1: case FS::DHV2:case FS::HBV3:case FS::HBV4:{
			this->labels[id]->fill_extent(x0, y0, nullptr, &label_height);
			valve->fill_margin(x0, y0, &margin, nullptr, nullptr, nullptr);
			dx = x0; dy = y0 - gridsize - label_height + margin; anchor = GraphletAnchor::CB;//字在上
		}; break;
		default: { // HBV9 - HBV15
			this->labels[id]->fill_extent(x0, y0, nullptr, &label_height);
			this->station->fill_anchor_location(id, nullptr, &vy);
			this->station->fill_anchor_location(_E(FS, _I(id) - _I(FS::HBV11) + _I(FS::h11)), nullptr, &hy);
			//dx = x0; dy = (hy - vy) - label_height; anchor = GraphletAnchor::CB;
			dx = x0; dy = y0 - label_height * 5; anchor = GraphletAnchor::CB;
		}
		}

		this->station->map_credit_graphlet(valve, GraphletAnchor::CC, x0, y0);
		this->station->map_credit_graphlet(this->captions[id], anchor, dx, dy);
		this->master->move_to(this->labels[id], this->captions[id], GraphletAnchor::CB, GraphletAnchor::CT);
	}

private:
	void set_door_progress(Door id, float value) {
		this->uhdoors[id]->set_value(value / 100.0F);
		this->progresses[id]->set_value(value, GraphletAnchor::CC);

		AI_hopper_door(this->uhdoors[id], value, upper_door_open_threshold, upper_door_closed_threshold);
	}

private:
	void try_flow_water(FS vid, FS eid1, FS eid2, CanvasSolidColorBrush^ color) {
		switch (this->bfvalves[vid]->get_state()) {
		case GateValveState::Open: {
			this->station->push_subtrack(vid, eid1, color);

			if (eid2 != FS::_) {
				this->station->push_subtrack(vid, eid2, color);
			}
		}
		}
	}

	void try_flow_water(FS vid, FS eid, CanvasSolidColorBrush^ color) {
		this->try_flow_water(vid, eid, FS::_, color);
	}
	
	void try_flow_water(int count,  ...) {
		va_list arguments;
		FS cur = FS::_;
		FS old = FS::_;
		va_start(arguments, count);
		for (int i = 0; i < count+1; i++) {
			if (cur == FS::_) {
				cur = va_arg(arguments, FS);
			}
			else {
				if ((old != FS::_) && (cur != FS::_)) {
					this->station->push_subtrack(old, cur, water_color);
				}
				old = cur;
				cur = va_arg(arguments, FS);
			}
		}
		va_end(arguments);
	}

// never deletes these graphlets mannually
private:
	Tracklet<FS>* station;
	Tracklet<FS>* pipeline18;
	Tracklet<FS>* hopper_room;
	Tracklet<FS>* hopper_water;
	std::map<FS, Credit<Labellet, FS>*> captions;
	std::map<FS, Credit<Labellet, FS>*> labels;
	std::map<FS, Credit<WaterPumplet, FS>*> pumps;
	std::map<FS, Credit<GateValvelet, FS>*> bfvalves;
	std::map<FS, Credit<ManualValvelet, FS>*> mvalves;
	std::map<Door, Credit<UpperHopperDoorlet, Door>*> uhdoors;
	std::map<Door, Credit<Percentagelet, Door>*> progresses;
	std::map<FS, Credit<Dimensionlet, FS>*> pressures;
	std::map<FS, Credit<Dimensionlet, FS>*> powers;
	std::map<FS, Credit<Dimensionlet, FS>*> rpms;
	std::map<FS, Credit<Dimensionlet, FS>*> flows;
	std::map<FS, Credit<Dimensionlet, FS>*> pumps_rpm;//泵转速设定
	std::map<FS, Omegalet*> nintercs;
	Buttonlet* diagnostics;
	Segmentlet* ps_draghead;
	Segmentlet* sb_draghead;
	Hatchlet* ps_sea;
	Hatchlet* sb_sea;

private:
	std::map<FSFunction, Credit<Buttonlet, FSFunction>*> functions;
	std::map<FlushingCommand, Credit<Buttonlet, FlushingCommand>*> shifts;
	
private:
	CanvasTextFormat^ caption_font;
	CanvasTextFormat^ label_font;
	DimensionStyle percentage_style;
	DimensionStyle pump_style;
	DimensionStyle highlight_style;
	DimensionStyle metrics_style;
	DimensionStyle setting_style;

private:
	FlushsPage* master;
	MenuFlyout^ ps_menu;
	MenuFlyout^ sb_menu;
};

FlushsPage::FlushsPage(PLCMaster* plc) : Planet(__MODULE__), device(plc) {
	//Flush* dashboard = new Flush(this);

	//this->dashboard = dashboard;

	if (this->device != nullptr) {
		this->diagnostics = new WaterPumpDiagnostics(plc);

		this->gate_valve_op = make_butterfly_valve_menu(DO_butterfly_valve_action, plc);
		this->upper_door_op = make_upper_door_menu(plc);
		this->ps_pump_op = make_ps_water_pump_menu(plc);
		this->sb_pump_op = make_sb_water_pump_menu(plc);

		this->ps_ps_op = make_water_pump_condition_menu(WaterPumpConditionAction::PS_PS, plc);
		this->ps_sb_op = make_water_pump_condition_menu(WaterPumpConditionAction::PS_SB, plc);
		this->ps_2_op = make_water_pump_condition_menu(WaterPumpConditionAction::PS_2, plc);
		this->sb_ps_op = make_water_pump_condition_menu(WaterPumpConditionAction::SB_PS, plc);
		this->sb_sb_op = make_water_pump_condition_menu(WaterPumpConditionAction::SB_SB, plc);
		this->sb_2_op = make_water_pump_condition_menu(WaterPumpConditionAction::SB_2, plc);
		this->s2_ps_op = make_water_pump_condition_menu(WaterPumpConditionAction::S2_PS, plc);
		this->s2_sb_op = make_water_pump_condition_menu(WaterPumpConditionAction::S2_SB, plc);
		//this->s2_2_op = make_water_pump_condition_menu(WaterPumpConditionAction::S2_2, plc);
		//this->p2_2_op = make_water_pump_condition_menu(WaterPumpConditionAction::P2_2, plc);
		this->i2_2_op = make_water_pump_condition_menu(WaterPumpConditionAction::I2_2, plc);
		this->ps_h_op = make_water_pump_condition_menu(WaterPumpConditionAction::PS_H, plc);
		this->sb_h_op = make_water_pump_condition_menu(WaterPumpConditionAction::SB_H, plc);
		this->s2_h_op = make_water_pump_condition_menu(WaterPumpConditionAction::S2_H, plc);
		this->p2_h_op = make_water_pump_condition_menu(WaterPumpConditionAction::P2_H, plc);

		{ // only highlight menu items of these two menu 
			Flush* dashboard = new Flush(this, this->ps_pump_op, this->sb_pump_op);

			this->dashboard = dashboard;
			this->device->push_confirmation_receiver(dashboard);
		}
	}
	else {
		this->dashboard = new Flush(this);
	}
	{ // load decorators
		this->grid = new GridDecorator();

#ifdef _DEBUG
		this->push_decorator(this->grid);
#else
		this->grid->set_active_planet(this);
#endif
	}
}

FlushsPage::~FlushsPage() {
	if (this->dashboard != nullptr) {
		delete this->dashboard;
	}

	delete this->diagnostics;

#ifndef _DEBUG
	delete this->grid;
#endif
}

void FlushsPage::load(CanvasCreateResourcesReason reason, float width, float height) {
	auto dashboard = dynamic_cast<Flush*>(this->dashboard);
	
	if (dashboard != nullptr) {
		float gwidth = width / 64.0F;
		float gheight = height / 36.0F;

		this->grid->set_grid_width(gwidth);
		this->grid->set_grid_height(gheight);
		
		dashboard->construct(gwidth, gheight);
		dashboard->load(width, height, gwidth, gheight);
	}
}

void FlushsPage::reflow(float width, float height) {
	auto dashboard = dynamic_cast<Flush*>(this->dashboard);
	
	if (dashboard != nullptr) {
		float gwidth = this->grid->get_grid_width();
		float gheight = this->grid->get_grid_height();

		dashboard->reflow(width, height, gwidth, gheight);
	}
}

void FlushsPage::on_timestream(long long timepoint_ms, size_t addr0, size_t addrn, uint8* data, size_t size, Syslog* logger) {
	auto dashboard = dynamic_cast<Flush*>(this->dashboard);

	if (dashboard != nullptr) {
		dashboard->on_all_signals(timepoint_ms, addr0, addrn, data, size, logger);
	}
}

bool FlushsPage::can_select(IGraphlet* g) {
	return ((this->device != nullptr)
		&& ((dynamic_cast<GateValvelet*>(g) != nullptr)
			|| (dynamic_cast<UpperHopperDoorlet*>(g) != nullptr)
			|| (dynamic_cast<WaterPumplet*>(g) != nullptr)
			|| (dynamic_cast<ArrowHeadlet*>(g) != nullptr)
			|| button_enabled(g)));
}

bool FlushsPage::can_select_multiple() {
	return (this->device != nullptr);
}

void FlushsPage::on_tap_selected(IGraphlet* g, float local_x, float local_y) {
	auto gvalve = dynamic_cast<GateValvelet*>(g);
	auto uhdoor = dynamic_cast<UpperHopperDoorlet*>(g);
	auto wpump = dynamic_cast<Credit<WaterPumplet, FS>*>(g);
	auto shift = dynamic_cast<Credit<Buttonlet, FlushingCommand>*>(g);
	auto diagnose = dynamic_cast<Credit<Buttonlet, FSFunction>*>(g);

	if (gvalve != nullptr) {
		menu_popup(this->gate_valve_op, g, local_x, local_y);
	} else if (uhdoor != nullptr) {
		menu_popup(this->upper_door_op, g, local_x, local_y);
	} else if (shift != nullptr) {
		this->device->send_command((shift->id == FlushingCommand::LeftShift) ? left_shifting_command : right_shifting_command);
	} else if (diagnose != nullptr) {
		this->diagnostics->show();
	} else if (wpump != nullptr) {
		switch (wpump->id) {
		case FS::PSPump: menu_popup(this->ps_pump_op, g, local_x, local_y); break;
		case FS::SBPump: menu_popup(this->sb_pump_op, g, local_x, local_y); break;
		}
	}
}


void FlushsPage::on_gesture(std::list<float2>& anchors, float x, float y) {
}


bool FlushsPage::on_key(VirtualKey key, bool wargrey_keyboard) {
	bool handled = Planet::on_key(key, wargrey_keyboard);

	if ((!handled) && (this->device != nullptr) && (this->device->authorized())) {
		auto db = dynamic_cast<Flush*>(this->dashboard);

		if (db != nullptr) {
			handled = db->on_key(key, this->device);
		}
	}

	return handled;
}

void FlushsPage::on_focus(IGraphlet* g, bool yes) {
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