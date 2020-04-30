#include <map>

#include "page/discharges.hpp"
#include "configuration.hpp"
#include "menu.hpp"

#include "module.hpp"
#include "text.hpp"
#include "paint.hpp"
#include "brushes.hxx"
#include "turtle.hpp"

#include "graphlet/shapelet.hpp"
#include "graphlet/ui/statuslet.hpp"
#include "graphlet/dashboard/alarmlet.hpp"
#include "graphlet/device/winchlet.hpp"

#include "graphlet/symbol/door/hatchlet.hpp"
#include "graphlet/symbol/door/hopper_doorlet.hpp"
#include "graphlet/symbol/pump/hopper_pumplet.hpp"
#include "graphlet/symbol/valve/gate_valvelet.hpp"
#include "graphlet/symbol/valve/tagged_valvelet.hpp"

#include "graphlet/cylinder/boltlet.hpp"
#include "graphlet/cylinder/holdhooplet.hpp"

#include "iotables/di_winches.hpp"
#include "iotables/di_pumps.hpp"
#include "iotables/di_hopper_pumps.hpp"
#include "iotables/di_valves.hpp"
#include "iotables/di_doors.hpp"

#include "iotables/ai_winches.hpp"
#include "iotables/ai_valves.hpp"
#include "iotables/ai_pumps.hpp"
#include "iotables/ai_hopper_pumps.hpp"
#include "iotables/ai_doors.hpp"

#include "iotables/do_doors.hpp"
#include "iotables/do_valves.hpp"
#include "iotables/do_winches.hpp"
#include "iotables/do_hopper_pumps.hpp"

#include "decorator/vessel.hpp"
#include <iotables\ao_pumps.hpp>

using namespace WarGrey::SCADA;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Numerics;
using namespace Windows::System;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::Brushes;
using namespace Microsoft::Graphics::Canvas::Geometry;

// WARNING: order matters
//ValvesConditionAction CurrentWorkingMode = ValvesConditionAction::_;
private enum class RS : unsigned int {
	//竖
	HV13, HV12, HV14, HV01, HV02, HV07, HV08, HV24,
	//横
	HV09, HV15, HV16, HV05, HV06, HV03, HV04, HV10, HV11, HV23, HV25, HV26,
	//
	HV17, HV18, HV19, HV20, HV21, HV22,
	A1, A2, C1, C2,
	//HBV9,HBV10,HBV11,HBV12,HBV13,HBV14,HBV15,
	M1, M2, M3, M4, M5, M6, M7,
	diagnostics,//诊断
	//舱内泵
	PSHPump, SBHPump,
	//左右耙头
	Port, Starboard,
	//绿点
	h19, h20, h17, h18, h3, h4, h0708, h0105, h0206, h2324, h25, h1315, h1216, h0307, h0408,to2526,h25end,
	//未命名
	_,
	alt, alb, art, arb, h1123, h10, h26, rh25,albr,/*rh26,*/
	tpump, bpump,
	ps, sb,
	deck_lx, deck_rx, deck_ty, deck_by, shd_joint,
	//HBV11, HBV12, HBV13, HBV14, HBV15, HBV16, HBV17,
	Hatch,
	h11, lb11, rb11,
	lTower, rTower, water, room,
	//弯
	jump0107, jump0802, jump1314,
	jump0107left, jump0107right,
	jump0802left, jump0802right,
	jump1314up, jump1314down,

	jump0107up, jump0107down,
	jump0802up, jump0802down,
	jump1314left, jump1314right,
};

static CanvasSolidColorBrush^ water_color = Colours::Green;
static CanvasSolidColorBrush^ door_pairing_color = Colours::Green;

static uint16 DO_gate_valve_action(GateValveAction cmd, GateValvelet* valve) {
	uint16 index = 0U;
	auto credit_valve = dynamic_cast<Credit<GateValvelet, RS>*>(valve);

	if (credit_valve != nullptr) {
		index = DO_gate_valve_command(cmd, credit_valve->id);
	}

	return index;
}
static uint16 DO_motor_valve_action(MotorValveAction cmd, GateValvelet* valve) {
	uint16 index = 0U;
	auto credit_valve = dynamic_cast<Credit<GateValvelet, RS>*>(valve);

	if (credit_valve != nullptr) {
		index = DO_motor_valve_command(cmd, credit_valve->id);
	}

	return index;
}

/*************************************************************************************************/
private class Rainbows final : public PLCConfirmation {
public:
	Rainbows(DischargesPage* master) : master(master) {
		this->setting_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::GhostWhite, Colours::RoyalBlue);
	}

public:
	void pre_read_data(Syslog* logger) override {
		this->master->enter_critical_section();
		this->master->begin_update_sequence();

		this->station->clear_subtacks();
	}

	void on_analog_input(long long timepoint_ms, const uint8* DB2, size_t count2, const uint8* DB203, size_t count203, Syslog* logger) override {
		this->pump_pressures[RS::A1]->set_value(RealData(DB203, pump_A1_pressure), GraphletAnchor::LB);
		this->pump_pressures[RS::A2]->set_value(RealData(DB203, pump_A2_pressure), GraphletAnchor::LT);
		this->pump_pressures[RS::C1]->set_value(RealData(DB203, pump_C1_pressure), GraphletAnchor::LB);
		this->pump_pressures[RS::C2]->set_value(RealData(DB203, pump_C2_pressure), GraphletAnchor::LT);

		//!!!注意一下 应该有的有
		//this->winch_pressures[ShipSlot::BowWinch]->set_value(RealData(DB203, bow_anchor_winch_pressure), GraphletAnchor::CC);
		//this->winch_pressures[ShipSlot::SternWinch]->set_value(RealData(DB203, stern_anchor_winch_pressure), GraphletAnchor::CC);
		//this->winch_pressures[ShipSlot::ShoreWinch]->set_value(RealData(DB203, shore_discharge_winch_pressure), GraphletAnchor::CC);
		//this->winch_pressures[ShipSlot::BargeWinch]->set_value(RealData(DB203, barge_winch_pressure), GraphletAnchor::CC);

		//!!!注意一下 这个不知道为什么会报错
		this->gvprogresses[RS::HV09]->set_value(RealData(DB203, gate_valve_HV09_progress), GraphletAnchor::CT);
		this->gvprogresses[RS::HV01]->set_value(RealData(DB203, gate_valve_HV01_progress), GraphletAnchor::LB);
		this->gvprogresses[RS::HV02]->set_value(RealData(DB203, gate_valve_HV02_progress), GraphletAnchor::LT);

		this->powers[RS::PSHPump]->set_value(RealData(DB203, ps_hopper_pump_power), GraphletAnchor::RC);
		this->rpms[RS::PSHPump]->set_value(RealData(DB203, ps_hopper_pump_rpm), GraphletAnchor::LC);
		this->dpressures[RS::PSHPump]->set_value(RealData(DB203, ps_hopper_pump_discharge_pressure), GraphletAnchor::LB);
		this->vpressures[RS::PSHPump]->set_value(RealData(DB203, ps_hopper_pump_vacuum_pressure), GraphletAnchor::RB);

		this->powers[RS::SBHPump]->set_value(RealData(DB203, sb_hopper_pump_power), GraphletAnchor::RC);
		this->rpms[RS::SBHPump]->set_value(RealData(DB203, sb_hopper_pump_rpm), GraphletAnchor::LC);
		this->dpressures[RS::SBHPump]->set_value(RealData(DB203, sb_hopper_pump_discharge_pressure), GraphletAnchor::LT);
		this->vpressures[RS::SBHPump]->set_value(RealData(DB203, sb_hopper_pump_vacuum_pressure), GraphletAnchor::RT);


		{ // door progresses
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
	}

	void on_digital_input(long long timepoint_ms, const uint8* DB4, size_t count4, const uint8* DB205, size_t count205, Syslog* logger) override {
		DI_hopper_pump(this->hoppers[RS::PSHPump], DB4, ps_hopper_pump_feedback, DB205, ps_hopper_pump_details);
		DI_hopper_pump(this->hoppers[RS::SBHPump], DB4, sb_hopper_pump_feedback, DB205, sb_hopper_pump_details);

		DI_hydraulic_pump_dimension(this->pump_pressures[RS::A1], DB4, pump_A1_feedback);
		DI_hydraulic_pump_dimension(this->pump_pressures[RS::A2], DB4, pump_A2_feedback);
		DI_hydraulic_pump_dimension(this->pump_pressures[RS::C1], DB4, pump_C1_feedback);
		DI_hydraulic_pump_dimension(this->pump_pressures[RS::C2], DB4, pump_C2_feedback);

		//!!!这个注意一下 应该有一部分有
		//DI_winch(this->winches[ShipSlot::ShoreWinch], DB205, shore_discharge_winch_details);
		//DI_winch(this->winches[ShipSlot::BowWinch], DB4, bow_anchor_winch_feedback, DB205, bow_anchor_winch_details);
		//DI_winch(this->winches[ShipSlot::SternWinch], DB4, stern_anchor_winch_feedback, DB205, stern_anchor_winch_details);
		//DI_winch(this->winches[ShipSlot::BargeWinch], DB4, barge_winch_feedback, barge_winch_limits, DB205, barge_winch_details);

		DI_bolt(this->bolts[RS::shd_joint], DB4, shore_discharge_bolt_feedback);////艏吹抱箍油缸 夹紧松开命令
		DI_holdhoop(this->holdhoops[RS::shd_joint], DB4, shore_discharge_holdhoop_feedback);//艏吹插销油缸 插入拔出命令

		//this->alarms[RS::BowCTension]->set_state(DI_winch_constant_tension(DB205, bow_anchor_winch_details), AlarmState::Notice, AlarmState::None);////艏靠驳绞车 反馈
		//this->alarms[RS::SternCTension]->set_state(DI_winch_constant_tension(DB205, stern_anchor_winch_details), AlarmState::Notice, AlarmState::None);////艉靠驳绞车 反馈

		//this->bolts[RS::Barge]->set_running(DI_winch_locker_open(DB205, barge_winch_feedback));
		//this->bolts[RS::Barge]->set_state(DI_winch_locked(DB205, barge_winch_details), BoltState::SlidedIn, BoltState::SlidedOut);


		//this->set_valves_status(RS::HV01, DB4, gate_valve_HV01_feedback, motor_valve_HV01_feedback, DB205, gate_valve_HV01_status, motor_valve_HV01_status);
		//this->set_valves_status(RS::HV02, DB4, gate_valve_HV02_feedback, motor_valve_HV02_feedback, DB205, gate_valve_HV02_status, motor_valve_HV02_status);
		this->set_valves_status(RS::HV03, DB4, gate_valve_HV03_feedback, motor_valve_HV03_feedback, DB205, gate_valve_HV03_status, motor_valve_HV03_status);
		this->set_valves_status(RS::HV04, DB4, gate_valve_HV04_feedback, motor_valve_HV04_feedback, DB205, gate_valve_HV04_status, motor_valve_HV04_status);
		this->set_valves_status(RS::HV05, DB4, gate_valve_HV05_feedback, motor_valve_HV05_feedback, DB205, gate_valve_HV05_status, motor_valve_HV05_status);
		this->set_valves_status(RS::HV06, DB4, gate_valve_HV06_feedback, motor_valve_HV06_feedback, DB205, gate_valve_HV06_status, motor_valve_HV06_status);
		this->set_valves_status(RS::HV07, DB4, gate_valve_HV07_feedback, motor_valve_HV07_feedback, DB205, gate_valve_HV07_status, motor_valve_HV07_status);
		this->set_valves_status(RS::HV08, DB4, gate_valve_HV08_feedback, motor_valve_HV08_feedback, DB205, gate_valve_HV08_status, motor_valve_HV08_status);
		//this->set_valves_status(RS::HV09, DB4, gate_valve_HV09_feedback, motor_valve_HV09_feedback, DB205, gate_valve_HV09_status, motor_valve_HV09_status);
		this->set_valves_status(RS::HV10, DB4, gate_valve_HV10_feedback, motor_valve_HV10_feedback, DB205, gate_valve_HV10_status, motor_valve_HV10_status);
		this->set_valves_status(RS::HV11, DB4, gate_valve_HV11_feedback, motor_valve_HV11_feedback, DB205, gate_valve_HV11_status, motor_valve_HV11_status);
		this->set_valves_status(RS::HV12, DB4, gate_valve_HV12_feedback, motor_valve_HV12_feedback, DB205, gate_valve_HV12_status, motor_valve_HV12_status);
		this->set_valves_status(RS::HV13, DB4, gate_valve_HV13_feedback, motor_valve_HV13_feedback, DB205, gate_valve_HV13_status, motor_valve_HV13_status);
		this->set_valves_status(RS::HV14, DB4, gate_valve_HV14_feedback, motor_valve_HV14_feedback, DB205, gate_valve_HV14_status, motor_valve_HV14_status);
		this->set_valves_status(RS::HV15, DB4, gate_valve_HV15_feedback, motor_valve_HV15_feedback, DB205, gate_valve_HV15_status, motor_valve_HV15_status);
		this->set_valves_status(RS::HV16, DB4, gate_valve_HV16_feedback, motor_valve_HV16_feedback, DB205, gate_valve_HV16_status, motor_valve_HV16_status);
		this->set_valves_status(RS::HV23, DB4, gate_valve_HV23_feedback, motor_valve_HV23_feedback, DB205, gate_valve_HV23_status, motor_valve_HV23_status);
		this->set_valves_status(RS::HV24, DB4, gate_valve_HV24_feedback, motor_valve_HV24_feedback, DB205, gate_valve_HV24_status, motor_valve_HV24_status);
		this->set_valves_status(RS::HV25, DB4, gate_valve_HV25_feedback, motor_valve_HV25_feedback, DB205, gate_valve_HV25_status, motor_valve_HV25_status);
		this->set_valves_status(RS::HV26, DB4, gate_valve_HV26_feedback, motor_valve_HV26_feedback, DB205, gate_valve_HV26_status, motor_valve_HV26_status);


		DI_motor_valve(this->gvalves[RS::M1], DB4, motor_valve_M1_feedback, DB205, motor_valve_M1_status);
		DI_motor_valve(this->gvalves[RS::M2], DB4, motor_valve_M2_feedback, DB205, motor_valve_M2_status);
		DI_motor_valve(this->gvalves[RS::M3], DB4, motor_valve_M3_feedback, DB205, motor_valve_M3_status);
		DI_motor_valve(this->gvalves[RS::M4], DB4, motor_valve_M4_feedback, DB205, motor_valve_M4_status);
		DI_motor_valve(this->gvalves[RS::M5], DB4, motor_valve_M5_feedback, DB205, motor_valve_M5_status);
		DI_motor_valve(this->gvalves[RS::M6], DB4, motor_valve_M6_feedback, DB205, motor_valve_M6_status);
		DI_motor_valve(this->gvalves[RS::M7], DB4, motor_valve_M7_feedback, DB205, motor_valve_M7_status);

		DI_gate_valve(this->gvalves[RS::HV09], DB205, gate_valve_HV09_feedback, DB205, gate_valve_HV09_status);
		//DI_gate_valve(this->gvalves[RS::HV01], DB205, gate_valve_HV01_feedback, DB205, gate_valve_HV01_status);!!!!!!!!!!!!!!这个应该有 但是没找到;
		DI_gate_valve(this->gvalves[RS::HV01], DB205, gate_valve_HV01_feedback, DB205, gate_valve_HV01_status);//
		DI_motor_valve(this->mvalves[RS::HV01], DB4, motor_valve_HV01_feedback, DB205, motor_valve_HV01_status);

		//DI_gate_valve(this->gvalves[RS::HV02], DB205, gate_valve_HV02_feedback, DB205, gate_valve_HV02_status);!!!!!!!!!!!!!!这个应该有 但是没找到
		DI_gate_valve(this->gvalves[RS::HV02], DB205, gate_valve_HV02_feedback, DB205, gate_valve_HV02_status);
		DI_motor_valve(this->mvalves[RS::HV02], DB4, motor_valve_HV02_feedback, DB205, motor_valve_HV02_status);

		//没有小泥门

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

		this->door_paired_color = (DBX(DB205, upper_door_paired - 1U) ? door_pairing_color : this->relationship_color);
		CurrentWorkingMode = DI_getvalveworkingmode(DB205);
	}

	void on_forat(long long timepoint_ms, const uint8* DB20, size_t count, Syslog* logger) override {
		this->pumps_rpm[RS::PSHPump]->set_value(DBD(DB20, 650U), GraphletAnchor::LB);//左舱内
		this->pumps_rpm[RS::SBHPump]->set_value(DBD(DB20, 662U), GraphletAnchor::LB);//右舱内
	}
	void post_read_data(Syslog* logger) override {
		
		{
			switch (CurrentWorkingMode) {
			case ValvesConditionAction::PSHPumpBowblow://左舱内泵艏吹
				try_flow_water(9, RS::HV09,RS::h0708, RS::HV03, RS::PSHPump, RS::HV23, RS::to2526, RS::h26,RS::HV26, RS::shd_joint);

				break;
			case ValvesConditionAction::SBHPumpBowblow://右舱内泵艏吹
				try_flow_water(10, RS::HV09, RS::h0708, RS::HV04, RS::SBHPump, RS::h10,RS::HV24, RS::to2526, RS::h26, RS::HV26, RS::shd_joint);
				break;
			case ValvesConditionAction::PSSBHPumpSeriesBowblow://左右泥泵串联艏吹
				try_flow_water(11, RS::HV09, RS::h0708, RS::HV08, RS::SBHPump, RS::h10, RS::jump0802right, RS::jump0802left, RS::jump1314down, RS::jump1314up, RS::HV13, RS::jump0107up);
				try_flow_water(7, RS::jump0107down ,RS::HV03, RS::PSHPump, RS::HV23, RS::to2526, RS::h26, RS::shd_joint);
				this->nintercs[RS::jump1314]->set_color(water_color);
				this->nintercs[RS::jump0107]->set_color(water_color);
				break;
			case ValvesConditionAction::PSHPumpStemSpray://左泥泵艏喷

				try_flow_water(9, RS::HV09, RS::h0708, RS::HV03, RS::PSHPump, RS::HV23, RS::to2526, RS::h25, RS::HV25, RS::h25end);
				break;
			case ValvesConditionAction::SBHPumpStemSpray://右泥泵艏喷
				try_flow_water(9, RS::HV09, RS::h0708, RS::HV04, RS::SBHPump, RS::h10, RS::HV24, RS::to2526, RS::HV25, RS::h25end);
				break;
			case ValvesConditionAction::PSSBHPumpSeriesStemSpray://左右泥泵串联艏喷

				try_flow_water(12, RS::HV09, RS::h0708, RS::HV08, RS::SBHPump, RS::h10, RS::jump0802right, RS::jump0802left,RS::albr, RS::jump1314down, RS::jump1314up, RS::HV13, RS::jump0107up);
				try_flow_water(7, RS::jump0107down, RS::HV03, RS::PSHPump, RS::HV23, RS::to2526, RS::h25, RS::h25end);
				this->nintercs[RS::jump1314]->set_color(water_color);
				this->nintercs[RS::jump0107]->set_color(water_color);
				break;
			}
		}
		this->master->end_update_sequence();
		this->master->leave_critical_section();
	}

public:
	void construct(float gwidth, float gheight) {
		this->caption_font = make_bold_text_format("Microsoft YaHei", normal_font_size);
		this->label_font = make_bold_text_format("Microsoft YaHei", small_font_size);
		this->pump_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0U, Colours::Background);
		this->highlight_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0U, Colours::Green);
		this->relationship_style = make_dash_stroke(CanvasDashStyle::DashDot);
		this->relationship_color = Colours::DarkGray;
		this->door_paired_color = this->relationship_color;

		this->metrics_style.number_font = make_bold_text_format("Cambria Math", large_metrics_font_size);
		this->metrics_style.unit_font = make_bold_text_format("Cambria", normal_font_size);

		this->percentage_style.precision = 1;
		this->percentage_style.number_font = make_bold_text_format("Cambria Math", large_metrics_font_size);
	}

	bool on_key(VirtualKey key, PLCMaster* plc) {//
		bool handled = false;

		if (key == VirtualKey::Enter) {
			auto editor = dynamic_cast<Credit<Dimensionlet, RS>*>(this->master->get_focus_graphlet());

			if (editor != nullptr) {
				float speed = float(editor->get_input_number());

				if (speed >= 0.0F) {
					plc->send_setting(AO_discharges_pump_setting(editor->id), speed);
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
		//Turtle<RS>* pTurtle = new Turtle<RS>(gwidth, gheight, false, RS::shd_joint);

		Turtle<RS>* pTurtle = new Turtle<RS>(gwidth, gheight, false, RS::shd_joint);
		Turtle<RS>* rTurtle = new Turtle<RS>(gwidth, gheight, false);
		Turtle<RS>* wTurtle = new Turtle<RS>(gwidth, gheight, false);

		////
		/////////////////////////////////////////////////////////////////////////////////////////////////
		pTurtle->move_left(2, RS::HV26)->move_left(2, RS::h26);
		pTurtle->jump_back(RS::HV26)->jump_up(5)->jump_right(2, RS::h25end)->move_left(2, RS::HV25)->move_left(2, RS::h25);
		pTurtle->jump_back(RS::h26)->move_to(RS::h25)->move_up(3,RS::to2526)->move_left(2, RS::h2324);
		pTurtle->move_left(2, RS::HV23)->move_left(5, RS::tpump)->move_left(2, RS::HV11)->move_left(2, RS::art);//到斜线
		pTurtle->jump_up(6, RS::diagnostics)->jump_back(RS::art);

		pTurtle->jump_down(2)->jump_left(8, RS::alt);//标记
		pTurtle->jump_back(RS::art)->move_to(RS::alt)->move_left(3, RS::HV15);

		pTurtle->jump_back(RS::h2324)->move_down(10, RS::HV24)->move_down(6)->move_left(7, RS::bpump)->move_left(2, RS::HV10)->move_left(2, RS::arb);
		pTurtle->jump_up(2)->jump_left(8, RS::alb);//标记
		pTurtle->jump_back(RS::arb)->move_to(RS::alb)->move_left(3, RS::HV16);
		//pTurtle->jump_back(RS::HV15)->jump_down(6)->jump_right(9)->move_left(36, RS::HV09)->move_left(4, RS::Hatch);
		float offset = 3.94;
		pTurtle->jump_back(RS::HV15)->jump_down(6)->jump_right(9)->
			move_left(11, RS::M1)->
			move_left(offset, RS::M2)->
			move_left(offset, RS::M3)->
			move_left(offset, RS::M4)->
			move_left(offset, RS::M5)->
			move_left(offset, RS::M6)->
			move_left(offset, RS::M7)->
			move_left(2, RS::HV09)->move_left(4, RS::Hatch);
		/*
		pTurtle->jump_back(RS::HV15)->jump_down(6)->jump_right(9)->move_left(9)->move_left(4.75f, RS::HBV11)
			->move_left(3.5f, RS::HBV12)
			->move_left(3.5f, RS::HBV13)
			->move_left(3.5f, RS::HBV14)
			->move_left(3.5f, RS::HBV15)
			->move_left(3.5f, RS::HBV16)
			->move_left(3.5f, RS::HBV17)
			->move_left(3.5f, RS::HV09)
			->move_left(2.5f, RS::Hatch);*/
		pTurtle->jump_back(RS::alt)->jump_down(6)->jump_right(6, RS::h0708);
		//pTurtle->jump_back(RS::HV15)->jump_down(6)->jump_left(28)->move_right(2, RS::HV09)//->move_right(35, RS::h0708);//中间线
			/*->move_right(3.5f, RS::Gatevalves7)
			->move_right(3.5f, RS::Gatevalves6)
			->move_right(3.5f, RS::Gatevalves5)
			->move_right(3.5f, RS::Gatevalves4)
			->move_right(3.5f, RS::Gatevalves3)
			->move_right(3.5f, RS::Gatevalves2)
			->move_right(3.5f, RS::Gatevalves1)
			->move_right(7.5f,RS::h0708);*/
		pTurtle->move_up(2, RS::HV07)->move_up(2, RS::h0307)->move_up(3)->jump_up(0.5F, RS::jump0107)->jump_up(0.5F)->move_up(2, RS::h0105)->move_up(2, RS::HV01)->move_up(RS::ps)->move_up(2, RS::Port);
		pTurtle->jump_back(RS::h0105)->move_left(3, RS::HV05)->move_left(3, RS::HV13)->move_down(4, RS::h1315)->move_down(5.5F)->jump_down(0.5F, RS::jump1314)->jump_down(0.5F)->move_down(2.5F, RS::HV14)->move_down(3, RS::h1216)->move_down(4, RS::HV12)->move_right(3, RS::HV06)->move_right(3, RS::h0206)->move_up(2)->jump_up(0.5F, RS::jump0802)->jump_up(0.5F)->move_up(3, RS::h0408)->move_up(2, RS::HV08)->move_up(2);
		pTurtle->jump_back(RS::h0206)->move_down(2, RS::HV02)->move_down(RS::sb)->move_down(2, RS::Starboard);

		pTurtle->jump_back(RS::Starboard)->jump_left(RS::A1);

		pTurtle->jump_back(RS::h0307)->move_right(2, RS::HV03)->move_right(5, RS::PSHPump)->move_up(4, RS::h1123);
		pTurtle->jump_back(RS::h0408)->move_right(2, RS::HV04)->move_right(5, RS::SBHPump)->move_down(4, RS::h10);


		pTurtle->jump_back(RS::h1315)->jump_down(6);
		pTurtle->jump_back(RS::jump0107)->jump_left(0.1F, RS::jump0107left);
		pTurtle->jump_back(RS::jump0107)->jump_right(0.1F, RS::jump0107right);
		pTurtle->jump_back(RS::jump0107)->jump_up(0.1F, RS::jump0107up);
		pTurtle->jump_back(RS::jump0107)->jump_down(0.1F, RS::jump0107down);

		pTurtle->jump_back(RS::jump0802)->jump_left(0.1F, RS::jump0802left);
		pTurtle->jump_back(RS::jump0802)->jump_right(0.1F, RS::jump0802right);
		pTurtle->jump_back(RS::jump0802)->jump_up(0.1F, RS::jump0802up);
		pTurtle->jump_back(RS::jump0802)->jump_down(0.1F, RS::jump0802down);

		pTurtle->jump_back(RS::jump1314)->jump_left(0.1F, RS::jump1314left);
		pTurtle->jump_back(RS::jump1314)->jump_right(0.1F, RS::jump1314right);
		pTurtle->jump_back(RS::jump1314)->jump_up(0.1F, RS::jump1314up);
		pTurtle->jump_back(RS::jump1314)->jump_down(0.1F, RS::jump1314down);

		pTurtle->jump_back(RS::alb)->jump_right(0.01F,RS::albr);
		////

		this->station = this->master->insert_one(new Tracklet<RS>(pTurtle, default_pipe_thickness, default_pipe_color));

		{ // load winches and cylinders //加载绞车有油缸锁 //!!!!!!!

			//this->load_winches(this->winches, this->winch_labels, radius * 3.2F); 绞车
			//this->load_alarms(this->alarms, this->alabels, RS::BowCTension, RS::SternCTension, radius);//恒张力!!!!

			this->holdhoops[RS::shd_joint] = this->master->insert_one(new Credit<HoldHooplet, RS>(radius), RS::shd_joint);
			this->bolts[RS::shd_joint] = this->master->insert_one(new Credit<Boltlet, RS>(radius, 90.0), RS::shd_joint);
			//this->bolts[RS::Barge] = this->master->insert_one(new Credit<Boltlet, RS>(radius), RS::Barge);
		}

		{ // load doors and valves
			this->load_doors(this->uhdoors, this->progresses, Door::PS1, Door::PS7, radius);
			this->load_doors(this->uhdoors, this->progresses, Door::SB1, Door::SB7, radius);

			this->load_valves(this->gvalves, this->mvalves, this->vlabels, this->captions, RS::HV09, RS::HV26, radius, 00.0);
			this->load_valves(this->gvalves, this->mvalves, this->vlabels, this->captions, RS::HV13, RS::HV24, radius, 90.0);

			this->load_valves(this->gvalves, this->mvalves, this->captions, RS::M1, RS::M7, radius, 0.0);
		}

		{ // load special nodes
			float sct_radius = radius * 0.5F;
			float nic_radius = radius * 0.25F;

			this->load_pump(this->hoppers, this->captions, RS::PSHPump, -radius, +2.0F);
			this->load_pump(this->hoppers, this->captions, RS::SBHPump, -radius, -2.0F);
			this->ps_suction = this->master->insert_one(new Circlelet(sct_radius, default_ps_color, default_pipe_thickness));//左吸口
			this->sb_suction = this->master->insert_one(new Circlelet(sct_radius, default_sb_color, default_pipe_thickness));//右吸口
			this->sea_inlet = this->master->insert_one(new Hatchlet(radius * 2.0F));

			/*
			for (RS id = RS::I0723; id <= RS::I0923; id++) {
				this->intercs[id] = this->master->insert_one(
					new Circlelet(default_pipe_thickness * 2.0F, Colours::Green));//半圆
			}*/

			for (RS id = RS::jump0107; id <= RS::jump1314; id++) {
				this->nintercs[id] = this->master->insert_one(
					new Omegalet(-90.0, nic_radius, default_pipe_thickness, default_pipe_color));
			}
		}

		{ // load labels and dimensions
			this->load_percentage(this->gvprogresses, RS::HV01);
			this->load_percentage(this->gvprogresses, RS::HV02);
			this->load_percentage(this->gvprogresses, RS::HV09);//可能有百分比!
			this->load_dimensions(this->pump_pressures, RS::A1, RS::C2, "bar");

			this->load_label(this->captions, RS::Hatch, Colours::SeaGreen, this->caption_font);//海水门!
			//this->load_label(this->captions, RS::Barge, Colours::Yellow, this->caption_font)//装驳架
			//泥门文本
			for (size_t idx = 0; idx < hopper_count; idx++) {
				Platform::String^ id = (idx + 1).ToString();

				this->ps_seqs[idx] = this->master->insert_one(new Labellet(speak("PS" + id), this->caption_font, Colours::Silver));
				this->sb_seqs[idx] = this->master->insert_one(new Labellet(speak("SB" + id), this->caption_font, Colours::Silver));
			}
		}
		//转速设定
		this->load_settings(this->pumps_rpm, RS::PSHPump, RS::SBHPump, "RPM", "S");
		this->load_buttons(this->functions,RS::diagnostics);
	}

public:
	void reflow(float width, float height, float gwidth, float gheight) {
		GraphletAnchor anchor;
		float dx, dy, margin, label_height, ox, oy;
		float gridsize = resolve_gridsize(gwidth, gheight);
		float x0 = 0.0F;
		float y0 = 0.0F;

		this->master->move_to(this->station, width * 0.5F, height * 0.5F, GraphletAnchor::CC);
		//this->station->map_graphlet_at_anchor(this->manual_pipe, RS::d0225, GraphletAnchor::CB);!

		//this->station->map_credit_graphlet(this->captions[RS::Barge], GraphletAnchor::CB);!
		this->station->map_graphlet_at_anchor(this->ps_suction, RS::Port, GraphletAnchor::CB);
		this->station->map_graphlet_at_anchor(this->sb_suction, RS::Starboard, GraphletAnchor::CT);
		this->station->map_graphlet_at_anchor(this->sea_inlet, RS::Hatch, GraphletAnchor::CC);
		this->master->move_to(this->captions[RS::Hatch], this->sea_inlet, GraphletAnchor::CB, GraphletAnchor::CT);

		for (auto it = this->intercs.begin(); it != this->intercs.end(); it++) {
			this->station->map_graphlet_at_anchor(it->second, it->first, GraphletAnchor::CC);
		}

		for (auto it = this->nintercs.begin(); it != this->nintercs.end(); it++) {
			/** NOTE
			 * Lines are brush-based shape, they do not have stroke, `Shapelet` does not know how width they are,
			 * thus, we have to do aligning on our own.
			 */
			this->station->map_graphlet_at_anchor(it->second, it->first, GraphletAnchor::LC, -default_pipe_thickness * 0.5F);
		}

		for (auto it = this->hoppers.begin(); it != this->hoppers.end(); it++) {
			it->second->fill_pump_origin(&ox);
			this->station->map_credit_graphlet(it->second, GraphletAnchor::CC, -ox);

			ox = flabs(ox);
			switch (it->first) {
			case RS::PSHPump: {
				this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::RC, GraphletAnchor::LC, ox);
				this->master->move_to(this->powers[it->first], it->second, GraphletAnchor::LB, GraphletAnchor::RB, -ox);
				this->master->move_to(this->rpms[it->first], it->second, GraphletAnchor::RB, GraphletAnchor::LB, ox);
				this->master->move_to(this->dpressures[it->first], it->second, GraphletAnchor::CT, GraphletAnchor::LB);
				this->master->move_to(this->vpressures[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RB, -ox);
			}; break;
			case RS::SBHPump: {
				this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::RC, GraphletAnchor::LC, ox);
				this->master->move_to(this->powers[it->first], it->second, GraphletAnchor::LT, GraphletAnchor::RT, -ox);
				this->master->move_to(this->rpms[it->first], it->second, GraphletAnchor::RT, GraphletAnchor::LT, ox);
				this->master->move_to(this->dpressures[it->first], it->second, GraphletAnchor::CB, GraphletAnchor::LT);
				this->master->move_to(this->vpressures[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RT, -ox);
			}; break;
			}
		}

		//this->vlabels[RS::D001]->fill_extent(0.0F, 0.0F, nullptr, &label_height);//!!!!!!!!!!!!!!这是什么

		for (auto it = this->gvalves.begin(); it != this->gvalves.end(); it++) {
			switch (it->first) {

			case RS::HV15:case RS::HV25:case RS::HV26:case RS::HV04:case RS::HV03:case RS::HV05:case RS::HV23: //下
				dx = x0; dy = y0 + gheight; anchor = GraphletAnchor::CB;
				break;
			case RS::HV16:case RS::HV06:case RS::HV09:   //上
				dx = x0; dy = y0 - gheight; anchor = GraphletAnchor::CB;
				break;
				/*case RS::HV14://左
					dx = x0 - gwidth; dy = y0; anchor = GraphletAnchor::RB;
					break;*/
			case RS::HV01:case RS::HV02:case RS::HV24:case RS::HV14://右
				dx = x0 + gwidth * 0.5f; dy = y0; anchor = GraphletAnchor::LB;
				break;
			case RS::HV07:case RS::HV08:case RS::HV12:case RS::HV13://更左一点
				dx = x0 - gwidth * 2; dy = y0; anchor = GraphletAnchor::RB;
				break;
				/*case RS::HV11: case RS::HBV11:case RS::HBV12:case RS::HBV13:case RS::HBV14:case RS::HBV15:case RS::HBV16:case RS::HBV17://更上一点
					dx = x0; dy = y0 - gheight * 5; anchor = GraphletAnchor::CB;
					break;
				case RS::HV10://更下一点
					dx = x0; dy = y0 + gheight * 3; anchor = GraphletAnchor::CB;
					break; */
			case RS::HV11: //更上一点
				dx = x0; dy = y0 - gheight * 3; anchor = GraphletAnchor::CB;
				break;
			case RS::HV10://更下一点
				dx = x0; dy = y0 + gheight * 3; anchor = GraphletAnchor::CB;
				break;

			default: {
				dx = x0; dy = y0 - gheight; anchor = GraphletAnchor::LB;
			};
			}

			this->station->map_credit_graphlet(it->second, GraphletAnchor::CC, x0, y0);
			this->station->map_credit_graphlet(this->captions[it->first], anchor, dx, dy);
			this->master->move_to(this->vlabels[it->first], this->captions[it->first], GraphletAnchor::CB, GraphletAnchor::CT);
		}

		float polar45 = gridsize * flsqrt(2.0F) * 0.618F;
		for (auto it = this->mvalves.begin(); it != this->mvalves.end(); it++) {

			switch (it->first) {

			case RS::HV07:case RS::HV08:case RS::HV01:case RS::HV02:case RS::HV13:case RS::HV12:case RS::HV24: {//左
				this->gvalves[RS::HV01]->fill_margin(x0, y0, nullptr, nullptr, nullptr, &margin);
				dx = x0 - gridsize + margin; dy = y0; anchor = GraphletAnchor::RC;
			}; break;
			case RS::HV14: {//右
				this->gvalves[RS::HV01]->fill_margin(x0, y0, nullptr, nullptr, nullptr, &margin);
				dx = x0 - gridsize + margin; dy = y0; anchor = GraphletAnchor::RC;
			}; break;
			case RS::HV10:case RS::HV16:case RS::HV06: {//下
				dx = x0; dy = y0 + gridsize; anchor = GraphletAnchor::CC;
			}; break;
				//case RS::M1:case RS::M2:case RS::M3:case RS::M4:case RS::M5:case RS::M6:case RS::M7:
				//	dx = x0; dy = y0; anchor = GraphletAnchor::CC;//上
				//	break;
				case RS::M1:case RS::M2:case RS::M3:case RS::M4:case RS::M5:case RS::M6:case RS::M7:
					dx = x0; dy = y0; anchor = GraphletAnchor::CC;//上
					break;
			default: {
				dx = x0; dy = y0 - gridsize; anchor = GraphletAnchor::CC;//上
			}
			}
			it->second->fill_valve_origin(&ox, &oy);
			this->station->map_credit_graphlet(it->second, anchor, dx - ox, dy - oy);
		}

		{ // reflow winches and cylinders
			float gapsize = gheight * 0.5F;
			/*
			this->station->map_graphlet_base_on_anchors(this->winches[ShipSlot::ShoreWinch],
				RS::deck_lx, RS::deck_by, GraphletAnchor::RT, -gwidth, gapsize);

			this->station->map_graphlet_base_on_anchors(this->winches[ShipSlot::BowWinch], RS::deck_lx, RS::ps, GraphletAnchor::RT, -gwidth);
			this->station->map_graphlet_base_on_anchors(this->winches[ShipSlot::SternWinch], RS::Hatch, RS::ps, GraphletAnchor::CT, gwidth);*/
			//this->station->map_graphlet_base_on_anchors(this->winches[ShipSlot::BargeWinch], RS::d024, RS::D006, GraphletAnchor::LC, gwidth, gapsize);

			//this->master->move_to(this->alarms[RS::BowCTension], this->winches[ShipSlot::BowWinch], GraphletAnchor::RC, GraphletAnchor::LC, gapsize);
			//this->master->move_to(this->alarms[RS::SternCTension], this->winches[ShipSlot::SternWinch], GraphletAnchor::RC, GraphletAnchor::LC, gapsize);

			{ // reflow cylinders
				this->holdhoops[RS::shd_joint]->fill_cylinder_origin(&ox, &oy);
				this->station->map_credit_graphlet(this->holdhoops[RS::shd_joint], GraphletAnchor::CC, -ox, -oy);
				this->master->move_to(this->bolts[RS::shd_joint], this->holdhoops[RS::shd_joint], GraphletAnchor::CT, GraphletAnchor::CC, 0.0F, -oy);

				//this->station->map_graphlet_at_anchor(this->bolts[RS::Barge], RS::d024, GraphletAnchor::CC, -gapsize, gapsize);
			}

			for (auto it = this->winches.begin(); it != this->winches.end(); it++) {
				this->master->move_to(this->winch_labels[it->first], it->second, GraphletAnchor::CT, GraphletAnchor::CB);
				this->master->move_to(this->winch_pressures[it->first], it->second, GraphletAnchor::CB, GraphletAnchor::CT);
			}

			for (auto it = this->alabels.begin(); it != this->alabels.end(); it++) {
				this->master->move_to(it->second, this->alarms[it->first], GraphletAnchor::RC, GraphletAnchor::LC, gapsize);
			}
		}

		{ // reflow doors
			float ps_x, ps_y, sb_x, sb_y;

			this->reflow_doors(this->uhdoors, this->progresses, Door::PS1, Door::PS7, gheight * -2.5F);
			this->reflow_doors(this->uhdoors, this->progresses, Door::SB1, Door::SB7, gheight * +2.5F);

			this->station->fill_anchor_location(RS::HV15, nullptr, &ps_y);
			this->station->fill_anchor_location(RS::HV16, nullptr, &sb_y);

			for (unsigned int idx = 0; idx < hopper_count; idx++) {
				this->master->fill_graphlet_location(this->uhdoors[_E(Door, idx + _I(Door::PS1))], &ps_x, nullptr, GraphletAnchor::CC);
				this->master->fill_graphlet_location(this->uhdoors[_E(Door, idx + _I(Door::SB1))], &sb_x, nullptr, GraphletAnchor::CC);

				this->master->move_to(this->ps_seqs[idx], ps_x, ps_y, GraphletAnchor::CT);
				this->master->move_to(this->sb_seqs[idx], sb_x, sb_y, GraphletAnchor::CB);
			}
		}

		{ // reflow dimensions
			float offset = default_pipe_thickness * 2.0F;
			//百分比
			this->master->move_to(this->gvprogresses[RS::HV09], this->gvalves[RS::HV09], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, -margin);
			this->master->move_to(this->gvprogresses[RS::HV01], this->gvalves[RS::HV01], GraphletAnchor::CB, GraphletAnchor::LT, offset, -offset);
			this->master->move_to(this->gvprogresses[RS::HV02], this->gvalves[RS::HV02], GraphletAnchor::CT, GraphletAnchor::LB, offset);

			this->station->map_credit_graphlet(this->pump_pressures[RS::A1], GraphletAnchor::RB, -gwidth * 3.0F);
			this->master->move_to(this->pump_pressures[RS::A2], this->pump_pressures[RS::A1], GraphletAnchor::RB, GraphletAnchor::RT, 0.0F, offset);
			this->master->move_to(this->pump_pressures[RS::C1], this->pump_pressures[RS::A1], GraphletAnchor::LC, GraphletAnchor::RC, -gwidth);
			this->master->move_to(this->pump_pressures[RS::C2], this->pump_pressures[RS::A2], GraphletAnchor::LC, GraphletAnchor::RC, -gwidth);
		}
		{// 泵转速设定
			this->master->move_to(this->pumps_rpm[RS::PSHPump], this->hoppers[RS::PSHPump], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, 0.0F);
			this->master->move_to(this->pumps_rpm[RS::SBHPump], this->hoppers[RS::SBHPump], GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, 0.0F);
		}
		this->station->map_graphlet_at_anchor(this->functions[RS::diagnostics], RS::diagnostics, GraphletAnchor::LB, gwidth * 3.0F);
	}

public:
	bool hopper_selected(RS pid) {
		return this->master->is_selected(this->hoppers[pid]);
	}

public:
	void draw_relationships(CanvasDrawingSession^ ds, float X, float Y, float Width, float Height) {
		//所有虚线
		float ox, oy, sx, sy, tx, ty;

		for (auto it = this->mvalves.begin(); it != this->mvalves.end(); it++) {
			this->master->fill_graphlet_location(it->second, &sx, &sy, GraphletAnchor::CC);
			this->master->fill_graphlet_location(this->gvalves[it->first], &tx, &ty, GraphletAnchor::CC);
			it->second->fill_valve_origin(&ox, &oy);

			ds->DrawLine(sx + ox + X, sy + oy + Y, tx + X, ty + Y, this->relationship_color, 1.0F, this->relationship_style);
		}
		for (auto it = this->gvalves.begin(); it != this->gvalves.end(); it++) {
			switch (it->first) {
			case RS::M1:case RS::M2:case RS::M3:case RS::M4:case RS::M5:case RS::M6:case RS::M7:
				this->master->fill_graphlet_location(it->second, &sx, &sy, GraphletAnchor::CC);
				this->master->fill_graphlet_location(this->gvalves[it->first], &tx, &ty, GraphletAnchor::CC);
				//it->second->fill_valve_origin(&ox, &oy);
			}
		}

		for (unsigned int idx = 0; idx < hopper_count; idx++) {
			this->master->fill_graphlet_location(this->uhdoors[_E(Door, idx + _I(Door::PS1))], &sx, &sy, GraphletAnchor::CC);
			this->master->fill_graphlet_location(this->uhdoors[_E(Door, idx + _I(Door::SB1))], &tx, &ty, GraphletAnchor::CC);
			ds->DrawLine(sx + X, sy + Y, tx + X, ty + Y, this->door_paired_color, 1.0F, this->relationship_style);
		}
	}

private:
	template<class G, typename E>
	void load_valve(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, std::map<E, Credit<Labellet, E>*>& cs
		, E id, float radius, double degrees) {
		this->load_label(ls, "(" + id.ToString() + ")", id, Colours::Silver, this->label_font);
		this->load_label(cs, id, Colours::Silver, this->label_font);

		gs[id] = this->master->insert_one(new G(radius, degrees), id);
	}
	template<class G, typename E>
	void load_valve(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, E id, float radius, double degrees) {
		this->load_label(ls, "(" + id.ToString() + ")", id, Colours::Silver, this->label_font);

		gs[id] = this->master->insert_one(new G(radius, degrees), id);
	}
	template<class G, typename E>
	void load_valve(std::map<E, G*>& gs, E id, float radius, double degrees) {

		gs[id] = this->master->insert_one(new G(radius, degrees), id);
	}

	template<class G, class M, typename E>
	void load_valves(std::map<E, G*>& gs, std::map<E, M*>& ms, std::map<E, Credit<Labellet, E>*>& ls
		, std::map<E, Credit<Labellet, E>*>& cs, E id0, E idn, float radius, double degrees) {
		float mradius = radius * 0.8F;

		for (E id = id0; id <= idn; id++) {
			double mdegrees = 0.0;

			switch (id) {//电动阀角度
			case RS::HV19: case RS::HV17: mdegrees = -45.0; break;
			case RS::HV20: case RS::HV18: mdegrees = -135.0; break;
			case RS::HV22:case RS::HV16:case RS::HV10:case RS::HV06: mdegrees = -180.0; break;
			}

			// moter-driven valves' second, catching events first 
			this->load_valve(gs, ls, cs, id, radius, degrees);
			if (id != RS::HV09) {
				ms[id] = this->master->insert_one(new M(mradius, mdegrees, false), id);
			}
		}
	}
	template<class G, class M, typename E>
	void load_valves(std::map<E, G*>& gs, std::map<E, M*>& ms, std::map<E, Credit<Labellet, E>*>& ls
		, E id0, E idn, float radius, double degrees) {
		//测试
		for (E id = id0; id <= idn; id++) {
			// moter-driven valves' second, catching events first 
			ms[id] = this->master->insert_one(new M(radius * 0.7F, 0, false), id);
			this->load_valve(gs, id, radius * 0.7F, degrees);
		}
	}

	template<class D, typename E>
	void load_doors(std::map<E, Credit<D, E>*>& ds, std::map<E, Credit<Percentagelet, E>*>& ps, E id0, E idn, float radius) {
		for (E id = id0; id <= idn; id++) {
			ds[id] = this->master->insert_one(new Credit<D, E>(radius), id);
			this->load_percentage(ps, id);
		}
	}

	template<class G, typename E>
	void load_pump(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, E id, float rx, float fy) {
		this->load_label(ls, id, Colours::Salmon, this->caption_font);

		gs[id] = this->master->insert_one(new G(rx, flabs(rx) * fy), id);

		this->load_dimension(this->powers, id, "kwatt", 0);
		this->load_dimension(this->rpms, id, "rpm", 0);
		this->load_dimension(this->dpressures, id, "bar", 1);
		this->load_dimension(this->vpressures, id, "bar", 1);
	}

	template<class A, typename E>
	void load_alarms(std::map<E, Credit<A, E>*>& as, std::map<E, Credit<Labellet, E>*>& ls, E id0, E idn, float size) {
		for (E id = id0; id <= idn; id++) {
			this->load_label(ls, id, Colours::Silver, this->label_font);
			as[id] = this->master->insert_one(new Credit<A, E>(size), id);
		}
	}

	template<class W, typename E>
	void load_winches(std::map<E, Credit<W, E>*>& ws, std::map<E, Credit<Labellet, E>*>& ls, float radius) {
		for (E id = _E0(E); id < E::_; id++) {
			ws[id] = this->master->insert_one(new Credit<W, E>(radius), id);

			this->load_label(ls, id, Colours::Salmon, this->caption_font);
			this->load_dimension(this->winch_pressures, id, "bar", 1);
		}
	}

	template<typename E>
	void load_percentage(std::map<E, Credit<Percentagelet, E>*>& ps, E id) {
		ps[id] = this->master->insert_one(new Credit<Percentagelet, E>(this->percentage_style), id);
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
	template<class B, typename E>
	void load_buttons(std::map<E, Credit<B, E>*>& bs, E id, float width = 128.0F, float height = 32.0F) {
		bs[id] = this->master->insert_one(new Credit<B, E>(_speak(id), width, height), id);
	}

private:
	template<class D, typename E>
	void reflow_doors(std::map<E, Credit<D, E>*>& ds, std::map<E, Credit<Percentagelet, E>*>& ps, E id0, E idn, float yoff) {
		GraphletAnchor d_anchor = GraphletAnchor::CT;
		GraphletAnchor p_anchor = GraphletAnchor::CB;
		float label_yoff = default_pipe_thickness * 2.0F;
		float lx, rx, y, cell_width;

		if (yoff > 0.0F) { // Starboard
			d_anchor = GraphletAnchor::CB;
			p_anchor = GraphletAnchor::CT;
		}

		this->station->fill_anchor_location(RS::HV09, &lx, &y);
		this->station->fill_anchor_location(RS::HV15, &rx, nullptr);
		cell_width = (rx - lx) / float(hopper_count);

		for (E id = id0; id <= idn; id++) {
			size_t idx = static_cast<size_t>(id) - static_cast<size_t>(id0) + 1;
			float x = lx + cell_width * (0.5F + float(hopper_count - idx));

			this->master->move_to(ds[id], x, y + yoff, GraphletAnchor::CC);
			this->master->move_to(ps[id], ds[id], d_anchor, p_anchor);
		}
	}

private:
	void set_door_progress(Door id, float value) {
		this->uhdoors[id]->set_value(value / 100.0F);
		this->progresses[id]->set_value(value, GraphletAnchor::CC);

		AI_hopper_door(this->uhdoors[id], value, bottom_door_open_threshold, upper_door_closed_threshold);
	}

	void set_valves_status(RS id
		, const uint8* db4, unsigned int gidx4_p1, unsigned int midx4_p1
		, const uint8* db205, unsigned int gidx205_p1, unsigned int midx205_p1) {
		DI_gate_valve(this->gvalves[id], db4, gidx4_p1, db205, gidx205_p1);

		if (this->mvalves.find(id) != this->mvalves.end()) {
			DI_motor_valve(this->mvalves[id], db4, midx4_p1, db205, midx205_p1);
		}
	}

private:
	bool valve_open(RS vid) {
		return (this->gvalves[vid]->get_state() == GateValveState::Open);
	}

	void try_flow_water(RS vid, RS eid1, RS eid2, CanvasSolidColorBrush^ color) {
		if (this->valve_open(vid)) {
			this->station->push_subtrack(vid, eid1, color);

			if (eid2 != RS::_) {
				this->station->push_subtrack(vid, eid2, color);
			}
		}
	}

	void try_flow_water(RS vid, RS* path, unsigned int count, CanvasSolidColorBrush^ color) {
		if (this->valve_open(vid)) {
			this->station->push_subtrack(vid, path[0], color);
			this->station->push_subtrack(path, count, color);
		}
	}

	template<unsigned int N>
	void try_flow_water(RS vid, RS(&path)[N], CanvasSolidColorBrush^ color) {
		this->try_flow_water(vid, path, N, color);
	}

	void try_flow_water(RS vid, RS eid, CanvasSolidColorBrush^ color) {
		this->try_flow_water(vid, eid, RS::_, color);
	}

	void try_flow_water(int count, ...) {
		va_list arguments;
		RS cur = RS::_;
		RS old = RS::_;
		va_start(arguments, count);
		for (int i = 0; i < count + 1; i++) {
			if (cur == RS::_) {
				cur = va_arg(arguments, RS);
			}
			else {
				if ((old != RS::_) && (cur != RS::_)) {
					this->station->push_subtrack(old, cur, water_color);
				}
				old = cur;
				cur = va_arg(arguments, RS);
			}
		}
		va_end(arguments);
	}

	// never deletes these graphlets mannually
private:
	Tracklet<RS>* station;
	std::map<RS, Credit<Labellet, RS>*> captions;
	std::map<RS, Credit<HopperPumplet, RS>*> hoppers;
	std::map<RS, Credit<GateValvelet, RS>*> gvalves;
	std::map<RS, Credit<MotorValvelet, RS>*> mvalves;
	std::map<RS, Credit<Labellet, RS>*> vlabels;
	std::map<RS, Credit<Alarmlet, RS>*> alarms;
	std::map<RS, Credit<Labellet, RS>*> alabels;
	std::map<RS, Credit<HoldHooplet, RS>*> holdhoops;
	std::map<RS, Credit<Boltlet, RS>*> bolts;
	std::map<ShipSlot, Credit<Winchlet, ShipSlot>*> winches;
	std::map<ShipSlot, Credit<Dimensionlet, ShipSlot>*> winch_pressures;
	std::map<ShipSlot, Credit<Labellet, ShipSlot>*> winch_labels;
	std::map<Door, Credit<UpperHopperDoorlet, Door>*> uhdoors;
	std::map<Door, Credit<Percentagelet, Door>*> progresses;
	std::map<RS, Credit<Percentagelet, RS>*> gvprogresses;
	std::map<RS, Credit<Dimensionlet, RS>*> pump_pressures;
	std::map<RS, Credit<Dimensionlet, RS>*> dpressures;
	std::map<RS, Credit<Dimensionlet, RS>*> vpressures;
	std::map<RS, Credit<Dimensionlet, RS>*> powers;
	std::map<RS, Credit<Dimensionlet, RS>*> rpms;
	std::map<RS, Credit<Dimensionlet, RS>*> pumps_rpm;//泵转速设定
	std::map<RS, Omegalet*> nintercs;
	std::map<RS, Circlelet*> intercs;
	Labellet* ps_seqs[hopper_count];
	Labellet* sb_seqs[hopper_count];
	Linelet* manual_pipe;
	Hatchlet* sea_inlet;
	Circlelet* ps_suction;
	Circlelet* sb_suction;

	std::map<RS, Credit<Buttonlet, RS>*> functions;
private:
	CanvasTextFormat^ caption_font;
	CanvasTextFormat^ label_font;
	ICanvasBrush^ relationship_color;
	ICanvasBrush^ door_paired_color;
	CanvasStrokeStyle^ relationship_style;
	DimensionStyle pump_style;
	DimensionStyle highlight_style;
	DimensionStyle percentage_style;
	DimensionStyle metrics_style;
	DimensionStyle setting_style;

private:
	DischargesPage* master;
};

private class RainbowsDecorator : public TVesselDecorator<Rainbows, RS> {
public:
	RainbowsDecorator(Rainbows* master) : TVesselDecorator<Rainbows, RS>(master) {}

public:
	void draw_non_important_lines(Tracklet<RS>* station, CanvasDrawingSession^ ds, float x, float y, CanvasStrokeStyle^ style) override {
		/* !不重要的线
		float d0525_x, d05_y, d25_y;
		float d07_x, d07_y;
		float d10_x, d10_y;

		station->fill_anchor_location(RS::D005, &d0525_x, &d05_y, false);
		station->fill_anchor_location(RS::D025, nullptr, &d25_y, false);
		station->fill_anchor_location(RS::d007, &d07_x, &d07_y, false);
		station->fill_anchor_location(RS::D010, &d10_x, &d10_y, false);

		ds->DrawLine(x + d0525_x, y + d05_y, x + d0525_x, y + d25_y,
			Colours::DimGray, default_pipe_thickness, style);

		ds->DrawLine(x + d0525_x, y + d07_y, x + d07_x, y + d07_y,
			Colours::DimGray, default_pipe_thickness, style);

		ds->DrawLine(d10_x, y + d10_y, x + d10_x, y + d10_y,
			Colours::DimGray, default_pipe_thickness, style);*/
	}
};

/*************************************************************************************************/
DischargesPage::DischargesPage(PLCMaster* plc) : Planet(__MODULE__), device(plc) {
	Rainbows* dashboard = new Rainbows(this);

	this->dashboard = dashboard;

	if (this->device != nullptr) {
		this->diagnostics = new HopperPumpDiagnostics(plc);
		this->anchor_winch_op = make_anchor_winch_menu(plc);
		this->barge_winch_op = make_barge_winch_menu(plc);
		this->barge_cylinder_op = make_barge_cylinder_menu(plc);
		this->shore_winch_op = make_shore_winch_menu(plc);
		this->shore_cylinder_op = make_shore_cylinder_menu(plc);

		this->gate_valve_op = make_gate_valve_menu(DO_gate_valve_action, plc);
		this->motor_valve_op = make_motor_valve_menu(DO_motor_valve_action, plc);
		this->upper_door_op = make_upper_door_menu(plc);
		this->ps_hopper_op = make_ps_hopper_pump_discharge_menu(plc);
		this->sb_hopper_op = make_sb_hopper_pump_discharge_menu(plc);
		this->gdischarge_op = make_discharge_condition_menu(plc);

		this->device->push_confirmation_receiver(dashboard);
	}

	{ // load decorators
		this->grid = new GridDecorator();

#ifdef _DEBUG
		this->push_decorator(this->grid);
#else
		this->grid->set_active_planet(this);
#endif

		this->push_decorator(new RainbowsDecorator(dashboard));
	}
}

DischargesPage::~DischargesPage() {
	if (this->dashboard != nullptr) {
		delete this->dashboard;
	}

#ifndef _DEBUG
	delete this->grid;
#endif
}

void DischargesPage::load(CanvasCreateResourcesReason reason, float width, float height) {
	auto dashboard = dynamic_cast<Rainbows*>(this->dashboard);

	if (dashboard != nullptr) {
		float vinset = statusbar_height();
		float gwidth = width / 64.0F;
		float gheight = height / 36.0F;

		this->grid->set_grid_width(gwidth);
		this->grid->set_grid_height(gheight, vinset);

		dashboard->construct(gwidth, gheight);
		dashboard->load(width, height, gwidth, gheight);
	}
}

void DischargesPage::reflow(float width, float height) {
	auto dashboard = dynamic_cast<Rainbows*>(this->dashboard);

	if (dashboard != nullptr) {
		float vinset = statusbar_height();
		float gwidth = this->grid->get_grid_width();
		float gheight = this->grid->get_grid_height();

		dashboard->reflow(width, height, gwidth, gheight);
	}
}

void DischargesPage::on_timestream(long long timepoint_ms, size_t addr0, size_t addrn, uint8* data, size_t size, Syslog* logger) {
	auto dashboard = dynamic_cast<Rainbows*>(this->dashboard);

	if (dashboard != nullptr) {
		dashboard->on_all_signals(timepoint_ms, addr0, addrn, data, size, logger);
	}
}

bool DischargesPage::can_select(IGraphlet* g) {
	return ((this->device != nullptr)
		&& ((dynamic_cast<GateValvelet*>(g) != nullptr)
	//		|| (dynamic_cast<MotorValvelet*>(g) != nullptr)
			|| (dynamic_cast<UpperHopperDoorlet*>(g) != nullptr)
			|| (dynamic_cast<HopperPumplet*>(g) != nullptr)
			|| (dynamic_cast<Winchlet*>(g) != nullptr)
			|| (dynamic_cast<Credit<Buttonlet, RS>*>(g)!=nullptr)
			|| (dynamic_cast<Boltlet*>(g) != nullptr)
			|| (dynamic_cast<HoldHooplet*>(g) != nullptr)));
}

bool DischargesPage::can_select_multiple() {
	return (this->device != nullptr);
}

void DischargesPage::on_tap_selected(IGraphlet* g, float local_x, float local_y) {
	//auto gvalve = dynamic_cast<GateValvelet*>(g);
	auto gvalve = dynamic_cast<Credit<GateValvelet, RS>*>(g);
	//auto mvalve = dynamic_cast<Credit<MotorValvelet, RS>*>(g);
	auto uhdoor = dynamic_cast<UpperHopperDoorlet*>(g);
	auto hpump = dynamic_cast<Credit<HopperPumplet, RS>*>(g);
	auto winch = dynamic_cast<Credit<Winchlet, ShipSlot>*>(g);
	auto bolt = dynamic_cast<Credit<Boltlet, RS>*>(g);
	auto holdhoop = dynamic_cast<Credit<HoldHooplet, RS>*>(g);
	auto function = dynamic_cast<Credit<Buttonlet, RS>*>(g);

	if (gvalve != nullptr) {
		switch (gvalve->id) {
		case RS::M1:case RS::M2:case RS::M3:case RS::M4:case RS::M5:case RS::M6:case RS::M7:
			menu_popup(this->motor_valve_op, g, local_x, local_y);
			break;
		default:
			menu_popup(this->gate_valve_op, g, local_x, local_y);
		}
	}
	else if (function != nullptr) {
		switch (function->id) {
		case RS::diagnostics: this->diagnostics->show(); break;
			//case CSFunction::MotorInfo: this->motor_info->show(); break;
		}
	}
	else if (uhdoor != nullptr) {
		menu_popup(this->upper_door_op, g, local_x, local_y);
	}
	else if (holdhoop != nullptr) {
		menu_popup(this->shore_cylinder_op, g, local_x, local_y);
	}
	else if (bolt != nullptr) {
		switch (bolt->id) {
		case RS::shd_joint: menu_popup(this->shore_cylinder_op, g, local_x, local_y); break;
			//case RS::Barge: menu_popup(this->barge_cylinder_op, g, local_x, local_y); break;
		}
	}
	else if (winch != nullptr) {
		switch (winch->id) {
		case ShipSlot::ShoreWinch: menu_popup(this->shore_winch_op, g, local_x, local_y); break;
		case ShipSlot::BargeWinch: menu_popup(this->barge_winch_op, g, local_x, local_y); break;
		case ShipSlot::BowWinch: case ShipSlot::SternWinch: menu_popup(this->anchor_winch_op, g, local_x, local_y); break;
		}
	}
	else if (hpump != nullptr) {
		switch (hpump->id) {
		case RS::PSHPump: menu_popup(this->ps_hopper_op, g, local_x, local_y); break;
		case RS::SBHPump: menu_popup(this->sb_hopper_op, g, local_x, local_y); break;
		}
	}
}

void DischargesPage::on_gesture(std::list<float2>& anchors, float x, float y) {
	auto dashboard = dynamic_cast<Rainbows*>(this->dashboard);

	if (dashboard != nullptr) {
		if (dashboard->hopper_selected(RS::PSHPump) && dashboard->hopper_selected(RS::SBHPump)) {
			menu_popup(this->gdischarge_op, this, x, y);
		}
	}
}

bool DischargesPage::on_key(VirtualKey key, bool wargrey_keyboard) {
	bool handled = Planet::on_key(key, wargrey_keyboard);

	if ((!handled) && (this->device != nullptr) && (this->device->authorized())) {
		auto db = dynamic_cast<Rainbows*>(this->dashboard);

		if (db != nullptr) {
			handled = db->on_key(key, this->device);
		}
	}

	return handled;
}

void DischargesPage::on_focus(IGraphlet* g, bool yes) {
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