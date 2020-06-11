#include <map>

#include "page/draughts.hpp"
#include "configuration.hpp"
#include "menu.hpp"

#include "schema/datalet/earthwork_ts.hpp"

#include "graphlet/dashboard/radarlet.hpp"
#include "graphlet/dashboard/cylinderlet.hpp"
#include "graphlet/device/overflowlet.hpp"
#include "graphlet/ui/buttonlet.hpp"
#include "graphlet/ui/statuslet.hpp"
#include "graphlet/ui/textlet.hpp"

#include "iotables/ai_metrics.hpp"
#include "iotables/ai_dredges.hpp"
#include "iotables/di_doors.hpp"

#include "iotables/ao_devices.hpp"
#include "iotables/do_devices.hpp"
#include "iotables/do_doors.hpp"

#include "decorator/ship.hpp"

#include "datum/path.hpp"

#include "module.hpp"
#include <iotables\ai_doors.hpp>

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

using namespace Windows::Foundation;
using namespace Windows::System;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::Brushes;

// WARNING: order matters
private enum class DL : unsigned int {
	SternDraft, psSternDraft, psSuctionDraft, psAmidshipDraft, sbSternDraft, psSternHeight, sbSternHeight,
	BowDraft, psBowDraft, sbSuctionDraft, sbAmidshipDraft, sbBowDraft, psBowHeight, sbBowHeight,

	LeftOverflow, RightOverflow,NetWeight, AverageDraft, 
	TrimDegrees, //纵倾
	HeelDegrees, //横倾
	ADC,//自动控制激活
	_
};

private enum class SM : unsigned int {
	//kn, vacuum, fvolume, fspeed, dpforce1, dpforce2, _
	kn, fvolume, fspeed, density, product_chance, payload_chance, _
};

/*************************************************************************************************/
private class Draughts final : public PLCConfirmation {
public:
	virtual ~Draughts() noexcept {
		this->datasource->destroy();
	}

	Draughts(DraughtsPage* master, ShipDecorator* ship, bool timemachine) : master(master), decorator(ship)
		, timemachine(timemachine), departure(0LL), destination(0LL) {
		Syslog* logger = make_system_logger(default_logging_level, "EarthWorkHistory");

		this->datasource = new EarthWorkDataSource(logger, RotationPeriod::Daily);
		this->datasource->reference();

		this->label_font = make_bold_text_format(large_font_size);
		this->plain_style = make_plain_dimension_style(small_metrics_font_size, 5U, 2U);
		this->flonum_style = make_plain_dimension_style(small_metrics_font_size, normal_font_size, 2U);
		this->fixnum_style = make_plain_dimension_style(small_metrics_font_size, normal_font_size, 0U);
		this->setting_style = make_highlight_dimension_style(small_metrics_font_size, 5U, 2, this->plain_style.label_color, Colours::Background);

		this->ps_address = make_ps_dredging_system_schema();
		this->sb_address = make_sb_dredging_system_schema();
	}

public:
	void pre_read_data(Syslog* logger) override {
		this->master->enter_critical_section();
		this->master->begin_update_sequence();
	}
	float cacuheight(const uint8* DB203, unsigned int pix) {
		return  RealData(DB203, pix);
	}

	void on_digital_input(long long timepoint_ms, const uint8* DB4, size_t count4, const uint8* DB205, size_t count205, Syslog* logger) override {
		DI_hopper_doors_checks_button(this->hdchecks[BottomDoorCommand::OpenDoorCheck], BottomDoorCommand::OpenDoorCheck, DB205);
		DI_hopper_doors_checks_button(this->hdchecks[BottomDoorCommand::CloseDoorCheck], BottomDoorCommand::CloseDoorCheck, DB205);
		this->buttons[DL::ADC]->set_state(DBX(DB205, 1298 - 1U), ButtonState::Executing, ButtonState::Default);
		
	}

	void on_analog_input(long long timepoint_ms, const uint8* DB2, size_t count2, const uint8* DB203, size_t count203, Syslog* logger) override {
		//this->overflowpipe[DL::LeftOverflow]->set_value(RealData(DB203, left_overflow_pipe_progress));//这里要处理一下
		//this->overflowpipe[DL::RightOverflow]->set_value(RealData(DB203, right_overflow_pipe_progress));//这里要处理一下
		this->overflowpipe[DL::LeftOverflow]->set_value(RealData(DB203, left_overflow_pipe_progress));//这里要处理一下
		this->overflowpipe[DL::RightOverflow]->set_value(RealData(DB203, right_overflow_pipe_progress));//这里要处理一下
		//this->overflowpipe[DL::LeftOverflow]->set_liquid_height(DBD(DB2, average_hopper_height));
		//this->overflowpipe[DL::RightOverflow]->set_liquid_height(DBD(DB2, average_hopper_height));
		this->overflowpipe[DL::LeftOverflow]->set_liquid_height(DBD(DB2, average_hopper_height));
		this->overflowpipe[DL::RightOverflow]->set_liquid_height(DBD(DB2, average_hopper_height));

		this->dimensions[DL::LeftOverflow]->set_value(this->overflowpipe[DL::LeftOverflow]->get_value(), GraphletAnchor::CC);
		this->dimensions[DL::RightOverflow]->set_value(this->overflowpipe[DL::RightOverflow]->get_value(), GraphletAnchor::CC);
		this->dimensions[DL::NetWeight]->set_value(DBD(DB2, vessal_netweight), GraphletAnchor::RC);
		this->dimensions[DL::AverageDraft]->set_value(DBD(DB2, average_draught), GraphletAnchor::LC);//平均吃水
		this->dimensions[DL::TrimDegrees]->set_value(DBD(DB2, trim_degrees), GraphletAnchor::LC);//纵倾
		this->dimensions[DL::HeelDegrees]->set_value(DBD(DB2, heel_degrees), GraphletAnchor::LC);//横倾

		this->dimensions[DL::psBowDraft]->set_value(DBD(DB2, ps_fixed_bow_draught));
		this->dimensions[DL::psSuctionDraft]->set_value(DBD(DB2, ps_suction_draught));
		this->dimensions[DL::psAmidshipDraft]->set_value(DBD(DB2, ps_fixed_center_draught));
		this->dimensions[DL::psSternDraft]->set_value(DBD(DB2, ps_fixed_stern_draught));

		this->dimensions[DL::sbBowDraft]->set_value(DBD(DB2, sb_fixed_bow_draught));
		this->dimensions[DL::sbSuctionDraft]->set_value(DBD(DB2, sb_suction_draught));
		this->dimensions[DL::sbAmidshipDraft]->set_value(DBD(DB2, sb_fixed_center_draught));
		this->dimensions[DL::sbSternDraft]->set_value(DBD(DB2, sb_fixed_stern_draught));

		this->dimensions[DL::BowDraft]->set_value(DBD(DB2, fixed_bow_draught));
		this->dimensions[DL::SternDraft]->set_value(DBD(DB2, fixed_stern_draught));
		//4个舱高
		//this->dimensions[DL::psBowHeight]->set_value(DBD(DB2, ps_bow_hopper_height));
		//this->dimensions[DL::sbBowHeight]->set_value(DBD(DB2, sb_bow_hopper_height));
		//this->dimensions[DL::psSternHeight]->set_value(DBD(DB2, ps_stern_hopper_height));
		//this->dimensions[DL::sbSternHeight]->set_value(DBD(DB2, sb_stern_hopper_height));
		this->dimensions[DL::psBowHeight]->set_value(cacuheight(DB203,ps_bow_hopper_height));
		this->dimensions[DL::sbBowHeight]->set_value(cacuheight(DB203, sb_bow_hopper_height));
		this->dimensions[DL::psSternHeight]->set_value(cacuheight(DB203, ps_stern_hopper_height));
		this->dimensions[DL::sbSternHeight]->set_value(cacuheight(DB203, sb_stern_hopper_height));

		{ // set timeseries and cylinders
			double values[_N(EWTS)];

			this->set_cylinder(EWTS::HopperHeight, values, DBD(DB2, average_hopper_height));
			this->set_cylinder(EWTS::Displacement, values, DBD(DB2, displacement_value));
			this->set_cylinder(EWTS::Payload, values, DBD(DB2, payload_value));//装舱量
			this->set_cylinder(EWTS::EarthWork, values, DBD(DB2, earthwork_value));//土方量
			this->set_cylinder(EWTS::Capacity, values, DBD(DB2, vessel_value));

			/*
			*/

			if (!this->timemachine) {
				this->timeseries->set_values(values, Door_Open_Flag, true,  timepoint_ms);
				Door_Open_Flag = false;
			}
			else  {
				this->timeseries->scroll_to_timepoint(timepoint_ms);
			}
		}

		{ // set radars
			double ps_metrics[_N(SM)];
			double sb_metrics[_N(SM)];
			//航速
			ps_metrics[_I(SM::kn)] = DBD(DB2, 584U) / dredging_speed_range;
			sb_metrics[_I(SM::kn)] = DBD(DB2, 584U) / dredging_speed_range;
			//流量
			this->set_flow_volume(ps_metrics, DB2, ps_flow, flow_volume_range);
			this->set_flow_volume(sb_metrics, DB2, sb_flow, flow_volume_range);
			//流速
			this->set_flow_speed(ps_metrics, DB203, this->ps_address->water_density_speed + 1, flow_speed_range);
			this->set_flow_speed(sb_metrics, DB203, this->sb_address->water_density_speed + 1, flow_speed_range);
			//密度
			this->set_density(ps_metrics, DB203, this->ps_address->water_density_speed, density_range);
			this->set_density(sb_metrics, DB203, this->sb_address->water_density_speed, density_range);
			//产量率
			this->set_product(ps_metrics, DB2, 264U, product_chance_range);
			this->set_product(sb_metrics, DB2, 268U, product_chance_range);
			//装舱率
			this->set_payload(ps_metrics, DB2, 636U, payload_chance_range);
			this->set_payload(sb_metrics, DB2, 640U, payload_chance_range);

			this->ps_radar->set_scales(ps_metrics);
			this->sb_radar->set_scales(sb_metrics);
		}

		//泥门打开状态
		{
			if (door_open(DB203) && this->Door_Open_Flag_Old == false) {
				this->Door_Open_Flag = true;
				this->Door_Open_Flag_Old = true;
			}
			else if(!door_open(DB203) && Door_Open_Flag == false) {
				this->Door_Open_Flag_Old = false;
			}
		}
	}

	void on_forat(long long timepoint_ms, const uint8* DB20, size_t count, Syslog* logger) override {
		float left_target_height = DBD(DB20, left_overflow_pipe_target_height);
		float right_target_height = DBD(DB20, right_overflow_pipe_target_height);

		this->overflowpipe[DL::LeftOverflow]->set_target_height(left_target_height, (left_target_height == 0.0F));
		this->overflowpipe[DL::RightOverflow]->set_target_height(right_target_height, (right_target_height == 0.0F));
	}

	void post_read_data(Syslog* logger) override {
		this->master->end_update_sequence();
		this->master->leave_critical_section();
	}

public:
	void on_timemachine_startover(long long departure_ms, long long destination_ms) {
		if (this->timeseries != nullptr) {
			this->timeseries->set_history_interval(departure_ms / 1000LL, destination_ms / 1000LL + 1LL);
			this->departure = 0LL;
			this->destination = 0LL;
		} else {
			this->departure = departure_ms;
			this->destination = destination_ms;
		}
	}

	void load(float width, float height, float vinset) {
		float ship_y, ship_height, cylinder_height, lines_width, overflow_height;
		
		this->decorator->fill_ship_extent(nullptr, &ship_y, &lines_width, &ship_height, true);
		
		overflow_height = ship_height * 0.382F;
		//this->overflowpipe[DL::LeftOverflow] = this->master->insert_one(new OverflowPipelet(hopper_height_range, overflow_height));
		this->load_overflow2(this->overflowpipe, DL::LeftOverflow, overflow_height * 0.7F);
		this->load_overflow2(this->overflowpipe, DL::RightOverflow, overflow_height * 0.7F);

		//this->load_overflow(this->overflowpipe, DL::LeftOverflow, overflow_height);
		//this->load_overflow(this->overflowpipe, DL::RightOverflow, overflow_height);
		this->ps_radar = this->master->insert_one(new Radarlet<SM>(__MODULE__, overflow_height * 0.5F, Colours::make(default_ps_color)));
		this->sb_radar = this->master->insert_one(new Radarlet<SM>(__MODULE__, overflow_height * 0.5F, Colours::make(default_sb_color)));

		cylinder_height = ship_height * 0.42F;
		this->load_cylinder(this->cylinders, EWTS::EarthWork, cylinder_height, earthwork_range, 0U, "meter3");
		this->load_cylinder(this->cylinders, EWTS::Capacity, cylinder_height, vessel_range, 0U, "meter3");
		this->load_cylinder(this->cylinders, EWTS::HopperHeight, cylinder_height, hopper_height_range, 2U, "meter");
		this->load_cylinder(this->cylinders, EWTS::Payload, cylinder_height, payload_range, 0U, "ton");
		this->load_cylinder(this->cylinders, EWTS::Displacement, cylinder_height, displacement_range, 0U, "ton");

		this->load_dimensions(this->dimensions, DL::SternDraft, DL::sbSternHeight, "meter");
		this->load_dimensions(this->dimensions, DL::BowDraft, DL::sbBowHeight, "meter");
		this->load_dimension(this->dimensions, DL::AverageDraft, "meter", this->flonum_style);
		this->load_dimension(this->dimensions, DL::NetWeight, "ton", this->fixnum_style);
		this->load_dimension(this->dimensions, DL::TrimDegrees, "degrees", this->flonum_style);
		this->load_dimension(this->dimensions, DL::HeelDegrees, "degrees", this->flonum_style);
		this->load_setting(this->dimensions, DL::LeftOverflow, "meter");
		this->load_setting(this->dimensions, DL::RightOverflow, "meter");

		this->load_buttons(this->hdchecks, BottomDoorCommand::OpenDoorCheck, BottomDoorCommand::CloseDoorCheck);
		this->buttons[DL::ADC] = this->master->insert_one(new Credit<Buttonlet, DL>(_speak(DL::ADC), 128, 32), DL::ADC);

		{ // load timeseries
			float lines_height = ship_y * 0.72F;
			TimeSeriesStyle style;
			TimeSeries ts = make_hour_series(8U, 8U);

			style.lookup_color = earthwork_line_color_dictionary;

			if (this->timemachine) {
				ts = make_hour_series(4U, 8U);
			}

			this->timeseries = this->master->insert_one(new TimeSerieslet<EWTS>(__MODULE__, this->datasource,
				timeseries_range, ts, lines_width, lines_height, 5U, 1U));

			if (this->timemachine && (this->departure != 0LL)) {
				this->on_timemachine_startover(this->departure, this->destination);
			}

			this->timeseries->set_style(style);
			this->timeseries->close_line(EWTS::EarthWork, 0.75);
			this->timeseries->hide_line(EWTS::Capacity, true);
			this->timeseries->hide_line(EWTS::HopperHeight, true);
		}
	}

	void reflow(float width, float height, float vinset) {
		float tsx, tsy, gapsize, lofpx, lofpy , rofpx,rofpy;

		this->dimensions[DL::LeftOverflow]->fill_extent(0.0F, 0.0F, nullptr, &gapsize);
		this->dimensions[DL::RightOverflow]->fill_extent(0.0F, 0.0F, nullptr, &gapsize);
		gapsize *= 0.5F;

		this->decorator->fill_ship_anchor(0.75F, 0.5F, &lofpx, &lofpy);
		this->decorator->fill_ship_anchor(0.95F, 0.5F, &rofpx, &rofpy);
		this->decorator->fill_ship_anchor(0.5F, 0.0F, &tsx, &tsy, true);
		tsy *= 0.5F;

		this->reflow_cylinders(this->cylinders, EWTS::EarthWork, EWTS::Displacement, gapsize);
		this->master->move_to(this->timeseries, tsx, tsy, GraphletAnchor::CC, 0.0F, -vinset);
		this->master->move_to(this->overflowpipe[DL::LeftOverflow], lofpx, lofpy, GraphletAnchor::CC, 0.0F, -gapsize);
		this->master->move_to(this->overflowpipe[DL::RightOverflow], rofpx, rofpy, GraphletAnchor::CC, 0.0F, -gapsize);
		this->master->move_to(this->dimensions[DL::LeftOverflow], this->overflowpipe[DL::LeftOverflow], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, gapsize);
		this->master->move_to(this->dimensions[DL::RightOverflow], this->overflowpipe[DL::RightOverflow], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, gapsize);
		this->master->move_to(this->ps_radar, this->overflowpipe[DL::RightOverflow], GraphletAnchor::RC, GraphletAnchor::LB, gapsize * 3.0F, -gapsize * 0.0F);
		this->master->move_to(this->sb_radar, this->overflowpipe[DL::RightOverflow], GraphletAnchor::RC, GraphletAnchor::LT, gapsize * 3.0F, +gapsize * 1.5F);
		
		{ // reflow dimensions
			float cpt_height, xoff, yoff;

			this->dimensions[DL::BowDraft]->fill_extent(0.0F, 0.0F, nullptr, &cpt_height);
			xoff = cpt_height * 0.50F;
			yoff = cpt_height * 0.20F;

			this->reflow_dimension(this->dimensions, DL::BowDraft, 1.0F, 0.5F, GraphletAnchor::RC, -xoff, 0.0F, true);

			this->reflow_dimension(this->dimensions, DL::psBowDraft, 1.0F, 0.0F, GraphletAnchor::RB, -yoff, -yoff);
			this->reflow_dimension(this->dimensions, DL::psBowHeight, 1.0F, 0.0F, GraphletAnchor::RT, -yoff, yoff);
			this->reflow_dimension(this->dimensions, DL::sbBowDraft, 1.0F, 1.0F, GraphletAnchor::RT, -yoff, yoff);
			this->reflow_dimension(this->dimensions, DL::sbBowHeight, 1.0F, 1.0F, GraphletAnchor::RB, -yoff, -yoff);
			
			this->reflow_dimension(this->dimensions, DL::psAmidshipDraft, 0.5F, 0.0F, GraphletAnchor::CB, 0.0F, -yoff);
			this->reflow_dimension(this->dimensions, DL::sbAmidshipDraft, 0.5F, 1.0F, GraphletAnchor::CT, 0.0F, yoff);
			this->reflow_dimension(this->dimensions, DL::psSuctionDraft, 0.7F, 0.0F, GraphletAnchor::CB, 0.0F, -yoff);
			this->reflow_dimension(this->dimensions, DL::sbSuctionDraft, 0.7F, 1.0F, GraphletAnchor::CT, 0.0F, yoff);

			this->reflow_dimension(this->dimensions, DL::psSternDraft, 0.0F, 0.0F, GraphletAnchor::LB, yoff, -yoff);//艉左吃水
			this->reflow_dimension(this->dimensions, DL::psSternHeight, 0.0F, 0.0F, GraphletAnchor::LT, yoff, yoff);//艉左舱高
			this->reflow_dimension(this->dimensions, DL::sbSternDraft, 0.0F, 1.0F, GraphletAnchor::LT, yoff, yoff);//艉右吃水
			this->reflow_dimension(this->dimensions, DL::sbSternHeight, 0.0F, 1.0F, GraphletAnchor::LB, yoff, -yoff);//艉右舱高

			this->reflow_dimension(this->dimensions, DL::SternDraft, 0.0F, 0.5F, GraphletAnchor::RC, -xoff);
		}

		{ // reflow buttons and relevant dimensions
			float shty, shby, lblty, dimby, tcx;

			this->decorator->fill_ship_anchor(0.0F, 0.0F, nullptr, &shty, true);
			this->decorator->fill_ship_anchor(0.0F, 1.0F, nullptr, &shby, true);

			this->master->fill_graphlet_location(this->cylabels[EWTS::HopperHeight], &tcx, &lblty, GraphletAnchor::CB);
			this->master->move_to(this->dimensions[DL::NetWeight], tcx, (shty + lblty) * 0.5F, GraphletAnchor::RC, -vinset);
			this->master->move_to(this->dimensions[DL::AverageDraft],this->dimensions[DL::NetWeight], GraphletAnchor::RC,GraphletAnchor::LC, vinset * 2.0F);
			this->master->move_to(this->dimensions[DL::TrimDegrees],this->dimensions[DL::AverageDraft], GraphletAnchor::RC,GraphletAnchor::LC, vinset * 2.0F);
			this->master->move_to(this->dimensions[DL::HeelDegrees],this->dimensions[DL::TrimDegrees], GraphletAnchor::RC,GraphletAnchor::LC, vinset * 2.0F);

			this->master->fill_graphlet_location(this->cydimensions[EWTS::HopperHeight], &tcx, &dimby, GraphletAnchor::CB);
			
			this->master->move_to(this->hdchecks[BottomDoorCommand::OpenDoorCheck], tcx, (dimby + shby) * 0.5F, GraphletAnchor::RC, -vinset);
			this->master->move_to(this->hdchecks[BottomDoorCommand::CloseDoorCheck],
				this->hdchecks[BottomDoorCommand::OpenDoorCheck], GraphletAnchor::RC,
				GraphletAnchor::LC, vinset * 2.0F);
			//this->master->move_to(this->buttons[DL::ADC], this->overflowpipe[DL::LeftOverflow],  GraphletAnchor::CB, GraphletAnchor::CT, (rofpx - lofpx)/2,vinset*2);
			this->master->move_to(this->buttons[DL::ADC],lofpx, (dimby + shby) * 0.5F,  GraphletAnchor::CB, vinset*2);
		}
	}

private:
	template<typename E>
	void load_overflow(std::map < E, Credit<OverflowPipelet, E>*>& ds, E id, float height) {
		ds[id] = this->master->insert_one(new Credit<OverflowPipelet, E>(hopper_height_range, height));
	}
	template<class G, typename E>
	void load_overflow2(std::map<E, G*>& ds,  E id, float height) {
		ds[id] = this->master->insert_one(new G(hopper_height_range, height), id);

	}
	template<typename E>
	void load_setting(std::map<E, Credit<Dimensionlet, E>*>& ds, E id, Platform::String^ unit) {
		ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(DimensionState::Input, this->setting_style, unit, _speak(id)), id);
		ds[id]->set_maximum(hopper_height_range);
	}

	template<typename E>
	void load_dimension(std::map<E, Credit<Dimensionlet, E>*>& ds, E id, Platform::String^ unit, DimensionStyle& style) {
		ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(style, unit, _speak(id)), id);
	}

	template<typename E>
	void load_dimensions(std::map<E, Credit<Dimensionlet, E>*>& ds, E id0, E idn, Platform::String^ unit) {
		for (E id = id0; id <= idn; id++) {
			this->load_dimension(ds, id, unit, this->plain_style);
		}
	}

	template<typename E>
	void load_dimension(std::map<E, Credit<Dimensionlet, E>*>& ds, std::map<E, Credit<Labellet, E>*>& ls, E id
		, Platform::String^ unit, unsigned int precision) {
		unsigned int saved_precision = this->flonum_style.precision;

		this->flonum_style.precision = precision;
		ls[id] = this->master->insert_one(new Credit<Labellet, E>(_speak(id), this->label_font, Colours::Silver), id);
		ds[id] = this->master->insert_one(new Credit<Dimensionlet, E>(this->flonum_style, unit), id);
		this->flonum_style.precision = saved_precision;
	}

	template<typename E>
	void load_dimensions(std::map<E, Credit<Dimensionlet, E>*>& ds, std::map<E, Credit<Labellet, E>*>& ls, E id0, E idn, Platform::String^ unit) {
		for (E id = id0; id <= idn; id++) {
			this->load_dimension(ds, ls, id, unit);
		}
	}

	template<typename E>
	void load_cylinder(std::map<E, Credit<Cylinderlet, E>*>& cs, E id, float height, double range
		, unsigned int precision, Platform::String^ unit) {
		cs[id] = this->master->insert_one(new Credit<Cylinderlet, E>(LiquidSurface::Convex,
			range, height * 0.2718F, height, 3.0F, 8U, precision));

		this->load_dimension(this->cydimensions, this->cylabels, id, unit, precision);
	}

	template<class B, typename CMD>
	void load_buttons(std::map<CMD, Credit<B, CMD>*>& bs, CMD cmd0, CMD cmdn, float width = 128.0F, float height = 32.0F) {
		for (CMD cmd = cmd0; cmd <= cmdn; cmd++) {
			bs[cmd] = this->master->insert_one(new Credit<B, CMD>(cmd.ToString(), width, height), cmd);
		}
	}

private:
	template<class C, typename E>
	void reflow_cylinders(std::map<E, Credit<C, E>*>& is, E id0, E idn, float gapsize) {
		float flcount = float(_I(idn) - _I(id0) + 1 + 4);
		float x, y;

		for (E id = id0; id <= idn; id++) {
			this->decorator->fill_ship_anchor(float(_I(id) - _I(id0) + 1) / flcount, 0.5F, &x, &y);

			this->master->move_to(is[id], x, y, GraphletAnchor::LC);
			this->master->move_to(this->cylabels[id], is[id], GraphletAnchor::CT, GraphletAnchor::CB, 0.0F, -gapsize);
			this->master->move_to(this->cydimensions[id], is[id], GraphletAnchor::CB, GraphletAnchor::CT, 0.0F, +gapsize);
		}
	}

	template<typename E>
	void reflow_dimension(std::map<E, Credit<Dimensionlet, E>*>& ds, E id , float fx, float fy
		, GraphletAnchor a, float dx = 0.0F, float dy = 0.0F, bool full = false) {
		float ax, ay;

		this->decorator->fill_ship_anchor(fx, fy, &ax, &ay, full);
		this->master->move_to(ds[id], ax, ay, a, dx, dy);
	}

private:
	void set_cylinder(EWTS id, double* values, float value) {
		this->cylinders[id]->set_value(value);
		this->cydimensions[id]->set_value(value, GraphletAnchor::CC);
		values[_I(id)] = value;
	}
	void set_flow_speed(double* metrics, const uint8* db203, unsigned int idx, double range) {
		metrics[_I(SM::fspeed)] = RealData(db203, idx + 1U) / range;
	}

	void set_flow_volume(double* metrics, const uint8* db2, unsigned int idx, double range) {
		metrics[_I(SM::fvolume)] = DBD(db2, idx) / range;
	}

	void set_density(double* metrics, const uint8* db203, unsigned int idx, double range) {
		metrics[_I(SM::density)] = RealData(db203, idx) / range;
	}
	void set_product(double* metrics, const uint8* db2, unsigned int idx, double range) {
		metrics[_I(SM::product_chance)] = DBD(db2, idx) / range;
	}
	void set_payload(double* metrics, const uint8* db2, unsigned int idx, double range) {
		metrics[_I(SM::payload_chance)] = DBD(db2, idx) / range;
	}
	bool door_open(const uint8* db203) {
		float opennum = 95;
		bool flag = (RealData(db203, bottom_door_PS1_progress) > opennum&&
			RealData(db203, bottom_door_PS2_progress) > opennum&&
			RealData(db203, bottom_door_PS3_progress) > opennum&&
			RealData(db203, bottom_door_PS4_progress) > opennum&&
			RealData(db203, bottom_door_PS5_progress) > opennum&&
			RealData(db203, bottom_door_PS6_progress) > opennum&&
			RealData(db203, bottom_door_PS7_progress) > opennum&&

			RealData(db203, bottom_door_SB1_progress) > opennum&&
			RealData(db203, bottom_door_SB2_progress) > opennum&&
			RealData(db203, bottom_door_SB3_progress) > opennum&&
			RealData(db203, bottom_door_SB4_progress) > opennum&&
			RealData(db203, bottom_door_SB5_progress) > opennum&&
			RealData(db203, bottom_door_SB6_progress) > opennum&&
			RealData(db203, bottom_door_SB7_progress) > opennum);
		return flag;
	}

private: // never delete these graphlets manually.
	std::map<DL, Credit<Labellet, DL>*> labels;
	std::map<DL, Credit<Dimensionlet, DL>*> dimensions;
	std::map<EWTS, Credit<Labellet, EWTS>*> cylabels;
	std::map<EWTS, Credit<Dimensionlet, EWTS>*> cydimensions;
	std::map<EWTS, Credit<Cylinderlet, EWTS>*> cylinders;
	std::map<BottomDoorCommand, Credit<Buttonlet, BottomDoorCommand>*> hdchecks;
	std::map<DL, Credit<Buttonlet, DL>*> buttons;
	TimeSerieslet<EWTS>* timeseries;
	//OverflowPipelet* overflowpipe;
	std::map<DL, Credit<OverflowPipelet, DL>*> overflowpipe;

	Radarlet<SM>* ps_radar;
	Radarlet<SM>* sb_radar;
	bool isOpen;

private:
	CanvasTextFormat^ label_font;
	DimensionStyle plain_style;
	DimensionStyle setting_style;
	DimensionStyle fixnum_style;
	DimensionStyle flonum_style;

private:
	DraughtsPage* master;
	ShipDecorator* decorator;
	EarthWorkDataSource* datasource;

private:
	long long departure;
	long long destination;
	bool timemachine;
private:
	DredgeAddress* ps_address;
	DredgeAddress* sb_address;

private:
	bool Door_Open_Flag_Old = false;//泥门打开 抛泥完毕
	bool Door_Open_Flag = false;//泥门打开 抛泥完毕
};

/*************************************************************************************************/
DraughtsPage::DraughtsPage(PLCMaster* plc) : Planet(__MODULE__), device(plc) {
	ShipDecorator* decorator = new ShipDecorator();
	Draughts* dashboard = new Draughts(this, decorator, this->device == nullptr);

	this->dashboard = dashboard;
	this->push_decorator(decorator);

	if (this->device != nullptr) {
		this->leftoverflow_op = make_leftoverflow_menu(plc);
		this->rightoverflow_op = make_rightoverflow_menu(plc);

		this->device->push_confirmation_receiver(dashboard);
	}
}

DraughtsPage::~DraughtsPage() {
	if (this->dashboard != nullptr) {
		delete this->dashboard;
	}
}

void DraughtsPage::load(CanvasCreateResourcesReason reason, float width, float height) {
	auto db = dynamic_cast<Draughts*>(this->dashboard);

	if (db != nullptr) {
		db->load(width, height, statusbar_height());
	}
}

void DraughtsPage::reflow(float width, float height) {
	auto db = dynamic_cast<Draughts*>(this->dashboard);
	
	if (db != nullptr) {
		db->reflow(width, height, statusbar_height());
	}
}

void DraughtsPage::on_startover(long long departure_ms, long long destination_ms) {
	auto dashboard = dynamic_cast<Draughts*>(this->dashboard);

	if (dashboard != nullptr) {
		dashboard->on_timemachine_startover(departure_ms, destination_ms);
	}
}

void DraughtsPage::on_timestream(long long timepoint_ms, size_t addr0, size_t addrn, uint8* data, size_t size, uint64 p_type, size_t p_size, Syslog* logger) {
	auto dashboard = dynamic_cast<Draughts*>(this->dashboard);

	if (dashboard != nullptr) {
		dashboard->on_all_signals(timepoint_ms, addr0, addrn, data, size, logger);
	}
}

bool DraughtsPage::can_select(IGraphlet* g) {
	return ((this->device != nullptr)
		&& ((dynamic_cast<OverflowPipelet*>(g) != nullptr)
			|| button_enabled(g)));
}

bool DraughtsPage::can_select_multiple() {
	return true; // Timeseries is available on timemachine.
}

bool DraughtsPage::on_key(VirtualKey key, bool wargrey_keyboard) {
	bool handled = Planet::on_key(key, wargrey_keyboard);

	if (!handled) {
		switch (key) {
		case VirtualKey::Print: {
			if (wargrey_keyboard) {
				this->hide_virtual_keyboard();
			}

			this->save(this->name() + "-" + file_basename_from_second(current_seconds()) + ".png",
				this->actual_width(), this->actual_height(), Colours::Background);

			if (wargrey_keyboard) {
				this->show_virtual_keyboard(ScreenKeyboard::Affinepad, this->get_focus_graphlet(), GraphletAnchor::CB, 0.0F, 4.0F);
			}

			handled = true;
		}; break;
		case VirtualKey::Enter: {
			if (this->device != nullptr) {
				auto editor = dynamic_cast<Credit<Dimensionlet, DL>*>(this->get_focus_graphlet());

				if (editor != nullptr) {
					float ofp_height = float(editor->get_input_number());

					if (ofp_height >= 0.0F) {
						switch (editor->id) {
						case DL::LeftOverflow:
							this->device->send_setting(left_overflow_pipe_target_height, ofp_height);//!!!!!!!!!!!!!!!!!!!!!!
							this->device->send_command(ps_overflow_pipe_move_to_target_height);
							break;
						case DL::RightOverflow:
							this->device->send_setting(right_overflow_pipe_target_height, ofp_height);//!!!!!!!!!!!!!!!!!!!!!!
							this->device->send_command(sb_overflow_pipe_move_to_target_height);
							break;
						}
					}

					this->hide_virtual_keyboard();
					this->set_caret_owner(nullptr);

					handled = true;
				}
			}
		}; break;
		}
	}

	return handled;
}

void DraughtsPage::on_focus(IGraphlet* g, bool yes) {
	if (yes) {
		auto timeseries = dynamic_cast<ITimeSerieslet*>(g);
		auto editor = dynamic_cast<IEditorlet*>(g);

		if (timeseries != nullptr) {
			this->show_virtual_keyboard(ScreenKeyboard::Affinepad, g, GraphletAnchor::CB, 0.0F, 4.0F);
		} else if (editor != nullptr) {
			if ((this->device != nullptr) && this->device->authorized()) {
				this->show_virtual_keyboard(ScreenKeyboard::Numpad);
			} else {
				this->set_caret_owner(nullptr);
			}
		}
	}
}

void DraughtsPage::on_tap_selected(IGraphlet* g, float local_x, float local_y) {
	//auto overflow = dynamic_cast<OverflowPipelet*>(g); 
	auto overflow = dynamic_cast<Credit<OverflowPipelet, DL>*>(g);
	auto hdchecker = dynamic_cast<Credit<Buttonlet, BottomDoorCommand>*>(g);
	auto button = dynamic_cast<Credit<Buttonlet, DL>*>(g);
	
	if (overflow != nullptr) {
		switch (overflow->id) {
		case DL::LeftOverflow:
			menu_popup(this->leftoverflow_op, g, local_x, local_y);
			break;
		case DL::RightOverflow:
			menu_popup(this->rightoverflow_op, g, local_x, local_y);
			break;
		}
		//menu_popup(this->overflow_op, g, local_x, local_y);
	} else if (hdchecker != nullptr) {
		this->device->send_command(DO_bottom_doors_special_command(hdchecker->id));
	}
	else if (button != nullptr) {
		this->device->send_command(871);
	}
}

void DraughtsPage::on_gesture(std::list<Windows::Foundation::Numerics::float2>& points, float x, float y) {
	//this->get_logger()->log_message(Log::Info, L"(%f, %f)", x, y);
}