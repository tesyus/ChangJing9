#include <map>

#include "page/lubrications.hpp"
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
#include "graphlet/symbol/pump/hydraulic_pumplet.hpp"

#include "iotables/ai_pumps.hpp"

#include "iotables/di_pumps.hpp"
#include "iotables/di_hopper_pumps.hpp"

#include "iotables/do_hopper_pumps.hpp"
#include "iotables/ai_hopper_pumps.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Foundation::Numerics;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::Brushes;

private enum class LUOperation { Start, Stop, _ };
private enum class LUGBOperation { Start, Stop, Auto, _ };

static CanvasSolidColorBrush^ region_background = Colours::make(diagnostics_region_background);
static CanvasSolidColorBrush^ alarm_background = Colours::make(diagnostics_alarm_background);

// WARNING: order matters
private enum class LU : unsigned int {
	// Key Labels
	//Gearbox, Unit, Master, Spare, //齿轮箱 润滑泵  主 备  文本

	// alarms
	/*
	Pressure, Level, Oil, Water,// 压力低 液位低 油温高 水温高
	Bearing1, Bearing2, Bearing3,//轴承1温度高 轴承2温度高 轴承3温度高
	GearboxOil, GearboxPressure,//油温低 压力低低
	*/




	/*
	MasterGearbox, SpareGearbox,//主 备齿轮箱
	Unit,//润滑泵
	Grease,//油脂泵,
	*/
	Gearbox, Unit,Grease, Master, Spare, //齿轮箱 润滑泵  主 备  文本
	//alarms
	GearboxOilLow,//油位低
	GearboxFlowLow,//流量低
	GearboxFilterPD75,//滤器压差75%
	GearboxFilterPD100,//滤器压差100%
	GearboxRunning,//齿轮箱运行
	GearboxPressures,//齿轮箱油压
	GearboxTemperatures,//齿轮箱油温
	Auto,

	UnitFlowLow,//流量低
	UnitFilterAlarm,//滤器报警
	UnitLevelLow,//液位低
	UnitTemperatures,//滑油温度 模拟量
	_
};

static CanvasSolidColorBrush^ color1 = Colours::Green;
private class Lubricatings final : public PLCConfirmation {
public:
	Lubricatings(LubricatingsPage* master, bool ps, unsigned int color) : master(master), ps(ps) {
		this->caption_font = make_bold_text_format("Microsoft YaHei", large_font_size);
		this->alarm_font = make_bold_text_format("Microsoft YaHei", normal_font_size);
		this->label_font = make_bold_text_format("Microsoft YaHei", small_font_size);

		this->color = Colours::make(color);

		this->flonum_style.number_font = make_bold_text_format("Cambria Math", large_font_size);
		this->flonum_style.unit_font = make_bold_text_format("Cambria", large_font_size);
		this->flonum_style.unit_color = Colours::Silver;
		this->flonum_style.precision = 1;

		this->flonum_alarm_style = flonum_style;
		this->flonum_alarm_style.number_color = Colours::Red;
	}

public:
	void pre_read_data(Syslog* logger) override {
		this->master->enter_critical_section();
		this->master->begin_update_sequence();
	}
	void on_analog_input(long long timepoint_ms, const uint8* DB2, size_t count2, const uint8* DB203, size_t count203, Syslog* logger) override {
		unsigned int gearboxtemperatures = (this->ps? ps_hopper_pump_gearbox_pump_temperatures: sb_hopper_pump_gearbox_pump_temperatures);
		unsigned int gearboxpressures = (this->ps? ps_hopper_pump_gearbox_pump_pressures : sb_hopper_pump_gearbox_pump_pressures);
		unsigned int unittemperatures = (this->ps? ps_hopper_pump_unit_pump_temperatures : sb_hopper_pump_unit_pump_temperatures);

		this->gearboxtemperatures[LU::GearboxTemperatures]->set_value(RealData(DB203, gearboxtemperatures));
		this->gearboxpressures[LU::GearboxPressures]->set_value(RealData(DB203, gearboxpressures));
		this->unittemperatures[LU::UnitTemperatures]->set_value(RealData(DB203, unittemperatures));

	}
	void on_digital_input(long long timepoint_ms, const uint8* DB4, size_t count4, const uint8* DB205, size_t count205, Syslog* logger) override {
		unsigned int master_unit_feedback = (this->ps ? ps_hopper_lubricating_unit_feedback : sb_hopper_lubricating_unit_feedback);//轴承润滑单元 状态  主
		unsigned int master_gearboxpump_feedback = (this->ps ? ps_hopper_gearboxpump_feedback : sb_hopper_gearboxpump_feedback);//油脂泵
		unsigned int master_gearboxpump_fault = (this->ps ? 463 : 479);//油脂泵
		unsigned int master_gearboxpump_details = (this->ps ? ps_hopper_gearboxpump_details : sb_hopper_gearboxpump_details);//油脂泵
		unsigned int spare_gearbox_feedback = (this->ps ? ps_hopper_gearbox_master_feedback : sb_hopper_gearbox_master_feedback);//备用齿轮箱状态
		unsigned int spare_gearbox_details = (this->ps ? ps_hopper_gearbox_details : sb_hopper_gearbox_details);//备用齿轮箱动作
		unsigned int unit_details = (this->ps ? ps_hopper_unit_details : sb_hopper_unit_details);//轴承润滑单元动作 长鲸9可能主备两个泵同一个运行信号
		unsigned int gearbox_alarms = (this->ps ? ps_hopper_gearbox_alarms : sb_hopper_gearbox_alarms);  //长鲸9报警开始位置
		unsigned int gearbox_auto = (this->ps ? 1216U : 1224U);  //齿轮箱 自动


		unsigned int unit_alarm = (this->ps ? ps_hopper_unit_alarms : sb_hopper_unit_alarms);  //齿轮箱 自动

		unsigned int gearboxtemperatures = (this->ps ? 2092U : 2094U)-1;  //齿轮箱 自动
		unsigned int gearboxpressures = (this->ps ? 2093U : 2095U)-1;  //齿轮箱 自动

		//unsigned int spare_feedback = (this->ps ? ps_hopper_gearbox_spare_feedback : sb_hopper_gearbox_spare_feedback);
		//unsigned int spare_details = (this->ps ? ps_hopper_gearbox_spare_details : sb_hopper_gearbox_spare_details);
		//unsigned int unit_alarms = (this->ps ? ps_hopper_lubricating_unit_alarms : sb_hopper_lubricating_unit_alarms);
		/*
		DI_hopper_pump_lubricating_unit(this->units[this->ps], DB4, unit_feedback, DB205, unit_feedback);
		DI_hopper_pump_gearbox(this->pumps[GearboxLubricator::Master], DB4, master_feedback, DB205, master_details);
		DI_hopper_pump_gearbox(this->pumps[GearboxLubricator::Spare], DB4, spare_feedback, DB205, spare_details);

		DI_hopper_lubricating_unit_alarm(this->alarms[LU::Pressure], DB4, unit_alarms + 0U);
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::Level],    DB4, unit_alarms + 1U);
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::Oil],      DB4, unit_alarms + 2U);
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::Water],    DB4, unit_alarms + 3U);
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::Bearing1], DB4, unit_alarms + 4U);
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::Bearing2], DB4, unit_alarms + 5U);
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::Bearing3], DB4, unit_alarms + 6U);

		DI_hopper_lubricating_unit_alarm(this->alarms[LU::GearboxOil],      DB4, gearbox_alarms + 0U);
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::GearboxPressure], DB4, gearbox_alarms + 1U);*/
		
		DI_hopper_pump_gearbox(this->gearboxpump[GearboxLubricator::Master], DB4, spare_gearbox_feedback, DB205, spare_gearbox_details);//
		DI_hopper_pump_lubricating_unit(this->units[UnitLubricator::Master], DB4, master_unit_feedback, DB205, unit_details,true);
		DI_hopper_pump_lubricating_unit(this->units[UnitLubricator::Spare], DB4, master_unit_feedback, DB205, unit_details,false);
		DI_hopper_pump_Greasepump(this->greasepump[OilLubricator::Master], DB4, master_gearboxpump_feedback, master_gearboxpump_fault, DB205, master_gearboxpump_details);//油脂泵

		DI_hopper_lubricating_unit_alarm(this->alarms[LU::GearboxOilLow], DB4, gearbox_alarms + 0U);
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::GearboxFlowLow], DB4, gearbox_alarms + 1U);
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::GearboxFilterPD75], DB4, gearbox_alarms + 2U);
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::GearboxFilterPD100], DB4, gearbox_alarms + 5U);
		DI_hopper_pump_control_mode(this->alarms[LU::GearboxRunning], DB4, gearbox_alarms + 4U);//绿	
		DI_hopper_pump_control_mode(this->alarms[LU::Auto], DB205, gearbox_auto);//绿

		DI_hopper_lubricating_unit_alarm(this->alarms[LU::UnitFlowLow], DB4, unit_alarm);//流量低
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::UnitFilterAlarm], DB4, unit_alarm+1);//滤器报警
		DI_hopper_lubricating_unit_alarm(this->alarms[LU::UnitLevelLow], DB4, unit_alarm+2);//液位低
		Alarm_Color(this->gearboxtemperatures[LU::GearboxTemperatures], DB205, gearboxtemperatures, flonum_alarm_style, flonum_style);
		Alarm_Color(this->gearboxpressures[LU::GearboxPressures], DB205, gearboxpressures ,flonum_alarm_style, flonum_style);

		//DI_hopper_lubricating_unit_alarm(this->alarms[LU::GearboxOil], DB4, gearbox_alarms + 0U);
		//DI_hopper_lubricating_unit_alarm(this->alarms[LU::GearboxPressure], DB4, gearbox_alarms + 1U);
	}

	void post_read_data(Syslog* logger) override {
		this->master->end_update_sequence();
		this->master->leave_critical_section();
	}

public:
	void load(float x, float width, float height, float vinset) {
		Turtle<UnitLubricator>* turtle = new Turtle<UnitLubricator>(vinset, vinset);
		float region_width = width * 0.618F;
		float region_height = (height - vinset * 6.0F /* 3x for the titlebar of the main window */) * 0.5F;
		float corner_radius = 8.0F;//圆角
		float pump_radius = vinset * 1.618F;
		
		turtle->move_down()->move_left(3);
		turtle->move_down(3, UnitLubricator::Master)->move_down(3);
		turtle->move_right(3)->move_down()->jump_up()->move_right(3);
		turtle->move_up(3, UnitLubricator::Spare)->move_up(3)->move_left(3);

		this->unit = this->master->insert_one(new RoundedRectanglet(region_width, region_height, corner_radius, region_background));//上方框
		this->gearbox = this->master->insert_one(new RoundedRectanglet(region_width, region_height, corner_radius, region_background));//下方框
		this->station = this->master->insert_one(new Tracklet<UnitLubricator>(turtle, default_pipe_thickness, default_pipe_color));//管路
		this->OilPump = this->master->insert_one(new RoundedRectanglet(region_width / 4, region_width / 4, corner_radius, region_background));//油脂泵

		this->load_label(this->captions, LU::Unit, this->color, this->caption_font, true);//上方文本
		this->load_label(this->captions, LU::Gearbox, this->color, this->caption_font, true);//下方文本
		this->load_label(this->captions, LU::Grease, this->color, this->caption_font, true);//下方文本
		//this->load_devices(this->gearboxpump, GearboxLubricator::Master, pump_radius);//加载上方泵
		//this->load_devices(this->greasepump, OilLubricator::Master, pump_radius);//加载上方泵
		this->load_devices(this->gearboxpump,  GearboxLubricator::Master, GearboxLubricator::Master, pump_radius);//加载上方泵
		this->load_devices(this->greasepump, OilLubricator::Master, OilLubricator::Master, pump_radius);//加载上方泵
		this->load_devices(this->units, this->unitlabels, UnitLubricator::Master, UnitLubricator::Spare, pump_radius);//加载下方泵


		{ // load alarms
			float alarm_box_width = (region_width - vinset * 3.0F) * 0.5F;
			float alarm_box_height = vinset * 2.0F;
			float alarm_size = alarm_box_height * 0.618F;
		
			for (LU id = LU::GearboxOilLow; id <= LU::UnitTemperatures; id++) {
				this->boxes[id] = this->master->insert_one(new Credit<RoundedRectanglet, LU>(alarm_box_width, alarm_box_height, corner_radius, alarm_background), id);
				switch (id) {
				case LU::GearboxTemperatures:
					this->load_dimension(this->gearboxtemperatures, LU::GearboxTemperatures, "℃");
					break;
				case LU::GearboxPressures:
					this->load_dimension(this->gearboxpressures, LU::GearboxPressures, "bar");
					break;
				case LU::UnitTemperatures:
					this->load_dimension(this->unittemperatures, LU::UnitTemperatures, "℃");
					break;
				default:
					this->alarms[id] = this->master->insert_one(new Credit<Alarmlet, LU>(alarm_size), id);
					this->load_label(this->captions, id, Colours::Silver, this->alarm_font);
					break;
				}
				
			}
		}
	}

public:
	void reflow(float x, float width, float height, float vinset) {
		float cx = x + width * 0.5F;
		float gapsize = vinset * 0.5F;

		//float offset = cx * 0.2F;

		this->master->move_to(this->unit, cx, vinset, GraphletAnchor::CT);//上方框坐标
		this->master->move_to(this->gearbox, cx, height - vinset, GraphletAnchor::CB);//下方框坐标
		if (x > 0) {
			this->master->move_to(this->OilPump, width + vinset / 2, vinset, GraphletAnchor::LT);//SB油脂泵框LT
		}
		else {
			this->master->move_to(this->OilPump, width - vinset / 2, vinset, GraphletAnchor::RT);//PS油脂泵框RT
		}
		this->master->move_to(this->gearboxpump[GearboxLubricator::Master], this->unit, 0.5F, 0.24F, GraphletAnchor::CC);
		this->master->move_to(this->greasepump[OilLubricator::Master], this->unit, 0.75F, 0.24F, GraphletAnchor::CC);
		this->master->move_to(this->greasepump[OilLubricator::Master], this->OilPump, GraphletAnchor::CC, GraphletAnchor::CC);//油脂泵
		this->master->move_to(this->station, this->gearbox, 0.5F, 0.46F, GraphletAnchor::CC);

		this->master->move_to(this->captions[LU::Gearbox], this->gearboxpump[GearboxLubricator::Master], GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, -gapsize);//上方
		this->master->move_to(this->captions[LU::Grease], this->greasepump[OilLubricator::Master], GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, -gapsize);//上方
		this->master->move_to(this->captions[LU::Unit], this->station, GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, -gapsize);//下方

		this->station->map_credit_graphlet(this->units[UnitLubricator::Master], GraphletAnchor::CC);
		this->station->map_credit_graphlet(this->units[UnitLubricator::Spare], GraphletAnchor::CC);

		this->master->move_to(this->unitlabels[UnitLubricator::Master], this->units[UnitLubricator::Master],
			GraphletAnchor::CT, GraphletAnchor::RB, -gapsize);
		
		this->master->move_to(this->unitlabels[UnitLubricator::Spare], this->units[UnitLubricator::Spare],
			GraphletAnchor::CT, GraphletAnchor::LB, +gapsize);

		{ // reflow alarm boxes
			float region_bottom, object_bottom;
			float alarm_cy = 0.0F;

			this->master->fill_graphlet_location(this->unit, nullptr, &region_bottom, GraphletAnchor::CB);
			this->master->fill_graphlet_location(this->unit, nullptr, &region_bottom, GraphletAnchor::CB);
			//this->master->fill_graphlet_location(this->gearboxpump[this->ps], nullptr, &object_bottom, GraphletAnchor::CB);
			//this->master->fill_graphlet_location(this->greasepump[this->ps], nullptr, &object_bottom, GraphletAnchor::CB);
			this->master->fill_graphlet_location(this->gearbox, nullptr, &object_bottom, GraphletAnchor::CB);
			this->master->fill_graphlet_location(this->OilPump, nullptr, &object_bottom, GraphletAnchor::CB);
			
			//上方
			alarm_cy = object_bottom + (region_bottom - object_bottom) * 0.5F;
			this->master->move_to(this->boxes[LU::GearboxFlowLow], cx, alarm_cy, GraphletAnchor::RB, -gapsize, -gapsize * 0.5F);
			this->master->move_to(this->boxes[LU::GearboxOilLow], this->boxes[LU::GearboxFlowLow], GraphletAnchor::LT, GraphletAnchor::LB, 0.0F, -gapsize);
			this->master->move_to(this->boxes[LU::GearboxFilterPD75], cx, alarm_cy, GraphletAnchor::RT, -gapsize, +gapsize * 0.5F);
			this->master->move_to(this->boxes[LU::GearboxFilterPD100], this->boxes[LU::GearboxFilterPD75], GraphletAnchor::LB, GraphletAnchor::LT, 0.0F, +gapsize);

			this->master->move_to(this->boxes[LU::GearboxRunning], this->boxes[LU::GearboxOilLow], GraphletAnchor::RC, GraphletAnchor::LC, vinset);
			this->master->move_to(this->boxes[LU::GearboxTemperatures], this->boxes[LU::GearboxFlowLow], GraphletAnchor::RC, GraphletAnchor::LC, vinset);
			this->master->move_to(this->boxes[LU::GearboxPressures], this->boxes[LU::GearboxFilterPD75], GraphletAnchor::RC, GraphletAnchor::LC, vinset);
			this->master->move_to(this->boxes[LU::Auto], this->boxes[LU::GearboxFilterPD100], GraphletAnchor::RC, GraphletAnchor::LC, vinset);

			this->master->fill_graphlet_location(this->gearbox, nullptr, &region_bottom, GraphletAnchor::CB);
			this->master->fill_graphlet_location(this->station, nullptr, &object_bottom, GraphletAnchor::CB);
			//this->master->fill_graphlet_location(this->OilPump, nullptr, &region_bottom, GraphletAnchor::CB);

			//下方
			alarm_cy = object_bottom + (region_bottom - object_bottom) * 0.5F;
			this->master->move_to(this->boxes[LU::UnitFlowLow], cx, alarm_cy, GraphletAnchor::RT, -gapsize);//左下
			this->master->move_to(this->boxes[LU::UnitFilterAlarm], cx, alarm_cy, GraphletAnchor::LT, gapsize);//右下

			this->master->move_to(this->boxes[LU::UnitLevelLow], this->boxes[LU::UnitFlowLow], GraphletAnchor::LT, GraphletAnchor::LB, 0.0F, -gapsize);//左上
			this->master->move_to(this->boxes[LU::UnitTemperatures], this->boxes[LU::UnitFilterAlarm], GraphletAnchor::LT, GraphletAnchor::LB, 0.0F, -gapsize);//左上
			//this->master->move_to(this->boxes[LU::UnitLevelLow], cx, alarm_cy, GraphletAnchor::LB, gapsize);//右上
		}

		for (LU id = LU::GearboxOilLow; id <= LU::UnitTemperatures; id++) {
			//if (id == LU::UnitTem) {
			switch (id) {
			case LU::GearboxTemperatures:
				this->master->move_to(this->gearboxtemperatures[LU::GearboxTemperatures], this->boxes[id], GraphletAnchor::LC, GraphletAnchor::LC , vinset);
				break;
			case LU::GearboxPressures	:
				this->master->move_to(this->gearboxpressures[LU::GearboxPressures], this->boxes[id], GraphletAnchor::LC, GraphletAnchor::LC, vinset);
				break;
			case LU::UnitTemperatures:
				this->master->move_to(this->unittemperatures[LU::UnitTemperatures], this->boxes[id], GraphletAnchor::LC, GraphletAnchor::LC, vinset);
				break;
			default:
				this->master->move_to(this->alarms[id], this->boxes[id], GraphletAnchor::LC, GraphletAnchor::LC, vinset);
				this->master->move_to(this->captions[id], this->alarms[id], GraphletAnchor::RC, GraphletAnchor::LC, vinset);
				break;
			}
			/*
			if (0) {
				//AI
			}
			else {
				this->master->move_to(this->alarms[id], this->boxes[id], GraphletAnchor::LC, GraphletAnchor::LC, vinset);
				this->master->move_to(this->captions[id], this->alarms[id], GraphletAnchor::RC, GraphletAnchor::LC, vinset);
			}*/
		}
	}

private:
	template<class P, typename E>
	void load_device(std::map<E, P*>& ps, E id, float radius) {
		ps[id] = this->master->insert_one(new P(radius, -90.0), id);
	}

	template<class P, typename E>
	void load_devices(std::map<E, P*>& ps, std::map<E, Credit<Labellet, E>*>& ls, E id0, E idn, float radius) {
		for (E id = id0; id <= idn; id++) {
			ps[id] = this->master->insert_one(new P(radius, -90.0), this->ps, id);
			this->load_label(ls, id, Colours::Silver, this->label_font);
		}
	}
	template<class P, typename E>
	void load_devices(std::map<E, P*>& ps,  E id0, E idn, float radius) {
		for (E id = id0; id <= idn; id++) {
			ps[id] = this->master->insert_one(new P(radius, -90.0), this->ps, id);
		}
	}

	template<typename E>
	void load_dimension(std::map<E, Credit<Dimensionlet, E>*>& ds, E id, Platform::String^ unit, bool label = true) {
		Platform::String^ caption = (label ? _speak(id) : nullptr);
		ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(this->flonum_style, unit, caption), id);
	}

	template<typename E>
	void load_label(std::map<E, Credit<Labellet, E>*>& ls, E id, CanvasSolidColorBrush^ color, CanvasTextFormat^ font = nullptr, bool prefix = false) {
		Platform::String^ label = (prefix ? _speak((this->ps ? "PS" : "SB") + id.ToString()) : _speak(id));

		ls[id] = this->master->insert_one(new Credit<Labellet, E>(label, font, color), id);
	}

// never deletes these graphlets mannually
private:
	Tracklet<UnitLubricator>* station;
	std::map<LU, Credit<Labellet, LU>*> captions;
	std::map<GearboxLubricator, Credit<Labellet, GearboxLubricator>*> gearlabels;
	std::map<UnitLubricator, Credit<Labellet, UnitLubricator>*> unitlabels;
	std::map<OilLubricator, Credit<Labellet, OilLubricator>*> oillabels;
	std::map<LU, Credit<Alarmlet, LU>*> alarms;
	std::map<LU, Credit<RoundedRectanglet, LU>*> boxes;
	RoundedRectanglet* unit;//润滑
	RoundedRectanglet* gearbox;//齿轮箱
	RoundedRectanglet* OilPump;//油脂泵

	/*
	std::map<bool, Credit<HydraulicPumplet, bool>*> gearboxpump;//齿轮箱
	std::map<bool, Credit<HydraulicPumplet, bool>*> greasepump;//油脂泵*/
	std::map<GearboxLubricator, GroupCredit<HydraulicPumplet, bool, GearboxLubricator>*> gearboxpump;//齿轮箱
	std::map<UnitLubricator, GroupCredit<HydraulicPumplet, bool, UnitLubricator>*> units;//润滑泵
	std::map<OilLubricator, GroupCredit<HydraulicPumplet, bool, OilLubricator>*> greasepump;//油脂泵

	std::map<LU, Credit<Dimensionlet, LU>*> gearboxtemperatures;//齿轮箱油温
	std::map<LU, Credit<Dimensionlet, LU>*> gearboxpressures;//齿轮箱油压
	std::map<LU, Credit<Dimensionlet, LU>*> unittemperatures;//滑油温度
private:
	CanvasSolidColorBrush^ color;
	CanvasTextFormat^ caption_font;
	CanvasTextFormat^ label_font;
	CanvasTextFormat^ alarm_font;
	DimensionStyle flonum_style;
	DimensionStyle flonum_alarm_style;

private:
	LubricatingsPage* master;
	bool ps;
};

LubricatingsPage::LubricatingsPage(PLCMaster* plc) : Planet(__MODULE__), device(plc) {
	Lubricatings* ps_dashboard = new Lubricatings(this, true, default_ps_color);
	Lubricatings* sb_dashboard = new Lubricatings(this, false, default_sb_color);

	this->ps_dashboard = ps_dashboard;
	this->sb_dashboard = sb_dashboard;

	if (this->device != nullptr) {
		this->unit_op = make_lubrication_unit_menu(plc);//63.0
		this->gearbox_op = make_gearbox_lubricator_menu(plc);//64.0
		this->oil_op = make_oil_lubricator_menu(plc);//63.0

		this->device->push_confirmation_receiver(ps_dashboard);
		this->device->push_confirmation_receiver(sb_dashboard);
	}
}

LubricatingsPage::~LubricatingsPage() {
	if (this->ps_dashboard != nullptr) {
		delete this->ps_dashboard;
		delete this->sb_dashboard;
	}
}

void LubricatingsPage::load(CanvasCreateResourcesReason reason, float width, float height) {
	auto ps_dashboard = dynamic_cast<Lubricatings*>(this->ps_dashboard);
	auto sb_dashboard = dynamic_cast<Lubricatings*>(this->sb_dashboard);
	
	if ((ps_dashboard != nullptr) && (sb_dashboard)) {
		float vinset = statusbar_height();
		float half_width = width * 0.5F;

		ps_dashboard->load(0.0F, half_width, height, vinset);
		sb_dashboard->load(half_width, half_width, height, vinset);
	}
}

void LubricatingsPage::reflow(float width, float height) {
	auto ps_dashboard = dynamic_cast<Lubricatings*>(this->ps_dashboard);
	auto sb_dashboard = dynamic_cast<Lubricatings*>(this->sb_dashboard);

	if ((ps_dashboard != nullptr) && (sb_dashboard != nullptr)) {
		float vinset = statusbar_height();
		float half_width = width * 0.5F;
		
		ps_dashboard->reflow(0.0F,       half_width, height, vinset);
		sb_dashboard->reflow(half_width, half_width, height, vinset);
	}
}

void LubricatingsPage::on_timestream(long long timepoint_ms, size_t addr0, size_t addrn, uint8* data, size_t size, uint64 p_type, size_t p_size, Syslog* logger) {
	auto ps_dashboard = dynamic_cast<Lubricatings*>(this->ps_dashboard);
	auto sb_dashboard = dynamic_cast<Lubricatings*>(this->sb_dashboard);

	if ((ps_dashboard != nullptr) && (sb_dashboard != nullptr)) {
		ps_dashboard->on_all_signals(timepoint_ms, addr0, addrn, data, size, logger);
		sb_dashboard->on_all_signals(timepoint_ms, addr0, addrn, data, size, logger);
	}
}

bool LubricatingsPage::can_select(IGraphlet* g) {
	return (((this->device != nullptr) && (this->device->authorized())
		&& (dynamic_cast<HydraulicPumplet*>(g) != nullptr)));
}

void LubricatingsPage::on_tap_selected(IGraphlet* g, float local_x, float local_y) {
	/*
	auto unit = dynamic_cast<Credit<HydraulicPumplet, bool>*>(g);
	auto pump = dynamic_cast<GroupCredit<HydraulicPumplet, bool, UnitLubricator>*>(g);//润滑单元
	*/

	auto gearbox = dynamic_cast<GroupCredit<HydraulicPumplet, bool, GearboxLubricator>*>(g);//齿轮箱
	auto unit = dynamic_cast<GroupCredit<HydraulicPumplet, bool, UnitLubricator>*>(g);//润滑单元
	auto oil = dynamic_cast<GroupCredit<HydraulicPumplet, bool, OilLubricator>*>(g);//润滑单元
	
	if (gearbox != nullptr) {
		menu_popup(this->gearbox_op, g, local_x, local_y);
	} else if (unit != nullptr) {
		menu_popup(this->unit_op, g, local_x, local_y);
	}else if (oil != nullptr) {
		menu_popup(this->oil_op, g, local_x, local_y);
	}
}
