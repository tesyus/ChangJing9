#include <map>

#include "page/glands.hpp"
#include "page/diagnostics/gland_pump_dx.hpp"

#include "configuration.hpp"
#include "menu.hpp"

#include "module.hpp"
#include "text.hpp"
#include "paint.hpp"
#include "brushes.hxx"
#include "turtle.hpp"

#include "graphlet/shapelet.hpp"
#include "graphlet/ui/statuslet.hpp"
#include "graphlet/symbol/door/hatchlet.hpp"
#include "graphlet/symbol/pump/hydraulic_pumplet.hpp"
#include "graphlet/symbol/pump/hopper_pumplet.hpp"
#include "graphlet/symbol/valve/manual_valvelet.hpp"

#include "iotables/ai_hopper_pumps.hpp"
#include "iotables/di_hopper_pumps.hpp"
#include "iotables/do_hopper_pumps.hpp"
#include "iotables/ao_gland_pumps.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

using namespace Windows::System;
using namespace Windows::Foundation;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::Brushes;
using namespace Microsoft::Graphics::Canvas::Geometry;

static CanvasSolidColorBrush^ water_color = Colours::Green;
static GlandPumpDiagnostics* satellite = nullptr; // this satellite will be destroyed by `atexit()`;

// WARNING: order matters
private enum class GP : unsigned int {
	// 泵
	PSFP,SBFP,
	PSHP1, PSHP2, PSHP3, SBHP1, SBHP2, SBHP3,
	PSUWP1, PSUWP2,SBUWP1,SBUWP2,

	PSHP, SBHP, PSUWP, SBUWP,
	//入口锚点
	ToPSFP, ToSBFP,
	ToPSHP1, ToPSHP2, ToPSHP3, ToSBHP1, ToSBHP2, ToSBHP3,
	ToPSUWP1, ToPSUWP2, ToSBUWP1, ToSBUWP2,
	
	//未命名锚点
	pssbfptoflush,
	pshp1,pshp3,sbhp1,sbhp3,
	pshp1p,sbhp1p,
	pshp1end, pshp3end, pshp3end2, sbhp1end, sbhp3end, sbhp3end2,
	pshp2u, pshp2d,sbhp2u,sbhp2d,
	psuwp2to1, sbuwp2to1,
	psuwp2, sbuwp2,

	/*
	PSHP, SBHP, PSUWP, 
	SBUWP, PSFP, SBFP,
	PSHPa, PSHPb, SBHPa, SBHPb,
	PSUWP1, PSUWP2, SBUWP1, SBUWP2,*/

	// Manual Valves
	 //长鲸9没有阀
	//DGV3, DGV4, DGV5, DGV6,
	//DGV12, DGV11, DGV13, DGV14, DGV15, DGV16,
	//DGV7, DGV44, DGV45, DGV8,
	
	// Labels
	ToFlushs, /*SS1, SS2, SS3, SS4,*/ Hatch, Sea,
	
	_,
	
	// anchors used as last jumping points

	//d3, d4, d5, d6,
	//d12, d11, d13, d14, d15, d16, d17, d18, d19, d20,
	//d44, d45, d46, d47,

	// anchors used for unnamed corners
	//pshp1, pshp3, sbhp1, sbhp3,
	flushs, psuwp, sbuwp,
	psbranch,sbbranch,
};

static uint16 DO_glands_action(GlandPumpAction cmd, HydraulicPumplet* pump) {
	auto credit_pump = dynamic_cast<Credit<HydraulicPumplet, GP>*>(pump);
	uint16 index = 0U;
	
	if (credit_pump != nullptr) {
		switch (credit_pump->id) {
		case GP::PSFP: index = DO_gate_flushing_pump_command(cmd, true); break;
		case GP::SBFP: index = DO_gate_flushing_pump_command(cmd, false); break;
		case GP::PSHP1: index = DO_gland_pump_command(cmd, true, true, 1); break;
		case GP::PSHP2: index = DO_gland_pump_command(cmd, true, true, 2); break;
		case GP::PSHP3: index = DO_gland_pump_command(cmd, true, true, 3); break;
		case GP::SBHP1: index = DO_gland_pump_command(cmd, false, true, 1); break;
		case GP::SBHP2: index = DO_gland_pump_command(cmd, false, true, 2); break;
		case GP::SBHP3: index = DO_gland_pump_command(cmd, false, true, 3); break;
		case GP::PSUWP1: index = DO_gland_pump_command(cmd, true, false, 1); break;
		case GP::PSUWP2: index = DO_gland_pump_command(cmd, true, false, 2); break;
		case GP::SBUWP1: index = DO_gland_pump_command(cmd, false, false, 1); break;
		case GP::SBUWP2: index = DO_gland_pump_command(cmd, false, false, 2); break;
		}
	}
	
	return index;
}

static void gland_pump_diagnostics(HydraulicPumplet* pump, PLCMaster* plc) {
	auto credit_pump = dynamic_cast<Credit<HydraulicPumplet, GP>*>(pump);
	/*
	if (credit_pump != nullptr) {
		unsigned int details_index = 0U;
		PumpType type = PumpType::_;

		if (satellite == nullptr) {
			satellite = new GlandPumpDiagnostics(plc);
		}

		switch (credit_pump->id) {
		case GP::PSFP: satellite->switch_id(ps_gate_flushing_pump_feedback); details_index = ps_gate_flushing_pump_status; break;
		case GP::SBFP: satellite->switch_id(sb_gate_flushing_pump_feedback); details_index = sb_gate_flushing_pump_status; break;
		case GP::PSHPa: satellite->switch_id(ps_hopper_master_gland_pump_feedback); type = PumpType::Hopper; break;
		case GP::PSHPb: satellite->switch_id(ps_hopper_spare_gland_pump_feedback); type = PumpType::Hopper; break;
		case GP::SBHPa: satellite->switch_id(sb_hopper_master_gland_pump_feedback); type = PumpType::Hopper; break;
		case GP::SBHPb: satellite->switch_id(sb_hopper_spare_gland_pump_feedback); type = PumpType::Hopper; break;
		case GP::PSUWP1: satellite->switch_id(ps_underwater_master_gland_pump_feedback); type = PumpType::Underwater; break;
		case GP::PSUWP2: satellite->switch_id(ps_underwater_spare_gland_pump_feedback); type = PumpType::Underwater; break;
		case GP::SBUWP1: satellite->switch_id(sb_underwater_master_gland_pump_feedback); type = PumpType::Underwater; break;
		case GP::SBUWP2: satellite->switch_id(sb_underwater_spare_gland_pump_feedback); type = PumpType::Underwater; break;
		}

		satellite->set_pump(credit_pump->id.ToString(), type, details_index);
		satellite->show();
	}*/
}

/*************************************************************************************************/
private class GlandPumps final : public PLCConfirmation {
public:
	GlandPumps(GlandsPage* master) : master(master), sea_oscillation(1.0F) {
		this->label_font = make_bold_text_format("Microsoft YaHei", small_font_size);
		this->dimension_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 1U);
		this->setting_style = make_highlight_dimension_style(large_metrics_font_size, 6U, 0, Colours::GhostWhite, Colours::RoyalBlue);
	}

public:
	void pre_read_data(Syslog* logger) override {
		this->master->enter_critical_section();
		this->master->begin_update_sequence();

		this->station->clear_subtacks();
	}

	void on_analog_input(long long timepoint_ms, const uint8* DB2, size_t count2, const uint8* DB203, size_t count203, Syslog* logger) override {
		//this->pressures[GP::PSFP]->set_value(RealData(DB203, ps_flushing_pump_pressure), GraphletAnchor::LB);

		this->pressures[GP::pshp1]->set_value(RealData(DB203, ps_hopper_gland_1_pump_pressure), GraphletAnchor::LB);
		this->flows[GP::pshp1]->set_value(RealData(DB203, ps_hopper_gland_1_pump_flow), GraphletAnchor::LT);
		this->pressures[GP::pshp3]->set_value(RealData(DB203, ps_hopper_gland_3_pump_pressure), GraphletAnchor::LB);
		this->flows[GP::pshp3]->set_value(RealData(DB203, ps_hopper_gland_3_pump_flow), GraphletAnchor::LT);

		this->pressures[GP::pshp1p]->set_value(RealData(DB203, ps_hopper_gland_pressure), GraphletAnchor::LB);//泥沙分离器

		this->powers[GP::PSHP]->set_value(RealData(DB203, ps_hopper_pump_power), GraphletAnchor::LB);
		this->rpms[GP::PSHP]->set_value(RealData(DB203, ps_hopper_pump_rpm), GraphletAnchor::LB);

		this->pressures[GP::sbhp1]->set_value(RealData(DB203, sb_hopper_gland_pump_1_pressure), GraphletAnchor::LB);
		this->flows[GP::sbhp1]->set_value(RealData(DB203, sb_hopper_gland_pump_1_flow), GraphletAnchor::LB);
		this->pressures[GP::sbhp3]->set_value(RealData(DB203, sb_hopper_gland_pump_3_pressure), GraphletAnchor::LB);
		this->flows[GP::sbhp3]->set_value(RealData(DB203, sb_hopper_gland_pump_3_flow), GraphletAnchor::LB);

		this->pressures[GP::sbhp1p]->set_value(RealData(DB203, sb_hopper_gland_pressure), GraphletAnchor::LB);//泥沙分离器

		this->powers[GP::SBHP]->set_value(RealData(DB203, sb_hopper_pump_power), GraphletAnchor::LB);
		this->rpms[GP::SBHP]->set_value(RealData(DB203, sb_hopper_pump_rpm), GraphletAnchor::LB);

		this->powers[GP::PSUWP]->set_value(RealData(DB203, ps_underwater_pump_power), GraphletAnchor::LB);
		this->rpms[GP::PSUWP]->set_value(RealData(DB203, ps_underwater_pump_rpm), GraphletAnchor::LB);
		this->powers[GP::SBUWP]->set_value(RealData(DB203, sb_underwater_pump_power), GraphletAnchor::LB);
		this->rpms[GP::SBUWP]->set_value(RealData(DB203, sb_underwater_pump_rpm), GraphletAnchor::LB);

		this->rpms[GP::PSHP1]->set_value(RealData(DB203, ps_hopper_gland_pump_1_rpm), GraphletAnchor::LB);
		this->rpms[GP::PSHP2]->set_value(RealData(DB203, ps_hopper_gland_pump_2_rpm), GraphletAnchor::LB);
		this->rpms[GP::PSHP3]->set_value(RealData(DB203, ps_hopper_gland_pump_3_rpm), GraphletAnchor::LB);
		this->rpms[GP::SBHP1]->set_value(RealData(DB203, sb_hopper_gland_pump_1_rpm), GraphletAnchor::LB);
		this->rpms[GP::SBHP2]->set_value(RealData(DB203, sb_hopper_gland_pump_2_rpm), GraphletAnchor::LB);
		this->rpms[GP::SBHP3]->set_value(RealData(DB203, sb_hopper_gland_pump_3_rpm), GraphletAnchor::LB);

		//this->pressures[GP::PSUWP1]->set_value(RealData(DB203, ps_underwater_gland_pump1_pressure), GraphletAnchor::LB);
		this->pressures[GP::PSUWP2]->set_value(RealData(DB203, ps_underwater_gland_pump1_pressure), GraphletAnchor::LB);
		this->flows[GP::PSUWP]->set_value(RealData(DB203, ps_underwater_pump_flow), GraphletAnchor::LB);
		
		//this->pressures[GP::SBUWP1]->set_value(RealData(DB203, sb_underwater_gland_pump1_pressure), GraphletAnchor::LB);
		this->pressures[GP::SBUWP2]->set_value(RealData(DB203, sb_underwater_gland_pump1_pressure), GraphletAnchor::LB);
		this->flows[GP::SBUWP]->set_value(RealData(DB203, sb_underwater_pump_flow), GraphletAnchor::LB);
	}

	void on_digital_input(long long timepoint_ms, const uint8* DB4, size_t count4, const uint8* DB205, size_t count25, Syslog* logger) override {
		//DI_hopper_pumps(this->hoppers[GP::PSHP], this->hoppers[GP::PSUWP], DB4, ps_hopper_pump_feedback, DB205, ps_hopper_pump_details, ps_underwater_pump_details);
		//DI_hopper_pumps(this->hoppers[GP::SBHP], this->hoppers[GP::SBUWP], DB4, sb_hopper_pump_feedback, DB205, sb_hopper_pump_details, sb_underwater_pump_details);
		
		DI_gate_flushing_pump(this->pumps[GP::PSFP], DB4, ps_gate_flushing_pump_feedback, DB205, ps_gate_flushing_pump_status);
		DI_gate_flushing_pump(this->pumps[GP::SBFP], DB4, sb_gate_flushing_pump_feedback, DB205, sb_gate_flushing_pump_status);

		DI_gland_pump(this->pumps[GP::PSHP1], true, DB4, ps_hopper_1_gland_pump_feedback, DB205, ps_hopper_1_gland_pump_status);
		DI_gland_pump(this->pumps[GP::PSHP2], true, DB4, ps_hopper_2_gland_pump_feedback, DB205, ps_hopper_2_gland_pump_status);
		DI_gland_pump(this->pumps[GP::PSHP3], true, DB4, ps_hopper_3_gland_pump_feedback, DB205, ps_hopper_3_gland_pump_status);
		DI_gland_pump(this->pumps[GP::SBHP1], true, DB4, sb_hopper_1_gland_pump_feedback, DB205, sb_hopper_1_gland_pump_status);
		DI_gland_pump(this->pumps[GP::SBHP2], true, DB4, sb_hopper_2_gland_pump_feedback, DB205, sb_hopper_2_gland_pump_status);
		DI_gland_pump(this->pumps[GP::SBHP3], true, DB4, sb_hopper_3_gland_pump_feedback, DB205, sb_hopper_3_gland_pump_status);

		DI_uw_gland_pump(this->pumps[GP::PSUWP1], false, DB4, ps_underwater_master_gland_pump_feedback, DB205, ps_underwater_master_gland_pump_status);
		DI_uw_gland_pump(this->pumps[GP::PSUWP2], false, DB4, ps_underwater_spare_gland_pump_feedback, DB205, ps_underwater_spare_gland_pump_status);
		DI_uw_gland_pump(this->pumps[GP::SBUWP1], false, DB4, sb_underwater_master_gland_pump_feedback, DB205, sb_underwater_master_gland_pump_status);
		DI_uw_gland_pump(this->pumps[GP::SBUWP2], false, DB4, sb_underwater_spare_gland_pump_feedback, DB205, sb_underwater_spare_gland_pump_status);

		/*
		DI_hopper_pump_control_mode(this->controlmodelocal[GP::PSHP1], DB4, pspump1controlmode_local);//
		DI_hopper_pump_control_mode(this->controlmodelocal[GP::PSHP2], DB4, pspump2controlmode_local);//
		DI_hopper_pump_control_mode(this->controlmodelocal[GP::PSHP3], DB4, pspump3controlmode_local);//
		DI_hopper_pump_control_mode(this->controlmodelocal[GP::SBHP1], DB4, sbpump1controlmode_local);//
		DI_hopper_pump_control_mode(this->controlmodelocal[GP::SBHP2], DB4, sbpump2controlmode_local);//
		DI_hopper_pump_control_mode(this->controlmodelocal[GP::SBHP3], DB4, sbpump3controlmode_local);//

		DI_hopper_pump_control_mode(this->controlmodemachine[GP::PSHP1], DB4, pspump1controlmode_machine);//
		DI_hopper_pump_control_mode(this->controlmodemachine[GP::PSHP2], DB4, pspump2controlmode_machine);//
		DI_hopper_pump_control_mode(this->controlmodemachine[GP::PSHP3], DB4, pspump3controlmode_machine);//
		DI_hopper_pump_control_mode(this->controlmodemachine[GP::SBHP1], DB4, sbpump1controlmode_machine);//
		DI_hopper_pump_control_mode(this->controlmodemachine[GP::SBHP2], DB4, sbpump2controlmode_machine);//
		DI_hopper_pump_control_mode(this->controlmodemachine[GP::SBHP3], DB4, sbpump3controlmode_machine);//
		*/
		DI_flush_alarm(this->alarm[GP::PSFP], DB4, ps_flushing_pump_alarm);//
		DI_flush_alarm(this->alarm[GP::SBFP], DB4, sb_flushing_pump_alarm);//
	}
	void on_signals_updated(long long timepoint_ms, Syslog* logger) override {

		{ // flow water

			this->station->push_subtrack(GP::Hatch, GP::SBUWP2, water_color);
			this->try_flow_water(GP::PSFP, GP::PSFP, GP::flushs, water_color);
			this->try_flow_water(GP::SBFP, GP::SBFP, GP::flushs, water_color);

			if (this->pump_open(GP::PSHP1)) {
				this->try_flow_water(3, GP::PSHP1, GP::pshp1, GP::pshp1end);
			}
			if (this->pump_open(GP::PSHP2)) {
				if (!this->pump_open(GP::PSHP1))
					this->try_flow_water(4, GP::PSHP2, GP::pshp2u, GP::pshp1, GP::pshp1end);
				if (!this->pump_open(GP::PSHP3))
					this->try_flow_water(4, GP::PSHP2, GP::pshp2d, GP::pshp3, GP::pshp3end, GP::pshp3end2);
			}
			if (this->pump_open(GP::PSHP3)) {
				this->try_flow_water(3, GP::PSHP3, GP::pshp3, GP::pshp3end, GP::pshp3end2);
			}

			if (this->pump_open(GP::SBHP1)) {
				this->try_flow_water(3, GP::SBHP1, GP::sbhp1, GP::sbhp1end);
			}
			if (this->pump_open(GP::SBHP2)) {
				if (!this->pump_open(GP::SBHP1))
					this->try_flow_water(4, GP::SBHP2, GP::sbhp2u, GP::sbhp1, GP::sbhp1end);
				if (!this->pump_open(GP::SBHP3))
					this->try_flow_water(4, GP::SBHP2, GP::sbhp2d, GP::sbhp3, GP::sbhp3end, GP::sbhp3end2);
			}
			if (this->pump_open(GP::SBHP3)) {
				this->try_flow_water(3, GP::SBHP3, GP::sbhp3, GP::sbhp3end, GP::sbhp3end2);
			}

			if (this->pump_open(GP::PSUWP1)) {
				this->try_flow_water(2, GP::PSUWP1, GP::PSUWP);
			}
			if (this->pump_open(GP::PSUWP2)) {
				this->try_flow_water(3, GP::PSUWP2, GP::psuwp2to1, GP::PSUWP);
			}
			if (this->pump_open(GP::SBUWP1)) {
				this->try_flow_water(2, GP::SBUWP1, GP::SBUWP);
			}
			if (this->pump_open(GP::SBUWP2)) {
				this->try_flow_water(3, GP::SBUWP2, GP::sbuwp2to1, GP::SBUWP);
			}
		}

	}
	void post_read_data(Syslog* logger) override {
		this->master->end_update_sequence();
		this->master->leave_critical_section();
	}

public:
	bool on_key(VirtualKey key, PLCMaster* plc) {
		bool handled = false;

		if (key == VirtualKey::Enter) {
			auto editor = dynamic_cast<Credit<Dimensionlet, GP>*>(this->master->get_focus_graphlet());

			if (editor != nullptr) {
				float rpm = float(editor->get_input_number());
				float percentage = rpm / float(gland_pump_rpm_range);

				if (rpm >= 0.0F) {
					plc->send_setting(AO_gland_pump_setting(editor->id), percentage * 100.0F);
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
		Turtle<GP>* pTurtle = new Turtle<GP>(gwidth, gheight, false, GP::Hatch);

		pTurtle->move_right(3);
		pTurtle->move_down(1,GP::ToPSFP)
			->move_down(3.5F, GP::ToSBFP)
			->move_down(3.5F, GP::ToPSHP1)
			->move_down(3.5F, GP::ToPSHP2)
			->move_down(3.5F, GP::ToPSHP3)
			->move_down(3.5F, GP::ToSBHP1)
			->move_down(3.5F, GP::ToSBHP2)
			->move_down(3.5F, GP::ToSBHP3)

			->jump_down(2.5F)->jump_left(1,GP::Sea)->move_right(1)->move_down(1, GP::ToPSUWP1)
			->move_down(3.5F, GP::ToPSUWP2)
			->move_down(3.5F, GP::ToSBUWP1)
			->move_down(3.5F, GP::ToSBUWP2);

		pTurtle->jump_back(GP::ToPSFP)->move_right(5)->move_right(6,GP::PSFP)->move_right(10)->move_down(1.75F,GP::pssbfptoflush);
		pTurtle->jump_back(GP::ToSBFP)->move_right(5)->move_right(6,GP::SBFP)->move_right(10)->move_up(1.75F);
		/*
		pTurtle->jump_back(GP::ToPSHP1)->move_right(5)->move_right(6, GP::PSHP1)->move_right(6)->move_right(11)->move_right(GP::pshp1)->move_right(15, GP::pshp1p)->move_right(8)->turn_right_down() ->move_down(2.5F,GP::pshp1end);
		pTurtle->jump_back(GP::ToPSHP2)->move_right(5)->move_right(6, GP::PSHP2)->move_right(6)->move_right(10, GP::psbranch)->turn_right_up(GP::pshp2u)->move_up(3)->jump_back(GP::psbranch)->turn_right_down(GP::pshp2d)->move_down(3)->jump_back(GP::psbranch)->jump_right(24.5F, GP::PSHP);
		pTurtle->jump_back(GP::ToPSHP3)->move_right(5)->move_right(6, GP::PSHP3)->move_right(6)->move_right(11)->move_right(GP::pshp3)->move_right(15)->move_right(8)->turn_right_up()->move_up(2.5F, GP::pshp3end);
		
		
		pTurtle->jump_back(GP::ToSBHP1)->move_right(5)->move_right(6, GP::SBHP1)->move_right(6)->move_right(11)->move_right(GP::sbhp1)->move_right(15, GP::sbhp1p)->move_right(8)->turn_right_down()->move_down(2.5F,GP::sbhp1end);
		pTurtle->jump_back(GP::ToSBHP2)->move_right(5)->move_right(6, GP::SBHP2)->move_right(6)->move_right(10, GP::sbbranch)->turn_right_up(GP::sbhp2u)->move_up(3)->jump_back(GP::sbbranch)->turn_right_down(GP::sbhp2d)->move_down(3)->jump_back(GP::sbbranch)->jump_right(24.5F, GP::SBHP);
		pTurtle->jump_back(GP::ToSBHP3)->move_right(5)->move_right(6, GP::SBHP3)->move_right(6)->move_right(11)->move_right(GP::sbhp3)->move_right(15)->move_right(8)->turn_right_up()->move_up(2.5F, GP::sbhp3end);
		*/
		pTurtle->jump_back(GP::ToPSHP1)->move_right(5)->move_right(6, GP::PSHP1)->move_right(6)->move_right(11)->move_right(GP::pshp1)->move_right(15, GP::pshp1p)->move_right(8.5F)->turn_right_down()->move_down(2.5F, GP::pshp1end);
		pTurtle->jump_back(GP::ToPSHP2)->move_right(5)->move_right(6, GP::PSHP2)->move_right(6)->move_right(10, GP::psbranch)->turn_right_up(GP::pshp2u)->move_up(3)->jump_back(GP::psbranch)->turn_right_down(GP::pshp2d)->move_down(3)->jump_back(GP::psbranch)->jump_right(25, GP::PSHP);
		pTurtle->jump_back(GP::ToPSHP3)->move_right(5)->move_right(6, GP::PSHP3)->move_right(6)->move_right(11)->move_right(GP::pshp3)->move_right(15)->move_right(7)->turn_right_up()->move_up(2.5F)->turn_up_right(GP::pshp3end);

		pTurtle->jump_back(GP::ToSBHP1)->move_right(5)->move_right(6, GP::SBHP1)->move_right(6)->move_right(11)->move_right(GP::sbhp1)->move_right(15, GP::sbhp1p)->move_right(8.5F)->turn_right_down()->move_down(2.5F, GP::sbhp1end);
		pTurtle->jump_back(GP::ToSBHP2)->move_right(5)->move_right(6, GP::SBHP2)->move_right(6)->move_right(10, GP::sbbranch)->turn_right_up(GP::sbhp2u)->move_up(3)->jump_back(GP::sbbranch)->turn_right_down(GP::sbhp2d)->move_down(3)->jump_back(GP::sbbranch)->jump_right(25, GP::SBHP);
		pTurtle->jump_back(GP::ToSBHP3)->move_right(5)->move_right(6, GP::SBHP3)->move_right(6)->move_right(11)->move_right(GP::sbhp3)->move_right(15)->move_right(7)->turn_right_up()->move_up(2.5F)->turn_up_right( GP::sbhp3end);

		pTurtle->jump_back(GP::ToPSUWP1)->move_right(5)->move_right(6, GP::PSUWP1)->move_right(18, GP::psuwp2)->move_right(11)->move_right(12, GP::PSUWP);
		pTurtle->jump_back(GP::ToPSUWP2)->move_right(5)->move_right(6, GP::PSUWP2)->move_right(6)->move_right(10)->turn_right_up(GP::psuwp2to1)->move_up(3);// ->turn_up_right();
		pTurtle->jump_back(GP::ToSBUWP1)->move_right(5)->move_right(6, GP::SBUWP1)->move_right(18, GP::sbuwp2)->move_right(11)->move_right(12, GP::SBUWP);
		pTurtle->jump_back(GP::ToSBUWP2)->move_right(5)->move_right(6, GP::SBUWP2)->move_right(6)->move_right(10)->turn_right_up(GP::sbuwp2to1)->move_up(3);// ->turn_up_right();

		pTurtle->jump_back(GP::pssbfptoflush)->move_right(4, GP::ToFlushs)->move_right(4, GP::flushs);//至闸阀冲洗管路
		//pTurtle->jump_back(GP::pshp)->move_up_right(2, GP::SS1)->move_up_right(2)->move_right(4, GP::pshp1)->move_right(14)->move_down(4);
		//pTurtle->jump_back(GP::pshp)->move_down_right(2, GP::SS2)->move_down_right(2)->move_right(4, GP::pshp3)->move_right(14)->move_up(4);

		//pTurtle->jump_back(GP::sbhp)->move_up_right(2, GP::SS3)->move_up_right(2)->move_right(4, GP::sbhp1)->move_right(14)->move_down(4);
		//pTurtle->jump_back(GP::sbhp)->move_down_right(2, GP::SS4)->move_down_right(2)->move_right(4, GP::sbhp3)->move_right(14)->move_up(4);
		/*
		pTurtle->jump_back(GP::ToPSUWP1)->move_right(5)->move_right(6, GP::PSUWP1)->move_right(6)->move_right(10)->turn_right_down()->move_down()->turn_down_right();
		pTurtle->jump_back(GP::ToPSUWP2)->move_right(5)->move_right(6, GP::PSUWP2)->move_right(6)->move_right(14)->move_right(9)->move_right(12, GP::PSUWP);

		pTurtle->jump_back(GP::ToSBUWP1)->move_right(5)->move_right(6, GP::SBUWP1)->move_right(6)->move_right(14)->move_right(9)->move_right(12, GP::SBUWP);
		pTurtle->jump_back(GP::ToSBUWP2)->move_right(5)->move_right(6, GP::SBUWP2)->move_right(6)->move_right(10)->turn_right_up()->move_up()->turn_up_right();*/


		/*
		pTurtle->move_right(3);
		pTurtle->move_down(1, GP::d12)->move_right(5, GP::DGV12)->move_right(6, GP::PSFP);
		pTurtle->move_right(10)->move_down(2)->jump_back();
		pTurtle->move_down(4, GP::d11)->move_right(5, GP::DGV11)->move_right(6, GP::SBFP);
		pTurtle->move_right(10)->move_up(2)->move_right(4, GP::ToFlushs)->move_right(4, GP::flushs)->jump_back(GP::d11);

		pTurtle->move_down(4, GP::d13)->move_right(5, GP::DGV13)->move_right(6, GP::PSHPa)->move_right(6, GP::DGV3);
		pTurtle->move_right(10)->turn_right_down(GP::pshp)->move_down(3)->turn_down_right()->jump_back(GP::d13);
		pTurtle->move_down(4, GP::d14)->move_right(5, GP::DGV14)->move_right(6, GP::PSHPb)->move_right(6, GP::DGV4);
		pTurtle->move_right(14, GP::d44)->move_right(9, GP::DGV44)->move_right(12, GP::PSHP)->jump_back();
		pTurtle->move_up_right(2.5F, GP::SS1)->move_up_right(2.5F)->move_right(4, GP::DGV8);
		pTurtle->move_right(12)->move_down(5)->jump_back(GP::d14);

		pTurtle->move_down(5, GP::d15)->move_right(5, GP::DGV15)->move_right(6, GP::SBHPa)->move_right(6, GP::DGV5);
		pTurtle->move_right(14, GP::d45)->move_right(9, GP::DGV45)->move_right(12, GP::SBHP)->jump_back();
		pTurtle->move_down_right(2.5F, GP::SS2)->move_down_right(2.5F)->move_right(4, GP::DGV7);
		pTurtle->move_right(12)->move_up(5)->jump_back(GP::d15);
		pTurtle->move_down(4, GP::d16)->move_right(5, GP::DGV16)->move_right(6, GP::SBHPb)->move_right(6, GP::DGV6);
		pTurtle->move_right(10)->turn_right_up(GP::sbhp)->move_up(3)->turn_up_right()->jump_back(GP::d16);

		pTurtle->jump_down(3)->jump_left(GP::Sea)->move_right();

		pTurtle->move_down(2, GP::d17)->move_right(5)->move_right(6, GP::PSUWP1)->move_right(6);
		pTurtle->move_right(10)->turn_right_down(GP::psuwp)->move_down(3)->turn_down_right()->jump_back(GP::d17);
		pTurtle->move_down(4, GP::d18)->move_right(5)->move_right(6, GP::PSUWP2)->move_right(6);
		pTurtle->move_right(14, GP::d46)->move_right(9)->move_right(12, GP::PSUWP)->jump_back(GP::d18);

		pTurtle->move_down(5, GP::d19)->move_right(5)->move_right(6, GP::SBUWP1)->move_right(6);
		pTurtle->move_right(14, GP::d47)->move_right(9)->move_right(12, GP::SBUWP)->jump_back(GP::d19);
		pTurtle->move_down(4, GP::d20)->move_right(5)->move_right(6, GP::SBUWP2)->move_right(6);
		pTurtle->move_right(10)->turn_right_up(GP::sbuwp)->move_up(3)->turn_up_right();
		*/
		this->station = this->master->insert_one(new Tracklet<GP>(pTurtle, default_pipe_thickness, default_pipe_color));
		this->to_flushs = this->master->insert_one(new ArrowHeadlet(gheight, 0.0, Colours::Silver));
		this->sea = this->master->insert_one(new VLinelet(gheight * 2.0F, default_pipe_thickness,
			water_color, make_dash_stroke(CanvasDashStyle::Dash)));
		
		{ // load devices
			float radius = resolve_gridsize(gwidth, gheight);
			float hpradius = std::fminf(gwidth, gheight);

			this->hatch = this->master->insert_one(new Hatchlet(hpradius * 2.5F));
			
			//this->load_devices(this->mvalves, this->labels, Colours::Silver, GP::DGV3, GP::DGV8, radius, 0.0);
			this->load_devices(this->pumps, this->labels, Colours::Salmon, GP::PSFP, GP::SBFP, radius, 0.0);
			this->load_devices(this->pumps, this->labels, Colours::Salmon, GP::PSHP1, GP::SBHP3, radius, 0.0);
			this->load_devices(this->pumps, this->labels, Colours::Salmon, GP::PSUWP1, GP::SBUWP2, radius, 0.0);
			
			this->load_device(this->hoppers, this->captions, GP::PSHP, hpradius, +2.0F);
			this->load_device(this->hoppers, this->captions, GP::SBHP, hpradius, +2.0F);
			this->load_device(this->hoppers, this->captions, GP::PSUWP, hpradius, +2.0F);
			this->load_device(this->hoppers, this->captions, GP::SBUWP, hpradius, -2.0F);
		}

		{ // load labels and dimensions
			//this->load_labels(this->captions, GP::ToFlushs, GP::SS4, Colours::Silver);
			this->load_labels(this->captions, GP::ToFlushs, GP::ToFlushs, Colours::Silver);
			this->load_labels(this->captions, GP::Hatch, GP::Sea, Colours::Salmon);
			this->load_labels(this->captions, GP::pshp1p, GP::sbhp1p, Colours::Salmon);

			//this->load_dimension(this->pressures, GP::PSFP, "bar", "P", 1);
			this->load_dimensions(this->pressures, GP::PSUWP2, GP::PSUWP2, "bar", "P", 1);
			this->load_dimensions(this->pressures, GP::SBUWP2, GP::SBUWP2, "bar", "P", 1);
			//this->load_dimensions(this->pressures, GP::PSHP, GP::SBHP, "bar", "P", 1);
			//this->load_dimensions(this->flows, GP::PSHP, GP::SBHP, "m3ph", "F", 1);
			this->load_dimensions(this->pressures, GP::pshp1, GP::sbhp1p, "bar", "P", 1);
			this->load_dimensions(this->flows, GP::pshp1, GP::sbhp3, "m3ph", "F", 1);
			this->load_dimensions(this->flows, GP::PSUWP, GP::SBUWP, "m3ph", "F", 1);

			this->load_settings(this->rpms, GP::PSHP1, GP::SBHP3, "rpm", "S");
		}
		{//报警灯
			//this->load_ControlMode(GP::PSHP1, GP::SBHP3, gwidth * 1.2F, this->controlmodelocal, this->controlmodelocallabel,true);
			//this->load_ControlMode(GP::PSHP1, GP::SBHP3, gwidth * 1.2F, this->controlmodemachine, this->controlmodemachinelabel,false);
			this->load_Alarm(GP::PSFP, GP::SBFP, gwidth * 0.8F, this->alarm, this->alarmlabel);
		}
	}

public:
	void reflow(float width, float height, float gwidth, float gheight) {
		float toff = default_pipe_thickness * 2.0F;
		
		this->master->move_to(this->station, width * 0.5F, height * 0.5F, GraphletAnchor::CC, -gwidth * 2.0F);
		this->station->map_credit_graphlet(this->captions[GP::Sea], GraphletAnchor::RC, -toff);
		this->station->map_graphlet_at_anchor(this->sea, GP::Sea, GraphletAnchor::LC, 0.0F, this->sea_oscillation);
		this->station->map_graphlet_at_anchor(this->hatch, GP::Hatch, GraphletAnchor::LC);
		this->master->move_to(this->captions[GP::Hatch], this->hatch, GraphletAnchor::CB, GraphletAnchor::CT);

		this->station->map_graphlet_at_anchor(this->to_flushs, GP::flushs, GraphletAnchor::CC);
		this->station->map_credit_graphlet(this->captions[GP::ToFlushs], GraphletAnchor::CB);

		//this->station->map_credit_graphlet(this->captions[GP::SS1], GraphletAnchor::LC);
		//this->station->map_credit_graphlet(this->captions[GP::SS2], GraphletAnchor::LC);
		//this->station->map_credit_graphlet(this->captions[GP::SS3], GraphletAnchor::LC);
		//this->station->map_credit_graphlet(this->captions[GP::SS4], GraphletAnchor::LC);

		{ // reflow devices and metrics
			float gridsize = resolve_gridsize(gwidth, gheight);
			float uwp_master_cy, uwp_spare_cy;
			
			this->reflow_valves(this->mvalves, this->labels, gridsize);
			this->station->fill_anchor_location(GP::PSUWP1, nullptr, &uwp_master_cy);
			this->station->fill_anchor_location(GP::PSUWP2, nullptr, &uwp_spare_cy);

			for (auto it = this->pumps.begin(); it != this->pumps.end(); it++) {
				this->station->map_credit_graphlet(it->second, GraphletAnchor::CC);
				this->master->move_to(this->labels[it->first], it->second, GraphletAnchor::CT, GraphletAnchor::CB);

				switch (it->first) {
				case GP::PSFP: {
					this->station->map_credit_graphlet(this->pressures[it->first], GraphletAnchor::LT, gwidth * 3.0F, gridsize);
					this->master->move_to(this->alarm[it->first], this->pumps[it->first], +2, 0, GraphletAnchor::RT, gwidth);//RT
				}; break;
				case GP::SBFP: {
					this->master->move_to(this->alarm[it->first], this->pumps[it->first], +2, 0, GraphletAnchor::RT, gwidth);//RT
				}; break;
				case GP::PSHP1: case GP::PSHP2:case GP::PSHP3: case GP::SBHP1: case GP::SBHP2:case GP::SBHP3: {
					this->station->map_credit_graphlet(this->rpms[it->first], GraphletAnchor::LB, gwidth * 9.0F, -toff);
					//this->master->move_to(this->controlmodelocal[it->first], this->pumps[it->first], -0.7F, -0.2F, GraphletAnchor::RT, gwidth);//RT
					//this->master->move_to(this->controlmodemachine[it->first], this->pumps[it->first], +0.7F, -0.2F, GraphletAnchor::LT, gwidth);
				}; break;
				case GP::PSUWP1: case GP::SBUWP1: { // the pressure is shared by the two gland pumps
					/*
					this->station->map_credit_graphlet(this->pressures[it->first], GraphletAnchor::RC,
						gwidth * -3.0F, (uwp_spare_cy - uwp_master_cy) * 0.5F);*/
				}; break;
				case GP::PSUWP2: case GP::SBUWP2: { // the pressure is shared by the two gland pumps
					/*
					this->station->map_credit_graphlet(this->pressures[it->first], GraphletAnchor::LC,
						gwidth * 6.0F, (uwp_master_cy - uwp_spare_cy) * 0.5F);*/
				}; break;
				}
			}

			for (auto it = this->hoppers.begin(); it != this->hoppers.end(); it++) {
				GP tanchor = GP::_;

				this->station->map_credit_graphlet(it->second, GraphletAnchor::LC);
				this->master->move_to(this->captions[it->first], it->second, GraphletAnchor::RC, GraphletAnchor::LC, toff);

				if (this->rpms.find(it->first) != this->rpms.end()) {
					this->master->move_to(this->rpms[it->first], it->second, GraphletAnchor::LC, GraphletAnchor::RB, -gwidth * 0.5F, -toff);
					this->master->move_to(this->powers[it->first], this->rpms[it->first], GraphletAnchor::LB, GraphletAnchor::LT, 0.0, toff * 2.0F);

				}
			}


			//this->master->move_to(this->controlmodelocal[GP::PSHP1], this->pumps[GP::PSHP1], 0.5F, -0.2F, GraphletAnchor::LT, gwidth);
			//this->master->move_to(this->controlmodemachine[GP::PSHP1], this->pumps[GP::PSHP1], -0.5F, -0.2F, GraphletAnchor::RT, gwidth);
			//for (auto it = this->hoppers.begin(); it != this->hoppers.end(); it++) {
			/*
			for (auto it = this->controlmodelocallabel.begin(); it != this->controlmodelocallabel.end(); it++) {
				this->master->move_to(it->second, this->controlmodelocal[it->first], GraphletAnchor::LC, GraphletAnchor::RC, -gheight * 0.5F);
			}

			for (auto it = this->controlmodemachinelabel.begin(); it != this->controlmodemachinelabel.end(); it++) {
				this->master->move_to(it->second, this->controlmodemachine[it->first], GraphletAnchor::RC, GraphletAnchor::LC, gheight * 0.5F);
			}*/ 

			for (auto it = this->alarmlabel.begin(); it != this->alarmlabel.end(); it++) {
				this->master->move_to(it->second, this->alarm[it->first], GraphletAnchor::RC, GraphletAnchor::LC);
			}
			//this->master->move_to(this->alarms[GP::PSHP2], this->pumps[GP::PSHP2], 1.0F, 0.25F, GraphletAnchor::LT, gwidth);
			
			this->station->map_graphlet_at_anchor(this->pressures[GP::pshp1], GP::pshp1, GraphletAnchor::LB, gwidth * 3.0F, -toff);
			this->station->map_graphlet_at_anchor(this->flows[GP::pshp1], GP::pshp1, GraphletAnchor::LT, gwidth * 3.0F, toff);
			this->station->map_graphlet_at_anchor(this->pressures[GP::pshp3], GP::pshp3, GraphletAnchor::LB, gwidth * 3.0F, -toff);
			this->station->map_graphlet_at_anchor(this->flows[GP::pshp3], GP::pshp3, GraphletAnchor::LT, gwidth * 3.0F, toff);
			this->station->map_graphlet_at_anchor(this->pressures[GP::pshp1p], GP::pshp1p, GraphletAnchor::LB, gwidth * 3.0F, -toff);
			//this->station->map_graphlet_at_anchor(this->pressures[GP::pshp3p], GP::pshp3p, GraphletAnchor::LT, gwidth * 3.0F, toff);

			this->station->map_graphlet_at_anchor(this->pressures[GP::sbhp1], GP::sbhp1, GraphletAnchor::LB, gwidth * 3.0F, -toff);
			this->station->map_graphlet_at_anchor(this->flows[GP::sbhp1], GP::sbhp1, GraphletAnchor::LT, gwidth * 3.0F, toff);
			this->station->map_graphlet_at_anchor(this->pressures[GP::sbhp3], GP::sbhp3, GraphletAnchor::LB, gwidth * 3.0F, -toff);
			this->station->map_graphlet_at_anchor(this->flows[GP::sbhp3], GP::sbhp3, GraphletAnchor::LT, gwidth * 3.0F, toff);
			this->station->map_graphlet_at_anchor(this->pressures[GP::sbhp1p], GP::sbhp1p, GraphletAnchor::LB, gwidth * 3.0F, -toff);
			//this->station->map_graphlet_at_anchor(this->pressures[GP::sbhp3p], GP::sbhp3p, GraphletAnchor::LT, gwidth * 3.0F, toff);

			this->station->map_graphlet_at_anchor(this->pressures[GP::PSUWP2], GP::psuwp2, GraphletAnchor::LT, gwidth * 3.0F, toff);
			this->master->move_to(this->flows[GP::PSUWP], this->pressures[GP::PSUWP2], GraphletAnchor::LB, GraphletAnchor::LT, 0,toff);//封水流量
			this->station->map_graphlet_at_anchor(this->pressures[GP::SBUWP2], GP::sbuwp2, GraphletAnchor::LT, gwidth * 3.0F, toff);
			this->master->move_to(this->flows[GP::SBUWP], this->pressures[GP::SBUWP2], GraphletAnchor::LB, GraphletAnchor::LT, 0, toff);//封水流量

			this->master->move_to(this->captions[GP::pshp1p], this->pressures[GP::pshp1p], GraphletAnchor::CT, GraphletAnchor::CB);//泥沙分离器文本
			this->master->move_to(this->captions[GP::sbhp1p], this->pressures[GP::sbhp1p], GraphletAnchor::CT, GraphletAnchor::CB);

		}
	}

public:
	void update(long long count, long long interval, long long uptime) {
		this->sea_oscillation *= -1.0F;
		this->master->move(this->sea, 0.0F, this->sea_oscillation);
		this->master->notify_graphlet_updated(this->sea);
	}

private:
	template<class G, typename E>
	void load_devices(std::map<E, G*>& gs, E id0, E idn, float radius, double degrees) {
		for (E id = id0; id <= idn; id++) {
			gs[id] = this->master->insert_one(new G(radius, degrees), id);
		}
	}

	template<class G, typename E>
	void load_devices(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, CanvasSolidColorBrush^ color
		, E id0, E idn, float radius, double degrees) {
		this->load_devices(gs, id0, idn, radius, degrees);
		this->load_labels(ls, id0, idn, color);
	}

	template<class G, typename E>
	void load_device(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, E id, float rx, float fy) {
		this->load_label(ls, id, Colours::Salmon);

		gs[id] = this->master->insert_one(new G(rx, flabs(rx) * fy), id);

		this->load_dimension(this->rpms, id, "rpm", "S", 0);
		this->load_dimension(this->powers, id, "kwatt", "P", 0);
	}

	template<typename E>
	void load_settings(std::map<E, Credit<Dimensionlet, E>*>& ds, E id0, E idn, Platform::String^ unit, Platform::String^ label) {
		for (E id = id0; id <= idn; id++) {
			ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(DimensionState::Input, this->setting_style, unit, label), id);
			ds[id]->set_maximum(double(gland_pump_rpm_range));
		}
	}

	template<typename E>
	void load_dimension(std::map<E, Credit<Dimensionlet, E>*>& ds, E id, Platform::String^ unit, Platform::String^label, int precision) {
		this->dimension_style.precision = precision;
		ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(this->dimension_style, unit, label), id);
	}

	template<typename E>
	void load_dimensions(std::map<E, Credit<Dimensionlet, E>*>& ds, E id0, E idn, Platform::String^ unit, Platform::String^label, int precision) {
		for (E id = id0; id <= idn; id++) {
			this->load_dimension(ds, id, unit, label, precision);
		}
	}

	template<typename E>
	void load_label(std::map<E, Credit<Labellet, E>*>& ls, E id, CanvasSolidColorBrush^ color) {
		ls[id] = this->master->insert_one(new Credit<Labellet, E>(_speak(id), this->label_font, color), id);
	}


	template<typename E>
	void load_label(std::map<E, Credit<Labellet, E>*>& ls, E id, CanvasSolidColorBrush^ color,bool isLeft) {
		Platform::String^ label = (_speak((isLeft ? "Left" : "Right")));

		ls[id] = this->master->insert_one(new Credit<Labellet, E>(label, this->label_font, color), id);
	}

	template<typename E>
	void load_labels(std::map<E, Credit<Labellet, E>*>& ls, E id0, E idn, CanvasSolidColorBrush^ color) {
		for (E id = id0; id <= idn; id++) {
			this->load_label(ls, id, color);
		}
	}
	template<class A, typename E>
	void load_ControlMode(E id0, E idn, float size, std::map<E, Credit<A, E>*>& cm, std::map<E, Credit<Labellet, E>*>& ls, bool isLeft) {
		for (E id = id0; id <= idn; id++) {
			this->load_label(ls, id, Colours::Silver, isLeft);
			cm[id] = this->master->insert_one(new Credit<A, E>(size), id);
		}
	}
	template<class A, typename E>
	void load_Alarm(E id0, E idn, float size, std::map<E, Credit<A, E>*>& al, std::map<E, Credit<Labellet, E>*>& ls) {
		for (E id = id0; id <= idn; id++) {
			//this->load_alarm_label(ls, id, Colours::Silver,true);
			ls[id] = this->master->insert_one(new Credit<Labellet, E>(_speak("Alarm"+id.ToString()), this->label_font, Colours::Silver), id);
			al[id] = this->master->insert_one(new Credit<A, E>(size), id);
		}
	}

private:
	template<class G, typename E>
	void reflow_valves(std::map<E, G*>& gs, std::map<E, Credit<Labellet, E>*>& ls, float gridsize) {
		float margin;

		for (auto it = gs.begin(); it != gs.end(); it++) {
			it->second->fill_margin(0.0F, 0.0F, nullptr, nullptr, &margin, nullptr);
			this->station->map_credit_graphlet(it->second, GraphletAnchor::CC);
			this->station->map_credit_graphlet(ls[it->first], GraphletAnchor::CT, 0.0F, gridsize - margin);
		}
	}

private:

	bool pump_open(GP vid) {
		return (this->pumps[vid]->get_state() == HydraulicPumpState::Running);
	}
	void try_flow_water(GP pid, GP start, GP end, CanvasSolidColorBrush^ color) {
		switch (this->pumps[pid]->get_state()) {
		case HydraulicPumpState::Running: {
			this->station->push_subtrack(start, end, color);
		}
		}
	}

	void try_flow_water(GP pid, GP* path, unsigned int count, CanvasSolidColorBrush^ color) {
		switch (this->pumps[pid]->get_state()) {
		case HydraulicPumpState::Running: {
			this->station->push_subtrack(path, count, color);
		}
		}
	}

	template<unsigned int N>
	void try_flow_water(GP pid, GP (&path)[N], CanvasSolidColorBrush^ color) {
		this->try_flow_water(pid, path, N, color);
	}

	void try_flow_water(int count, ...) {
		va_list arguments;
		GP cur = GP::_;
		GP old = GP::_;
		va_start(arguments, count);
		for (int i = 0; i < count + 1; i++) {
			if (cur == GP::_) {
				cur = va_arg(arguments, GP);
			}
			else {
				if ((old != GP::_) && (cur != GP::_)) {
					this->station->push_subtrack(old, cur, water_color);
				}
				old = cur;
				cur = va_arg(arguments, GP);
			}
		}
		va_end(arguments);
	}

// never deletes these graphlets mannually
private:
	Tracklet<GP>* station;
	Hatchlet* hatch;
	ArrowHeadlet* to_flushs;
	Linelet* sea;
	std::map<GP, Credit<Labellet, GP>*> captions;
	std::map<GP, Credit<Labellet, GP>*> labels;
	std::map<GP, Credit<HydraulicPumplet, GP>*> pumps;
	std::map<GP, Credit<HopperPumplet, GP>*> hoppers;
	std::map<GP, Credit<ManualValvelet, GP>*> mvalves;
	std::map<GP, Credit<Dimensionlet, GP>*> pressures;
	std::map<GP, Credit<Dimensionlet, GP>*> flows;
	std::map<GP, Credit<Dimensionlet, GP>*> rpms;
	std::map<GP, Credit<Dimensionlet, GP>*> powers;
	std::map<GP, Credit<Alarmlet, GP>*> controlmodelocal;
	std::map<GP, Credit<Alarmlet, GP>*> controlmodemachine;
	std::map<GP, Credit<Labellet, GP>*> controlmodelocallabel;
	std::map<GP, Credit<Labellet, GP>*> controlmodemachinelabel;
	std::map<GP, Credit<Alarmlet, GP>*> alarm;
	std::map<GP, Credit<Labellet, GP>*> alarmlabel;

private:
	CanvasTextFormat^ label_font;
	DimensionStyle dimension_style;
	DimensionStyle setting_style;
	float sea_oscillation;

private:
	GlandsPage* master;
};

GlandsPage::GlandsPage(PLCMaster* plc) : Planet(__MODULE__), device(plc) {
	GlandPumps* dashboard = new GlandPumps(this);

	this->dashboard = dashboard;
	
	if (this->device != nullptr) {
		this->pump_op = make_gland_pump_menu(DO_glands_action, gland_pump_diagnostics, plc);
	
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

GlandsPage::~GlandsPage() {
	if (this->dashboard != nullptr) {
		delete this->dashboard;
	}

#ifndef _DEBUG
	delete this->grid;
#endif
}

void GlandsPage::load(CanvasCreateResourcesReason reason, float width, float height) {
	auto dashboard = dynamic_cast<GlandPumps*>(this->dashboard);
	
	if (dashboard != nullptr) {
		float gwidth = width / 64.0F;
		float gheight = height / 44.0F;

		this->grid->set_grid_width(gwidth);
		this->grid->set_grid_height(gheight);

		dashboard->load(width, height, gwidth, gheight);
	}
}

void GlandsPage::reflow(float width, float height) {
	auto dashboard = dynamic_cast<GlandPumps*>(this->dashboard);
	
	if (dashboard != nullptr) {
		float gwidth = this->grid->get_grid_width();
		float gheight = this->grid->get_grid_height();

		dashboard->reflow(width, height, gwidth, gheight);
	}
}

void GlandsPage::update(long long count, long long interval, long long uptime) {
	auto dashboard = dynamic_cast<GlandPumps*>(this->dashboard);

	if (dashboard != nullptr) {
		dashboard->update(count, interval, uptime);
	}
}

void GlandsPage::on_timestream(long long timepoint_ms, size_t addr0, size_t addrn, uint8* data, size_t size, uint64 p_type, size_t p_size, Syslog* logger) {
	auto dashboard = dynamic_cast<GlandPumps*>(this->dashboard);

	if (dashboard != nullptr) {
		dashboard->on_all_signals(timepoint_ms, addr0, addrn, data, size, logger);
	}
}

bool GlandsPage::can_select(IGraphlet* g) {
	return ((this->device != nullptr)
		&& (dynamic_cast<HydraulicPumplet*>(g) != nullptr));
}


bool GlandsPage::on_key(VirtualKey key, bool wargrey_keyboard) {
	bool handled = Planet::on_key(key, wargrey_keyboard);

	if ((!handled) && (this->device != nullptr) && (this->device->authorized())) {
		auto db = dynamic_cast<GlandPumps*>(this->dashboard);

		if (db != nullptr) {
			handled = db->on_key(key, this->device);
		}
	}

	return handled;
}

void GlandsPage::on_focus(IGraphlet* g, bool yes) {
	if (yes) {
		auto editor = dynamic_cast<IEditorlet*>(g);

		if (editor != nullptr) {
			if ((this->device != nullptr) && (this->device->authorized())) {
				this->show_virtual_keyboard(ScreenKeyboard::Numpad);
			} else {
				this->set_caret_owner(nullptr);
			}
		}
	}
}

void GlandsPage::on_tap_selected(IGraphlet* g, float local_x, float local_y) {
	auto pump = dynamic_cast<HydraulicPumplet*>(g);
	
	if (pump != nullptr) {
		menu_popup(this->pump_op, g, local_x, local_y);
	}
}
