﻿#include "application.hxx"
#include "configuration.hpp"

#include "monitor.hpp"
#include "widget.hxx"

#include "navigator/thumbnail.hpp"
#include "planet.hpp"
#include "timer.hxx"

#include "compass.hpp"
#include "transponder.hpp"
#include "moxa.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::DTPM;
using namespace WarGrey::GYDM;

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Background;

using namespace Windows::System;
using namespace Windows::Foundation;

using namespace Windows::UI::Input;

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Controls;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;

/*************************************************************************************************/
private ref class DredgerUniverse : public UniverseDisplay {
public:
	virtual ~DredgerUniverse() {
		if (this->plc != nullptr) {
			delete this->plc;
		}

		if (this->compass != nullptr) {
			delete this->compass;
		}

		if (this->transponder != nullptr) {
			delete this->transponder;
		}

		moxa_tcp_teardown(); // also destroy `this->ais`;
	}

internal:
	DredgerUniverse(Platform::String^ name, IUniverseNavigator* navigator, IHeadUpPlanet* heads_up)
		: UniverseDisplay(make_system_logger(default_logging_level, name), name, navigator, heads_up) {
		Syslog* plc_logger = make_system_logger(default_plc_master_logging_level, "PLC");
		
		moxa_tcp_setup();

		this->plc = new PLCMaster(plc_logger, plc_hostname, dtpm_plc_master_port, plc_master_suicide_timeout);
		this->compass = new Compass();
		this->transponder = new Transponder();

		system_set_subnet_prefix(system_subnet_prefix);
		ui_thread_initialize();
	}

protected:
	void construct(CanvasCreateResourcesReason reason) override {
		this->push_planet(new DTPMonitor(this->compass, this->transponder, this->plc));
	}

internal:
	Compass* compass;
	Transponder* transponder;
	PLCMaster* plc;
};

/*************************************************************************************************/
private ref class CH13800m3 sealed : public SplitView {
public:
	CH13800m3() : SplitView() {
		this->Margin = ThicknessHelper::FromUniformLength(0.0);
		this->PanePlacement = SplitViewPanePlacement::Left;
		this->DisplayMode = SplitViewDisplayMode::Overlay;
		this->IsPaneOpen = false;

		this->PointerMoved += ref new PointerEventHandler(this, &CH13800m3::on_pointer_moved);
		this->PointerReleased += ref new PointerEventHandler(this, &CH13800m3::on_pointer_released);

		this->SizeChanged += ref new SizeChangedEventHandler(this, &CH13800m3::on_size_changed);
	}

public:
	void construct(Platform::String^ name, Size region) {
		Platform::String^ localhost = system_ipv4_address();
		IUniverseNavigator* navigator = new ThumbnailNavigator(default_logging_level, name, region.Width / region.Height, 160.0F);

		this->universe = ref new DredgerUniverse(name, navigator, nullptr);
		
		// TODO: Why SplitView::Content cannot do it on its own?
		this->universe->register_virtual_keydown_event_handler(this);

		this->Content = this->universe->canvas;
		this->timeline = ref new CompositeTimerListener();
		this->timer = ref new Timer(this->timeline, frame_per_second);
		this->timeline->push_timer_listener(this->universe);

		{ // construct the functional panel
			StackPanel^ panel = ref new StackPanel();
			
			this->widget = ref new UniverseWidget(this, this->universe, this->universe->plc);
			this->widget->min_width = this->universe->navigator->min_width();
			
			panel->Orientation = ::Orientation::Vertical;
			panel->HorizontalAlignment = ::HorizontalAlignment::Stretch;
			panel->VerticalAlignment = ::VerticalAlignment::Stretch;

			panel->Children->Append(this->universe->navigator->user_interface());
			panel->Children->Append(this->widget->canvas);

			this->Pane = panel;
			this->OpenPaneLength = this->universe->navigator->min_width();
			this->timeline->push_timer_listener(this->widget);
		}
	}
	
	void on_foreground_activated(Platform::String^ argv) {}
	void on_entered_background(EnteredBackgroundEventArgs^ args) {}
	void on_background_activated(IBackgroundTaskInstance^ task) {}
	void on_leaving_background(LeavingBackgroundEventArgs^ args) {}
	void on_suspending(SuspendingEventArgs^ args) {}
	void on_resuming() {}
	
private:
	void on_size_changed(Platform::Object^ sender, SizeChangedEventArgs^ args) {
		float widget_height = widget_evaluate_height();

		this->universe->navigator->min_height(args->NewSize.Height - widget_height);
		this->widget->min_height = widget_height;
	}

private:
	void on_pointer_moved(Platform::Object^ sender, PointerRoutedEventArgs^ args) {
		auto pt = args->GetCurrentPoint(this);
		float x = pt->Position.X;

		if (!pt->Properties->IsLeftButtonPressed) {
			if (x <= this->Margin.Left) {
				this->IsPaneOpen = true;
				args->Handled = true;
			} 
		}
	}

	void on_pointer_released(Platform::Object^ sender, PointerRoutedEventArgs^ args) {
		auto pt = args->GetCurrentPoint(this);
		float x = pt->Position.X;

		if (pt->Properties->PointerUpdateKind == PointerUpdateKind::LeftButtonReleased) {
			if (x <= normal_font_size) {
				this->IsPaneOpen = true;
				args->Handled = true;
			}
		}
	}

private:
	CompositeTimerListener^ timeline;
	Timer^ timer;

private:
	DredgerUniverse^ universe;
	UniverseWidget^ widget;
};

int main(Platform::Array<Platform::String^>^ args) {
	return launch_universal_windows_application<CH13800m3>(default_logging_level, rsyslog_host, rsyslog_prot);
}
