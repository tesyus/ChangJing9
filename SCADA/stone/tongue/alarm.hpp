#pragma once

#include "tongue.hpp"

namespace WarGrey::SCADA {
    private class Alarms : public WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms> {
        friend class WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>;
    public:
        static Platform::String^ type() { return "alarm"; }

    public:
        static WarGrey::SCADA::Alarms* Alarm262359() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262359); }//液压主系统油箱液位高L1
        static WarGrey::SCADA::Alarms* Alarm262360() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262360); }//液压主系统油箱液位低L2
        static WarGrey::SCADA::Alarms* Alarm262361() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262361); }//液压主系统油箱液位过低L3
        static WarGrey::SCADA::Alarms* Alarm262362() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262362); }//液压主系统油箱油温高T1
        static WarGrey::SCADA::Alarms* Alarm262363() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262363); }//液压主系统油箱油温低T2
        static WarGrey::SCADA::Alarms* Alarm262372() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262372); }//耙唇液压系统油箱液位低
        static WarGrey::SCADA::Alarms* Alarm262373() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262373); }//耙唇液压系统油箱液位过低
        static WarGrey::SCADA::Alarms* Alarm262374() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262374); }//耙唇液压系统油箱油温高T1
        static WarGrey::SCADA::Alarms* Alarm262375() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262375); }//耙唇液压系统油箱油温低T2
        static WarGrey::SCADA::Alarms* Alarm262410() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262410); }//液压油箱加热器故障反馈
        static WarGrey::SCADA::Alarms* Alarm262414() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262414); }//耙唇油箱加热器故障反馈
        static WarGrey::SCADA::Alarms* Alarm262418() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262418); }//左舷耙头液压泵A1故障反馈
        static WarGrey::SCADA::Alarms* Alarm262422() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262422); }//左舷耙中液压泵B1故障反馈
        static WarGrey::SCADA::Alarms* Alarm262426() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262426); }//左舷耙中液压泵B2故障反馈
        static WarGrey::SCADA::Alarms* Alarm262430() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262430); }//左舷弯管液压泵C1故障反馈
        static WarGrey::SCADA::Alarms* Alarm262434() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262434); }//右舷耙头液压泵A2故障反馈
        static WarGrey::SCADA::Alarms* Alarm262438() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262438); }//右舷耙中液压泵B3故障反馈
        static WarGrey::SCADA::Alarms* Alarm262442() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262442); }//右舷耙中液压泵B4故障反馈
        static WarGrey::SCADA::Alarms* Alarm262446() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262446); }//右舷弯管液压泵C2故障反馈
        static WarGrey::SCADA::Alarms* Alarm262450() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262450); }//蝶阀控制泵D1 故障反馈
        static WarGrey::SCADA::Alarms* Alarm262454() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262454); }//泥泵锁紧D2故障反馈
        static WarGrey::SCADA::Alarms* Alarm262458() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262458); }//应急液压泵E故障反馈
        static WarGrey::SCADA::Alarms* Alarm262462() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262462); }//液压冷却泵F1故障反馈
        static WarGrey::SCADA::Alarms* Alarm262466() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262466); }//液压冷却泵F2故障反馈
        static WarGrey::SCADA::Alarms* Alarm262470() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262470); }//左舷耙唇液压泵J故障反馈
        static WarGrey::SCADA::Alarms* Alarm262474() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262474); }//右舷耙唇液压泵I故障反馈
        static WarGrey::SCADA::Alarms* Alarm262478() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262478); }//1#闸阀冲洗泵故障反馈
        static WarGrey::SCADA::Alarms* Alarm262482() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262482); }//2#闸阀冲洗泵故障反馈
        static WarGrey::SCADA::Alarms* Alarm262492() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262492); }//左舱内泥泵1#封水泵变频器变频器故障反馈
        static WarGrey::SCADA::Alarms* Alarm262493() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262493); }//左舱内泥泵1#封水泵变频器主电机过载反馈
        static WarGrey::SCADA::Alarms* Alarm262500() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262500); }//左舱内泥泵2#封水泵变频器变频器故障反馈
        static WarGrey::SCADA::Alarms* Alarm262501() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262501); }//左舱内泥泵2#封水泵变频器主电机过载反馈
        static WarGrey::SCADA::Alarms* Alarm262506() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262506); }//左舱内泥泵3#封水泵变频器变频器故障反馈
        static WarGrey::SCADA::Alarms* Alarm262507() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262507); }//左舱内泥泵3#封水泵变频器主电机过载反馈
        static WarGrey::SCADA::Alarms* Alarm262513() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262513); }//右舱内泥泵1#封水泵变频器变频器故障反馈
        static WarGrey::SCADA::Alarms* Alarm262514() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262514); }//右舱内泥泵1#封水泵变频器主电机过载反馈
        static WarGrey::SCADA::Alarms* Alarm262519() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262519); }//右舱内泥泵2#封水泵变频器变频器故障反馈
        static WarGrey::SCADA::Alarms* Alarm262520() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262520); }//右舱内泥泵2#封水泵变频器主电机过载反馈
        static WarGrey::SCADA::Alarms* Alarm262525() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262525); }//右舱内泥泵3#封水泵变频器变频器故障反馈
        static WarGrey::SCADA::Alarms* Alarm262526() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262526); }//右舱内泥泵3#封水泵变频器主电机过载反馈
        static WarGrey::SCADA::Alarms* Alarm262529() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262529); }//左舱内泵齿轮箱备用泵故障
        static WarGrey::SCADA::Alarms* Alarm262530() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262530); }//左舱内泵齿轮箱油位开关
        static WarGrey::SCADA::Alarms* Alarm262531() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262531); }//左舱内泵齿轮箱流量开关
        static WarGrey::SCADA::Alarms* Alarm262532() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262532); }//左舱内泵齿轮箱滤器压差75%
        static WarGrey::SCADA::Alarms* Alarm262535() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262535); }//左舱内泵齿轮箱滤器压差100%
        static WarGrey::SCADA::Alarms* Alarm262537() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262537); }//右舱内泵齿轮箱备用泵故障
        static WarGrey::SCADA::Alarms* Alarm262538() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262538); }//右舱内泵齿轮箱油位开关
        static WarGrey::SCADA::Alarms* Alarm262539() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262539); }//右舱内泵齿轮箱流量开关
        static WarGrey::SCADA::Alarms* Alarm262540() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262540); }//右舱内泵齿轮箱滤器压差75%
        static WarGrey::SCADA::Alarms* Alarm262543() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262543); }//右舱内泵齿轮箱滤器压差100%
        static WarGrey::SCADA::Alarms* Alarm262546() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262546); }//左疏浚驱动设备UPS电池低压
        static WarGrey::SCADA::Alarms* Alarm262547() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262547); }//左疏浚驱动设备UPSUPS旁路
        static WarGrey::SCADA::Alarms* Alarm262548() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262548); }//左疏浚驱动设备UPS电池保护
        static WarGrey::SCADA::Alarms* Alarm262549() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262549); }//左疏浚驱动设备UPS风扇故障
        static WarGrey::SCADA::Alarms* Alarm262550() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262550); }//左疏浚驱动设备UPS接地故障
        static WarGrey::SCADA::Alarms* Alarm262551() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262551); }//左疏浚驱动设备UPS主电源故障
        static WarGrey::SCADA::Alarms* Alarm262554() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262554); }//右疏浚驱动设备UPS电池低压
        static WarGrey::SCADA::Alarms* Alarm262555() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262555); }//右疏浚驱动设备UPSUPS旁路
        static WarGrey::SCADA::Alarms* Alarm262556() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262556); }//右疏浚驱动设备UPS电池保护
        static WarGrey::SCADA::Alarms* Alarm262557() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262557); }//右疏浚驱动设备UPS风扇故障
        static WarGrey::SCADA::Alarms* Alarm262558() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262558); }//右疏浚驱动设备UPS接地故障
        static WarGrey::SCADA::Alarms* Alarm262559() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262559); }//右疏浚驱动设备UPS主电源故障
        static WarGrey::SCADA::Alarms* Alarm262560() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262560); }//左高压冲水泵变频器UPS电源故障
        static WarGrey::SCADA::Alarms* Alarm262561() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262561); }//左高压冲水泵变频器风扇故障
        static WarGrey::SCADA::Alarms* Alarm262564() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262564); }//右高压冲水泵变频器UPS电源故障
        static WarGrey::SCADA::Alarms* Alarm262565() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262565); }//右高压冲水泵变频器风扇故障
        static WarGrey::SCADA::Alarms* Alarm262572() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262572); }//左舷舱内泥泵轴承润滑单元流量开关
        static WarGrey::SCADA::Alarms* Alarm262573() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262573); }//左舷舱内泥泵轴承润滑单元滤器指示
        static WarGrey::SCADA::Alarms* Alarm262574() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262574); }//左舷舱内泥泵轴承润滑单元液位低
        static WarGrey::SCADA::Alarms* Alarm262580() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262580); }//右舷舱内泥泵轴承润滑单元流量开关
        static WarGrey::SCADA::Alarms* Alarm262581() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262581); }//右舷舱内泥泵轴承润滑单元滤器指示
        static WarGrey::SCADA::Alarms* Alarm262582() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262582); }//右舷舱内泥泵轴承润滑单元液位低
        static WarGrey::SCADA::Alarms* Alarm262595() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262595); }//左舷水下泵1#封水泵故障反馈
        static WarGrey::SCADA::Alarms* Alarm262599() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262599); }//左舷水下泵2#封水泵故障反馈
        static WarGrey::SCADA::Alarms* Alarm262601() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262601); }//左舷水下泵系统综合报警
        static WarGrey::SCADA::Alarms* Alarm262602() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262602); }//左舷水下泵水下电机绕组温度报警
        static WarGrey::SCADA::Alarms* Alarm262603() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262603); }//左舷水下泵水下电机轴承温度报警
        static WarGrey::SCADA::Alarms* Alarm262604() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262604); }//左舷水下泵水下电机漏水报警
        static WarGrey::SCADA::Alarms* Alarm262605() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262605); }//左舷水下泵水下电机压差报警
        static WarGrey::SCADA::Alarms* Alarm262606() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262606); }//左舷水下泵水下电机润滑系统报警
        static WarGrey::SCADA::Alarms* Alarm262607() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262607); }//左舷水下泵水下电机控制系统报警
        static WarGrey::SCADA::Alarms* Alarm262611() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262611); }//右舷水下泵1#封水泵故障反馈
        static WarGrey::SCADA::Alarms* Alarm262615() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262615); }//右舷水下泵2#封水泵故障反馈
        static WarGrey::SCADA::Alarms* Alarm262617() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262617); }//右舷水下泵系统综合报警
        static WarGrey::SCADA::Alarms* Alarm262618() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262618); }//右舷水下泵水下电机绕组温度报警
        static WarGrey::SCADA::Alarms* Alarm262619() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262619); }//右舷水下泵水下电机轴承温度报警
        static WarGrey::SCADA::Alarms* Alarm262620() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262620); }//右舷水下泵水下电机漏水报警
        static WarGrey::SCADA::Alarms* Alarm262621() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262621); }//右舷水下泵水下电机压差报警
        static WarGrey::SCADA::Alarms* Alarm262622() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262622); }//右舷水下泵水下电机润滑系统报警
        static WarGrey::SCADA::Alarms* Alarm262623() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262623); }//右舷水下泵水下电机控制系统报警
        static WarGrey::SCADA::Alarms* Alarm13436300() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436300); }//左耙中角度过大0
        static WarGrey::SCADA::Alarms* Alarm13436301() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436301); }//右耙中角度过大0
        static WarGrey::SCADA::Alarms* Alarm13436689() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436689); }//回油压力小于3bar,所有绞车不能动作0
        static WarGrey::SCADA::Alarms* Alarm13436936() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436936); }//5#站ET200M-艉PLC通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436937() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436937); }//6#站ET200M-艏PLC柜第1屏通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436938() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436938); }//7#站ET200M-艏PLC柜第2屏-1通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436939() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436939); }//8#站ET200M-艏PLC柜第2屏-2通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436940() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436940); }//9#站ET200M-艏PLC柜第3屏通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436941() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436941); }//10#站ET200M-艏PLC柜第4屏-1通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436942() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436942); }//11#站ET200M-艏PLC柜第4屏-2通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436943() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436943); }//12#站ET200M-左疏浚台通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436944() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436944); }//13#站ET201M-右疏浚台通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436945() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436945); }//21#站-左弯管绞车编码器通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436946() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436946); }//22#站-左耙中绞车编码器通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436947() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436947); }//23#站-左耙头绞车编码器通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436948() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436948); }//24#站-右弯管绞车编码器通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436949() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436949); }//25#站-右耙中绞车编码器通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436950() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436950); }//26#站-右耙头绞车编码器通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436951() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436951); }//27#站-PMS DP/DP COUPLER通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436952() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436952); }//31#站-左舱内泵DP/DP COUPLER通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436953() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436953); }//32#站-左高压冲水泵DP/DP COUPLER通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436954() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436954); }//33#站-右舱内泵DP/DP COUPLER通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436955() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436955); }//34#站-右高压冲水泵DP/DP COUPLER通讯故障
        static WarGrey::SCADA::Alarms* Alarm13436959() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436959); }//PLC电源模块电池故障0
        static WarGrey::SCADA::Alarms* Alarm13436967() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436967); }//左泥泵封水泵压力异常0
        static WarGrey::SCADA::Alarms* Alarm13436968() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436968); }//左泥泵封水泵流量异常0
        static WarGrey::SCADA::Alarms* Alarm13436969() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436969); }//右泥泵封水泵压力异常0
        static WarGrey::SCADA::Alarms* Alarm13436970() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436970); }//右泥泵封水泵流量异常0
        static WarGrey::SCADA::Alarms* Alarm13436971() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436971); }//左舱内泵齿轮箱温度高报警（>58°）0
        static WarGrey::SCADA::Alarms* Alarm13436972() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436972); }//左舱内泵齿轮箱压力低报警(<0.4bar)0
        static WarGrey::SCADA::Alarms* Alarm13436973() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436973); }//右舱内泵齿轮箱温度高报警（>58°）0
        static WarGrey::SCADA::Alarms* Alarm13436974() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436974); }//右舱内泵齿轮箱压力低报警(<0.4bar)0
        static WarGrey::SCADA::Alarms* Alarm13437016() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437016); }//左舷1#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437017() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437017); }//左舷2#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437018() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437018); }//左舷3#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437019() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437019); }//左舷4#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437020() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437020); }//左舷5#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437021() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437021); }//左舷6#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437022() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437022); }//左舷7#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437023() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437023); }//右舷1#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437024() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437024); }//右舷2#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437025() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437025); }//右舷3#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437026() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437026); }//右舷4#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437027() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437027); }//右舷5#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437028() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437028); }//右舷6#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437029() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437029); }//右舷7#大泥门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437030() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437030); }//抽舱引水闸阀行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437031() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437031); }//左舷艉吃水断线报警
        static WarGrey::SCADA::Alarms* Alarm13437032() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437032); }//左舷波浪补偿器蓄能器压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437033() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437033); }//左波浪补偿器行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437034() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437034); }//右舷艉吃水断线报警
        static WarGrey::SCADA::Alarms* Alarm13437035() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437035); }//右舷波浪补偿器蓄能器压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437036() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437036); }//右波浪补偿器（短耙）行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437037() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437037); }//右波浪补偿器（长耙）行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437038() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437038); }//左舷耙头绞车放缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437039() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437039); }//左舷耙头绞车收缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437040() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437040); }//左舷耙中绞车放缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437041() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437041); }//左舷耙中绞车收缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437042() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437042); }//左舷弯管绞车放缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437043() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437043); }//左舷弯管绞车收缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437044() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437044); }//右舷耙头绞车放缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437045() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437045); }//右舷耙头绞车收缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437046() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437046); }//右舷耙中绞车放缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437047() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437047); }//右舷耙中绞车收缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437048() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437048); }//右舷弯管绞车放缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437049() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437049); }//右舷弯管绞车收缆压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437056() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437056); }//左1#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437057() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437057); }//左2#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437058() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437058); }//左3#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437059() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437059); }//左4#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437060() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437060); }//左5#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437061() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437061); }//左6#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437062() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437062); }//左7#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437063() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437063); }//右1#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437064() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437064); }//右2#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437065() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437065); }//右3#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437066() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437066); }//右4#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437067() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437067); }//右5#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437068() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437068); }//右6#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437069() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437069); }//右7#抽舱门行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437070() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437070); }//左舷艏泥舱液位断线报警
        static WarGrey::SCADA::Alarms* Alarm13437071() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437071); }//右舷艏泥舱液位断线报警
        static WarGrey::SCADA::Alarms* Alarm13437072() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437072); }//左舷艉泥舱液位断线报警
        static WarGrey::SCADA::Alarms* Alarm13437073() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437073); }//右舷艉泥舱液位断线报警
        static WarGrey::SCADA::Alarms* Alarm13437074() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437074); }//左舷舱内泵吸入压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437075() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437075); }//左舷舱内泵排出压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437076() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437076); }//左舷舱内泵吸入端封水压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437077() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437077); }//左舷舱内泵轴端封水压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437078() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437078); }//左舷舱内泵砂水分离器出口压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437079() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437079); }//右高压冲水泵出口压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437080() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437080); }//左舷艏吃水断线报警
        static WarGrey::SCADA::Alarms* Alarm13437081() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437081); }//左舷吸口吃水断线报警
        static WarGrey::SCADA::Alarms* Alarm13437082() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437082); }//左吸口充气压力 断线报警
        static WarGrey::SCADA::Alarms* Alarm13437083() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437083); }//右舷舱内泵吸入压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437084() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437084); }//右舷舱内泵排出压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437085() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437085); }//右舷舱内泵吸入端封水压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437086() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437086); }//右舷舱内泵轴端封水压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437087() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437087); }//右舷舱内泵砂水分离器出口压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437088() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437088); }//左高压冲水泵出口压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437089() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437089); }//右舷艏吃水断线报警
        static WarGrey::SCADA::Alarms* Alarm13437090() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437090); }//右舷吸口吃水断线报警
        static WarGrey::SCADA::Alarms* Alarm13437091() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437091); }//右吸口充气压力 断线报警
        static WarGrey::SCADA::Alarms* Alarm13437092() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437092); }//左舷舱内泵吸入端封水流量断线报警
        static WarGrey::SCADA::Alarms* Alarm13437093() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437093); }//左舷舱内泵轴端封水流量断线报警
        static WarGrey::SCADA::Alarms* Alarm13437094() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437094); }//右舷舱内泵吸入端封水流量断线报警
        static WarGrey::SCADA::Alarms* Alarm13437095() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437095); }//右舷舱内泵轴端封水流量断线报警
        static WarGrey::SCADA::Alarms* Alarm13437096() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437096); }//左高压冲水泵流量断线报警
        static WarGrey::SCADA::Alarms* Alarm13437097() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437097); }//右高压冲水泵流量断线报警
        static WarGrey::SCADA::Alarms* Alarm13437098() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437098); }//左舷吸口闸阀行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437099() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437099); }//右舷吸口闸阀行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437100() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437100); }//左舷溢流筒行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437101() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437101); }//右舷溢流筒行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437102() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437102); }//泥门、闸阀压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437103() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437103); }//回油背压压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437104() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437104); }//泥门保压泵油压断线报警
        static WarGrey::SCADA::Alarms* Alarm13437105() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437105); }//蝶阀泵油压断线报警
        static WarGrey::SCADA::Alarms* Alarm13437106() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437106); }//液压主油箱油温断线报警
        static WarGrey::SCADA::Alarms* Alarm13437107() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437107); }//左耙头油泵油压断线报警
        static WarGrey::SCADA::Alarms* Alarm13437108() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437108); }//左耙中油泵油压断线报警
        static WarGrey::SCADA::Alarms* Alarm13437109() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437109); }//左弯管油泵油压断线报警
        static WarGrey::SCADA::Alarms* Alarm13437110() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437110); }//右耙头油泵油压断线报警
        static WarGrey::SCADA::Alarms* Alarm13437111() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437111); }//右耙中油泵油压断线报警
        static WarGrey::SCADA::Alarms* Alarm13437112() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437112); }//右弯管油泵油压断线报警
        static WarGrey::SCADA::Alarms* Alarm13437113() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437113); }//左耙唇油泵油压断线报警
        static WarGrey::SCADA::Alarms* Alarm13437114() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437114); }//右耙唇油泵油压断线报警
        static WarGrey::SCADA::Alarms* Alarm13437115() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437115); }//耙唇油箱油温断线报警
        static WarGrey::SCADA::Alarms* Alarm13437116() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437116); }//左舷上垂直断线报警
        static WarGrey::SCADA::Alarms* Alarm13437117() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437117); }//左舷上水平断线报警
        static WarGrey::SCADA::Alarms* Alarm13437118() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437118); }//左舷下垂直断线报警
        static WarGrey::SCADA::Alarms* Alarm13437119() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437119); }//左舷下水平断线报警
        static WarGrey::SCADA::Alarms* Alarm13437120() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437120); }//左舷耙管拉力1 断线报警
        static WarGrey::SCADA::Alarms* Alarm13437121() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437121); }//左舷耙管拉力2 断线报警
        static WarGrey::SCADA::Alarms* Alarm13437122() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437122); }//左舷耙头行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437123() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437123); }//左舷耙头压差断线报警
        static WarGrey::SCADA::Alarms* Alarm13437124() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437124); }//左水下泵吸入真空断线报警
        static WarGrey::SCADA::Alarms* Alarm13437125() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437125); }//左舷水下泵封水压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437126() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437126); }//左舷水下泵排出压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437127() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437127); }//备用断线报警
        static WarGrey::SCADA::Alarms* Alarm13437128() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437128); }//右舷上垂直断线报警
        static WarGrey::SCADA::Alarms* Alarm13437129() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437129); }//右舷上水平断线报警
        static WarGrey::SCADA::Alarms* Alarm13437130() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437130); }//右舷下垂直断线报警
        static WarGrey::SCADA::Alarms* Alarm13437131() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437131); }//右舷下水平断线报警
        static WarGrey::SCADA::Alarms* Alarm13437132() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437132); }//右舷耙管拉力1 断线报警
        static WarGrey::SCADA::Alarms* Alarm13437133() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437133); }//右舷耙管拉力2 断线报警
        static WarGrey::SCADA::Alarms* Alarm13437134() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437134); }//右舷耙头行程断线报警
        static WarGrey::SCADA::Alarms* Alarm13437135() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437135); }//右舷耙头压差断线报警
        static WarGrey::SCADA::Alarms* Alarm13437136() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437136); }//右水下泵吸入真空断线报警
        static WarGrey::SCADA::Alarms* Alarm13437137() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437137); }//右舷水下泵封水压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437138() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437138); }//右舷水下泵排出压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437139() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437139); }//备用断线报警
        static WarGrey::SCADA::Alarms* Alarm13437140() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437140); }//左舱内泥泵1#封水泵电机转速断线报警
        static WarGrey::SCADA::Alarms* Alarm13437141() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437141); }//左舱内泥泵1#封水泵电机电流断线报警
        static WarGrey::SCADA::Alarms* Alarm13437142() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437142); }//左舱内泥泵2#封水泵电机转速断线报警
        static WarGrey::SCADA::Alarms* Alarm13437143() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437143); }//左舱内泥泵2#封水泵电机电流断线报警
        static WarGrey::SCADA::Alarms* Alarm13437144() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437144); }//左舱内泥泵3#封水泵电机转速断线报警
        static WarGrey::SCADA::Alarms* Alarm13437145() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437145); }//左舱内泥泵3#封水泵电机电流断线报警
        static WarGrey::SCADA::Alarms* Alarm13437146() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437146); }//右舱内泥泵1#封水泵电机转速断线报警
        static WarGrey::SCADA::Alarms* Alarm13437147() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437147); }//右舱内泥泵1#封水泵电机电流断线报警
        static WarGrey::SCADA::Alarms* Alarm13437148() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437148); }//右舱内泥泵2#封水泵电机转速断线报警
        static WarGrey::SCADA::Alarms* Alarm13437149() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437149); }//右舱内泥泵2#封水泵电机电流断线报警
        static WarGrey::SCADA::Alarms* Alarm13437150() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437150); }//右舱内泥泵3#封水泵电机转速断线报警
        static WarGrey::SCADA::Alarms* Alarm13437151() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437151); }//右舱内泥泵3#封水泵电机电流断线报警
        static WarGrey::SCADA::Alarms* Alarm13437152() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437152); }//左舱内泥泵1#封水泵电机绕组U温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437153() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437153); }//左舱内泥泵1#封水泵电机绕组V温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437154() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437154); }//左舱内泥泵1#封水泵电机绕组W温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437155() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437155); }//左舱内泥泵2#封水泵电机绕组U温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437156() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437156); }//左舱内泥泵2#封水泵电机绕组V温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437157() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437157); }//左舱内泥泵2#封水泵电机绕组W温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437158() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437158); }//左舱内泥泵3#封水泵电机绕组U温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437159() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437159); }//左舱内泥泵3#封水泵电机绕组V温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437160() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437160); }//左舱内泥泵3#封水泵电机绕组W温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437161() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437161); }//右舱内泥泵1#封水泵电机绕组U温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437162() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437162); }//右舱内泥泵1#封水泵电机绕组V温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437163() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437163); }//右舱内泥泵1#封水泵电机绕组W温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437164() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437164); }//右舱内泥泵2#封水泵电机绕组U温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437165() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437165); }//右舱内泥泵2#封水泵电机绕组V温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437166() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437166); }//右舱内泥泵2#封水泵电机绕组W温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437167() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437167); }//右舱内泥泵3#封水泵电机绕组U温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437168() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437168); }//右舱内泥泵3#封水泵电机绕组V温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437169() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437169); }//右舱内泥泵3#封水泵电机绕组W温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437176() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437176); }//左泥泵齿轮箱进机油温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437177() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437177); }//左泥泵齿轮箱进机油压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437178() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437178); }//右泥泵齿轮箱进机油温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437179() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437179); }//右泥泵齿轮箱进机油压力断线报警
        static WarGrey::SCADA::Alarms* Alarm13437180() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437180); }//左舷舱内泥泵轴承润滑单元滑油温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437181() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437181); }//右舷舱内泥泵轴承润滑单元滑油温度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437184() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437184); }//右舷弯管绞车遥控速度设定断线报警
        static WarGrey::SCADA::Alarms* Alarm13437185() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437185); }//右舷耙中绞车遥控速度设定断线报警
        static WarGrey::SCADA::Alarms* Alarm13437186() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437186); }//右舷耙头绞车遥控速度设定断线报警
        static WarGrey::SCADA::Alarms* Alarm13437188() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437188); }//右舷舱内泵/水下泵电机遥控速度设定断线报警
        static WarGrey::SCADA::Alarms* Alarm13437189() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437189); }//右高压冲水泵电机遥控速度设定断线报警
        static WarGrey::SCADA::Alarms* Alarm13437192() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437192); }//左舷弯管绞车遥控速度设定断线报警
        static WarGrey::SCADA::Alarms* Alarm13437193() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437193); }//左舷耙头绞车遥控速度设定断线报警
        static WarGrey::SCADA::Alarms* Alarm13437194() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437194); }//左舷耙中绞车遥控速度设定断线报警
        static WarGrey::SCADA::Alarms* Alarm13437195() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437195); }//备用断线报警
        static WarGrey::SCADA::Alarms* Alarm13437196() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437196); }//左舷舱内泵/水下泵电机遥控速度设定断线报警
        static WarGrey::SCADA::Alarms* Alarm13437197() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437197); }//左高压冲水泵电机遥控速度设定断线报警
        static WarGrey::SCADA::Alarms* Alarm13437198() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437198); }//疏浚台显示器调光断线报警
        static WarGrey::SCADA::Alarms* Alarm13437199() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437199); }//航行台显示器调光断线报警
        static WarGrey::SCADA::Alarms* Alarm13437200() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437200); }//左舷水下泵密度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437201() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437201); }//左舷水下泵流速断线报警
        static WarGrey::SCADA::Alarms* Alarm13437202() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437202); }//右舷水下泵密度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437203() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437203); }//右舷水下泵流速断线报警
        static WarGrey::SCADA::Alarms* Alarm13437204() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437204); }//左舷舱内泵密度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437205() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437205); }//左舷舱内泵流速断线报警
        static WarGrey::SCADA::Alarms* Alarm13437206() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437206); }//右舷舱内泵密度断线报警
        static WarGrey::SCADA::Alarms* Alarm13437207() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437207); }//右舷舱内泵流速断线报警
        static WarGrey::SCADA::Alarms* Alarm13437248() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437248); }//左耙上耙管角度过大报警0
        static WarGrey::SCADA::Alarms* Alarm13437249() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437249); }//左耙下耙管角度过大报警0
        static WarGrey::SCADA::Alarms* Alarm13437250() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437250); }//左耙上下耙管夹角角度过大报警0
        static WarGrey::SCADA::Alarms* Alarm13437251() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437251); }//左耙弯管钢丝绳下放长度过大报警0
        static WarGrey::SCADA::Alarms* Alarm13437252() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437252); }//右耙上耙管角度过大报警0
        static WarGrey::SCADA::Alarms* Alarm13437253() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437253); }//右耙下耙管角度过大报警0
        static WarGrey::SCADA::Alarms* Alarm13437254() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437254); }//右耙上下耙管夹角角度过大报警0
        static WarGrey::SCADA::Alarms* Alarm13437255() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437255); }//右耙弯管钢丝绳下放长度过大报警0

        /*
        static WarGrey::SCADA::Alarms* PSHopperMaintenance() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262151U); } // PS Hopper/Underwater Converter Mainenance
        static WarGrey::SCADA::Alarms* PSGlandPumpABroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262155U); } // PS Master Gland Pump Broken
        static WarGrey::SCADA::Alarms* PSGlandPumpBBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262159U); } // PS Spare Gland Pump Broken
        static WarGrey::SCADA::Alarms* PSWaterPumpAlert() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262163U); } // PS Water Pump Alert
        static WarGrey::SCADA::Alarms* PSWaterPumpBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262164U); } // PS Water Pump Broken
        static WarGrey::SCADA::Alarms* PSWaterPumpMaintenance() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262166U); } // PS Water Pump Maintenance
        static WarGrey::SCADA::Alarms* PSWaterPumpEmergence() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262167U); } // PS Water Pump Emergence
        static WarGrey::SCADA::Alarms* SBHopperMaintenance() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262175U); } // SB Hopper/Underwater Converter Mainenance
        static WarGrey::SCADA::Alarms* SBGlandPumpABroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262179U); } // SB Master Gland Pump Broken
        static WarGrey::SCADA::Alarms* SBGlandPumpBBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262183U); } // SB Spare Gland Pump Broken
        static WarGrey::SCADA::Alarms* SBWaterPumpAlert() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262187U); } // SB Water Pump Alert
        static WarGrey::SCADA::Alarms* SBWaterPumpBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262188U); } // SB Water Pump Broken
        static WarGrey::SCADA::Alarms* SBWaterPumpMaintenance() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262191U); } // SB Water Pump Emergence
        static WarGrey::SCADA::Alarms* PSDHPumpABroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262194U); } // PS Drag Head Pump A Broken
        static WarGrey::SCADA::Alarms* PSDIPumpBBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262198U); } // PS Drag Intermediate Pump B Broken
        static WarGrey::SCADA::Alarms* PSDTPumpCBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262202U); } // PS Drag Offset Pump C Broken
        static WarGrey::SCADA::Alarms* SBDVPumpIBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262206U); } // SB Drag Visor Pump I Broken
        static WarGrey::SCADA::Alarms* SBDHPumpHBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262210U); } // SB Drag Head Pump H Broken
        static WarGrey::SCADA::Alarms* SBDIPumpGBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262214U); } // SB Drag Intermediate Pump G Broken
        static WarGrey::SCADA::Alarms* SBDTPumpFBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262218U); } // SB Drag Offset Pump F Broken
        static WarGrey::SCADA::Alarms* SBDVPumpJBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262222U); } // SB Drag Visor Pump J Broken
        static WarGrey::SCADA::Alarms* DLBVPumpDBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262226U); } // Door Locker/Butterfly Valve Pump D Broken
        static WarGrey::SCADA::Alarms* DLBVPumpEBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262230U); } // Door Locker/Butterfly Valve Pump E Broken
        static WarGrey::SCADA::Alarms* CoolanPumpKBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262234U); } // Coolant Pump K Broken
        static WarGrey::SCADA::Alarms* MotorFlushingPumpLBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262238U); } // Motor Flushing Pump L Broken
        static WarGrey::SCADA::Alarms* SparePumpMBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262242U); } // Spare Pump M Broken
        static WarGrey::SCADA::Alarms* EmergencePumpYBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262246U); } // Emergence Pump Y Broken
        static WarGrey::SCADA::Alarms* PSGVFlushingPumpBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262250U); } // PS Gate Valve Flushing Pump Broken
        static WarGrey::SCADA::Alarms* SBGVFlushingPumpBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262254U); } // SB Gate Valve Flushing Pump Broken
        static WarGrey::SCADA::Alarms* MasterTankLS2() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262262U); } // Master Tank Level Ultra Low
        static WarGrey::SCADA::Alarms* VisorTankLS2() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262274U); } // Drag Visor Tank Level Ultra Low
        static WarGrey::SCADA::Alarms* PumpA2C() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262288U); } // Pump A Replace C
        static WarGrey::SCADA::Alarms* PumpC2A() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262289U); } // Pump C Replace A
        static WarGrey::SCADA::Alarms* PumpB2C() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262290U); } // Pump B Replace C
        static WarGrey::SCADA::Alarms* PumpC2B() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262291U); } // Pump C Replace B
        static WarGrey::SCADA::Alarms* PumpF2C() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262292U); } // Pump F Replace C
        static WarGrey::SCADA::Alarms* PumpC2F() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262293U); } // Pump C Replace F
        static WarGrey::SCADA::Alarms* PumpH2F() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262294U); } // Pump H Replace F
        static WarGrey::SCADA::Alarms* PumpF2H() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262295U); } // Pump F Replace H
        static WarGrey::SCADA::Alarms* PumpG2F() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262296U); } // Pump G Replace F
        static WarGrey::SCADA::Alarms* PumpF2G() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262297U); } // Pump F Replace G
        static WarGrey::SCADA::Alarms* PumpI2J() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262298U); } // Pump I Replace J
        static WarGrey::SCADA::Alarms* PumpJ2I() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262299U); } // Pump J Replace I
        static WarGrey::SCADA::Alarms* ShoreWinchEmergence() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262307U); } // Shore Dischange Winch Emergence
        static WarGrey::SCADA::Alarms* PSDTEmergence() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262319U); } // PS Drag Offset Emergence
        static WarGrey::SCADA::Alarms* PSDIEmergence() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262327U); } // PS Drag Intermediate Emergence
        static WarGrey::SCADA::Alarms* PSDHEmergence() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262335U); } // PS Drag Head Emergence
        static WarGrey::SCADA::Alarms* SBDTEmergence() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262343U); } // SB Drag Offset Emergence
        static WarGrey::SCADA::Alarms* SBDIEmergence() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262351U); } // SB Drag Intermediate Emergence
        static WarGrey::SCADA::Alarms* SBDHEmergence() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262359U); } // SB Drag Head Emergence
        static WarGrey::SCADA::Alarms* BargeWinchEmergence() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262367U); } // Barge Winch Emergence
        static WarGrey::SCADA::Alarms* PSHopperUnitPressureLow() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262629U); } // PS Hopper Lubricating Unit Pressure Low
        static WarGrey::SCADA::Alarms* PSHopperUnitLevelLow() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262630U); } // PS Hopper Lubricating Unit Level Low
        static WarGrey::SCADA::Alarms* PSHopperUnitOilTemperatureHigh() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262631U); } // PS Hopper Lubricating Unit Oil Temperature High
        static WarGrey::SCADA::Alarms* PSHopperUnitWaterTemperatureHigh() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262632U); } // PS Hopper Lubricating Unit Water Temperature High
        static WarGrey::SCADA::Alarms* PSHopperUnitBearingTemperatureHigh1() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262633U); } // PS Hopper Lubricating Unit Bearing Temperature 1# High
        static WarGrey::SCADA::Alarms* PSHopperUnitBearingTemperatureHigh2() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262634U); } // PS Hopper Lubricating Unit Bearing Temperature 2# High
        static WarGrey::SCADA::Alarms* PSHopperUnitBearingTemperatureHigh3() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262635U); } // PS Hopper Lubricating Unit Bearing Temperature 3# High
        static WarGrey::SCADA::Alarms* SBHopperUnitPressureLow() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262645U); } // SB Hopper Lubricating Unit Pressure Low
        static WarGrey::SCADA::Alarms* SBHopperUnitLevelLow() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262646U); } // SB Hopper Lubricating Unit Level Low
        static WarGrey::SCADA::Alarms* SBHopperUnitOilTemperatureHigh() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262647U); } // SB Hopper Lubricating Unit Oil Temperature High
        static WarGrey::SCADA::Alarms* SBHopperUnitWaterTemperatureHigh() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262648U); } // SB Hopper Lubricating Unit Water Temperature High
        static WarGrey::SCADA::Alarms* SBHopperUnitBearingTemperatureHigh1() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262649U); } // SB Hopper Lubricating Unit Bearing Temperature 1# High
        static WarGrey::SCADA::Alarms* SBHopperUnitBearingTemperatureHigh2() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262650U); } // SB Hopper Lubricating Unit Bearing Temperature 2# High
        static WarGrey::SCADA::Alarms* SBHopperUnitBearingTemperatureHigh3() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262651U); } // SB Hopper Lubricating Unit Bearing Temperature 3# High
        static WarGrey::SCADA::Alarms* PSUnderWaterGlandPump1Broken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262659U); } // PS Underwater Master Gland Pump 1# Broken
        static WarGrey::SCADA::Alarms* PSUnderWaterGlandPump2Broken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262663U); } // PS Underwater Master Gland Pump 2# Broken
        static WarGrey::SCADA::Alarms* PSHopperGearboxMasterPumpBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262666U); } // PS Hopper Gearbox Master Pump Broken
        static WarGrey::SCADA::Alarms* PSHopperGearboxSparePumpBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262669U); } // PS Hopper Gearbox Spare Pump Broken
        static WarGrey::SCADA::Alarms* PSHopperGearboxTemperatureHigh() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262670U); } // PS Hopper Gearbox Temperature High
        static WarGrey::SCADA::Alarms* PSHopperGearboxPressureLow() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262671U); } // PS Hopper Gearbox Pressure Low
        static WarGrey::SCADA::Alarms* SBUnderWaterGlandPump1Broken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262675U); } // SB Underwater Master Gland Pump 1# Broken
        static WarGrey::SCADA::Alarms* SBUnderWaterGlandPump2Broken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262679U); } // SB Underwater Master Gland Pump 2# Broken
        static WarGrey::SCADA::Alarms* SBHopperGearboxMasterPumpBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262682U); } // SB Hopper Gearbox Master Pump Broken
        static WarGrey::SCADA::Alarms* SBHopperGearboxSparePumpBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262685U); } // SB Hopper Gearbox Spare Pump Broken
        static WarGrey::SCADA::Alarms* SBHopperGearboxTemperatureHigh() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262686U); } // SB Hopper Gearbox Temperature High
        static WarGrey::SCADA::Alarms* SBHopperGearboxPressureLow() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262687U); } // SB Hopper Gearbox Pressure Low
        static WarGrey::SCADA::Alarms* HydraulicsStoppedBySailing() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262812U); } // Hydraulics Stopped(Sailing Console)
        static WarGrey::SCADA::Alarms* PSWaterValveOpenTimeout() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13435914U); } // PS Water Pump Outlet Valve Open Timeout
        static WarGrey::SCADA::Alarms* SBWaterValveOpenTimeout() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13435915U); } // SB Water Pump Outlet Valve Open Timeout
        static WarGrey::SCADA::Alarms* PSDIAngleExceed() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436298U); } // PS Drag Intermediate Angle Exceed
        static WarGrey::SCADA::Alarms* SBDIAngleExceed() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436299U); } // SB Drag Intermediate Angle Exceed
        static WarGrey::SCADA::Alarms* BackOilPressureFail() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436689U); } // Back Oil Pressure Less Than 3bar, All Winches are Limited
        static WarGrey::SCADA::Alarms* ET200M_BowPLC1Lost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436936U); } // 11#ET200M - Bow PLC 1 Lost
        static WarGrey::SCADA::Alarms* ET200M_BowPLC2Lost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436937U); } // 12#ET200M - Bow PLC 2 ET200M Lost
        static WarGrey::SCADA::Alarms* ET200M_BowPLC3Lost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436938U); } // 13#ET200M - Bow PLC 3 ET200M Lost
        static WarGrey::SCADA::Alarms* ET200M_BowPLC4Lost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436939U); } // 14#ET200M - Bow PLC 4 ET200M Lost
        static WarGrey::SCADA::Alarms* ET200M_BowPLC5Lost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436940U); } // 15#ET200M - Bow PLC 5 ET200M Lost
        static WarGrey::SCADA::Alarms* ET200M_ConsoleDCC1Lost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436941U); } // 16#ET200M - Dredging Console DCC1 Lost
        static WarGrey::SCADA::Alarms* ET200M_ConsoleDCC2Lost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436942U); } // 17#ET200M - Dredging Console DCC2 Lost
        static WarGrey::SCADA::Alarms* PSDTWinchLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436943U); } // 21# - PS Drag Offset Winch Lost
        static WarGrey::SCADA::Alarms* SBDTWinchLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436944U); } // 22# - SB Drag Offset Winch Lost
        static WarGrey::SCADA::Alarms* PSDIWinchLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436945U); } // 23# - PS Drag Intermediate Winch Lost
        static WarGrey::SCADA::Alarms* SBDIWinchLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436946U); } // 24# - SB Drag Intermediate Winch Lost
        static WarGrey::SCADA::Alarms* PSDHWinchLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436947U); } // 25# - PS Drag Head Winch Lost
        static WarGrey::SCADA::Alarms* SBDHWinchLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436948U); } // 26# - SB Drag Head Winch Lost
        static WarGrey::SCADA::Alarms* BargeWinchLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436949U); } // 27# - Barge Winch Lost
        static WarGrey::SCADA::Alarms* MasterCPU_BowPLC1Lost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436950U); } // 2#CPU(Master) - Bow PLC 1 Lost
        static WarGrey::SCADA::Alarms* SpareCPU_BowPLC1Lost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436951U); } // 3#CPU(Spare) - Bow PLC 1 Lost
        static WarGrey::SCADA::Alarms* DCSDPLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436952U); } // 18# DCS DP Lost
        static WarGrey::SCADA::Alarms* PSHPDPLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436953U); } // 31# PS Hopper Pump DP Lost
        static WarGrey::SCADA::Alarms* SBHPDPLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436954U); } // 32# SB Hopper Pump DP Lost
        static WarGrey::SCADA::Alarms* PSWPDPLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436955U); } // 33# PS Water Pump DP Lost
        static WarGrey::SCADA::Alarms* SBWPDPLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436956U); } // 34# SB Water Pump DP Lost
        static WarGrey::SCADA::Alarms* PSUWPDPLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436957U); } // 35# PS Underwater Pump DP Lost
        static WarGrey::SCADA::Alarms* SBUWPDPLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436958U); } // 36# PS Underwater Pump DP Lost
        static WarGrey::SCADA::Alarms* PLCPowerBroken() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436959U); } // PLC Power Broken
        static WarGrey::SCADA::Alarms* TildemeterLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436961U); } // Tildemeter Lost
        static WarGrey::SCADA::Alarms* GPSLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436962U); } // GPS Lost
        static WarGrey::SCADA::Alarms* GyrocompassLost() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436963U); } // Gyrocompass Lost
        static WarGrey::SCADA::Alarms* PSFAAngleExceed() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437168U); } // PS Forearm Drag Angle Exceed
        static WarGrey::SCADA::Alarms* PSBAAngleExceed() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437169U); } // PS Backarm Drag Angle Exceed
        static WarGrey::SCADA::Alarms* PSFBAngleExceed() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437170U); } // PS Fore-Back Angle Exceed
        static WarGrey::SCADA::Alarms* PSDTCableExceed() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437171U); } // PS Drag Offset Cable Exceed
        static WarGrey::SCADA::Alarms* SBFAAngleExceed() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437172U); } // SB Forearm Drag Angle Exceed
        static WarGrey::SCADA::Alarms* SBBAAngleExceed() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437173U); } // SB Backarm Drag Angle Exceed
        static WarGrey::SCADA::Alarms* SBFBAngleExceed() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437174U); } // SB Fore-Back Angle Exceed
        static WarGrey::SCADA::Alarms* SBDTCableExceed() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437175U); } // SB Drag Offset Cable Exceed
        */
    private:
        static size_t indices_size() { return  305U; }
        static const unsigned int* indices() {
            static const unsigned int indexes[] = {
                /*
                262151U, 262155U, 262159U, 262163U, 262164U, 262166U, 262167U, 262175U,
                262179U, 262183U, 262187U, 262188U, 262191U, 262194U, 262198U, 262202U,
                262206U, 262210U, 262214U, 262218U, 262222U, 262226U, 262230U, 262234U,
                262238U, 262242U, 262246U, 262250U, 262254U, 262262U, 262274U, 262288U,
                262289U, 262290U, 262291U, 262292U, 262293U, 262294U, 262295U, 262296U,
                262297U, 262298U, 262299U, 262307U, 262319U, 262327U, 262335U, 262343U,
                262351U, 262359U, 262367U, 262629U, 262630U, 262631U, 262632U, 262633U,
                262634U, 262635U, 262645U, 262646U, 262647U, 262648U, 262649U, 262650U,
                262651U, 262659U, 262663U, 262666U, 262669U, 262670U, 262671U, 262675U,
                262679U, 262682U, 262685U, 262686U, 262687U, 262812U, 13435914U, 13435915U,
                13436298U, 13436299U, 13436689U, 13436936U, 13436937U, 13436938U, 13436939U, 13436940U,
                13436941U, 13436942U, 13436943U, 13436944U, 13436945U, 13436946U, 13436947U, 13436948U,
                13436949U, 13436950U, 13436951U, 13436952U, 13436953U, 13436954U, 13436955U, 13436956U,
                13436957U, 13436958U, 13436959U, 13436961U, 13436962U, 13436963U, 13437168U, 13437169U,
                13437170U, 13437171U, 13437172U, 13437173U, 13437174U, 13437175U*/
                262359,262360,262361,262362,262363,262372,262373,262374,262375,262410,262414,262418,262422,262426,262430,262434,
                262438,262442,262446,262450,262454,262458,262462,262466,262470,262474,262478,262482,262492,262493,262500,262501,
                262506,262507,262513,262514,262519,262520,262525,262526,262529,262530,262531,262532,262535,262537,262538,262539,
                262540,262543,262546,262547,262548,262549,262550,262551,262554,262555,262556,262557,262558,262559,262560,262561,
                262564,262565,262572,262573,262574,262580,262581,262582,262595,262599,262601,262602,262603,262604,262605,262606,
                262607,262611,262615,262617,262618,262619,262620,262621,262622,262623,13436300,13436301,13436689,13436936,13436937,13436938,
                13436939,13436940,13436941,13436942,13436943,13436944,13436945,13436946,13436947,13436948,13436949,13436950,13436951,13436952,13436953,13436954,
                13436955,13436959,13436967,13436968,13436969,13436970,13436971,13436972,13436973,13436974,13437016,13437017,13437018,13437019,13437020,13437021,
                13437022,13437023,13437024,13437025,13437026,13437027,13437028,13437029,13437030,13437031,13437032,13437033,13437034,13437035,13437036,13437037,
                13437038,13437039,13437040,13437041,13437042,13437043,13437044,13437045,13437046,13437047,13437048,13437049,13437056,13437057,13437058,13437059,
                13437060,13437061,13437062,13437063,13437064,13437065,13437066,13437067,13437068,13437069,13437070,13437071,13437072,13437073,13437074,13437075,
                13437076,13437077,13437078,13437079,13437080,13437081,13437082,13437083,13437084,13437085,13437086,13437087,13437088,13437089,13437090,13437091,
                13437092,13437093,13437094,13437095,13437096,13437097,13437098,13437099,13437100,13437101,13437102,13437103,13437104,13437105,13437106,13437107,
                13437108,13437109,13437110,13437111,13437112,13437113,13437114,13437115,13437116,13437117,13437118,13437119,13437120,13437121,13437122,13437123,
                13437124,13437125,13437126,13437127,13437128,13437129,13437130,13437131,13437132,13437133,13437134,13437135,13437136,13437137,13437138,13437139,
                13437140,13437141,13437142,13437143,13437144,13437145,13437146,13437147,13437148,13437149,13437150,13437151,13437152,13437153,13437154,13437155,
                13437156,13437157,13437158,13437159,13437160,13437161,13437162,13437163,13437164,13437165,13437166,13437167,13437168,13437169,13437176,13437177,
                13437178,13437179,13437180,13437181,13437184,13437185,13437186,13437188,13437189,13437192,13437193,13437194,13437195,13437196,13437197,13437198,
                13437199,13437200,13437201,13437202,13437203,13437204,13437205,13437206,13437207,13437248,13437249,13437250,13437251,13437252,13437253,13437254,
                13437255
            };

            return indexes;
        }

    private:
        Alarms(unsigned int idx) : Tongue(idx) {}
    };
}

