#include <map>

#include "page/hydraulics.hpp"
#include "page/diagnostics/hydraulic_pump_dx.hpp"

#include "configuration.hpp"
#include "menu.hpp"

#include "module.hpp"
#include "brushes.hxx"
#include "turtle.hpp"

#include "graphlet/shapelet.hpp"
#include "graphlet/ui/buttonlet.hpp"
#include "graphlet/ui/statuslet.hpp"
#include "graphlet/symbol/heaterlet.hpp"
#include "graphlet/symbol/pump/hydraulic_pumplet.hpp"
#include "graphlet/symbol/valve/manual_valvelet.hpp"
#include "graphlet/device/tanklet.hpp"
#include "graphlet/dashboard/fueltanklet.hpp"
#include "graphlet/dashboard/thermometerlet.hpp"
#include "graphlet/dashboard/alarmlet.hpp"

#include "iotables/ai_metrics.hpp"
#include "iotables/ai_winches.hpp"
#include "iotables/ai_pumps.hpp"

#include "iotables/di_valves.hpp"
#include "iotables/di_pumps.hpp"
#include "iotables/di_devices.hpp"
#include "iotables/di_doors.hpp"
#include "iotables/di_pipelines.hpp"
#include "iotables/di_winches.hpp"
#include "iotables/di_dredges.hpp"

#include "iotables/do_pumps.hpp"
#include "iotables/do_devices.hpp"

using namespace WarGrey::SCADA;

using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Foundation::Numerics;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::Brushes;

private enum class HSFunction { BOPOverride, _ };

private enum class HSMTState { Empty, UltraLow, Low, Normal, High, Full, _ };

static CanvasSolidColorBrush^ oil_color = Colours::Yellow;
static CanvasSolidColorBrush^ label_color = Colours::Salmon;
static CanvasSolidColorBrush^ running_color = Colours::Green;

// WARNING: order matters
private enum class HS : unsigned int {
	// Pumps
	/*
	F1, F2,////循环冷却泵
	C2, B4, B3, A2,
	E,//应急泵
	I, J,
	D1, D2,// 蝶阀/泥门锁紧
	C1, B2, B1, A1,*/
	F1,F2,E,I,J,
	C1,B2,B1,A1,D1,D2,
	C2,B4,B3,A2,

	// Valves
	S1, S2, S3, S6, S7,
	S24, S23, S22, S21,S4, S5, S14, S13, S12, S11, 

	// Key Labels
	Port, Starboard, Master, Visor, VisorState, /*MasterHeater, VisorHeater,*/VisorOil, //Storage,没有储油柜

	// Pump-driven subsystems
	/*
	PSTrunnion, PSGatevalves, Bowblowing, WateringValve,//C1 左弯管绞车和吊架/左舷闸阀/艏吹装置/抽舱引水闸阀
	PSIntermediate1,//B2 B1 左耙中绞车和吊架
	PSIntermediate2,//B2 B1 左耙中绞车和吊架
	PSDraghead, PSDoors, PSCompensator,//A1 左耙头绞车和吊架/左舷大抽舱门/左波浪补偿器

	SBTrunnion, SBGatevalves, SBCompensator,//C2 右弯管绞车和吊架/右舷闸阀/右波浪补偿器
	SBIntermediate3,//B4 B3 右耙中绞车和吊架
	SBIntermediate4,//B4 B3 右耙中绞车和吊架
	SBDraghead, SBDoors, Overflow,//A2 右耙头绞车和吊架/右舷大抽舱门/左右溢流筒
	*/
	PSTrunnion, FrontDoor2,BackDoor2, Gatevalves,FlushValve,//C1 左弯管/泥门4-7/抽舱门4-7/闸阀/耙头冲水蝶阀
	PSIntermediate1, PSIntermediate2,//B1B2	 左耙中
	PSDraghead, PSCompensator,AllDoorOpen,//A1	左耙头/左补偿

	SBTrunnion, SBCompensator, Bowblowing, StemSpray,//C2  右弯管/右补偿/艏吹/艏喷
	SBIntermediate3, SBIntermediate4,//B1B2  右耙中
	SBDraghead, Overflow,FrontDoor1, BackDoor1, EnergyDissipationGateValve,//A2  右耙头/溢流桶/泥门1-3/抽舱门1-3/消能箱闸阀


	ButterflyValves, DoorsLocking,//D1 D2 蝶阀 泥门锁紧

	// Dimensions
	BackOil, BowWinch, SternWinch,

	//泵替代
	A1ToA2, A2ToA1, B12ToB34, B34ToB12, C1ToC2, C2ToC1, JToI, IToJ,

	//报警
	masterhigh,masterlow,visorhigh,visorlow,

	_,

	// anchors used as last jumping points
	a1,a2,b1,b2,b3,b4,c1,c2,d1,d2,f1,f2,e,toS7, toS4S5, toS6, te,

	// anchors used for unnamed corners
	lt, tl, rt, tr, cl, cr, i, j, master, sb
};
static HS C1[] = { HS::PSTrunnion, HS::Gatevalves,HS::FlushValve ,HS::FrontDoor2,HS::BackDoor2};
static HS B1[] = { HS::PSIntermediate1 };
static HS B2[] = { HS::PSIntermediate2 };
static HS A1[] = { HS::PSDraghead ,HS::PSCompensator,HS::AllDoorOpen };

static HS C2[] = { HS::SBTrunnion,  HS::Overflow, HS::Bowblowing ,HS::FrontDoor1, HS::BackDoor1 };//HS::StemSpray 
static HS B3[] = { HS::SBIntermediate3 };
static HS B4[] = { HS::SBIntermediate4 };
static HS A2[] = { HS::SBDraghead ,HS::SBCompensator};// HS::EnergyDissipationGateValve
/*************************************************************************************************/
static HydraulicPumpDiagnostics* satellite; // it will be destroyed by `atexit()`;

static void hydraulics_diagnostics(HydraulicPumplet* pump, PLCMaster* plc) {
	auto credit_pump = dynamic_cast<Credit<HydraulicPumplet, HS>*>(pump);

	if (credit_pump != nullptr) {
		HPDX group = HPDX::Other;
		unsigned int feedback = 0U;
		unsigned int details = 0U;

		if (satellite == nullptr) {
			satellite = new HydraulicPumpDiagnostics(plc);
		}

		switch (credit_pump->id) {
		case HS::F1: feedback = pump_F1_feedback; group = HPDX::Other; details = pump_F1_status; break;
		case HS::F2: feedback = pump_F2_feedback; group = HPDX::Other; details = pump_F2_status; break;

		case HS::C2: feedback = pump_C2_feedback; group = HPDX::SB; details = pump_C2_status; break;
		case HS::B4: feedback = pump_B4_feedback; group = HPDX::SB; details = pump_B4_status; break;
		case HS::B3: feedback = pump_B3_feedback; group = HPDX::SB; details = pump_B3_status; break;
		case HS::A2: feedback = pump_A2_feedback; group = HPDX::SB; details = pump_A2_status; break;

		case HS::E: feedback = pump_E_feedback; group = HPDX::Other; details = pump_E_status; break;

		case HS::D1: feedback = pump_D1_feedback; group = HPDX::PS; details = pump_D1_status; break;
		case HS::D2: feedback = pump_D2_feedback; group = HPDX::PS; details = pump_D2_status; break;

		case HS::C1: feedback = pump_C1_feedback; group = HPDX::PS; details = pump_C1_status; break;
		case HS::B2: feedback = pump_B2_feedback; group = HPDX::PS; details = pump_B2_status; break;
		case HS::B1: feedback = pump_B1_feedback; group = HPDX::PS; details = pump_B1_status; break;
		case HS::A1: feedback = pump_A1_feedback; group = HPDX::PS; details = pump_A1_status; break;

		case HS::I: feedback = pump_I_feedback; group = HPDX::Visor; details = pump_I_status; break;
		case HS::J: feedback = pump_J_feedback; group = HPDX::Visor; details = pump_J_status; break;
		}

		// WARNING: invoking `set_pump` before `switch_id`. 
		satellite->set_pump(credit_pump->id.ToString(), group, feedback,details);
		satellite->switch_id(feedback);
		satellite->show();
	}
}

static uint16 DO_hydraulics_action(HydraulicPumpAction cmd, HydraulicPumplet* pump) {
	auto credit_pump = dynamic_cast<Credit<HydraulicPumplet, HS>*>(pump);
	uint16 index = 0U;

	if (credit_pump != nullptr) {
		index = DO_hydraulic_pump_command(cmd, credit_pump->id);
	}

	return index;
}

static const HS* select_captions(HS pid, unsigned int* count) {
	HS* captions = nullptr;

	switch (pid) {
	case HS::C1: captions = C1; SET_BOX(count, sizeof(C1) / sizeof(HS)); break;
	case HS::B1: captions = B1; SET_BOX(count, sizeof(B1) / sizeof(HS)); break;
	case HS::B2: captions = B2; SET_BOX(count, sizeof(B2) / sizeof(HS)); break;
	case HS::A1: captions = A1; SET_BOX(count, sizeof(A1) / sizeof(HS)); break;

	case HS::C2: captions = C2; SET_BOX(count, sizeof(C2) / sizeof(HS)); break;
	case HS::B3: captions = B3; SET_BOX(count, sizeof(B3) / sizeof(HS)); break;
	case HS::B4: captions = B4; SET_BOX(count, sizeof(B4) / sizeof(HS)); break;
	case HS::A2: captions = A2; SET_BOX(count, sizeof(A2) / sizeof(HS)); break;

	default: SET_BOX(count, 0);
	}

	return captions;
}

/*************************************************************************************************/
private class Hydraulics final : public PLCConfirmation {
public:
	Hydraulics(HydraulicsPage* master) : master(master) {}

public:
	void pre_read_data(Syslog* logger) override {
		this->master->enter_critical_section();
		this->master->begin_update_sequence();

		this->station->clear_subtacks();
	}

	void on_analog_input(long long timepoint_ms, const uint8* DB2, size_t count2, const uint8* DB203, size_t count203, Syslog* logger) override {
		this->set_temperature(HS::Visor, RealData(DB203, visor_tank_temperature));
		this->set_temperature(HS::Master, RealData(DB203, master_tank_temperature));
		//this->set_visor_tank_level(RealData(DB203, visor_tank_level));

		this->pressures[HS::BackOil]->set_value(RealData(DB203, master_back_oil_pressure));
		//this->pressures[HS::BowWinch]->set_value(RealData(DB203, bow_anchor_winch_pressure));
		//this->pressures[HS::SternWinch]->set_value(RealData(DB203, stern_anchor_winch_pressure));

		{ // pump pressures
			GraphletAnchor psa = GraphletAnchor::LB;
			GraphletAnchor sba = GraphletAnchor::RB;
			
			this->pressures[HS::C1]->set_value(RealData(DB203, pump_C1_pressure), psa);
			this->pressures[HS::B1]->set_value(RealData(DB203, pump_B1_pressure), psa);
			this->pressures[HS::B2]->set_value(RealData(DB203, pump_B2_pressure), psa);
			this->pressures[HS::A1]->set_value(RealData(DB203, pump_A1_pressure), psa);
			this->pressures[HS::D1]->set_value(RealData(DB203, pump_D1_pressure), psa);
			this->pressures[HS::D2]->set_value(RealData(DB203, pump_D2_pressure), psa);

			this->pressures[HS::C2]->set_value(RealData(DB203, pump_C2_pressure), sba);
			this->pressures[HS::B3]->set_value(RealData(DB203, pump_B3_pressure), sba);
			this->pressures[HS::B4]->set_value(RealData(DB203, pump_B4_pressure), sba);
			this->pressures[HS::A2]->set_value(RealData(DB203, pump_A2_pressure), sba);
			
			this->pressures[HS::I]->set_value(RealData(DB203, pump_I_pressure), GraphletAnchor::LB);
			this->pressures[HS::J]->set_value(RealData(DB203, pump_J_pressure), GraphletAnchor::RB);
		}
	}

	void on_digital_input(long long timepoint_ms, const uint8* DB4, size_t count4, const uint8* DB205, size_t count205, Syslog* logger) override {
		DI_backoil_pressure_override(this->functions[HSFunction::BOPOverride], DB205, backoil_pressure_override_status);

		{ // tank states
			DI_tank_heater(this->heater[HeaterSlot::Master], DB4, master_tank_heater_feedback, DB205, master_tank_heater_status);
			DI_tank_heater(this->heater[HeaterSlot::Visor], DB4, visor_tank_heater_feedback, DB205, visor_tank_heater_status);

			DI_master_tank(this->master_tank, DB4, master_tank_status);
			DI_visor_tank(this->visor_tank, DB4, visor_tank_status);
			/*
			switch (this->visor_tank->get_state()) {
			case TankState::Low: {
				this->labels[HS::VisorState]->set_color(Colours::Yellow);
				this->labels[HS::VisorState]->set_text("[" + speak(TankState::Low) + "]");
			}; break;
			case TankState::UltraLow: {
				this->labels[HS::VisorState]->set_color(Colours::Red);
				this->labels[HS::VisorState]->set_text("[" + speak(TankState::UltraLow) + "]");
			}; break;
			default: {
				this->labels[HS::VisorState]->set_text("");
			}
			}*/
		}

		{ // pumps state
			DI_hydraulic_pump(this->pumps[HS::C1], DB4, pump_C1_feedback, DB205, pump_C1_status);
			DI_hydraulic_pump(this->pumps[HS::B2], DB4, pump_B2_feedback, DB205, pump_B2_status);
			DI_hydraulic_pump(this->pumps[HS::B1], DB4, pump_B1_feedback, DB205, pump_B1_status);
			DI_hydraulic_pump(this->pumps[HS::A1], DB4, pump_A1_feedback, DB205, pump_A1_status);
			DI_hydraulic_pump(this->pumps[HS::D1], DB4, pump_D1_feedback, DB205, pump_D1_status);
			DI_hydraulic_pump(this->pumps[HS::D2], DB4, pump_D2_feedback, DB205, pump_D2_status);

			DI_hydraulic_pump(this->pumps[HS::C2], DB4, pump_C2_feedback, DB205, pump_C2_status);
			DI_hydraulic_pump(this->pumps[HS::B4], DB4, pump_B4_feedback, DB205, pump_B4_status);
			DI_hydraulic_pump(this->pumps[HS::B3], DB4, pump_B3_feedback, DB205, pump_B3_status);
			DI_hydraulic_pump(this->pumps[HS::A2], DB4, pump_A2_feedback, DB205, pump_A2_status);
			DI_hydraulic_pump(this->pumps[HS::E], DB4, pump_E_feedback, DB205, pump_E_status);
			DI_hydraulic_pump(this->pumps[HS::F1], DB4, pump_F1_feedback, DB205, pump_F1_status);
			DI_hydraulic_pump(this->pumps[HS::F2], DB4, pump_F2_feedback, DB205, pump_F2_status);

			DI_hydraulic_pump(this->pumps[HS::I], DB4, pump_I_feedback, DB205, pump_I_status);
			DI_hydraulic_pump(this->pumps[HS::J], DB4, pump_J_feedback, DB205, pump_J_status);
		}

		{ // valves state
			DI_manual_valve(this->valves[HS::S4], DB4, manual_valve_S4_status);
			DI_manual_valve(this->valves[HS::S5], DB4, manual_valve_S5_status);

			DI_manual_valve(this->valves[HS::S14], DB4, manual_valve_S14_status);
			DI_manual_valve(this->valves[HS::S13], DB4, manual_valve_S13_status);
			DI_manual_valve(this->valves[HS::S12], DB4, manual_valve_S12_status);
			DI_manual_valve(this->valves[HS::S11], DB4, manual_valve_S11_status);
			DI_manual_valve(this->valves[HS::S1], DB4, manual_valve_S1_status);

			DI_manual_valve(this->valves[HS::S6], DB4, manual_valve_S6_status);
			DI_manual_valve(this->valves[HS::S7], DB4, manual_valve_S7_status);

			DI_manual_valve(this->valves[HS::S24], DB4, manual_valve_S24_status);
			DI_manual_valve(this->valves[HS::S23], DB4, manual_valve_S23_status);
			DI_manual_valve(this->valves[HS::S22], DB4, manual_valve_S22_status);
			DI_manual_valve(this->valves[HS::S21], DB4, manual_valve_S21_status);
			DI_manual_valve(this->valves[HS::S2], DB4, manual_valve_S2_status);
			DI_manual_valve(this->valves[HS::S3], DB4, manual_valve_S3_status);
		}

		{ // filters state
			DI_alarm(this->alarms[HS::masterhigh], DB4, master_tank_temperature_high_alarm);
			DI_alarm(this->alarms[HS::masterlow], DB4, master_tank_temperature_low_alarm);
			DI_alarm(this->alarms[HS::visorhigh], DB4, visor_tank_temperature_high_alarm);
			DI_alarm(this->alarms[HS::visorlow], DB4, visor_tank_temperature_hlow_alarm);

			DI_alarm(this->alarms[HS::A1ToA2], DB4, pump_A1_replace_A2, AlarmState::Notice);
			DI_alarm(this->alarms[HS::A2ToA1], DB4, pump_A2_replace_A1, AlarmState::Notice);
			DI_alarm(this->alarms[HS::B12ToB34], DB4, pump_B12_replace_B34, AlarmState::Notice);
			DI_alarm(this->alarms[HS::B34ToB12], DB4, pump_B34_replace_B12, AlarmState::Notice);
			DI_alarm(this->alarms[HS::C1ToC2], DB4, pump_C1_replace_C2, AlarmState::Notice);
			DI_alarm(this->alarms[HS::C2ToC1], DB4, pump_C2_replace_C1, AlarmState::Notice);
			DI_alarm(this->alarms[HS::JToI], DB4, pump_J_replace_I, AlarmState::Notice);
			DI_alarm(this->alarms[HS::IToJ], DB4, pump_I_replace_J, AlarmState::Notice);
		}

		{ // captions color
			bool ps_offset_winch_moving = DI_winch_winding(DB205, winch_ps_offset_details.status);//左弯管绞车
			bool ps_intermediate_winch_moving = DI_winch_winding(DB205, winch_ps_intermediate_details.status);//左耙中绞车
			bool ps_draghead_winch_moving = DI_winch_winding(DB205, winch_ps_draghead_details.status);//左耙头绞车
			bool ps_offset_gantry_moving = DI_gantry_moving(DB205, gantry_ps_offset_details);//左弯管A字架
			bool ps_intermediate_gantry_moving = DI_gantry_moving(DB205, gantry_ps_intermediate_details);//左耙中A字架
			bool ps_draghead_gantry_moving = DI_gantry_moving(DB205, gantry_ps_draghead_details);//左耙头A字架
			bool ps_visor_moving = DI_visor_curling(DB205, ps_visor_details);//左耙唇
			bool ps_compensator_moving = DI_compensator_working(DB205, ps_compensator_details);//左补偿
			bool ps_door = bottom_door_PS1_status
				|| bottom_door_PS2_status
				|| bottom_door_PS3_status
				|| bottom_door_PS4_status
				|| bottom_door_PS5_status
				|| bottom_door_PS6_status
				|| bottom_door_PS7_status;//左泥门

			bool sb_offset_winch_moving = DI_winch_winding(DB205, winch_sb_offset_details.status);//右弯管绞车
			bool sb_intermediate_winch_moving = DI_winch_winding(DB205, winch_sb_intermediate_details.status);//右耙中绞车
			bool sb_draghead_winch_moving = DI_winch_winding(DB205, winch_sb_draghead_details.status);//右耙头绞车
			bool sb_offset_gantry_moving = DI_gantry_moving(DB205, gantry_ps_offset_details);//右弯管A字架
			bool sb_intermediate_gantry_moving = DI_gantry_moving(DB205, gantry_ps_intermediate_details);//右耙中A字架
			bool sb_draghead_gantry_moving = DI_gantry_moving(DB205, gantry_ps_draghead_details);//右耙头A子架
			bool sb_visor_moving = DI_visor_curling(DB205, sb_visor_details);//右耙唇
			bool sb_compensator_moving = DI_compensator_working(DB205, sb_compensator_details);//右补偿
			bool sb_door = bottom_door_SB1_status
				|| bottom_door_SB2_status
				|| bottom_door_SB3_status
				|| bottom_door_SB4_status
				|| bottom_door_SB5_status
				|| bottom_door_SB6_status
				|| bottom_door_SB7_status;//左泥门

			bool shore_dischange_winch_moving = DI_winch_winding(DB205, shore_discharge_winch_details);//艏吹绞车
			//bool shore_dischange_cylinder_moving = DI_bolt_moving(DB205, shore_discharge_bolt_details) || DI_holdhoop_moving(DB205, shore_discharge_holdhoop_details);//艏吹锁紧液压缸  安全销液压缸
			//bool barging = DI_winch_winding(DB205, barge_winch_details) || DI_bolt_moving(DB205, barge_bolt_details);长鲸9没有

			//!!!!使用设备的绿字
			this->captions[HS::PSTrunnion]->set_color((ps_offset_gantry_moving || ps_offset_winch_moving) ? running_color : label_color);//左弯管绞车和吊架
			//左闸阀
			this->captions[HS::Bowblowing]->set_color(shore_dischange_winch_moving ? running_color : label_color);//艏吹
			//this->captions[HS::WateringValve]->set_color(DI_gate_value_moving(DB205, gate_valve_HV09_status) ? running_color : label_color);//引水闸阀
			this->captions[HS::PSIntermediate1]->set_color((ps_intermediate_gantry_moving || ps_intermediate_winch_moving) ? running_color : label_color);//左耙中绞车和吊架
			this->captions[HS::PSIntermediate2]->set_color((ps_intermediate_gantry_moving || ps_intermediate_winch_moving) ? running_color : label_color);//左耙中绞车和吊架
			this->captions[HS::PSDraghead]->set_color((ps_draghead_gantry_moving || ps_draghead_winch_moving) ? running_color : label_color);//左耙头绞车和吊架
			//this->captions[HS::PSDoors]->set_color(ps_door ? running_color : label_color);//左泥门
			this->captions[HS::PSCompensator]->set_color(ps_compensator_moving ? running_color : label_color);//左补偿
			this->captions[HS::SBTrunnion]->set_color((sb_offset_gantry_moving || sb_offset_winch_moving) ? running_color : label_color);//右弯管绞车和吊架
			//右闸阀
			this->captions[HS::SBCompensator]->set_color(sb_compensator_moving ? running_color : label_color);//右补偿
			this->captions[HS::SBIntermediate3]->set_color((sb_intermediate_gantry_moving || sb_intermediate_winch_moving) ? running_color : label_color);//右耙中绞车和吊架
			this->captions[HS::SBIntermediate4]->set_color((sb_intermediate_gantry_moving || sb_intermediate_winch_moving) ? running_color : label_color);//右耙中绞车和吊架
			this->captions[HS::SBDraghead]->set_color((sb_draghead_gantry_moving || sb_draghead_winch_moving) ? running_color : label_color);//右耙头绞车和吊架
			//this->captions[HS::PSDoors]->set_color(ps_door ? running_color : label_color);//左泥门//右泥门
			this->captions[HS::Overflow]->set_color(DI_overflow_moving(DB205, overflow_pipe_status) ? running_color : label_color);//溢流桶
			//问一下柏工this->captions[HS::ButterflyValves]->set_color(DI_pipeline_ready(DB205, pipeline_L2_W) ? running_color : label_color);//蝶阀
			//this->captions[HS::DoorsLocking]->set_color(DI_hopper_doors_locked(DB205) ? running_color : label_color);//泥门锁紧
			//应急泵
			//循环冷却泵
			this->captions[HS::J]->set_color(ps_visor_moving ? running_color : label_color);//左耙唇
			this->captions[HS::I]->set_color(sb_visor_moving ? running_color : label_color);//右耙唇

			//这部分删除
			//this->captions[HS::PSDoors]->set_color(DI_pipeline_ready(DB205, pipeline_L2_G) ? running_color : label_color);
			//this->captions[HS::SBDoors]->set_color(DI_pipeline_ready(DB205, pipeline_L3_W) ? running_color : label_color);
			

			/*
			this->captions[HS::PSOffset]->set_color((ps_offset_gantry_moving || ps_offset_winch_moving) ? running_color : label_color);
			this->captions[HS::PSIntermediate]->set_color((ps_intermediate_gantry_moving || ps_intermediate_winch_moving) ? running_color : label_color);
			this->captions[HS::PSDraghead]->set_color((ps_draghead_gantry_moving || ps_draghead_winch_moving) ? running_color : label_color);
			this->captions[HS::J]->set_color(ps_visor_moving ? running_color : label_color);
			this->captions[HS::PSCompensator]->set_color(ps_compensator_moving ? running_color : label_color);
			this->captions[HS::SBOffset]->set_color((sb_offset_gantry_moving || sb_offset_winch_moving) ? running_color : label_color);
			this->captions[HS::SBIntermediate]->set_color((sb_intermediate_gantry_moving || sb_intermediate_winch_moving) ? running_color : label_color);
			this->captions[HS::SBDraghead]->set_color((sb_draghead_gantry_moving || sb_draghead_winch_moving) ? running_color : label_color);
			this->captions[HS::I]->set_color(sb_visor_moving ? running_color : label_color);
			this->captions[HS::SBCompensator]->set_color(sb_compensator_moving ? running_color : label_color);

			this->captions[HS::PSGateValves]->set_color(DI_pipeline_ready(DB205, pipeline_L1_W) ? running_color : label_color);
			this->captions[HS::SBGateValves]->set_color(DI_pipeline_ready(DB205, pipeline_L1_G) ? running_color : label_color);
			this->captions[HS::ButterflyValves]->set_color(DI_pipeline_ready(DB205, pipeline_L2_W) ? running_color : label_color);
			this->captions[HS::PSDoors]->set_color(DI_pipeline_ready(DB205, pipeline_L2_G) ? running_color : label_color);
			this->captions[HS::SBDoors]->set_color(DI_pipeline_ready(DB205, pipeline_L3_W) ? running_color : label_color);
			this->captions[HS::DoorsLocking]->set_color(DI_hopper_doors_locked(DB205) ? running_color : label_color);
			this->captions[HS::WateringValve]->set_color(DI_gate_value_moving(DB205, gate_valve_D01_status) ? running_color : label_color);
			this->captions[HS::BowAnchor]->set_color(DI_gate_value_moving(DB205, bow_anchor_winch_details) ? running_color : label_color);
			this->captions[HS::SternAnchor]->set_color(DI_winch_winding(DB205, stern_anchor_winch_details) ? running_color : label_color);
			this->captions[HS::ShoreDischarge]->set_color((shore_dischange_winch_moving || shore_dischange_cylinder_moving) ? running_color : label_color);
			this->captions[HS::Barge]->set_color(barging ? running_color : label_color);
			this->captions[HS::Overflow]->set_color(DI_overflow_moving(DB205, overflow_pipe_status) ? running_color : label_color);
			*/
		}
	}

	void on_analog_io(long long timepoint_ms, const uint8* DB204, size_t count204, Syslog* logger) override {
		{ // pump flows
			GraphletAnchor psa = GraphletAnchor::RB;
			GraphletAnchor sba = GraphletAnchor::LB;
			this->flows[HS::A1]->set_value(DBD(DB204, pump_A1_flow), psa);
			this->flows[HS::B1]->set_value(DBD(DB204, pump_B1B2_flow), psa);
			this->flows[HS::B2]->set_value(DBD(DB204, pump_B1B2_flow), psa);
			this->flows[HS::C1]->set_value(DBD(DB204, pump_C1_flow), psa);
			this->flows[HS::A2]->set_value(DBD(DB204, pump_A2_flow), sba);
			this->flows[HS::B3]->set_value(DBD(DB204, pump_B3B4_flow), sba);
			this->flows[HS::B4]->set_value(DBD(DB204, pump_B3B4_flow), sba);
			this->flows[HS::C2]->set_value(DBD(DB204, pump_C2_flow), sba);
		}
	}

	void post_read_data(Syslog* logger) override {
		
		{ // flow oil
			HS ps_path[] = { HS::lt, HS::tl, HS::cl, HS::Master };
			HS sb_path[] = { HS::rt, HS::tr, HS::cr, HS::Master };
			HS mt_path[] = { HS::te,HS::master };
			HS temp_path[] = {HS::toS7, HS::S7 };

			this->station->push_subtrack(HS::Master, HS::S2, oil_color);
			this->station->push_subtrack(HS::Master, HS::S1, oil_color);
			//this->station->push_subtrack(HS::Visor, HS::SQi, oil_color);
			//this->station->push_subtrack(HS::Visor, HS::SQj, oil_color);
			//this->station->push_subtrack(HS::Storage, HS::SQk1, oil_color);

			//this->try_flow_oil( HS::I, HS::i, oil_color);
			//this->try_flow_oil( HS::J, HS::j, oil_color);

			this->try_flow_oil(HS::S4, HS::D1, HS::d1, ps_path, oil_color);
			this->try_flow_oil(HS::S5, HS::D2, HS::d2, ps_path, oil_color);
			this->try_flow_oil(HS::S14, HS::C1, HS::c1, ps_path, oil_color);
			this->try_flow_oil(HS::S12, HS::B1, HS::b1, ps_path, oil_color);
			this->try_flow_oil(HS::S13, HS::B2, HS::b2, ps_path, oil_color);
			this->try_flow_oil(HS::S11, HS::A1, HS::a1, ps_path, oil_color);

			this->try_flow_oil(HS::S24, HS::C2, HS::c2, sb_path, oil_color);
			this->try_flow_oil(HS::S22, HS::B3, HS::b3, sb_path, oil_color);
			this->try_flow_oil(HS::S23, HS::B4, HS::b4 ,sb_path, oil_color);
			this->try_flow_oil(HS::S21, HS::A2, HS::a2, sb_path, oil_color);

			this->try_flow_oil(HS::S3, HS::E, HS::e, mt_path, oil_color);
			this->try_flow_oil(HS::S6, HS::F1, HS::f1, mt_path, oil_color);
			this->try_flow_oil(HS::S7, HS::F2, HS::f2, mt_path, oil_color);
			//this->try_flow_oil(HS::S7, HS::F1, HS::f1, mt_path, oil_color);
			//this->try_flow_oil(HS::S7, HS::F2, HS::F2, mt_path, oil_color);
			//this->try_flow_oil(HS::SQk2, HS::K /* , HS::k, mt_path */, oil_color);

			this->try_flow_oil(HS::S1, HS::Port, HS::S11, oil_color);
			this->try_flow_oil(HS::S1, HS::Port, HS::S4, oil_color);;
			//this->try_flow_oil(HS::S1, HS::Port, HS::S5, oil_color);

			if (this->valve_open(HS::S2)) {
				this->station->push_subtrack(HS::S2, HS::toS7, oil_color);
				this->station->push_subtrack(HS::S2, HS::S3, oil_color);
				this->station->push_subtrack(HS::sb, HS::S7, oil_color);
				this->station->push_subtrack(HS::sb, HS::S21, oil_color);
				/*
				this->station->push_subtrack(HS::S2, HS::sb, oil_color);
				this->station->push_subtrack(HS::sb, HS::S3, oil_color);
				this->station->push_subtrack(HS::sb, HS::toS7, oil_color);
				this->station->push_subtrack(HS::toS7, HS::S7, oil_color);
				this->station->push_subtrack(HS::sb, HS::S21, oil_color);*/
			}
			/*
			if (this->valve_open(HS::S6)) {
				this->station->push_subtrack(HS::S6, HS::S4, oil_color);
				this->station->push_subtrack(HS::S6, HS::S5, oil_color);
			}*/

			if (this->pump_open(HS::I)) {
				this->station->push_subtrack(HS::Visor, HS::i, oil_color);
			}
			if (this->pump_open(HS::J)) {
				this->station->push_subtrack(HS::Visor, HS::j, oil_color);
			}
			
		}

		this->master->end_update_sequence();
		this->master->leave_critical_section();
	}

public:
	void construct(float gwidth, float gheight) {
		this->caption_font = make_bold_text_format("Microsoft YaHei", large_font_size);
		this->label_font = make_bold_text_format("Microsoft YaHei", normal_font_size);

		this->button_style.font = make_bold_text_format("Consolas", small_font_size);
		this->button_style.corner_radius = 2.0F;
		this->button_style.thickness = 2.0F;

		this->fixnum_style.number_font = make_bold_text_format("Cambria Math", large_metrics_font_size);
		this->fixnum_style.unit_font = make_bold_text_format("Cambria", normal_font_size);
		this->flonum_style = this->fixnum_style;

		this->fixnum_style.precision = 0;
		this->flonum_style.precision = 1;
	}

public:
	void load_pipelines(float width, float height, float gwidth, float gheight) {
		Turtle<HS>* pTurtle = new Turtle<HS>(gwidth, gheight, true, HS::Master);

		pTurtle->move_right(2)->move_down(5.5F, HS::S2)->move_down()->turn_down_right();
		pTurtle->move_right(6.5F, HS::BowWinch)->move_right(6.5F, HS::sb)->turn_right_down()->move_down(17);

		pTurtle->jump_right(20, HS::a2)->move_left(5, HS::A2)->move_left(10, HS::S21)->move_left(5)->jump_back();
		pTurtle->move_up(3, HS::b4)->move_left(5, HS::B4)->move_left(10, HS::S23)->move_left(5)->jump_back();
		pTurtle->move_up(3, HS::b3)->move_left(5, HS::B3)->move_left(10, HS::S22)->move_left(5)->jump_back();
		pTurtle->move_up(3, HS::c2)->move_left(5, HS::C2)->move_left(10, HS::S24)->move_left(5)->jump_back();

		pTurtle->move_up(3, HS::Starboard)->move_up(21, HS::rt)->turn_up_left(HS::tr)->move_left(35, HS::cr);
		pTurtle->turn_left_down()->move_down(4)->jump_up(4);
		pTurtle->turn_up_left(HS::cl)->move_left(35, HS::tl)->turn_left_down(HS::lt)->move_down(15);

		pTurtle->move_down(3, HS::d1)->move_right(5, HS::D1)->move_right(10, HS::S4)->move_right(5)->jump_back();
		pTurtle->move_down(3, HS::d2)->move_right(5, HS::D2)->move_right(10, HS::S5)->move_right(5)->jump_back();
		//pTurtle->move_down(3, HS::d1)->move_right(5, HS::D1)->move_right(7, HS::S4)->move_right(2)->turn_right_down()->move_down(0.5F,HS::toS4S5)->move_right(2.5F,HS::S6)->move_right(2.5F,HS::toS6)->jump_back(HS::d1);
		//pTurtle->move_down(3, HS::d2)->move_right(5, HS::D2)->move_right(7, HS::S5)->move_right(2)->turn_right_up()->move_up(0.5F)->jump_back(HS::d2);
		pTurtle->move_down(3, HS::c1)->move_right(5, HS::C1)->move_right(10, HS::S14)->move_right(5)->jump_back();
		pTurtle->move_down(3, HS::b1)->move_right(5, HS::B1)->move_right(10, HS::S12)->move_right(5)->jump_back();
		pTurtle->move_down(3, HS::b2)->move_right(5, HS::B2)->move_right(10, HS::S13)->move_right(5)->jump_back();
		pTurtle->move_down(3, HS::a1)->move_right(5, HS::A1)->move_right(10, HS::S11)->move_right(5);

		pTurtle->move_up(12, HS::Port)->move_up(5)->turn_up_right()->move_right(6.5F, HS::SternWinch)->move_right(6.5F)->turn_right_up();
		pTurtle->move_up(HS::S1)->move_up(5.5F)->move_to(HS::Master);

		pTurtle->jump_back(HS::Master)->jump_right(4, HS::master)->move_up(6.5F)->turn_up_right()->move_right(2);
		pTurtle->move_right(4, HS::te)->move_right(HS::e)->move_down(6, HS::E)->move_down(4, HS::S3)->move_down(4)->turn_down_left()->jump_back();
		/*
		pTurtle->move_right(5, HS::f1)->move_down(6, HS::F1)->move_down(3)->turn_down_right()->move_right(1.5F, HS::S7)->jump_back();
		pTurtle->move_right(4, HS::f2)->turn_right_down()->move_down(5, HS::F2)->move_down(3)->turn_down_left()->move_left(2.5f);
		pTurtle->jump_back(HS::S7)->move_down(4, HS::toS7)->turn_down_left()->move_to(HS::sb);
		*/
		pTurtle->move_right(5, HS::f1)->move_down(6, HS::F1)->move_down(4, HS::S6)->move_down(4)->turn_down_left()->jump_back();
		pTurtle->move_right(4, HS::f2)->turn_right_down()->move_down(5, HS::F2)->move_down(4, HS::S7)->move_down(4)->turn_down_left()->jump_back();
		pTurtle->jump_back(HS::sb)->turn_right_up()->move_up(4, HS::S6);
		pTurtle->jump_back(HS::S7)->move_down(4)->turn_down_left(HS::toS7)->move_to(HS::sb);

		//pTurtle->jump_back(HS::S7)->move_down(5,HS::toS7)->move_to(HS::sb);

		//pTurtle->move_right(5, HS::f1)->move_down(6, HS::F1)->move_down(4, HS::S7)->move_down(4)->turn_down_left()->jump_back();
		//pTurtle->move_right(5, HS::F2)->move_down(6, HS::M)->move_down(4, HS::SQm)->move_down(4)->turn_down_left()->jump_back();
		//pTurtle->move_right(4, HS::F2)->turn_right_down()->move_down(5, HS::K)->move_down(9, HS::k12);
		//pTurtle->move_left(3, HS::SQk2)->move_left(8)->jump_back()->move_right(3, HS::SQk1)->move_right(2.5F, HS::Storage);

		pTurtle->jump_back(HS::Master)->jump_down(14, HS::Visor);
		pTurtle->move_right(2)->move_down(5)->move_down(3, HS::I)->move_down(3, HS::i);
		pTurtle->jump_left(4, HS::j)->move_up(3, HS::J)->move_up(3)->move_up(5)->move_right(2 /* HS::Visor */);

		this->station = this->master->insert_one(new Tracklet<HS>(pTurtle, default_pipe_thickness, default_pipe_color));

		//this->load_label(this->captions, HS::Port, Colours::DarkKhaki, this->caption_font);
		//this->load_label(this->captions, HS::Starboard, Colours::DarkKhaki, this->caption_font);
		//this->load_label(this->captions, HS::Storage, Colours::Silver);长鲸9没有储油罐 储油柜文本

		this->load_dimension(this->pressures, HS::BackOil, "bar");
		//this->load_dimension(this->pressures, HS::BowWinch, "bar");
		//this->load_dimension(this->pressures, HS::SternWinch, "bar");

		this->load_buttons(this->functions, 48.0F, 24.0F);
	}

	void load_tanks(float width, float height, float gwidth, float gheight) {
		float thickness = default_pipe_thickness * 2.0F;
		float alarm_size = gwidth * 1.2F;

		this->master_tank = this->make_tank(HSMTState::Empty, gwidth * 20.0F, gheight * 8.0F, thickness);//主油箱
		this->visor_tank = this->make_tank(HSMTState::Empty, gwidth * 18.0F, gheight * 7.0F, thickness);//耙唇
		//this->master->insert_one(new Tanklet(drag_visor_tank_range, gwidth * 18.0F, gheight * 7.0F, 8U, thickness));//耙唇油箱

		//this->heater[HS::MasterHeater] = this->master->insert_one(new Heaterlet(gwidth * 1.618F));//加热器
		//this->heater[HS::VisorHeater] = this->master->insert_one(new Heaterlet(gwidth * 1.618F));//加热器

		//this->load_heater(this->heater, HeaterSlot::Master, gwidth * 1.618F);//加热器
		//this->load_heater(this->heater, HeaterSlot::Visor, gwidth * 1.618F);//加热器
		this->heater[HeaterSlot::Master] = this->master->insert_one(new Credit<Heaterlet, HeaterSlot>(gwidth * 1.618F), HeaterSlot::Master);
		this->heater[HeaterSlot::Visor] = this->master->insert_one(new Credit<Heaterlet, HeaterSlot>(gwidth * 1.618F), HeaterSlot::Visor);
		//this->heater[HeaterSlot::Visor] = this->master->insert_one(new Credit<Heaterlet, HeaterSlot>(height));

		this->load_label(this->heaterlabels, HeaterSlot::Master, label_color);//加热器文本
		this->load_label(this->heaterlabels, HeaterSlot::Visor, label_color);//加热器文本


		this->load_thermometer(this->thermometers, this->temperatures, HS::Master, gwidth * 2.5F, gheight * 4.5F);
		this->load_thermometer(this->thermometers, this->temperatures, HS::Visor, gwidth * 2.5F, gheight * 4.5F);
		//this->load_dimension(this->levels, HS::VisorOil, "centimeter");
		//this->load_label(this->labels, "", HS::VisorState, Colours::Silver, this->label_font);

		//this->storage_tank = this->master->insert_one(new FuelTanklet(gwidth * 2.5F, 0.0F, thickness, Colours::WhiteSmoke));//储油箱

		this->load_alarms(HS::A1ToA2, HS::IToJ, alarm_size, this->alarms, this->alabels);
		this->load_alarms(HS::masterhigh, HS::visorlow, alarm_size, this->alarms, this->alabels);
	}

	void load_devices(float width, float height, float gwidth, float gheight) {
		float radius = resolve_gridsize(gwidth, gheight);
		float pradius = radius * 1.2F;

		{ // load pumps
			//this->load_devices(this->pumps, this->labels, this->captions, HS::F1, HS::J, pradius, -90, label_color);
			//this->load_devices(this->pumps, this->labels, this->captions, HS::D1, HS::A1, pradius, 90.00, label_color);

			this->load_devices(this->pumps, this->labels, this->captions, HS::F1, HS::J, pradius, -90, label_color);
			this->load_devices(this->pumps, this->labels, this->captions, HS::C1, HS::D2, pradius, 0, label_color);
			this->load_devices(this->pumps, this->labels, this->captions, HS::C2, HS::A2, pradius, 180, label_color);

			this->load_dimensions(this->pressures, HS::C1, HS::A1, "bar");//压力 
			this->load_dimensions(this->pressures, HS::C2, HS::A2, "bar");//
			this->load_dimensions(this->pressures, HS::I, HS::J, "bar");
			this->load_dimensions(this->pressures, HS::D1, HS::D2, "bar");
			this->load_percentages(this->flows, HS::C1, HS::A1);
			this->load_percentages(this->flows, HS::C2, HS::A2);
		}

		{ // load valves
			this->load_devices(this->valves, this->labels, HS::S1, HS::S7, radius, 90.000);
			this->load_devices(this->valves, this->labels, HS::S24, HS::S11, radius, 0);
		}
	}

public:
	void reflow_pump_station(float width, float height, float gwidth, float gheight) {
		float cx = width * 0.5F;
		float cy = height * 0.5F;
		float sq1_y;

		this->master->move_to(this->station, cx, cy, GraphletAnchor::CC);
		//this->station->map_graphlet_at_anchor(this->storage_tank, HS::Storage, GraphletAnchor::LC);//长鲸9好像没有储油罐
		//this->station->fill_anchor_location(HS::SQ1, nullptr, &sq1_y, true);
		this->station->map_graphlet_at_anchor(this->master_tank, HS::Master, GraphletAnchor::CC);
		this->station->map_graphlet_at_anchor(this->visor_tank, HS::Visor, GraphletAnchor::CC);
		this->master->move_to(this->thermometers[HS::Master], this->master_tank, 0.25F, 0.5F, GraphletAnchor::CC);
		this->master->move_to(this->thermometers[HS::Visor], this->visor_tank, 0.25F, 0.5F, GraphletAnchor::CC);

		//this->station->map_credit_graphlet(this->captions[HS::Port], GraphletAnchor::CB, -gwidth * 10.0F);//文本
		//this->station->map_credit_graphlet(this->captions[HS::Starboard], GraphletAnchor::CB, -gwidth * 10.0F);//文本
		//this->master->move_to(this->captions[HS::Storage], this->storage_tank, GraphletAnchor::CB, GraphletAnchor::CT);

		this->master->move_to(this->pressures[HS::BackOil], this->station, GraphletAnchor::CT, GraphletAnchor::CB);
		//this->station->map_credit_graphlet(this->pressures[HS::BowWinch], GraphletAnchor::CT);//船艏绞车
		//this->station->map_credit_graphlet(this->pressures[HS::SternWinch], GraphletAnchor::CT);//船艉绞车;
		this->master->move_to(this->functions[HSFunction::BOPOverride],
			this->pressures[HS::BackOil], GraphletAnchor::RB, GraphletAnchor::LB, gwidth);

		{ // reflow heater
			float hspace, vspace;

			this->heater[HeaterSlot::Master]->fill_margin(0.0F, 0.0F, &vspace, &hspace);
			this->master->move_to(this->heater[HeaterSlot::Master], this->master_tank, GraphletAnchor::CB,
				GraphletAnchor::CB, 0.0F, vspace - hspace);
			this->heater[HeaterSlot::Visor]->fill_margin(0.0F, 0.0F, &vspace, &hspace);
			this->master->move_to(this->heater[HeaterSlot::Visor], this->visor_tank, GraphletAnchor::CB,
				GraphletAnchor::CB, 0.0F, vspace - hspace);

			this->master->move_to(this->heaterlabels[HeaterSlot::Master], this->master_tank, GraphletAnchor::CB, GraphletAnchor::CT);
			this->master->move_to(this->heaterlabels[HeaterSlot::Visor], this->visor_tank, GraphletAnchor::CB, GraphletAnchor::CT);
		}
	}

	void reflow_devices(float width, float height, float gwidth, float gheight) {
		GraphletAnchor lbl_a, cpt_a, bar_a, per_a;
		float lbl_dx, lbl_dy, cpt_dx, cpt_dy, bar_dx, bar_dy, per_dx, per_dy, margin;
		float pradius = this->pumps[HS::A1]->get_radiusX();
		float vradius = this->valves[HS::S4]->get_radiusY();
		float text_hspace = default_pipe_thickness * 2.0F;
		float x0 = 0.0F;
		float y0 = 0.0F;

		for (auto it = this->pumps.begin(); it != this->pumps.end(); it++) {
			switch (it->second->id) {
			case HS::C1: case HS::B1: case HS::B2: case HS::A1:case HS::D1:case HS::D2: {
				lbl_dx = x0 - pradius; lbl_dy = y0; lbl_a = GraphletAnchor::RT;
				cpt_dx = x0 + pradius; cpt_dy = y0; cpt_a = GraphletAnchor::LT;
				bar_dx = x0 + pradius; bar_dy = y0; bar_a = GraphletAnchor::LB;
				per_dx = x0 - pradius; per_dy = y0; per_a = GraphletAnchor::RB;
			} break;
			case HS::C2: case HS::B3: case HS::B4: case HS::A2: {
				lbl_dx = x0 + pradius; lbl_dy = y0; lbl_a = GraphletAnchor::LT;
				cpt_dx = x0 - pradius; cpt_dy = y0; cpt_a = GraphletAnchor::RT;
				bar_dx = x0 - pradius; bar_dy = y0; bar_a = GraphletAnchor::RB;
				per_dx = x0 + pradius; per_dy = y0; per_a = GraphletAnchor::LB;
			} break;
			case HS::E: case HS::F1: case HS::F2:  {
				lbl_dx = x0 - pradius; lbl_dy = y0; lbl_a = GraphletAnchor::RB;
				cpt_dx = x0 + text_hspace; cpt_dy = y0 - pradius; cpt_a = GraphletAnchor::LB;
				bar_dx = x0; bar_dy = y0; bar_a = GraphletAnchor::CC; // these devices have no metrics
			} break;
			default: {
				cpt_dx = x0; cpt_dy = y0 + gheight * 3.0F; cpt_a = GraphletAnchor::CT;

				if (it->second->id == HS::I) {
					lbl_dx = x0 + pradius; lbl_dy = y0; lbl_a = GraphletAnchor::LT;
					bar_dx = x0 + text_hspace; bar_dy = y0 + pradius; bar_a = GraphletAnchor::LT;
				}
				else { // HS::J
					lbl_dx = x0 - pradius; lbl_dy = y0; lbl_a = GraphletAnchor::RT;
					bar_dx = x0 - text_hspace; bar_dy = y0 + pradius; bar_a = GraphletAnchor::RT;
				}
			}
			}

			this->station->map_credit_graphlet(it->second, GraphletAnchor::CC, x0, y0);
			this->station->map_credit_graphlet(this->labels[it->first], lbl_a, lbl_dx, lbl_dy);

			if (this->captions.find(it->first) != this->captions.end()) {
				this->station->map_credit_graphlet(this->captions[it->first], cpt_a, cpt_dx, cpt_dy);
			}
			else {
				unsigned int column = 3U;
				unsigned int count = 0U;
				const HS* cpts = select_captions(it->first, &count);
				IGraphlet* target = this->captions[cpts[0]];
				GraphletAnchor tgt_a = ((cpt_a == GraphletAnchor::LT) ? GraphletAnchor::RT : GraphletAnchor::LT);
				float hgapsize = ((cpt_dx > 0.0F) ? text_hspace : -text_hspace) * 2.0F;
				float cpt_height;

				target->fill_extent(0.0F, 0.0F, nullptr, &cpt_height);
				for (unsigned int idx = 0; idx < count; idx += column) {
					this->station->map_graphlet_at_anchor(this->captions[cpts[idx]], it->first, cpt_a,
						cpt_dx, cpt_dy + cpt_height * (idx / column));
				}

				for (unsigned int idx = 1; idx < count; idx++) {
					if ((idx % column) != 0) {
						this->master->move_to(this->captions[cpts[idx]], target, tgt_a, cpt_a, hgapsize);
					}

					target = this->captions[cpts[idx]];
				}
			}

			if (this->pressures.find(it->first) != this->pressures.end()) {
				this->station->map_credit_graphlet(this->pressures[it->first], bar_a, bar_dx, bar_dy);
			}

			if (this->flows.find(it->first) != this->flows.end()) {
				this->station->map_credit_graphlet(this->flows[it->first], per_a, per_dx, per_dy);
			}
		}

		for (auto it = this->valves.begin(); it != this->valves.end(); it++) {
			if (it->second->get_direction_degrees() == 90.0) {
				switch (it->first) {
				case HS::S1:case HS::S2:case HS::S3: case HS::S6: case HS::S7: {//竖着的
					it->second->fill_margin(x0, y0, nullptr, nullptr, nullptr, &margin);
					lbl_dx = x0 - vradius + margin; lbl_dy = y0; lbl_a = GraphletAnchor::RC;
				}; break;
				case HS::S24: case HS::S11: default: {//横着的
					lbl_dx = x0 + vradius; lbl_dy = y0; lbl_a = GraphletAnchor::LC;
				}; break;
				}
			}
			else {
				it->second->fill_margin(x0, y0, &margin, nullptr, nullptr, nullptr);
				lbl_dx = x0; lbl_dy = y0 - vradius + margin; lbl_a = GraphletAnchor::CB;
			}

			this->station->map_credit_graphlet(it->second, GraphletAnchor::CC, x0, y0);
			this->station->map_credit_graphlet(this->labels[it->first], lbl_a, lbl_dx, lbl_dy);
		}
	}

	void reflow_metrics(float width, float height, float gwidth, float gheight) {
		//this->master->move_to(this->temperatures[HS::Master], this->thermometers[HS::Master], 1.0F, 0.75F, GraphletAnchor::LC, gwidth);//油温 文本LC LB LT
		//this->master->move_to(this->temperatures[HS::Visor], this->thermometers[HS::Visor], 1.0F, 0.75F, GraphletAnchor::LB, gwidth);
		this->master->move_to(this->temperatures[HS::Master], this->thermometers[HS::Master], 0, 1.1F, GraphletAnchor::LC, gwidth);//油温 文本LC LB LT
		this->master->move_to(this->temperatures[HS::Visor], this->thermometers[HS::Visor], 0, 1.2F, GraphletAnchor::LB, gwidth);
		//this->master->move_to(this->levels[HS::VisorOil], this->thermometers[HS::Visor], 1.0F, 0.75F, GraphletAnchor::LT, gwidth);

		{ // reflow alarms
			float lblgap = gheight * 0.25F;
			float vgap = lblgap * 0.5F;


			this->master->move_to(this->alarms[HS::masterhigh], this->thermometers[HS::Master], 1.0F, 0.25F, GraphletAnchor::LB, gwidth, -vgap);
			this->master->move_to(this->alarms[HS::masterlow], this->thermometers[HS::Master], 1.0F, 0.25F, GraphletAnchor::LT, gwidth, +vgap);
			this->master->move_to(this->alarms[HS::visorhigh], this->thermometers[HS::Visor], 1.0F, 0.25F, GraphletAnchor::LB, gwidth);
			this->master->move_to(this->alarms[HS::visorlow], this->thermometers[HS::Visor], 1.0F, 0.25F, GraphletAnchor::LT, gwidth);
			//this->master->move_to(this->alarms[HS::F10], this->thermometers[HS::Visor], 1.0F, 0.25F, GraphletAnchor::LC, gwidth);

			this->master->move_to(this->alarms[HS::A1ToA2], this->pumps[HS::D1], 0.5F, this->master_tank, 0.0F, GraphletAnchor::LT,gwidth * 3.0F,-gheight * 1);
			//this->master->move_to(this->alarms[HS::C2A], this->valves[HS::SQc], 0.0F, this->master_tank, 0.0F, GraphletAnchor::RT, -gwidth * 2.0F);
			for (HS id = HS::A1ToA2; id < HS::IToJ; id++) {
				//if (id != HS::I2J) {
					this->master->move_to(this->alarms[_E(HS, _I(id) + 1U)],
						this->alarms[id], GraphletAnchor::CB, GraphletAnchor::CT,
						0.0F, vgap * 3.0F);
				//}
			}

			for (auto it = this->alabels.begin(); it != this->alabels.end(); it++) {
				this->master->move_to(it->second, this->alarms[it->first],
					GraphletAnchor::RC, GraphletAnchor::LC, lblgap);
			}
		}
	}

public:
	bool pumps_selected(HS id0, HS idn, int tolerance) {
		bool okay = false;
		int ok = 0;

		for (HS id = id0; id <= idn; id++) {
			if (this->master->is_selected(this->pumps[id])) {
				ok += 1;

				if (ok >= tolerance) {
					okay = true;
					break;
				}
			}
		}

		return okay;
	}

private:
	template<class G, typename E>
	void load_devices(std::map<E, G*>& gs, E id0, E idn, float radius, double degrees) {
		for (E id = id0; id <= idn; id++) {
			gs[id] = this->master->insert_one(new G(radius, degrees), id);
		}
	}

	template<class G, typename E>
	void load_devices(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, E id0, E idn, float radius, double degrees) {
		this->load_devices(gs, id0, idn, radius, degrees);

		for (E id = id0; id <= idn; id++) {
			this->load_label(ls, id.ToString(), id, Colours::Silver);
		}
	}

	template<class G, typename E>
	void load_devices(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, std::map<E, Credit<Labellet, E>*>& cs, E id0, E idn
		, float radius, double degrees, CanvasSolidColorBrush^ color = Colours::Silver) {
		unsigned int count = 0U;

		this->load_devices(gs, id0, idn, radius, degrees);

		for (E id = id0; id <= idn; id++) {
			const HS* captions = select_captions(id, &count);

			this->load_label(ls, id.ToString(), id, color);

			if (count == 0) {
				this->load_label(cs, id, color);
			}
			else {
				for (unsigned int idx = 0; idx < count; idx++) {
					this->load_label(cs, captions[idx], color);
				}
			}
		}
	}

	template<typename E>
	void load_dimension(std::map<E, Credit<Dimensionlet, E>*>& ds, E id, Platform::String^ unit, bool label = true) {
		Platform::String^ caption = (label ? _speak(id) : nullptr);
		ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(this->flonum_style, unit, caption), id);
	}

	template<typename E>
	void load_dimensions(std::map<E, Credit<Dimensionlet, E>*>& ds, E id0, E idn, Platform::String^ unit) {
		for (E id = id0; id <= idn; id++) {
			ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(this->fixnum_style, unit), id);
		}
	}

	template<typename E>
	void load_percentages(std::map<E, Credit<Percentagelet, E>*>& gs, E id0, E idn) {
		for (E id = id0; id <= idn; id++) {
			gs[id] = this->master->insert_one(new Credit<Percentagelet, E>(this->flonum_style), id);
		}
	}

	template<class A, typename E>
	void load_alarms(E id0, E idn, float size, std::map<E, Credit<A, E>*>& bs, std::map<E, Credit<Labellet, E>*>& ls) {
		for (E id = id0; id <= idn; id++) {
			this->load_label(ls, id, Colours::Silver);
			bs[id] = this->master->insert_one(new Credit<A, E>(size), id);
		}
	}

	template<class B, typename CMD>
	void load_buttons(std::map<CMD, Credit<B, CMD>*>& bs, float width = 128.0F, float height = 32.0F) {
		for (CMD cmd = _E(CMD, 0); cmd < CMD::_; cmd++) {
			bs[cmd] = this->master->insert_one(new Credit<B, CMD>(cmd.ToString(), width, height), cmd);
			bs[cmd]->set_style(this->button_style);
		}
	}

	template<class T, typename E>
	void load_thermometer(std::map<E, Credit<T, E>*>& ts, std::map<E, Credit<Dimensionlet, E>*>& ds, E id, float width, float height) {
		ts[id] = this->master->insert_one(new Credit<T, E>(100.0, width, height, 2.5F), id);
		this->load_dimension(ds, id, "celsius", false);//油温文本
	}
	template<typename E>
	void load_heater(std::map < E, Credit<Heaterlet, E>*>& hs, E id, float height) {
		//hs[id] = this->master->insert_one(new Heaterlet(height));
		hs[id] = this->master->insert_one(new Credit<Heaterlet, E>(height));
		//ds[id] = this->master->insert_one(new Credit<Heaterlet, E>(hopper_height_range, height));
	}

	template<typename E>
	void load_label(std::map<E, Credit<Labellet, E>*>& ls, Platform::String^ caption, E id
		, CanvasSolidColorBrush^ color, CanvasTextFormat^ font = nullptr) {
		ls[id] = this->master->insert_one(new Credit<Labellet, E>(caption, ((font == nullptr) ? this->label_font : font), color), id);
	}

	template<typename E>
	void load_label(std::map<E, Credit<Labellet, E>*>& ls, E id, CanvasSolidColorBrush^ color, CanvasTextFormat^ font = nullptr) {
		this->load_label(ls, _speak(id), id, color, font);
	}

	template<typename E>
	StateTanklet<E>* make_tank(E id, float width, float height, float thickness) {
		StateTanklet<E>* tank = new StateTanklet<E>(id, width, height, thickness);
		StateTankStyle ulow, low, normal;

		normal.mark_weight = 0.50F;
		low.mark_weight = 0.20F;
		ulow.mark_weight = 0.10F;

		tank->set_style(E::Normal, normal);
		tank->set_style(E::Low, low);
		tank->set_style(E::UltraLow, ulow);

		// WARNING: set styles before inserting 

		return this->master->insert_one(tank);
	}

private:
	void set_temperature(HS id, float t) {
		this->thermometers[id]->set_value(t);
		this->temperatures[id]->set_value(t, GraphletAnchor::LB);
	}
	/*
	void set_visor_tank_level(float t) {
		this->visor_tank->set_value(t);
		this->levels[HS::VisorOil]->set_value(t, GraphletAnchor::LB);
		this->master->move_to(this->labels[HS::VisorState], this->levels[HS::VisorOil], GraphletAnchor::RC, GraphletAnchor::LC);
	}*/
private:
	bool valve_open(HS vid) {
		return (this->valves[vid]->get_state() == ManualValveState::Open);
	}
	bool pump_open(HS vid) {
		return (this->pumps[vid]->get_state() == HydraulicPumpState::Running);
	}
private:
	void try_flow_oil(HS vid, HS pid, CanvasSolidColorBrush^ color) {
		switch (this->valves[vid]->get_state()) {
		case ManualValveState::Open: {
			this->station->push_subtrack(vid, pid, color);
		}
		}
	}

	void try_flow_oil(HS vid, HS mid, HS eid, CanvasSolidColorBrush^ color) {
		switch (this->valves[vid]->get_state()) {
		case ManualValveState::Open: {
			this->station->push_subtrack(vid, mid, color);
			this->station->push_subtrack(mid, eid, color);
		}
		}
	}

	void try_flow_oil(HS vid, HS pid, HS _id, HS* path, unsigned int count, CanvasSolidColorBrush^ color) {
		this->try_flow_oil(vid, pid, color);

		switch (this->pumps[pid]->get_state()) {
		case HydraulicPumpState::Running: case HydraulicPumpState::StopReady: {
			this->station->push_subtrack(pid, _id, oil_color);

			if (path != nullptr) {
				this->station->push_subtrack(_id, path[0], oil_color);
				this->station->push_subtrack(path, count, color);
			}
		}
		}
	}

	template<unsigned int N>
	void try_flow_oil(HS vid, HS pid, HS _id, HS(&path)[N], CanvasSolidColorBrush^ color) {
		this->try_flow_oil(vid, pid, _id, path, N, color);
	}

private: // never deletes these graphlets mannually
	Tracklet<HS>* station;
	//Tanklet* visor_tank;
	FuelTanklet* storage_tank;
	StateTanklet<HSMTState>* master_tank;
	StateTanklet<HSMTState>* visor_tank;
	std::map<HSFunction, Credit<Buttonlet, HSFunction>*> functions;
	std::map<HS, Credit<Thermometerlet, HS>*> thermometers;
	std::map<HS, Credit<Dimensionlet, HS>*> temperatures;
	std::map<HS, Credit<Dimensionlet, HS>*> levels;
	std::map<HS, Credit<Labellet, HS>*> captions;
	std::map<HS, Credit<Labellet, HS>*> labels;
	std::map<HeaterSlot, Credit<Labellet, HeaterSlot>*> heaterlabels;
	std::map<HS, Credit<HydraulicPumplet, HS>*> pumps;
	std::map<HS, Credit<ManualValvelet, HS>*> valves;
	std::map<HS, Credit<Dimensionlet, HS>*> pressures;
	std::map<HS, Credit<Percentagelet, HS>*> flows;
	std::map<HS, Credit<Alarmlet, HS>*> alarms;
	std::map<HS, Credit<Labellet, HS>*> alabels;
	std::map<HeaterSlot, Credit<Heaterlet, HeaterSlot>*> heater;

private:
	CanvasTextFormat^ caption_font;
	CanvasTextFormat^ label_font;
	DimensionStyle fixnum_style;
	DimensionStyle flonum_style;
	ButtonStyle button_style;

private:
	HydraulicsPage* master;
};

HydraulicsPage::HydraulicsPage(PLCMaster* plc) : Planet(__MODULE__), device(plc) {
	Hydraulics* dashboard = new Hydraulics(this);

	this->dashboard = dashboard;

	if (this->device != nullptr) {
		this->gbs_op = make_hydraulics_group_menu(HydraulicsGroup::BothPumps, plc);
		this->gps_op = make_hydraulics_group_menu(HydraulicsGroup::PSPumps, plc);
		this->gsb_op = make_hydraulics_group_menu(HydraulicsGroup::SBPumps, plc);
		this->gvisor_op = make_hydraulics_group_menu(HydraulicsGroup::VisorPumps, plc);
		this->pump_op = make_hydraulic_pump_menu(DO_hydraulics_action, hydraulics_diagnostics, plc);
		this->heater_op = make_tank_heater_menu(plc);

		this->device->push_confirmation_receiver(dashboard);
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

HydraulicsPage::~HydraulicsPage() {
	if (this->dashboard != nullptr) {
		delete this->dashboard;
	}

#ifndef _DEBUG
	delete this->grid;
#endif
}

void HydraulicsPage::load(CanvasCreateResourcesReason reason, float width, float height) {
	auto dashboard = dynamic_cast<Hydraulics*>(this->dashboard);

	if (dashboard != nullptr) {
		float gwidth = width / 76.0F;
		float gheight = height / 38.0F;

		this->grid->set_grid_width(gwidth);
		this->grid->set_grid_height(gheight);

		dashboard->construct(gwidth, gheight);

		dashboard->load_pipelines(width, height, gwidth, gheight);
		dashboard->load_tanks(width, height, gwidth, gheight);
		dashboard->load_devices(width, height, gwidth, gheight);
	}
}

void HydraulicsPage::reflow(float width, float height) {
	auto dashboard = dynamic_cast<Hydraulics*>(this->dashboard);

	if (dashboard != nullptr) {
		float gwidth = this->grid->get_grid_width();
		float gheight = this->grid->get_grid_height();

		dashboard->reflow_pump_station(width, height, gwidth, gheight);
		dashboard->reflow_devices(width, height, gwidth, gheight);
		dashboard->reflow_metrics(width, height, gwidth, gheight);
	}
}

void HydraulicsPage::on_timestream(long long timepoint_ms, size_t addr0, size_t addrn, uint8* data, size_t size, Syslog* logger) {
	auto dashboard = dynamic_cast<Hydraulics*>(this->dashboard);

	if (dashboard != nullptr) {
		dashboard->on_all_signals(timepoint_ms, addr0, addrn, data, size, logger);
	}
}

bool HydraulicsPage::can_select(IGraphlet* g) {
	return ((this->device != nullptr)
		&& ((dynamic_cast<HydraulicPumplet*>(g) != nullptr)
			|| (dynamic_cast<Heaterlet*>(g) != nullptr)
			|| (dynamic_cast<Buttonlet*>(g) != nullptr)));
}

bool HydraulicsPage::can_select_multiple() {
	return (this->device != nullptr);
}

void HydraulicsPage::on_tap_selected(IGraphlet* g, float local_x, float local_y) {
	auto pump = dynamic_cast<HydraulicPumplet*>(g);
	//auto heater = dynamic_cast<Heaterlet*>(g);
	auto heater =  dynamic_cast<Credit<Heaterlet, HeaterSlot>*>(g);
	auto override = dynamic_cast<Buttonlet*>(g);

	if (pump != nullptr) {
		menu_popup(this->pump_op, g, local_x, local_y);
	}
	else if (heater != nullptr) {
		menu_popup(this->heater_op, g, local_x, local_y);
	}
	else if (override != nullptr) {
		this->device->send_command(backoil_pressure_override_command);
	}
}

void HydraulicsPage::on_gesture(std::list<float2>& anchors, float x, float y) {
	/*
	auto dashboard = dynamic_cast<Hydraulics*>(this->dashboard);

	if (dashboard != nullptr) {
		if (dashboard->pumps_selected(HS::C, HS::E, 1) && dashboard->pumps_selected(HS::A, HS::H, 1)) {
			group_menu_popup(this->gbs_op, this, x, y);
		}
		else if (dashboard->pumps_selected(HS::C, HS::E, 2)) {
			group_menu_popup(this->gps_op, this, x, y);
		}
		else if (dashboard->pumps_selected(HS::A, HS::H, 2)) {
			group_menu_popup(this->gsb_op, this, x, y);
		}
		else if (dashboard->pumps_selected(HS::J, HS::I, 2)) {
			group_menu_popup(this->gvisor_op, this, x, y);
		}
	}*/
}
