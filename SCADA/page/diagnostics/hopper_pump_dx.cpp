#include <map>

#include "page/diagnostics/hopper_pump_dx.hpp"
#include "configuration.hpp"

#include "module.hpp"
#include "brushes.hxx"

#include "datum/flonum.hpp"

#include "graphlet/shapelet.hpp"
#include "graphlet/ui/buttonlet.hpp"
#include "graphlet/dashboard/alarmlet.hpp"

#include "iotables/di_hopper_pumps.hpp"
#include "iotables/do_hopper_pumps.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::Brushes;

static CanvasSolidColorBrush^ region_background = Colours::make(diagnostics_region_background);
static CanvasSolidColorBrush^ diagnosis_background = Colours::make(diagnostics_alarm_background);

// WARNING: order matters
private enum class HP : unsigned int {
	// Groups
	HopperCondition, UnderWaterCondition, UnderWaterAlarmCondition,
	/*
	// Hopper Conditions
	HPReady,//舱内泵备妥反馈
	HPRemote,//舱内泵遥控反馈
	HPNoEmergence,//舱内泵急停反馈
	HPGlandPumpsRunning,//舱内泵封水泵运行
	HPGearBoxRunning,//舱内泵齿轮箱运行 //备用泵运行
	HPLubricatingUnitRunning,//舷舱内泵轴承润滑单元主泵运行反馈
	HPisHopperPump,//舱内泵变频器切换到舱内泵位置
	HPPipeReady,//舱内泵管系备妥
	HPStartReady,//舱内泵启动条件成立

	// Underwater Conditions
	UWPRemoteControl, UWPStartReady,
	UWPNotRunning, UWPNoEmergence, UWPNoMaintenance,
	UWPPipelineReady, UWPGlandPumpsRunning, UWPSpeedKnobMoved,
	*/
	HPRemote,//变频器遥控
	HPReady,//变频器备妥
	HPNoEmergence,//变频器无急停
	HPPipeReady,//管系备妥
	HPSuctionInPlace,//吸口到位（挖泥有效）
	HPLeaveThe0,//旋钮离开0位
	HPMode,//舱内泵模式
	HPTireInflatable,//汽胎充气
	HPLubricatingOilPump,//齿轮箱滑油泵运行
	HPLubricatingFlow,//轴承润滑单元运行且流量正常
	HPGlandPump,//封水泵运行

	UWPRemote,//变频器压控
	UWPReady,//变频器备妥
	UWPNoEmergence,//变频器无急停
	UWPPipeReady,//管系备妥
	UWPSuctionInPlace,//吸口到位
	UWPLeaveThe0,//旋钮离开0位
	UWPMode,//水下泵模式
	UWPTireInflatable,//汽胎充气
	UWPOilPump,//油脂泵运行
	UWPInterLocking,//启动连锁
	UWPGlandPump,//封水泵运行



	//水下泵变频器报警
	UWPAlarm,//综合报警
	UWPMotorWindinTemperatureAlarm,//电机绕组温度报警
	UWPMotorBearinTemperatureAlarm,//电机轴承温度报警
	UWPMotorWaterLeakingAlarm,//水下电机漏水报警
	UWPMotorDFAlarm,//水下电机压差报警
	UWPMotorLubricatingAlarm,//水下电机润滑系统报警
	UWPMotorControlgAlarm,//水下电机控制系统报警
	_
};

private class HopperPumpDx final : public PLCConfirmation {
public:
	HopperPumpDx(HopperPumpDiagnostics* master, bool ps, unsigned int color) : master(master), ps(ps) {
		this->region_font = make_bold_text_format("Microsoft YaHei", normal_font_size);
		this->diagnosis_font = make_bold_text_format("Microsoft YaHei", small_font_size);

		this->color = Colours::make(color);

		this->hp_start = HP::HPRemote;
		this->hp_end = HP::HPGlandPump;
		this->uwp_start = HP::UWPRemote;
		this->uwp_end = HP::UWPGlandPump;
		this->uwp_alarm_start = HP::UWPAlarm;
		this->uwp_alarm_end = HP::UWPMotorControlgAlarm;
	}

public:
	void pre_read_data(Syslog* logger) override {
		this->master->enter_critical_section();
		this->master->begin_update_sequence();
	}

	void on_digital_input(long long timepoint_ms, const uint8* DB4, size_t count4, const uint8* DB205, size_t count205, Syslog* logger) override {
		unsigned int feedback = (this->ps ? ps_hopper_pump_feedback : sb_hopper_pump_feedback);
		//!!!!unsigned int lunit = (this->ps ? ps_hopper_lubricating_unit_feedback : sb_hopper_lubricating_unit_feedback);
		unsigned int hpg1 = (this->ps ? ps_hopper_1_gland_pump_feedback : sb_hopper_1_gland_pump_feedback)+2;
		unsigned int hpg2 = (this->ps ? ps_hopper_2_gland_pump_feedback : sb_hopper_2_gland_pump_feedback)+2;
		unsigned int hpg3 = (this->ps ? ps_hopper_3_gland_pump_feedback : sb_hopper_3_gland_pump_feedback)+2;
		unsigned int uwpgmaster = (this->ps ? ps_underwater_master_gland_pump_feedback : sb_underwater_master_gland_pump_feedback);
		unsigned int uwpgspare = (this->ps ? ps_underwater_spare_gland_pump_feedback : sb_underwater_spare_gland_pump_feedback);
		unsigned int gbmaster = (this->ps ? ps_hopper_gearbox_master_feedback : sb_hopper_gearbox_master_feedback);
		unsigned int gearbox = (this->ps ? 391U : 399U)-1U;
		unsigned int sparegearbox = (this->ps ? 385U : 393U)-1U;
		unsigned int masterlubricatingunit = (this->ps ? 427U : 435U) - 1U;
		unsigned int sparelubricatingunit = (this->ps ? 428U : 436U) - 1U;
		unsigned int ishopperpump = (this->ps ? 825 : 857) - 1;
		unsigned int hopperpumpdetails = (this->ps ? ps_hopper_pump_details : sb_hopper_pump_details);
		//!!!!unsigned int gbspare = (this->ps ? ps_hopper_gearbox_spare_feedback : sb_hopper_gearbox_spare_feedback);
		unsigned int hplready = (this->ps ? ps_hopper_pipeline_ready : sb_hopper_pipeline_ready) - 1U;
		unsigned int emergence = (this->ps ? ps_hopper_pump_emergence_feedback : sb_hopper_pump_emergence_feedback) - 1U;
		unsigned int knob = (this->ps ? ps_hopper_pump_speed_knob_moved : sb_hopper_pump_speed_knob_moved) - 1U;
		unsigned int ps_uwplready = ps_underwater_pipeline_ready - 1U;
		unsigned int sb_uw_dready = sb_underwater_dredging_ready - 1U;
		unsigned int d_uw_dready = double_underwater_dredging_ready - 1U;
		bool hp = DI_hopper_type(DB4, feedback);
		bool uwp = DI_underwater_type(DB4, feedback);
		//长鲸9水下泵报警
		/*
		unsigned int uwpalarm = (this->ps ? ps_uwpalarm : sb_uwpalarm)-1U;
		this->diagnoses[HP::HPReady]->set_state(DBX(DB4, feedback - 1), AlarmState::Notice, AlarmState::None);//舱内泵备妥反馈
		this->diagnoses[HP::HPRemote]->set_state(DBX(DB4, feedback), AlarmState::Notice, AlarmState::None);//舱内泵遥控反馈
		this->diagnoses[HP::HPNoEmergence]->set_state(!(DB4, feedback + 3), AlarmState::Notice, AlarmState::None);//舱内泵无急停反馈
		this->diagnoses[HP::HPGlandPumpsRunning]->set_state(DBX(DB4, hpg1)|| DBX(DB4, hpg2)|| DBX(DB4, hpg3), AlarmState::Notice, AlarmState::None);//舱内泵封水泵运行
		this->diagnoses[HP::HPGearBoxRunning]->set_state(DBX(DB4, gearbox)|| DBX(DB4, sparegearbox), AlarmState::Notice, AlarmState::None);//舱内泵齿轮箱运行 //备用泵运行
		this->diagnoses[HP::HPLubricatingUnitRunning]->set_state(DBX(DB4, masterlubricatingunit)|| DBX(DB4, sparelubricatingunit), AlarmState::Notice, AlarmState::None);//舷舱内泵轴承润滑单元主泵运行反馈
		this->diagnoses[HP::HPisHopperPump]->set_state(DBX(DB4, ishopperpump), AlarmState::Notice, AlarmState::None);//舱内泵变频器切换到舱内泵位置
		this->diagnoses[HP::HPPipeReady]->set_state(DBX(DB205, hplready), AlarmState::Notice, AlarmState::None);//舱内泵管系备妥	
		this->diagnoses[HP::HPStartReady]->set_state(DBX(DB205, hopperpumpdetails+3), AlarmState::Notice, AlarmState::None);//舱内泵启动条件成立


		this->diagnoses[HP::UWPStartReady]->set_state(DI_hopper_pump_ready(DB4, feedback, uwp), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPRemoteControl]->set_state(DI_hopper_pump_remote_control(DB4, feedback, uwp), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPNotRunning]->set_state(DI_hopper_pump_running(DB4, feedback, true), AlarmState::None, AlarmState::Notice);
		this->diagnoses[HP::UWPNoEmergence]->set_state(DBX(DB205, emergence), AlarmState::None, AlarmState::Notice);
		this->diagnoses[HP::UWPNoMaintenance]->set_state(DI_hopper_pump_repair(DB4, feedback, true), AlarmState::None, AlarmState::Notice);
		this->diagnoses[HP::UWPSpeedKnobMoved]->set_state(uwp && DBX(DB4, knob), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPGlandPumpsRunning]->set_state(DI_gland_pump_running(DB4, uwpgmaster, false), AlarmState::Notice, AlarmState::None);
		this->spare_diagnoses[HP::UWPGlandPumpsRunning]->set_state(DI_gland_pump_running(DB4, uwpgspare, false), AlarmState::Notice, AlarmState::None);

		if (this->ps) {
			this->diagnoses[HP::UWPPipelineReady]->set_state(DBX(DB205, ps_uwplready), AlarmState::Notice, AlarmState::None);
		} else {
			this->diagnoses[HP::UWPPipelineReady]->set_state(DBX(DB205, sb_uw_dready) || DBX(DB205, d_uw_dready), AlarmState::Notice, AlarmState::None);
		}
		//长鲸9水下泵报警
		this->diagnoses[HP::UWPInterLocking]->set_state(DBX(DB4, uwpalarm), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPAlarm]->set_state(DBX(DB4, uwpalarm + 1), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorWindinTemperatureAlarm]->set_state(DBX(DB4, uwpalarm + 2), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorBearinTemperatureAlarm]->set_state(DBX(DB4, uwpalarm + 3), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorWaterLeakingAlarm]->set_state(DBX(DB4, uwpalarm + 4), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorDFAlarm]->set_state(DBX(DB4, uwpalarm + 5), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorLubricatingAlarm]->set_state(DBX(DB4, uwpalarm + 6), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorControlgAlarm]->set_state(DBX(DB4, uwpalarm + 7), AlarmState::Alert, AlarmState::None);
		*/
		unsigned int hpgp1 = DBX(DB4, !this->ps ? 368U : 347U);
		unsigned int hpgp2 = DBX(DB4, !this->ps ? 374U : 355U);
		unsigned int hpgp3 = DBX(DB4, !this->ps ? 380U : 361U);
		this->diagnoses[HP::HPRemote]->set_state(DBX(DB4, !this->ps ? 833U : 801), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::HPReady]->set_state(DBX(DB4, !this->ps ? 832U : 800U), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::HPNoEmergence]->set_state(!DBX(DB4, !this->ps ? 836U : 804U), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::HPPipeReady]->set_state(DBX(DB205, !this->ps ? 819U : 818U), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::HPSuctionInPlace]->set_state(DBX(DB205, !this->ps ? 2546U : 2402U), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::HPLeaveThe0]->set_state(DBX(DB4, !this->ps ? 580U : 644U), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::HPMode]->set_state(DBX(DB4, !this->ps ? 856U : 824U), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::HPTireInflatable]->set_state(DBX(DB205, !this->ps ? 1912U : 1912U), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::HPLubricatingOilPump]->set_state(DBX(DB4, !this->ps ? 392U : 384U)|| DBX( DB4, !this->ps ? 398U : 390U), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::HPLubricatingFlow]->set_state((DBX(DB4, !this->ps ? 434U : 426U) || DBX(DB4, !this->ps ? 435U : 427U)) && !DBX(DB4, !this->ps ? 436U : 428U), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::HPGlandPump]->set_state((hpgp1&& hpgp2) || (hpgp2 && hpgp3) || (hpgp3 && hpgp1), AlarmState::Notice, AlarmState::None);

		this->diagnoses[HP::UWPRemote]->set_state(DBX(DB4, this->ps ? 809 : 841), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPReady]->set_state(DBX(DB4, this->ps ? 808 : 840), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPNoEmergence]->set_state(!DBX(DB4, this->ps ? 812 : 844), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPPipeReady]->set_state(DBX(DB205, this->ps ? 816 : 817), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPSuctionInPlace]->set_state(DBX(DB205, this->ps ? 2402 : 2546), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPLeaveThe0]->set_state(DBX(DB4, this->ps ? 644 : 580), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPMode]->set_state(DBX(DB4, this->ps ? 825 : 857), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPTireInflatable]->set_state(DBX(DB205, this->ps ? 1912 : 1920), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPOilPump]->set_state(DBX(DB4, this->ps ? 441 : 443), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPInterLocking]->set_state(DBX(DB4, this->ps ? 456 : 472), AlarmState::Notice, AlarmState::None);
		this->diagnoses[HP::UWPGlandPump]->set_state(DBX(DB4, this->ps ? 450 : 466)|| DBX(DB4, this->ps ? 454 : 470), AlarmState::Notice, AlarmState::None);

		unsigned int uwpalarm = (this->ps ? ps_uwpalarm : sb_uwpalarm) - 1U;
		this->diagnoses[HP::UWPAlarm]->set_state(DBX(DB4, uwpalarm + 1), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorWindinTemperatureAlarm]->set_state(DBX(DB4, uwpalarm + 2), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorBearinTemperatureAlarm]->set_state(DBX(DB4, uwpalarm + 3), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorWaterLeakingAlarm]->set_state(DBX(DB4, uwpalarm + 4), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorDFAlarm]->set_state(DBX(DB4, uwpalarm + 5), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorLubricatingAlarm]->set_state(DBX(DB4, uwpalarm + 6), AlarmState::Alert, AlarmState::None);
		this->diagnoses[HP::UWPMotorControlgAlarm]->set_state(DBX(DB4, uwpalarm + 7), AlarmState::Alert, AlarmState::None);
	}

	void post_read_data(Syslog* logger) override {
		this->master->end_update_sequence();
		this->master->leave_critical_section();
	}

public:
	void fill_extent(float title_height, float vgapsize, float* width, float* height) {
		unsigned int sc_count = _I(this->hp_end) - _I(this->hp_start) + 1U;
		unsigned int rc_count = _I(this->uwp_end) - _I(this->uwp_start) + 1U;
		unsigned int wc_count = _I(this->uwp_alarm_end) - _I(this->uwp_alarm_start) + 1U;//也许用不到?
		float region_reserved_height = vgapsize * 4.0F + this->region_font->FontSize;
		
		this->diagnosis_height = this->diagnosis_font->FontSize * 2.0F;
		this->hp_region_height = (this->diagnosis_height + vgapsize) * float(sc_count) + region_reserved_height;
		this->uwp_region_height = (this->diagnosis_height + vgapsize) * float(rc_count) + region_reserved_height;
		this->uwp_alarm_region_height = diagnosis_height*2+ hp_region_height+ uwp_region_height;//水下泵报警诊断高度

		SET_BOX(width, 800.0F);//长鲸9
		SET_BOX(height, this->hp_region_height + this->uwp_region_height + title_height * 5.0F);
	}

	void load(float x, float width, float height, float title_height, float vgapsize) {
		float region_width = width * 0.90F;
		float diagnosis_width = (region_width - title_height * 1.5F);
		float corner_radius = 8.0F;
		
		this->hp_region = this->master->insert_one(
			new RoundedRectanglet(region_width, this->hp_region_height, corner_radius, region_background));

		this->uwp_region = this->master->insert_one(
			new RoundedRectanglet(region_width, this->uwp_region_height, corner_radius, region_background));

		this->uwp_alarm_region = this->master->insert_one(
			new RoundedRectanglet(region_width, this->uwp_alarm_region_height, corner_radius, region_background));//长鲸9水下泵报警

		this->load_label(this->labels, HP::HopperCondition, this->color, this->region_font, true);
		this->load_label(this->labels, HP::UnderWaterCondition, this->color, this->region_font, true);
		this->load_label(this->labels, HP::UnderWaterAlarmCondition, this->color, this->region_font, true);

		{ // load buttons
			Platform::String^ label = PSHopperPumpChargeAction::Reset.ToString();

			for (HP id = HP::HopperCondition; id <= HP::UnderWaterCondition; id++) {
				this->resets[id] = this->master->insert_one(new GroupCredit<Buttonlet, bool, HP>(label), this->ps, id);
			}
		}

		{ // load diagnoses
			float icon_size = this->diagnosis_height * 0.618F;
		
			for (HP id = this->hp_start; id <= this->uwp_alarm_end; id++) {
				this->slots[id] = this->master->insert_one(new Credit<RoundedRectanglet, HP>(
					diagnosis_width, this->diagnosis_height, corner_radius, diagnosis_background), id);

				switch (id) {
				//case HP::HPGearboxPumpsRunning: case HP::HPGlandPumpsRunning:case HP::HPLubricatingUnitRunning:
				case HP::UWPGlandPump: {
					//this->spare_diagnoses[id] = this->master->insert_one(new Credit<Alarmlet, HP>(icon_size), id);
				}; break;
				}

				this->diagnoses[id] = this->master->insert_one(new Credit<Alarmlet, HP>(icon_size), id);
				this->load_label(this->labels, id, Colours::Silver, this->diagnosis_font);
			}
		}
	}

	void reflow(float x, float width, float height, float title_height, float vgapsize) {
		float btn_height;

		this->resets[HP::HopperCondition]->fill_extent(0.0F, 0.0F, nullptr, &btn_height);

		{ // reflow layout
			float gapsize = (height - title_height - this->hp_region_height - this->uwp_region_height - btn_height * 2.0F) / 6.0F;

			this->master->move_to(this->hp_region, x + width * 0.25F, title_height + gapsize, GraphletAnchor::CT);//外框
			this->master->move_to(this->resets[HP::HopperCondition], this->hp_region, GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, gapsize);
			this->master->move_to(this->uwp_region, this->resets[HP::HopperCondition], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, gapsize);
			this->master->move_to(this->resets[HP::UnderWaterCondition], this->uwp_region, GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, gapsize);
			this->master->move_to(this->uwp_alarm_region, x + width * 0.5F, title_height + gapsize, GraphletAnchor::LT);//长鲸9 水下泵报警诊断
			this->master->move_to(this->labels[HP::UnderWaterAlarmCondition], this->uwp_alarm_region, GraphletAnchor::CT, GraphletAnchor::CT, 0.0F, vgapsize);//长鲸9水下泵报警诊断

			this->master->move_to(this->labels[HP::HopperCondition], this->hp_region, GraphletAnchor::CT, GraphletAnchor::CT, 0.0F, vgapsize);
			this->master->move_to(this->labels[HP::UnderWaterCondition], this->uwp_region, GraphletAnchor::CT, GraphletAnchor::CT, 0.0F, vgapsize);
		}

		{ // reflow hopper pump condition boxes
			IGraphlet* target = this->labels[HP::HopperCondition];

			for (HP id = this->hp_start; id <= this->hp_end; id++) {
				this->master->move_to(this->slots[id], target, GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, vgapsize);
				target = this->slots[id];
			}
		}

		{ // reflow underwater pump condition boxes
			IGraphlet* target = this->labels[HP::UnderWaterCondition];

			for (HP id = this->uwp_start; id <= this->uwp_end; id++) {
				this->master->move_to(this->slots[id], target, GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, vgapsize);
				target = this->slots[id];
			}
		}

		{ // reflow underwater pump condition boxes
			IGraphlet* target = this->labels[HP::UnderWaterAlarmCondition];

			for (HP id = this->uwp_alarm_start; id <= this->uwp_alarm_end; id++) {
				this->master->move_to(this->slots[id], target, GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, vgapsize);
				target = this->slots[id];
			}
		}

		{ // reflow diagnostics
			float inset = vgapsize * 1.618F;
			float icon_width;

			this->diagnoses[this->hp_start]->fill_extent(0.0F, 0.0F, &icon_width, nullptr);

			for (HP id = this->hp_start; id <= this->uwp_alarm_end; id++) {//长鲸9
				if (this->spare_diagnoses.find(id) == this->spare_diagnoses.end()) {
					this->master->move_to(this->diagnoses[id], this->slots[id], GraphletAnchor::LC,
						GraphletAnchor::LC, (icon_width + vgapsize) * 1.0F + inset);
				} else {
					this->master->move_to(this->diagnoses[id], this->slots[id], GraphletAnchor::LC, GraphletAnchor::LC, inset);
					this->master->move_to(this->spare_diagnoses[id], this->slots[id], GraphletAnchor::LC,
						GraphletAnchor::LC, (icon_width + vgapsize) * 2.0F + inset);
				}

				this->master->move_to(this->labels[id], this->slots[id], GraphletAnchor::LC,
					GraphletAnchor::LC, (icon_width + vgapsize) * 3.0F + inset + vgapsize);
			}
		}
	}

public:
	bool available() override {
		return (this->master->surface_ready() && this->master->shown());
	}

private:
	template<typename E>
	void load_label(std::map<E, Credit<Labellet, E>*>& ls, E id, CanvasSolidColorBrush^ color
		, CanvasTextFormat^ font = nullptr, bool prefix = false) {
		Platform::String^ label = (prefix ? _speak((this->ps ? "PS" : "SB") + id.ToString()) : _speak(id));

		ls[id] = this->master->insert_one(new Credit<Labellet, E>(label, font, color), id);
	}

private: // never delete these graphlets mannually
	std::map<HP, Credit<Labellet, HP>*> labels;
	std::map<HP, Credit<Alarmlet, HP>*> diagnoses;
	std::map<HP, Credit<Alarmlet, HP>*> spare_diagnoses;
	std::map<HP, Credit<RoundedRectanglet, HP>*> slots;
	std::map<HP, GroupCredit<Buttonlet, bool, HP>*> resets;
	RoundedRectanglet* hp_region;
	RoundedRectanglet* uwp_region;
	RoundedRectanglet* uwp_alarm_region;
	
private:
	CanvasSolidColorBrush^ color;
	CanvasTextFormat^ region_font;
	CanvasTextFormat^ diagnosis_font;

private:
	float diagnosis_height;
	float hp_region_height;
	float uwp_region_height;
	float uwp_alarm_region_height;

private:
	HP hp_start;
	HP hp_end;
	HP uwp_start;
	HP uwp_end;
	HP uwp_alarm_start;
	HP uwp_alarm_end;

private:
	HopperPumpDiagnostics* master;
	bool ps;
};

HopperPumpDiagnostics::HopperPumpDiagnostics(PLCMaster* plc) : ISatellite(plc->get_logger(), __MODULE__), device(plc) {
	HopperPumpDx* ps_dashboard = new HopperPumpDx(this, true, default_ps_color);
	HopperPumpDx* sb_dashboard = new HopperPumpDx(this, false, default_sb_color);

	this->ps_dashboard = ps_dashboard;
	this->sb_dashboard = sb_dashboard;
	
	this->device->push_confirmation_receiver(ps_dashboard);
	this->device->push_confirmation_receiver(sb_dashboard);
}

HopperPumpDiagnostics::~HopperPumpDiagnostics() {
	if (this->ps_dashboard != nullptr) {
		delete this->ps_dashboard;
		delete this->sb_dashboard;
	}
}

void HopperPumpDiagnostics::fill_extent(float* width, float* height) {
	auto ps_dashboard = dynamic_cast<HopperPumpDx*>(this->ps_dashboard);
	auto sb_dashboard = dynamic_cast<HopperPumpDx*>(this->sb_dashboard);
	float ps_width = 400.0F;
	float ps_height = 600.0F;
	float sb_width = 400.0F;
	float sb_height = 600.0F;

	this->title_height = large_font_size * 2.0F;
	this->vgapsize = this->title_height * 0.16F;

	if (ps_dashboard != nullptr) {
		ps_dashboard->fill_extent(this->title_height, this->vgapsize, &ps_width, &ps_height);
	}

	if (sb_dashboard != nullptr) {
		sb_dashboard->fill_extent(this->title_height, this->vgapsize, &sb_width, &sb_height);
	}

	SET_BOX(width, ps_width + sb_width);
	SET_BOX(height, flmax(ps_height, sb_height));
}

void HopperPumpDiagnostics::load(CanvasCreateResourcesReason reason, float width, float height) {
	auto ps_dashboard = dynamic_cast<HopperPumpDx*>(this->ps_dashboard);
	auto sb_dashboard = dynamic_cast<HopperPumpDx*>(this->sb_dashboard);
	
	if ((ps_dashboard != nullptr) && (sb_dashboard)) {
		auto caption_font = make_bold_text_format("Microsoft YaHei", large_font_size);
		float half_width = width * 0.25F;//长鲸9

		ps_dashboard->load(0.0F, half_width, height, this->title_height, this->vgapsize);
		sb_dashboard->load(half_width, half_width, height, this->title_height, this->vgapsize);

		this->titlebar = this->insert_one(new Rectanglet(width, this->title_height, Colours::make(diagnostics_caption_background)));
		this->title = this->insert_one(new Labellet(this->display_name(), caption_font, diagnostics_caption_foreground));
	}
}

void HopperPumpDiagnostics::reflow(float width, float height) {
	auto ps_dashboard = dynamic_cast<HopperPumpDx*>(this->ps_dashboard);
	auto sb_dashboard = dynamic_cast<HopperPumpDx*>(this->sb_dashboard);

	if ((ps_dashboard != nullptr) && (sb_dashboard != nullptr)) {
		float half_width = width * 0.5F;
		
		ps_dashboard->reflow(0.0F,       half_width, height, this->title_height, this->vgapsize);
		sb_dashboard->reflow(half_width, half_width, height, this->title_height, this->vgapsize);

		this->move_to(this->title, this->titlebar, GraphletAnchor::CC, GraphletAnchor::CC);
	}
}

bool HopperPumpDiagnostics::can_select(IGraphlet* g) {
	return (dynamic_cast<Buttonlet*>(g) != nullptr);
}

void HopperPumpDiagnostics::on_tap_selected(IGraphlet* g, float local_x, float local_y) {
	auto reset = dynamic_cast<GroupCredit<Buttonlet, bool, HP>*>(g);

	if (reset != nullptr) {
		this->device->send_command(DO_hopper_pump_reset_command(reset->gid, (reset->id == HP::HopperCondition)));
	}
}
