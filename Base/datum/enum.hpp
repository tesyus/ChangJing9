#pragma once

namespace WarGrey::SCADA {
#define _N(e) static_cast<unsigned int>(e::_)
#define _I(e) static_cast<unsigned int>(e)
#define _S(e) static_cast<unsigned short>(e)
#define _C(e) static_cast<unsigned char>(e)
#define _F(e) static_cast<float>(e)
#define _D(e) static_cast<double>(e)

#define _E(E, i) static_cast<E>(i)
#define _E0(E) static_cast<E>(0)
}
