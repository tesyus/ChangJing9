#include <map>

#include "page/charges.hpp"
#include "page/diagnostics/hopper_pump_dx.hpp"
#include "page/subpage/underwater_pump_motor.hpp"

#include "configuration.hpp"
#include "menu.hpp"

#include "module.hpp"
#include "text.hpp"
#include "paint.hpp"
#include "brushes.hxx"
#include "turtle.hpp"

#include "datum/flonum.hpp"

#include "graphlet/shapelet.hpp"
#include "graphlet/ui/statuslet.hpp"
#include "graphlet/ui/buttonlet.hpp"
#include "graphlet/symbol/pump/hopper_pumplet.hpp"
#include "graphlet/symbol/valve/gate_valvelet.hpp"
#include "graphlet/symbol/valve/tagged_valvelet.hpp"

#include "iotables/ai_pumps.hpp"
#include "iotables/ai_valves.hpp"
#include "iotables/ai_hopper_pumps.hpp"
#include "iotables/ai_dredges.hpp"

#include "iotables/di_pumps.hpp"
#include "iotables/di_hopper_pumps.hpp"
#include "iotables/di_valves.hpp"

#include "iotables/do_valves.hpp"
#include "iotables/do_hopper_pumps.hpp"

#include "decorator/vessel.hpp"
#include <iotables\ao_pumps.hpp>
#include <iotables\di_menu.hpp>
using namespace Windows::UI::Xaml::Controls;

using namespace Windows::System;

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Numerics;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::Brushes;
using namespace Microsoft::Graphics::Canvas::Geometry;

//private enum class CSFunction { Diagnostics, MotorInfo, _ };
private enum class CSFunction { Diagnostics, _ };

// WARNING: order matters
private enum class CS : unsigned int {
	//泵
	//C1,C2,
	//阀
	HV13, HV12, HV14, HV01, HV02, HV07, HV08, HV24,
	HV09, HV21, HV22, HV15, HV16, HV05, HV06, HV03, HV04, HV10, HV11, HV23, HV25, HV26,
	//
	HV19, HV17,
	HV20, HV18,
	//泵
	PSUWPump, SBUWPump, //水下泵
	PSHPump, SBHPump, //舱内泵
	lLMOD, rLMOD,
	//左右耙头
	Port, Starboard,
	_,
	//未命名
	ps, sb, gantry, deck_lx, deck_rx, deck_ty, deck_by,
	//
	h19, h20, h17, h18, h3, h4, h0708, h0105, h0206, h2324, h25, h1315, h1216, h0307, h0408,
	//未命名
	alt, alb, art, arb, h1123, h10, h26,h21end,h19end,h17end,h22end,h20end,h18end, rh25, rh26,
	tpump, bpump,
	diagnostics,//诊断
	altr, albr,
	//弯
	jump0107,jump0802,jump1314,
	jump0107left,jump0107right,
	jump0802left,jump0802right,
	jump1314up,jump1314down,

	jump0107up, jump0107down,
	jump0802up, jump0802down,
	jump1314left, jump1314right,
	//20200316 长鲸9柏工临时要加 之后可以删除
	//Left_underwater_pump_speed_setting,
};

static CanvasSolidColorBrush^ water_color = Colours::Green;

static uint16 DO_gate_valve_action(GateValveAction cmd, GateValvelet* valve) {
	uint16 index = 0U;
	auto credit_valve = dynamic_cast<Credit<GateValvelet, CS>*>(valve);

	if (credit_valve != nullptr) {
		index = DO_gate_valve_command(cmd, credit_valve->id);
	}

	return index;
}

/*************************************************************************************************/
private class Vessel final : public PLCConfirmation {
public:
	Vessel(ChargesPage* master, 
		MenuFlyout^ PSHopper_menu = nullptr,
		MenuFlyout^ SBHopper_menu = nullptr,
		MenuFlyout^ PSUnderWater_menu = nullptr,
		MenuFlyout^ SBUnderWater_menu = nullptr
	) : master(master), 
		PSHopper_menu(PSHopper_menu),
		SBHopper_menu(SBHopper_menu),
		PSUnderWater_menu(PSUnderWater_menu),
		SBUnderWater_menu(SBUnderWater_menu) {
		this->setting_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::GhostWhite, Colours::RoyalBlue);
	}

public:
	void pre_read_data(Syslog* logger) override {
		this->master->enter_critical_section();
		this->master->begin_update_sequence();

		this->station->clear_subtacks();
	}

	void on_analog_input(long long timepoint_ms, const uint8* DB2, size_t count2, const uint8* DB203, size_t count203, Syslog* logger) override {

		//this->pump_pressures[CS::C]->set_value(RealData(DB203, pump_C_pressure), GraphletAnchor::LC);
		//this->pump_pressures[CS::F]->set_value(RealData(DB203, pump_F_pressure), GraphletAnchor::LC);
		//this->pump_pressures[CS::H]->set_value(RealData(DB203, pump_H_pressure), GraphletAnchor::LC);

		this->progresses[CS::HV01]->set_value(RealData(DB203, gate_valve_HV01_progress), GraphletAnchor::LB);
		this->progresses[CS::HV02]->set_value(RealData(DB203, gate_valve_HV02_progress), GraphletAnchor::LT);
		this->progresses[CS::HV09]->set_value(RealData(DB203, gate_valve_HV09_progress), GraphletAnchor::LT);

		this->powers[CS::PSHPump]->set_value(RealData(DB203, ps_hopper_pump_power), GraphletAnchor::RC);
		this->rpms[CS::PSHPump]->set_value(RealData(DB203, ps_hopper_pump_rpm), GraphletAnchor::LC);
		this->dpressures[CS::PSHPump]->set_value(RealData(DB203, ps_hopper_pump_discharge_pressure), GraphletAnchor::LB);
		this->vpressures[CS::PSHPump]->set_value(RealData(DB203, ps_hopper_pump_vacuum_pressure), GraphletAnchor::RB);

		this->powers[CS::SBHPump]->set_value(RealData(DB203, sb_hopper_pump_power), GraphletAnchor::RC);
		this->rpms[CS::SBHPump]->set_value(RealData(DB203, sb_hopper_pump_rpm), GraphletAnchor::LC);
		this->dpressures[CS::SBHPump]->set_value(RealData(DB203, sb_hopper_pump_discharge_pressure), GraphletAnchor::LT);
		this->vpressures[CS::SBHPump]->set_value(RealData(DB203, sb_hopper_pump_vacuum_pressure), GraphletAnchor::RT);
		
		this->powers[CS::PSUWPump]->set_value(RealData(DB203, ps_underwater_pump_power), GraphletAnchor::RC);
		this->rpms[CS::PSUWPump]->set_value(RealData(DB203, ps_underwater_pump_rpm), GraphletAnchor::LC);
		this->dpressures[CS::PSUWPump]->set_value(RealData(DB203, ps_underwater_pump_discharge_pressure), GraphletAnchor::LB);
		this->dfpressures[CS::PSUWPump]->set_value(RealData(DB203, ps_draghead_differential_pressure), GraphletAnchor::LB);
		this->vpressures[CS::PSUWPump]->set_value(RealData(DB203, ps_underwater_pump_vacuum_pressure), GraphletAnchor::RC);
		//this->flow[CS::PSUWPump]->set_value(RealData(DB203, ps_underwater_pump_flow), GraphletAnchor::RC);

		this->powers[CS::SBUWPump]->set_value(RealData(DB203, sb_underwater_pump_power), GraphletAnchor::RC);
		this->rpms[CS::SBUWPump]->set_value(RealData(DB203, sb_underwater_pump_rpm), GraphletAnchor::LC);
		this->dpressures[CS::SBUWPump]->set_value(RealData(DB203, sb_underwater_pump_discharge_pressure), GraphletAnchor::LT);
		this->dfpressures[CS::SBUWPump]->set_value(RealData(DB203, sb_draghead_differential_pressure), GraphletAnchor::LT);
		this->vpressures[CS::SBUWPump]->set_value(RealData(DB203, sb_underwater_pump_vacuum_pressure), GraphletAnchor::RC);
		//this->flow[CS::SBUWPump]->set_value(RealData(DB203, sb_underwater_pump_flow), GraphletAnchor::RC);

	}

	void on_digital_input(long long timepoint_ms, const uint8* DB4, size_t count4, const uint8* DB205, size_t count205, Syslog* logger) override {

		//DI_hopper_pumps(this->hoppers[CS::PSHPump], this->hoppers[CS::PSUWPump], DB4, ps_hopper_pump_feedback, DB205, ps_hopper_pump_details, ps_underwater_pump_details);
		//DI_hopper_pumps(this->hoppers[CS::SBHPump], this->hoppers[CS::SBUWPump], DB4, sb_hopper_pump_feedback, DB205, sb_hopper_pump_details, sb_underwater_pump_details);
		DI_hopper_pump(this->hoppers[CS::PSHPump], DB4, ps_hopper_pump_feedback, DB205, ps_hopper_pump_details);
		DI_hopper_pump(this->hoppers[CS::SBHPump], DB4, sb_hopper_pump_feedback, DB205, sb_hopper_pump_details);
		DI_hopper_pump(this->hoppers[CS::PSUWPump], DB4, ps_underwater_pump_feedback, DB205, ps_underwater_pump_details);
		DI_hopper_pump(this->hoppers[CS::SBUWPump], DB4, sb_underwater_pump_feedback, DB205, sb_underwater_pump_details);

		//暂时不需要
		//DI_hydraulic_pump_dimension(this->pump_pressures[CS::C], DB4, pump_C_feedback);
		//DI_hydraulic_pump_dimension(this->pump_pressures[CS::F], DB4, pump_F_feedback);
		//DI_hydraulic_pump_dimension(this->pump_pressures[CS::H], DB4, pump_H_feedback);
		this->set_valves_status(CS::HV03, DB4, gate_valve_HV03_feedback, motor_valve_HV03_feedback, DB205, gate_valve_HV03_status, motor_valve_HV03_status);
		this->set_valves_status(CS::HV04, DB4, gate_valve_HV04_feedback, motor_valve_HV04_feedback, DB205, gate_valve_HV04_status, motor_valve_HV04_status);
		this->set_valves_status(CS::HV05, DB4, gate_valve_HV05_feedback, motor_valve_HV05_feedback, DB205, gate_valve_HV05_status, motor_valve_HV05_status);
		this->set_valves_status(CS::HV06, DB4, gate_valve_HV06_feedback, motor_valve_HV06_feedback, DB205, gate_valve_HV06_status, motor_valve_HV06_status);
		this->set_valves_status(CS::HV07, DB4, gate_valve_HV07_feedback, motor_valve_HV07_feedback, DB205, gate_valve_HV07_status, motor_valve_HV07_status);
		this->set_valves_status(CS::HV08, DB4, gate_valve_HV08_feedback, motor_valve_HV08_feedback, DB205, gate_valve_HV08_status, motor_valve_HV08_status);
		//this->set_valves_status(CS::HV09, DB4, gate_valve_HV09_feedback, motor_valve_HV09_feedback, DB205, gate_valve_HV09_status, motor_valve_HV09_status);
		this->set_valves_status(CS::HV10, DB4, gate_valve_HV10_feedback, motor_valve_HV10_feedback, DB205, gate_valve_HV10_status, motor_valve_HV10_status);
		this->set_valves_status(CS::HV11, DB4, gate_valve_HV11_feedback, motor_valve_HV11_feedback, DB205, gate_valve_HV11_status, motor_valve_HV11_status);
		this->set_valves_status(CS::HV12, DB4, gate_valve_HV12_feedback, motor_valve_HV12_feedback, DB205, gate_valve_HV12_status, motor_valve_HV12_status);
		this->set_valves_status(CS::HV13, DB4, gate_valve_HV13_feedback, motor_valve_HV13_feedback, DB205, gate_valve_HV13_status, motor_valve_HV13_status);
		this->set_valves_status(CS::HV14, DB4, gate_valve_HV14_feedback, motor_valve_HV14_feedback, DB205, gate_valve_HV14_status, motor_valve_HV14_status);
		this->set_valves_status(CS::HV15, DB4, gate_valve_HV15_feedback, motor_valve_HV15_feedback, DB205, gate_valve_HV15_status, motor_valve_HV15_status);
		this->set_valves_status(CS::HV16, DB4, gate_valve_HV16_feedback, motor_valve_HV16_feedback, DB205, gate_valve_HV16_status, motor_valve_HV16_status);
		this->set_valves_status(CS::HV17, DB4, gate_valve_HV17_feedback, motor_valve_HV17_feedback, DB205, gate_valve_HV17_status, motor_valve_HV17_status);
		this->set_valves_status(CS::HV18, DB4, gate_valve_HV18_feedback, motor_valve_HV18_feedback, DB205, gate_valve_HV18_status, motor_valve_HV18_status);
		this->set_valves_status(CS::HV19, DB4, gate_valve_HV19_feedback, motor_valve_HV19_feedback, DB205, gate_valve_HV19_status, motor_valve_HV19_status);
		this->set_valves_status(CS::HV20, DB4, gate_valve_HV20_feedback, motor_valve_HV20_feedback, DB205, gate_valve_HV20_status, motor_valve_HV20_status);
		this->set_valves_status(CS::HV21, DB4, gate_valve_HV21_feedback, motor_valve_HV21_feedback, DB205, gate_valve_HV21_status, motor_valve_HV21_status);
		this->set_valves_status(CS::HV22, DB4, gate_valve_HV22_feedback, motor_valve_HV22_feedback, DB205, gate_valve_HV22_status, motor_valve_HV22_status);
		this->set_valves_status(CS::HV23, DB4, gate_valve_HV23_feedback, motor_valve_HV23_feedback, DB205, gate_valve_HV23_status, motor_valve_HV23_status);
		this->set_valves_status(CS::HV24, DB4, gate_valve_HV24_feedback, motor_valve_HV24_feedback, DB205, gate_valve_HV24_status, motor_valve_HV24_status);
		this->set_valves_status(CS::HV25, DB4, gate_valve_HV25_feedback, motor_valve_HV25_feedback, DB205, gate_valve_HV25_status, motor_valve_HV25_status);
		this->set_valves_status(CS::HV26, DB4, gate_valve_HV26_feedback, motor_valve_HV26_feedback, DB205, gate_valve_HV26_status, motor_valve_HV26_status);


		DI_gate_valve(this->gvalves[CS::HV09], DB205, gate_valve_HV09_feedback, DB205, gate_valve_HV09_status);

		DI_gate_valve(this->gvalves[CS::HV01],  DB205, gate_valve_HV01_feedback,DB205, gate_valve_HV01_status);//
		DI_motor_valve(this->mvalves[CS::HV01], DB4, motor_valve_HV01_feedback, DB205, motor_valve_HV01_status);//

		DI_gate_valve(this->gvalves[CS::HV02], DB205, gate_valve_HV02_feedback, DB205, gate_valve_HV02_status);
		DI_motor_valve(this->mvalves[CS::HV02], DB4, motor_valve_HV02_feedback, DB205, motor_valve_HV02_status);


		CurrentWorkingMode = DI_getvalveworkingmode(DB205);

		ui_thread_run_async([=]() {
			if (this->PSHopper_menu != nullptr) {
				DI_condition_menu(this->PSHopper_menu, PSHopperPumpChargeAction::PSHopper, DB205, PSHopper);
				DI_condition_menu(this->PSHopper_menu, PSHopperPumpChargeAction::BothHopper, DB205, BothHopper);
			}
			if (this->SBHopper_menu != nullptr) {
				DI_condition_menu(this->SBHopper_menu, SBHopperPumpChargeAction::SBHopper, DB205, SBHopper);
				DI_condition_menu(this->SBHopper_menu, SBHopperPumpChargeAction::BothHopper, DB205, BothHopper);
			}
			if (this->PSUnderWater_menu != nullptr) {
				DI_condition_menu(this->PSUnderWater_menu, PSUnderWaterPumpChargeAction::PSUnderWater, DB205, PSUnderWater);
				DI_condition_menu(this->PSUnderWater_menu, PSUnderWaterPumpChargeAction::BothUnderWater, DB205, BothUnderWater);
			}
			if (this->SBUnderWater_menu != nullptr) {
				DI_condition_menu(this->SBUnderWater_menu, SBUnderWaterPumpChargeAction::SBUnderWater, DB205, SBUnderWater);
				DI_condition_menu(this->SBUnderWater_menu, PSUnderWaterPumpChargeAction::BothUnderWater, DB205, BothUnderWater);
			}
			});
	}

	void on_forat(long long timepoint_ms, const uint8* DB20, size_t count, Syslog* logger) override {
		this->pumps_rpm[CS::PSUWPump]->set_value(DBD(DB20, 654U), GraphletAnchor::LB);//左水下
		this->pumps_rpm[CS::SBUWPump]->set_value(DBD(DB20, 666U), GraphletAnchor::LB);//右水下
		this->pumps_rpm[CS::PSHPump]->set_value(DBD(DB20, 650U), GraphletAnchor::LB);//左舱内
		this->pumps_rpm[CS::SBHPump]->set_value(DBD(DB20, 662U), GraphletAnchor::LB);//右舱内
	}
	void on_signals_updated(long long timepoint_ms, Syslog* logger) override {
		{
			//工况 填充
			switch (CurrentWorkingMode) {
			case ValvesConditionAction::PSUWPumpSingleDredging://左水下泵单耙挖泥
				this->try_flow_water(5, CS::Port, CS::PSUWPump, CS::HV01, CS::HV13, CS::HV15);
				if (this->valve_open(CS::HV17))
					this->try_flow_water(3, CS::HV15, CS::HV17, CS::h17end);
				if (this->valve_open(CS::HV19))
					this->try_flow_water(4, CS::HV15, CS::h19, CS::HV19, CS::h19end);
				if (this->valve_open(CS::HV21))
					this->try_flow_water(3, CS::HV15, CS::HV21, CS::h21end);
				break;
			case ValvesConditionAction::SBUWPumpSingleDredging://右水下泵单耙挖泥
				this->try_flow_water(5, CS::Starboard, CS::SBUWPump, CS::HV02, CS::HV12, CS::HV16);
				if (this->valve_open(CS::HV18))
					this->try_flow_water(3, CS::HV16, CS::HV18, CS::h18end);
				if (this->valve_open(CS::HV20))
					this->try_flow_water(4, CS::HV16, CS::h20, CS::HV20, CS::h20end);
				if (this->valve_open(CS::HV22))
					this->try_flow_water(3, CS::HV16, CS::HV22, CS::h22end);
				break;
			case ValvesConditionAction::PSSBUWPumpDoubleDredging://左右水下泵双耙挖泥
				this->try_flow_water(5, CS::Port, CS::PSUWPump, CS::HV01, CS::HV13, CS::HV15);
				if (this->valve_open(CS::HV17))
					this->try_flow_water(3, CS::HV15, CS::HV17, CS::h17end);
				if (this->valve_open(CS::HV19))
					this->try_flow_water(4, CS::HV15, CS::h19, CS::HV19, CS::h19end);
				if (this->valve_open(CS::HV21))
					this->try_flow_water(3, CS::HV15, CS::HV21, CS::h21end);
				this->try_flow_water(5, CS::Starboard, CS::SBUWPump, CS::HV02, CS::HV12, CS::HV16);
				if (this->valve_open(CS::HV18))
					this->try_flow_water(3, CS::HV16, CS::HV18, CS::h18end);
				if (this->valve_open(CS::HV20))
					this->try_flow_water(4, CS::HV16, CS::h20, CS::HV20, CS::h20end);
				if (this->valve_open(CS::HV22))
					this->try_flow_water(3, CS::HV16, CS::HV22, CS::h22end);
				break;
			case ValvesConditionAction::PSHPumpSingleDredging://左舱内泵单耙挖泥
				this->try_flow_water(4, CS::Port, CS::PSUWPump, CS::HV01, CS::jump0107up);
				this->try_flow_water(9, CS::jump0107down, CS::HV03, CS::PSHPump, CS::h1123, CS::HV11, CS::jump0107right, CS::jump0107left, CS::altr, CS::HV15);
				if (this->valve_open(CS::HV17))
					this->try_flow_water(3, CS::HV15, CS::HV17, CS::h17end);
				if (this->valve_open(CS::HV19))
					this->try_flow_water(4, CS::HV15, CS::h19, CS::HV19, CS::h19end);
				if (this->valve_open(CS::HV21))
					this->try_flow_water(3, CS::HV15, CS::HV21, CS::h21end);
				this->nintercs[CS::jump0107]->set_color(water_color);
				break;
			case ValvesConditionAction::SBHPumpSingleDredging://右舱内泵单耙挖泥
				this->try_flow_water(4, CS::Starboard, CS::SBUWPump, CS::HV02, CS::jump0802down);
				this->try_flow_water(9, CS::jump0802up, CS::HV04, CS::SBHPump, CS::h10, CS::HV10, CS::jump0802right, CS::jump0802left, CS::albr, CS::HV16);
				if (this->valve_open(CS::HV18))
					this->try_flow_water(3, CS::HV16, CS::HV18, CS::h18end);
				if (this->valve_open(CS::HV20))
					this->try_flow_water(4, CS::HV16, CS::h20, CS::HV20, CS::h20end);
				if (this->valve_open(CS::HV22))
					this->try_flow_water(3, CS::HV16, CS::HV22, CS::h22end);
				this->nintercs[CS::jump0802]->set_color(water_color);
				break;
			case ValvesConditionAction::PSSBHPumpDoubleDredging://左右舱内泵双耙挖泥
				this->try_flow_water(4, CS::Port, CS::PSUWPump, CS::HV01, CS::jump0107up);
				this->try_flow_water(9, CS::jump0107down, CS::HV03, CS::PSHPump, CS::h1123, CS::HV11, CS::jump0107right, CS::jump0107left, CS::altr, CS::HV15);
				if (this->valve_open(CS::HV17))
					this->try_flow_water(3, CS::HV15, CS::HV17, CS::h17end);
				if (this->valve_open(CS::HV19))
					this->try_flow_water(4, CS::HV15, CS::h19, CS::HV19, CS::h19end);
				if (this->valve_open(CS::HV21))
					this->try_flow_water(3, CS::HV15, CS::HV21, CS::h21end);
				this->nintercs[CS::jump0107]->set_color(water_color);
				this->try_flow_water(4, CS::Starboard, CS::SBUWPump, CS::HV02, CS::jump0802down);
				this->try_flow_water(9, CS::jump0802up, CS::HV04, CS::SBHPump, CS::h10, CS::HV10, CS::jump0802right, CS::jump0802left, CS::albr, CS::HV16);
				if (this->valve_open(CS::HV18))
					this->try_flow_water(3, CS::HV16, CS::HV18, CS::h18end);
				if (this->valve_open(CS::HV20))
					this->try_flow_water(4, CS::HV16, CS::h20, CS::HV20, CS::h20end);
				if (this->valve_open(CS::HV22))
					this->try_flow_water(3, CS::HV16, CS::HV22, CS::h22end);
				this->nintercs[CS::jump0802]->set_color(water_color);
				break;

			case ValvesConditionAction::PSHPumpBowblow://左泥泵艏吹
				//this->try_flow_water(4, CS::Port, CS::SBUWPump, CS::HV02, CS::jump0802down);
				break;
			case ValvesConditionAction::SBHPumpBowblow://右泥泵艏吹
				break;
			case ValvesConditionAction::PSSBHPumpSeriesBowblow://左右泥泵串联艏吹
				break;
			case ValvesConditionAction::PSHPumpStemSpray://左泥泵艏喷
				break;
			case ValvesConditionAction::SBHPumpStemSpray://右泥泵艏喷
				break;
			case ValvesConditionAction::PSSBHPumpSeriesStemSpray://左右泥泵串联艏喷
				break;
			}
		}
	}
	void post_read_data(Syslog* logger) override {//管道信号

		this->master->end_update_sequence();
		this->master->leave_critical_section();
	}

public:
	void construct(float gwidth, float gheight) {
		this->caption_font = make_bold_text_format("Microsoft YaHei", normal_font_size);
		this->label_font = make_bold_text_format("Microsoft YaHei", small_font_size);
		this->special_font = make_text_format(tiny_font_size);

		this->pump_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::Background);
		this->highlight_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::Green);

		this->relationship_style = make_dash_stroke(CanvasDashStyle::DashDot);
		this->relationship_color = Colours::DarkGray;

		this->hopper_style.number_font = make_bold_text_format("Cambria Math", large_metrics_font_size);
		this->hopper_style.unit_font = make_bold_text_format("Cambria", normal_font_size);

		this->percentage_style.precision = 1;
		this->percentage_style.number_font = make_bold_text_format("Cambria Math", large_metrics_font_size);
	}

public:

		bool on_key(VirtualKey key, PLCMaster* plc) {//
			bool handled = false;

			if (key == VirtualKey::Enter) {
				auto editor = dynamic_cast<Credit<Dimensionlet, CS>*>(this->master->get_focus_graphlet());

				if (editor != nullptr) {
					float speed = float(editor->get_input_number());

					if (speed >= 0.0F) {
						plc->send_setting(AO_charges_pump_setting(editor->id), speed);
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
		Turtle<CS>* pTurtle = new Turtle<CS>(gwidth, gheight, false,CS::rh26);

		/////////////////////////////////////////////////////////////////////////////////////////////////
		pTurtle->move_left(2, CS::HV26)->move_left(2, CS::h26);
		pTurtle->jump_back(CS::HV26)->jump_up(5)->jump_right(2,CS::rh25)->move_left(2, CS::HV25)->move_left(2, CS::h25);
		pTurtle->jump_back(CS::h26)->move_to(CS::h25)->move_up(3)->move_left(2, CS::h2324);
		pTurtle->move_left(2, CS::HV23)->move_left(5)->move_left(2, CS::HV11)->move_left(2, CS::art);//到斜线

		//诊断
		pTurtle->jump_up(6, CS::diagnostics)->jump_back(CS::art);

		pTurtle->jump_down(2)->jump_left(8, CS::alt);//标记
		pTurtle->jump_back(CS::art)->move_to(CS::alt)->move_left(3, CS::HV15)->move_left(2, CS::h17)->move_left(5, CS::h19)->move_left(6, CS::HV21)->move_left(2)->move_down_left(4,CS::h21end);//左到底

		pTurtle->jump_back(CS::h2324)->move_down(10, CS::HV24)->move_down(6)->move_left(7)->move_left(2, CS::HV10)->move_left(2, CS::arb);
		pTurtle->jump_up(2)->jump_left(8, CS::alb);//标记
		pTurtle->jump_back(CS::arb)->move_to(CS::alb)->move_left(3, CS::HV16)->move_left(2, CS::h18)->move_left(5, CS::h20)->move_left(6, CS::HV22)->move_left(2)->move_up_left(4,CS::h22end);//右到底

		pTurtle->jump_back(CS::HV21)->jump_down(6)->jump_left(16)->move_right(2, CS::HV09)->move_right(36, CS::h0708);//中间线
		pTurtle->move_up(2, CS::HV07)->move_up(2, CS::h0307)->move_up(3)->jump_up(0.5F,CS::jump0107)->jump_up(0.5F)->move_up(2, CS::h0105)->move_up(2, CS::HV01)->move_up(CS::ps)->move_up()->turn_up_left()->move_left(8, CS::PSUWPump)->move_left(10, CS::Port);
		pTurtle->jump_back(CS::h0105)->move_left(3, CS::HV05)->move_left(3, CS::HV13)->move_down(4, CS::h1315)->move_down(5.5F)->jump_down(0.5F,CS::jump1314)->jump_down(0.5F)->move_down(2.5F, CS::HV14)->move_down(3, CS::h1216)->move_down(4, CS::HV12)->move_right(3, CS::HV06)->move_right(3, CS::h0206)->move_up(2)->jump_up(0.5F,CS::jump0802)->jump_up(0.5F)->move_up(3, CS::h0408)->move_up(2, CS::HV08)->move_up(2);
		pTurtle->jump_back(CS::h0206)->move_down(2, CS::HV02)->move_down(CS::sb)->move_down()->turn_down_left()->move_left(8, CS::SBUWPump)->move_left(10, CS::Starboard);

		pTurtle->jump_back(CS::h19)->move_down_left(2, CS::HV19)->move_down_left(2,CS::h19end);
		pTurtle->jump_back(CS::h17)->move_down_left(2, CS::HV17)->move_down_left(1.25F,CS::h17end)->jump_down_left(0.75F,CS::lLMOD);

		pTurtle->jump_back(CS::h20)->move_up_left(2, CS::HV20)->move_up_left(2,CS::h20end);
		pTurtle->jump_back(CS::h18)->move_up_left(2, CS::HV18)->move_up_left(1.25F,CS::h18end)->jump_up_left(0.75F, CS::rLMOD);

		pTurtle->jump_back(CS::h0307)->move_right(2, CS::HV03)->move_right(5, CS::PSHPump)->move_up(4, CS::h1123);
		pTurtle->jump_back(CS::h0408)->move_right(2, CS::HV04)->move_right(5, CS::SBHPump)->move_down(4, CS::h10);

		pTurtle->jump_back(CS::jump0107)->jump_left(0.1F, CS::jump0107left);
		pTurtle->jump_back(CS::jump0107)->jump_right(0.1F, CS::jump0107right);
		pTurtle->jump_back(CS::jump0107)->jump_up(0.1F, CS::jump0107up);
		pTurtle->jump_back(CS::jump0107)->jump_down(0.1F, CS::jump0107down);

		pTurtle->jump_back(CS::jump0802)->jump_left(0.1F, CS::jump0802left);
		pTurtle->jump_back(CS::jump0802)->jump_right(0.1F, CS::jump0802right);
		pTurtle->jump_back(CS::jump0802)->jump_up(0.1F, CS::jump0802up);
		pTurtle->jump_back(CS::jump0802)->jump_down(0.1F, CS::jump0802down);

		pTurtle->jump_back(CS::jump1314)->jump_left(0.1F, CS::jump1314left);
		pTurtle->jump_back(CS::jump1314)->jump_right(0.1F, CS::jump1314right);
		pTurtle->jump_back(CS::jump1314)->jump_up(0.1F, CS::jump1314up);
		pTurtle->jump_back(CS::jump1314)->jump_down(0.1F, CS::jump1314down);

		pTurtle->jump_back(CS::alt)->jump_right(0.1F,CS::altr);
		pTurtle->jump_back(CS::alb)->jump_right(0.1F,CS::albr);

		/////////////////////////////////////////////////////////////////////////////////////////////////
		this->station = this->master->insert_one(new Tracklet<CS>(pTurtle, default_pipe_thickness, default_pipe_color));
		this->load_buttons(this->functions);
		/*
		{ // load gantry pipe segement
			CanvasStrokeStyle^ gantry_style = make_dash_stroke(CanvasDashStyle::Dash);
			float gantry_y, vessel_y;

			this->station->fill_anchor_location(CS::gantry, nullptr, &gantry_y);
			this->station->fill_anchor_location(CS::HV16, nullptr, &vessel_y);

			this->gantry_pipe = this->master->insert_one(
				new Linelet(0.0F, 0.0F, 0.0F, vessel_y - gantry_y,
					default_pipe_thickness, default_pipe_color, gantry_style));
		}
		*/
		{ // load valves
			//this->load_valve(this->gvalves, this->vlabels, this->captions, CS::HV09, radius, 0.0);//没有电动阀?
			this->load_valves(this->gvalves, this->mvalves, this->vlabels, this->captions, CS::HV09, CS::HV26, radius, 00.0);
			this->load_valves(this->gvalves, this->mvalves, this->vlabels, this->captions, CS::HV13, CS::HV24, radius, 90.0);
			this->load_valves(this->gvalves, this->mvalves, this->vlabels, this->captions, CS::HV19, CS::HV17, radius, -45.0);
			this->load_valves(this->gvalves, this->mvalves, this->vlabels, this->captions, CS::HV20, CS::HV18, radius, 45.0);
		}

		{ // load special nodes
			float nic_radius = gheight * 0.25F;

			this->load_pump(this->hoppers, this->captions, CS::PSHPump, -radius, +2.0F, 0.0, gwidth);
			this->load_pump(this->hoppers, this->captions, CS::SBHPump, -radius, -2.0F, 0.0, gwidth);
			this->load_pump(this->hoppers, this->captions, CS::PSUWPump, -radius, -2.0F, -90.0, gwidth);
			this->load_pump(this->hoppers, this->captions, CS::SBUWPump, -radius, +2.0F, +90.0, gwidth);

			this->lLMOD = this->master->insert_one(new Arclet(0.0, 360.0, gheight, gheight, 1.0F, Colours::Green));
			this->rLMOD = this->master->insert_one(new Arclet(0.0, 360.0, gheight, gheight, 1.0F, Colours::Green));

			this->tips[CS::Port] = this->master->insert_one(
				new Segmentlet(-90.0, 90.0, gwidth * 2.0F, gheight,
					default_ps_color, default_pipe_thickness));

			this->tips[CS::Starboard] = this->master->insert_one(
				new Segmentlet(-90.0, 90.0, gwidth * 2.0F, gheight,
					default_sb_color, default_pipe_thickness));

			//绿点
			/*
			for (CS id = CS::h19; id <= CS::h0408; id++) {
				this->intercs[id] = this->master->insert_one(
					new Circlelet(default_pipe_thickness * 2.0F, Colours::Green));
			}*/
			
			for (CS id = CS::jump0107; id <= CS::jump1314; id++) {
				this->nintercs[id] = this->master->insert_one(
					new Omegalet(-90.0, nic_radius, default_pipe_thickness, default_pipe_color));
			}
		}

		{ // load labels and dimensions
			this->load_percentage(this->progresses, CS::HV01);
			this->load_percentage(this->progresses, CS::HV02);
			this->load_percentage(this->progresses, CS::HV09);
			//this->load_dimensions(this->pump_pressures, CS::C, CS::H, "bar");

			this->load_label(this->captions, CS::lLMOD, Colours::Cyan, this->special_font);
			this->load_label(this->captions, CS::rLMOD, Colours::Cyan, this->special_font);
		}
		this->load_settings(this->pumps_rpm, CS::PSUWPump, CS::SBHPump, "RPM", "S");
	}

public:
	void reflow(float width, float height, float gwidth, float gheight, float vinset) {
		GraphletAnchor anchor;
		float dx, dy, margin, label_height, ox, oy;
		float gridsize = flmin(gwidth, gheight);
		float x0 = 0.0F;
		float y0 = 0.0F;

		this->master->move_to(this->station, width * 0.5F, height * 0.5F, GraphletAnchor::CC);
		this->station->map_graphlet_at_anchor(this->gantry_pipe, CS::gantry, GraphletAnchor::CT);

		this->station->map_credit_graphlet(this->captions[CS::lLMOD], GraphletAnchor::CC);
		this->station->map_credit_graphlet(this->captions[CS::rLMOD], GraphletAnchor::CC);

		this->station->map_graphlet_at_anchor(this->tips[CS::Port], CS::Port, GraphletAnchor::RC);
		this->station->map_graphlet_at_anchor(this->tips[CS::Starboard], CS::Starboard, GraphletAnchor::RC);
		this->station->map_graphlet_at_anchor(this->lLMOD, CS::lLMOD, GraphletAnchor::CC);
		this->station->map_graphlet_at_anchor(this->rLMOD, CS::rLMOD, GraphletAnchor::CC);

		for (auto it = this->intercs.begin(); it != this->intercs.end(); it++) {
			this->station->map_graphlet_at_anchor(it->second, it->first, GraphletAnchor::CC);
		}

		for (auto it = this->nintercs.begin(); it != this->nintercs.end(); it++) {
			/** NOTE
			 * Lines are brush-based shape, they do not have stroke, `Shapelet` does not know how width they are,
			 * thus, we have to do aligning on our own.
			 */
			this->station->map_graphlet_at_anchor(it->second, it->first, GraphletAnchor::LC, -default_pipe_thickness * 0.5F);//
		}

		for (auto it = this->hoppers.begin(); it != this->hoppers.end(); it++) {
			it->second->fill_pump_origin(&ox, &oy);

			if (ox == 0.0F) {
				this->station->map_credit_graphlet(it->second, GraphletAnchor::CC, 0.0F, -oy);
			}
			else {
				this->station->map_credit_graphlet(it->second, GraphletAnchor::CC, -ox, 0.0F);
			}
			ox = flmax(flabs(ox), flabs(oy));
			switch (it->first) {
			case CS::PSHPump: {
				this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::RC, GraphletAnchor::LC, ox);
				this->master->move_to(this->powers[it->first], it->second, GraphletAnchor::LB, GraphletAnchor::RB, -ox);
				this->master->move_to(this->rpms[it->first], it->second, GraphletAnchor::RB, GraphletAnchor::LB, ox);
				this->master->move_to(this->dpressures[it->first], it->second, GraphletAnchor::CT, GraphletAnchor::LB);
				this->master->move_to(this->vpressures[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RB, -ox);
			}; break;
			case CS::PSUWPump: {
				//this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RB, -ox);
				this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::CC, GraphletAnchor::CB, -ox,-oy*5);//?? 下对齐
				this->master->move_to(this->powers[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RT, -ox, ox);
				this->master->move_to(this->rpms[it->first], it->second, GraphletAnchor::RC, GraphletAnchor::LT, ox, ox);
				this->master->move_to(this->dpressures[it->first], it->second, GraphletAnchor::RC, GraphletAnchor::LB, ox);
				this->master->move_to(this->vpressures[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RB, 0.0F, -ox);//真空
				this->master->move_to(this->dfpressures[it->first], this->tips[CS::Port], GraphletAnchor::RC, GraphletAnchor::LB, 0.0F, -ox);//耙头压差
				this->master->move_to(this->flow[it->first], it->second, GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, 0.0F);//封水流量
			}; break;
			case CS::SBHPump: {
				this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::RC, GraphletAnchor::LC, ox);
				this->master->move_to(this->powers[it->first], it->second, GraphletAnchor::LT, GraphletAnchor::RT, -ox);
				this->master->move_to(this->rpms[it->first], it->second, GraphletAnchor::RT, GraphletAnchor::LT, ox);
				this->master->move_to(this->dpressures[it->first], it->second, GraphletAnchor::CB, GraphletAnchor::LT);
				this->master->move_to(this->vpressures[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RT, -ox);
			}; break;
			case CS::SBUWPump: {
				//this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RT, -ox);
				this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::CC, GraphletAnchor::CT, -ox,-oy*5);
				this->master->move_to(this->powers[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RB, -ox, -ox);
				this->master->move_to(this->rpms[it->first], it->second, GraphletAnchor::RC, GraphletAnchor::LB, ox, -ox);
				this->master->move_to(this->dpressures[it->first], it->second, GraphletAnchor::RC, GraphletAnchor::LT, ox);
				this->master->move_to(this->vpressures[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RT, 0.0F, ox);//真空
				this->master->move_to(this->dfpressures[it->first], this->tips[CS::Starboard], GraphletAnchor::RC, GraphletAnchor::LT, 0.0F, ox);//耙头压差
				this->master->move_to(this->flow[it->first], it->second, GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, 0.0F);//封水流量
			}; break;
			}
		}

		this->vlabels[CS::HV09]->fill_extent(0.0F, 0.0F, nullptr, &label_height);

		for (auto it = this->gvalves.begin(); it != this->gvalves.end(); it++) {
			switch (it->first) {

			case CS::HV19:case CS::HV17: {
				dx = x0 + gwidth / 2; dy = y0 + gheight / 2; anchor = GraphletAnchor::LB;
				break;
			}
			case CS::HV20:case CS::HV18:
				dx = x0 + gwidth / 2; dy = y0 - gheight / 2; anchor = GraphletAnchor::LB;
				break;
			case CS::HV21:case CS::HV15:case CS::HV25:case CS::HV26:case CS::HV04:case CS::HV03:case CS::HV05:case CS::HV23://下
				dx = x0; dy = y0 + gheight; anchor = GraphletAnchor::CB;
				break;
			case CS::HV22:case CS::HV16:case CS::HV06: case CS::HV09: //上
				dx = x0; dy = y0 - gheight; anchor = GraphletAnchor::CB;
				break;
				/*case RS::HV14://左
					dx = x0 - gwidth; dy = y0; anchor = GraphletAnchor::RB;
					break;*/
			case CS::HV01:case CS::HV02:case CS::HV24:case CS::HV14://右
				dx = x0 + gwidth * 0.5f; dy = y0; anchor = GraphletAnchor::LB;
				break;
			case CS::HV07:case CS::HV08:case CS::HV12:case CS::HV13://更左一点
				dx = x0 - gwidth * 2; dy = y0; anchor = GraphletAnchor::RB;
				break;
			case CS::HV11: //更上一点
				dx = x0; dy = y0 - gheight * 3; anchor = GraphletAnchor::CB;
				break;
			case CS::HV10://更下一点
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

		{ // reflow motor-driven valves
			float polar45 = gridsize * flsqrt(2.0F) * 0.618F;

			for (auto it = this->mvalves.begin(); it != this->mvalves.end(); it++) {

				switch (it->first) {

				case CS::HV17: case CS::HV19: {
					dx = x0 - polar45; dy = y0 - polar45; anchor = GraphletAnchor::CC;
				}; break;
				case CS::HV18: case CS::HV20: {
					dx = x0 - polar45; dy = y0 + polar45; anchor = GraphletAnchor::CC;
				}; break;
				case CS::HV07:case CS::HV08:case CS::HV01:case CS::HV02:case CS::HV13:case CS::HV12:case CS::HV24: {//左
					this->gvalves[CS::HV01]->fill_margin(x0, y0, nullptr, nullptr, nullptr, &margin);
					dx = x0 - gridsize + margin; dy = y0; anchor = GraphletAnchor::RC;
				}; break;
				case CS::HV14: {//右
					this->gvalves[CS::HV01]->fill_margin(x0, y0, nullptr, nullptr, nullptr, &margin);
					dx = x0 - gridsize + margin; dy = y0; anchor = GraphletAnchor::RC;
				}; break;
				case CS::HV10:case CS::HV22:case CS::HV16:case CS::HV06: {//下
					dx = x0; dy = y0 + gridsize; anchor = GraphletAnchor::CC;
				}; break;
				default: {
					dx = x0; dy = y0 - gridsize; anchor = GraphletAnchor::CC;//上
				}
				}

				it->second->fill_valve_origin(&ox, &oy);
				this->station->map_credit_graphlet(it->second, anchor, dx - ox, dy - oy);
			}
		}

		{ // reflow dimensions
			float offset = default_pipe_thickness * 2.0F;

			this->master->move_to(this->progresses[CS::HV01], this->gvalves[CS::HV01], GraphletAnchor::CB, GraphletAnchor::LT, offset, -offset);
			this->master->move_to(this->progresses[CS::HV02], this->gvalves[CS::HV02], GraphletAnchor::CT, GraphletAnchor::LB, offset);
			this->master->move_to(this->progresses[CS::HV09], this->gvalves[CS::HV09], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, -margin);

			//this->station->map_credit_graphlet(this->pump_pressures[CS::C], GraphletAnchor::LT, gwidth * 3.0F);
			//this->master->move_to(this->pump_pressures[CS::F], this->pump_pressures[CS::C], GraphletAnchor::RC, GraphletAnchor::LC, gwidth);
			//this->master->move_to(this->pump_pressures[CS::H], this->pump_pressures[CS::F], GraphletAnchor::RC, GraphletAnchor::LC, gwidth);
		}

		this->station->map_graphlet_at_anchor(this->functions[CSFunction::Diagnostics], CS::diagnostics, GraphletAnchor::LB, gwidth * 3.0F);
		//this->master->move_to(this->functions[CSFunction::MotorInfo], this->functions[CSFunction::Diagnostics], GraphletAnchor::RC, GraphletAnchor::LC, gwidth * 2.0F);
		this->master->move_to(this->pumps_rpm[CS::PSUWPump], this->hoppers[CS::PSUWPump], GraphletAnchor::CT, GraphletAnchor::CB, 0.0F,-gheight);
		this->master->move_to(this->pumps_rpm[CS::SBUWPump], this->hoppers[CS::SBUWPump], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F,gheight);
		this->master->move_to(this->pumps_rpm[CS::PSHPump], this->hoppers[CS::PSHPump], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, 0.0F);
		this->master->move_to(this->pumps_rpm[CS::SBHPump], this->hoppers[CS::SBHPump], GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, 0.0F);
	}

public:
	bool hopper_selected(CS pid) {
		return this->master->is_selected(this->hoppers[pid]);
	}

	bool valves_selected(CS vids[], unsigned int count, int tolerance) {
		bool okay = false;
		int ok = 0;

		for (unsigned int idx = 0; idx < count; idx++) {
			if (this->master->is_selected(this->gvalves[vids[idx]])) {
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
	bool valves_selected(CS(&vids)[N], int tolerance) {
		return this->valves_selected(vids, N, tolerance);
	}

public:
	void draw_relationships(CanvasDrawingSession^ ds, float X, float Y, float Width, float Height) {
		float ox, oy, gx, gy, mx, my;

		for (auto it = this->mvalves.begin(); it != this->mvalves.end(); it++) {
			this->master->fill_graphlet_location(it->second, &mx, &my, GraphletAnchor::CC);
			this->master->fill_graphlet_location(this->gvalves[it->first], &gx, &gy, GraphletAnchor::CC);
			it->second->fill_valve_origin(&ox, &oy);

			ds->DrawLine(mx + ox + X, my + oy + Y, gx + X, gy + Y, this->relationship_color, 1.0F, this->relationship_style);
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

	template<class G, class M, typename E>
	void load_valves(std::map<E, G*>& gs, std::map<E, M*>& ms, std::map<E, Credit<Labellet, E>*>& ls
		, std::map<E, Credit<Labellet, E>*>& cs, E id0, E idn, float radius, double degrees) {
		float mradius = radius * 0.8F;

		for (E id = id0; id <= idn; id++) {
			double mdegrees = 0.0;

			switch (id) {//电动阀角度
			case CS::HV19: case CS::HV17: mdegrees = -45.0; break;
			case CS::HV20: case CS::HV18: mdegrees = -135.0; break;
			case CS::HV22:case CS::HV16:case CS::HV10:case CS::HV06: mdegrees = -180.0; break;
			}

			// moter-driven valves' second, catching events first 
			this->load_valve(gs, ls, cs, id, radius, degrees);
			if (id != CS::HV09) {
				ms[id] = this->master->insert_one(new M(mradius, mdegrees, false), id);
			}
		}
	}

	template<class B, typename CMD>
	void load_buttons(std::map<CMD, Credit<B, CMD>*>& bs, float width = 128.0F, float height = 32.0F) {
		for (CMD cmd = _E(CMD, 0); cmd < CMD::_; cmd++) {
			bs[cmd] = this->master->insert_one(new Credit<B, CMD>(cmd.ToString(), width, height), cmd);
		}
	}

	template<typename E>
	void load_settings(std::map<E, Credit<Dimensionlet, E>*>& ds, E id0, E idn, Platform::String^ unit, Platform::String^ label) {
		for (E id = id0; id <= idn; id++) {
			ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(DimensionState::Input, this->setting_style, unit, label), id);
			ds[id]->set_maximum(double(gland_pump_rpm_range));
		}
	}
	template<class G, typename E>
	void load_pump(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, E id, float rx, float fy, double degrees, float gapsize) {
		this->load_label(ls, id, Colours::Salmon, this->caption_font);

		gs[id] = this->master->insert_one(new G(rx, std::fabsf(rx) * fy, degrees), id);

		this->load_dimension(this->powers, id, "kwatt", 0);
		this->load_dimension(this->rpms, id, "rpm", 0);
		this->load_dimension(this->dpressures, id, "bar", 1);

		switch (id) {
		case E::PSHPump:  case E::SBHPump:  this->load_dimension(this->vpressures, id, "bar", 1);  break;
		case E::PSUWPump: case E::SBUWPump: 
			this->load_dimension(this->dfpressures, id, "bar", 1); 
			this->load_dimension(this->vpressures, id, "bar", 1);
			//this->load_dimension(this->flow, id, "m³/h", 1);
			break;
		}
	}

	template<typename E>
	void load_percentage(std::map<E, Credit<Percentagelet, E>*>& ps, E id) {
		ps[id] = this->master->insert_one(new Credit<Percentagelet, E>(this->percentage_style), id);
	}

	template<typename E>
	void load_dimension(std::map<E, Credit<Dimensionlet, E>*>& ds, E id, Platform::String^ unit, int precision) {
		this->hopper_style.precision = precision;
		ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(this->hopper_style, unit), id);
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

private:
	void set_valves_status(CS id
		, const uint8* db4, unsigned int gidx4_p1, unsigned int midx4_p1
		, const uint8* db205, unsigned int gidx205_p1, unsigned int midx205_p1) {
		DI_gate_valve(this->gvalves[id], db4, gidx4_p1, db205, gidx205_p1);//先打开再关闭

		if (this->mvalves.find(id) != this->mvalves.end()) {
			DI_motor_valve(this->mvalves[id], db4, midx4_p1, db205, midx205_p1);//先关闭再打开 
		}
	}

private:
	bool valve_open(CS vid) {
		return (this->gvalves[vid]->get_state() == GateValveState::Open);
	}

	void try_flow_water(CS vid, CS eid1, CS eid2, CanvasSolidColorBrush^ color) {
		if (this->valve_open(vid)) {
			this->station->push_subtrack(vid, eid1, color);

			if (eid2 != CS::_) {
				this->station->push_subtrack(vid, eid2, color);
			}
		}
	}

	void try_flow_water(CS vid, CS* path, unsigned int count, CanvasSolidColorBrush^ color) {
		if (this->valve_open(vid)) {
			this->station->push_subtrack(vid, path[0], color);
			this->station->push_subtrack(path, count, color);
		}
	}

	template<unsigned int N>
	void try_flow_water(CS vid, CS(&path)[N], CanvasSolidColorBrush^ color) {
		this->try_flow_water(vid, path, N, color);
	}

	void try_flow_water(CS vid, CS eid, CanvasSolidColorBrush^ color) {
		this->try_flow_water(vid, eid, CS::_, color);
	}

	void try_flow_water(int count, ...) {
		va_list arguments;
		CS cur = CS::_;
		CS old = CS::_;
		va_start(arguments, count);
		for (int i = 0; i < count + 1; i++) {
			if (cur == CS::_) {
				cur = va_arg(arguments, CS);
			}
			else {
				if ((old != CS::_) && (cur != CS::_)) {
					this->station->push_subtrack(old, cur, water_color);
				}
				old = cur;
				cur = va_arg(arguments, CS);
			}
		}
		va_end(arguments);
	}


	// never deletes these graphlets mannually
private:
	Tracklet<CS>* station;
	Linelet* gantry_pipe;
	std::map<CS, Credit<Labellet, CS>*> captions;
	std::map<CS, Credit<HopperPumplet, CS>*> hoppers;
	std::map<CS, Credit<GateValvelet, CS>*> gvalves;
	std::map<CS, Credit<MotorValvelet, CS>*> mvalves;
	std::map<CS, Credit<Labellet, CS>*> vlabels;
	std::map<CS, Credit<Percentagelet, CS>*> progresses;
	std::map<CS, Credit<Dimensionlet, CS>*> pump_pressures;
	std::map<CS, Credit<Dimensionlet, CS>*> dpressures;//排压
	std::map<CS, Credit<Dimensionlet, CS>*> vpressures; //吸入真空
	std::map<CS, Credit<Dimensionlet, CS>*> dfpressures;//耙头压差
	std::map<CS, Credit<Dimensionlet, CS>*> flow; //水下泵封水流量
	std::map<CS, Credit<Dimensionlet, CS>*> motors;
	std::map<CS, Credit<Dimensionlet, CS>*> powers;
	std::map<CS, Credit<Dimensionlet, CS>*> rpms;
	std::map<CS, Credit<Dimensionlet, CS>*> pumps_rpm;//泵转速设定
	std::map<CS, Omegalet*> nintercs;
	std::map<CS, Circlelet*> intercs;
	std::map<CS, Shapelet*> tips;
	Arclet* lLMOD;
	Arclet* rLMOD;

private:
	std::map<CSFunction, Credit<Buttonlet, CSFunction>*> functions;

private:
	CanvasTextFormat^ caption_font;
	CanvasTextFormat^ label_font;
	CanvasTextFormat^ special_font;
	ICanvasBrush^ relationship_color;
	CanvasStrokeStyle^ relationship_style;
	DimensionStyle pump_style;
	DimensionStyle highlight_style;
	DimensionStyle percentage_style;
	DimensionStyle hopper_style;
	DimensionStyle setting_style;

private:
	ChargesPage* master;
	MenuFlyout^ PSHopper_menu;
	MenuFlyout^ SBHopper_menu;
	MenuFlyout^ PSUnderWater_menu;
	MenuFlyout^ SBUnderWater_menu;
};

private class VesselDecorator : public TVesselDecorator<Vessel, CS> {
public:
	VesselDecorator(Vessel* master) : TVesselDecorator<Vessel, CS>(master) {}

public:
	void draw_non_important_lines(Tracklet<CS>* station, CanvasDrawingSession^ ds, float x, float y, CanvasStrokeStyle^ style) override {
		/*
		float d0525_x, d05_y, d25_y;

		station->fill_anchor_location(CS::HV05, &d0525_x, &d05_y, false);
		station->fill_anchor_location(CS::HV25, nullptr, &d25_y, false);

		ds->DrawLine(x + d0525_x, y + d05_y, x + d0525_x, y + d25_y,
			Colours::DimGray, default_pipe_thickness, style);
			*/
	}
};

/*************************************************************************************************/
ChargesPage::ChargesPage(PLCMaster* plc) : Planet(__MODULE__), device(plc) {
	Vessel* dashboard = nullptr;

	if (this->device != nullptr) {
		this->diagnostics = new HopperPumpDiagnostics(plc);
		this->motor_info = new UnderwaterPumpMotorMetrics(plc);

		this->gate_valve_op = make_gate_valve_menu(DO_gate_valve_action, plc);
		this->ghopper_op = make_charge_condition_menu(GroupChargeAction::BothHopper, plc);
		this->gunderwater_op = make_charge_condition_menu(GroupChargeAction::BothUnderWater, plc);
		//this->ghbarge_op = make_charge_condition_menu(GroupChargeAction::HPBarge, plc);
		//this->guwbarge_op = make_charge_condition_menu(GroupChargeAction::UWPBarge, plc);
		this->ghps_op = make_charge_condition_menu(GroupChargeAction::PSHopper, plc);
		this->guwps_op = make_charge_condition_menu(GroupChargeAction::PSUnderWater, plc);
		this->ghsb_op = make_charge_condition_menu(GroupChargeAction::SBHopper, plc);
		this->guwsb_op = make_charge_condition_menu(GroupChargeAction::SBUnderWater, plc);
		this->ps_hopper_op = make_ps_hopper_pump_charge_menu(plc);
		this->sb_hopper_op = make_sb_hopper_pump_charge_menu(plc);
		this->ps_underwater_op = make_ps_underwater_pump_charge_menu(plc);
		this->sb_underwater_op = make_sb_underwater_pump_charge_menu(plc);


		{ // only highlight menu items of these four menus
			dashboard = new Vessel(this, this->ps_hopper_op, this->sb_hopper_op, this->ps_underwater_op, this->sb_underwater_op);
			this->device->push_confirmation_receiver(dashboard);
		}
	}
	else {
		dashboard = new Vessel(this);
	}

	{ // load decorators
		this->grid = new GridDecorator();

#ifdef _DEBUG
		this->push_decorator(this->grid);
#else
		this->grid->set_active_planet(this);
#endif

		this->push_decorator(new VesselDecorator(dashboard));
		this->dashboard = dashboard;
	}
}

ChargesPage::~ChargesPage() {
	if (this->dashboard != nullptr) {
		delete this->dashboard;
	}

#ifndef _DEBUG
	delete this->grid;
#endif
}

void ChargesPage::load(CanvasCreateResourcesReason reason, float width, float height) {
	auto dashboard = dynamic_cast<Vessel*>(this->dashboard);

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

void ChargesPage::reflow(float width, float height) {
	auto dashboard = dynamic_cast<Vessel*>(this->dashboard);

	if (dashboard != nullptr) {
		float vinset = statusbar_height();
		float gwidth = this->grid->get_grid_width();
		float gheight = this->grid->get_grid_height();

		dashboard->reflow(width, height, gwidth, gheight, vinset);
	}
}

void ChargesPage::on_timestream(long long timepoint_ms, size_t addr0, size_t addrn, uint8* data, size_t size, uint64 p_type, size_t p_size, Syslog* logger) {
	auto dashboard = dynamic_cast<Vessel*>(this->dashboard);

	if (dashboard != nullptr) {
		dashboard->on_all_signals(timepoint_ms, addr0, addrn, data, size, logger);
	}
}

bool ChargesPage::can_select(IGraphlet* g) {
	return ((this->device != nullptr)
		&& (dynamic_cast<Credit<Buttonlet, CSFunction>*>(g)
			|| (dynamic_cast<GateValvelet*>(g) != nullptr)
			|| (dynamic_cast<HopperPumplet*>(g) != nullptr)));
}

bool ChargesPage::can_select_multiple() {
	return (this->device != nullptr);
}

void ChargesPage::on_tap_selected(IGraphlet* g, float local_x, float local_y) {
		auto gvalve = dynamic_cast<GateValvelet*>(g);
		auto hpump = dynamic_cast<Credit<HopperPumplet, CS>*>(g);
	auto function = dynamic_cast<Credit<Buttonlet, CSFunction>*>(g);

	if (gvalve != nullptr) {
		menu_popup(this->gate_valve_op, g, local_x, local_y);
	}
	else if (function != nullptr) {
		switch (function->id) {
		case CSFunction::Diagnostics: this->diagnostics->show(); break;
			//case CSFunction::MotorInfo: this->motor_info->show(); break;
		}
	}
	else if (hpump != nullptr) {
		switch (hpump->id) {
		case CS::PSHPump: menu_popup(this->ps_hopper_op, g, local_x, local_y); break;
		case CS::SBHPump: menu_popup(this->sb_hopper_op, g, local_x, local_y); break;
		case CS::PSUWPump: menu_popup(this->ps_underwater_op, g, local_x, local_y); break;
		case CS::SBUWPump: menu_popup(this->sb_underwater_op, g, local_x, local_y); break;
		}
	}
}

void ChargesPage::on_gesture(std::list<float2>& anchors, float x, float y) {
	/*
	auto dashboard = dynamic_cast<Vessel*>(this->dashboard);

	if (dashboard != nullptr) {
		bool ps_underwater = dashboard->hopper_selected(CS::PSUWPump);
		bool sb_underwater = dashboard->hopper_selected(CS::SBUWPump);
		bool ps_hopper = dashboard->hopper_selected(CS::PSHPump);
		bool sb_hopper = dashboard->hopper_selected(CS::SBHPump);
		CS barges[] = { CS::D024 };
		CS uwps[] = { CS::D004, CS::D006, CS::D009 };
		CS hps[] = { CS::D004, CS::D005, CS::D017 };
		CS uwsb[] = { CS::D003, CS::D026, CS::D007 };
		CS hsb[] = { CS::D003, CS::D025, CS::D018 };

		if (ps_underwater && sb_underwater) {
			group_menu_popup(this->gunderwater_op, this, x, y);
		} else if (ps_hopper && sb_hopper) {
			group_menu_popup(this->ghopper_op, this, x, y);
		} else if (sb_underwater) {
			if (dashboard->valves_selected(barges, 1)) {
				group_menu_popup(this->guwbarge_op, this, x, y);
			} else if (dashboard->valves_selected(uwsb, 3)) {
				group_menu_popup(this->guwsb_op, this, x, y);
			}
		} else if (sb_hopper) {
			if (dashboard->valves_selected(barges, 1)) {
				group_menu_popup(this->ghbarge_op, this, x, y);
			} else if (dashboard->valves_selected(uwsb, 1)) {
				group_menu_popup(this->ghsb_op, this, x, y);
			}
		} else if (ps_underwater) {
			if (dashboard->valves_selected(uwps, 3)) {
				group_menu_popup(this->guwps_op, this, x, y);
			}
		} else if (ps_hopper) {
			if (dashboard->valves_selected(hps, 3)) {
				group_menu_popup(this->ghps_op, this, x, y);
			}
		}
	}*/
}

bool ChargesPage::on_key(VirtualKey key, bool wargrey_keyboard) {
	bool handled = Planet::on_key(key, wargrey_keyboard);

	if ((!handled) && (this->device != nullptr) && (this->device->authorized())) {
		auto db = dynamic_cast<Vessel*>(this->dashboard);

		if (db != nullptr) {
			handled = db->on_key(key, this->device);
		}
	}

	return handled;
}

void ChargesPage::on_focus(IGraphlet* g, bool yes) {
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