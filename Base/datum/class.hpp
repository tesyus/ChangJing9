#pragma once

namespace WarGrey::SCADA {
#define vpure_read_only_property(T, name) property T name { virtual T get() = 0; }
#define virtual_read_only_property(T, name) property T name { virtual T get(); }
#define override_read_only_property(T, name) property T name { T get() override; }
#define static_read_only_property(T, name)  static property T name { T get(); }

#define read_only_property(T, name)  property T name { T get(); }
#define write_only_property(T, name)  property T name { void set(T v); }
#define read_write_property(T, name) property T name { T get(); void set(T v); }
}
