#pragma once

#include "tongue.hpp"

namespace WarGrey::SCADA {
    private class Alarms : public WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms> {
        friend class WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>;
    public:
        static Platform::String^ type() { return "alarm"; }

    public:
        static WarGrey::SCADA::Alarms* Alarm262359() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262359); }//Һѹ��ϵͳ����Һλ��L1
        static WarGrey::SCADA::Alarms* Alarm262360() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262360); }//Һѹ��ϵͳ����Һλ��L2
        static WarGrey::SCADA::Alarms* Alarm262361() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262361); }//Һѹ��ϵͳ����Һλ����L3
        static WarGrey::SCADA::Alarms* Alarm262362() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262362); }//Һѹ��ϵͳ�������¸�T1
        static WarGrey::SCADA::Alarms* Alarm262363() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262363); }//Һѹ��ϵͳ�������µ�T2
        static WarGrey::SCADA::Alarms* Alarm262372() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262372); }//�Ҵ�Һѹϵͳ����Һλ��
        static WarGrey::SCADA::Alarms* Alarm262373() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262373); }//�Ҵ�Һѹϵͳ����Һλ����
        static WarGrey::SCADA::Alarms* Alarm262374() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262374); }//�Ҵ�Һѹϵͳ�������¸�T1
        static WarGrey::SCADA::Alarms* Alarm262375() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262375); }//�Ҵ�Һѹϵͳ�������µ�T2
        static WarGrey::SCADA::Alarms* Alarm262410() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262410); }//Һѹ������������Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262414() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262414); }//�Ҵ�������������Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262418() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262418); }//���ϰ�ͷҺѹ��A1���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262422() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262422); }//���ϰ���Һѹ��B1���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262426() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262426); }//���ϰ���Һѹ��B2���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262430() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262430); }//�������Һѹ��C1���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262434() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262434); }//���ϰ�ͷҺѹ��A2���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262438() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262438); }//���ϰ���Һѹ��B3���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262442() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262442); }//���ϰ���Һѹ��B4���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262446() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262446); }//�������Һѹ��C2���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262450() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262450); }//�������Ʊ�D1 ���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262454() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262454); }//�������D2���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262458() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262458); }//Ӧ��Һѹ��E���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262462() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262462); }//Һѹ��ȴ��F1���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262466() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262466); }//Һѹ��ȴ��F2���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262470() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262470); }//���ϰҴ�Һѹ��J���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262474() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262474); }//���ϰҴ�Һѹ��I���Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262478() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262478); }//1#բ����ϴ�ù��Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262482() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262482); }//2#բ����ϴ�ù��Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262492() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262492); }//��������1#��ˮ�ñ�Ƶ����Ƶ�����Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262493() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262493); }//��������1#��ˮ�ñ�Ƶ����������ط���
        static WarGrey::SCADA::Alarms* Alarm262500() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262500); }//��������2#��ˮ�ñ�Ƶ����Ƶ�����Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262501() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262501); }//��������2#��ˮ�ñ�Ƶ����������ط���
        static WarGrey::SCADA::Alarms* Alarm262506() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262506); }//��������3#��ˮ�ñ�Ƶ����Ƶ�����Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262507() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262507); }//��������3#��ˮ�ñ�Ƶ����������ط���
        static WarGrey::SCADA::Alarms* Alarm262513() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262513); }//�Ҳ������1#��ˮ�ñ�Ƶ����Ƶ�����Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262514() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262514); }//�Ҳ������1#��ˮ�ñ�Ƶ����������ط���
        static WarGrey::SCADA::Alarms* Alarm262519() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262519); }//�Ҳ������2#��ˮ�ñ�Ƶ����Ƶ�����Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262520() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262520); }//�Ҳ������2#��ˮ�ñ�Ƶ����������ط���
        static WarGrey::SCADA::Alarms* Alarm262525() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262525); }//�Ҳ������3#��ˮ�ñ�Ƶ����Ƶ�����Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262526() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262526); }//�Ҳ������3#��ˮ�ñ�Ƶ����������ط���
        static WarGrey::SCADA::Alarms* Alarm262529() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262529); }//����ڱó����䱸�ñù���
        static WarGrey::SCADA::Alarms* Alarm262530() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262530); }//����ڱó�������λ����
        static WarGrey::SCADA::Alarms* Alarm262531() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262531); }//����ڱó�������������
        static WarGrey::SCADA::Alarms* Alarm262532() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262532); }//����ڱó���������ѹ��75%
        static WarGrey::SCADA::Alarms* Alarm262535() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262535); }//����ڱó���������ѹ��100%
        static WarGrey::SCADA::Alarms* Alarm262537() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262537); }//�Ҳ��ڱó����䱸�ñù���
        static WarGrey::SCADA::Alarms* Alarm262538() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262538); }//�Ҳ��ڱó�������λ����
        static WarGrey::SCADA::Alarms* Alarm262539() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262539); }//�Ҳ��ڱó�������������
        static WarGrey::SCADA::Alarms* Alarm262540() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262540); }//�Ҳ��ڱó���������ѹ��75%
        static WarGrey::SCADA::Alarms* Alarm262543() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262543); }//�Ҳ��ڱó���������ѹ��100%
        static WarGrey::SCADA::Alarms* Alarm262546() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262546); }//���迣�����豸UPS��ص�ѹ
        static WarGrey::SCADA::Alarms* Alarm262547() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262547); }//���迣�����豸UPSUPS��·
        static WarGrey::SCADA::Alarms* Alarm262548() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262548); }//���迣�����豸UPS��ر���
        static WarGrey::SCADA::Alarms* Alarm262549() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262549); }//���迣�����豸UPS���ȹ���
        static WarGrey::SCADA::Alarms* Alarm262550() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262550); }//���迣�����豸UPS�ӵع���
        static WarGrey::SCADA::Alarms* Alarm262551() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262551); }//���迣�����豸UPS����Դ����
        static WarGrey::SCADA::Alarms* Alarm262554() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262554); }//���迣�����豸UPS��ص�ѹ
        static WarGrey::SCADA::Alarms* Alarm262555() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262555); }//���迣�����豸UPSUPS��·
        static WarGrey::SCADA::Alarms* Alarm262556() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262556); }//���迣�����豸UPS��ر���
        static WarGrey::SCADA::Alarms* Alarm262557() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262557); }//���迣�����豸UPS���ȹ���
        static WarGrey::SCADA::Alarms* Alarm262558() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262558); }//���迣�����豸UPS�ӵع���
        static WarGrey::SCADA::Alarms* Alarm262559() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262559); }//���迣�����豸UPS����Դ����
        static WarGrey::SCADA::Alarms* Alarm262560() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262560); }//���ѹ��ˮ�ñ�Ƶ��UPS��Դ����
        static WarGrey::SCADA::Alarms* Alarm262561() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262561); }//���ѹ��ˮ�ñ�Ƶ�����ȹ���
        static WarGrey::SCADA::Alarms* Alarm262564() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262564); }//�Ҹ�ѹ��ˮ�ñ�Ƶ��UPS��Դ����
        static WarGrey::SCADA::Alarms* Alarm262565() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262565); }//�Ҹ�ѹ��ˮ�ñ�Ƶ�����ȹ���
        static WarGrey::SCADA::Alarms* Alarm262572() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262572); }//���ϲ����������󻬵�Ԫ��������
        static WarGrey::SCADA::Alarms* Alarm262573() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262573); }//���ϲ����������󻬵�Ԫ����ָʾ
        static WarGrey::SCADA::Alarms* Alarm262574() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262574); }//���ϲ����������󻬵�ԪҺλ��
        static WarGrey::SCADA::Alarms* Alarm262580() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262580); }//���ϲ����������󻬵�Ԫ��������
        static WarGrey::SCADA::Alarms* Alarm262581() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262581); }//���ϲ����������󻬵�Ԫ����ָʾ
        static WarGrey::SCADA::Alarms* Alarm262582() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262582); }//���ϲ����������󻬵�ԪҺλ��
        static WarGrey::SCADA::Alarms* Alarm262595() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262595); }//����ˮ�±�1#��ˮ�ù��Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262599() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262599); }//����ˮ�±�2#��ˮ�ù��Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262601() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262601); }//����ˮ�±�ϵͳ�ۺϱ���
        static WarGrey::SCADA::Alarms* Alarm262602() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262602); }//����ˮ�±�ˮ�µ�������¶ȱ���
        static WarGrey::SCADA::Alarms* Alarm262603() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262603); }//����ˮ�±�ˮ�µ������¶ȱ���
        static WarGrey::SCADA::Alarms* Alarm262604() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262604); }//����ˮ�±�ˮ�µ��©ˮ����
        static WarGrey::SCADA::Alarms* Alarm262605() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262605); }//����ˮ�±�ˮ�µ��ѹ���
        static WarGrey::SCADA::Alarms* Alarm262606() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262606); }//����ˮ�±�ˮ�µ����ϵͳ����
        static WarGrey::SCADA::Alarms* Alarm262607() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262607); }//����ˮ�±�ˮ�µ������ϵͳ����
        static WarGrey::SCADA::Alarms* Alarm262611() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262611); }//����ˮ�±�1#��ˮ�ù��Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262615() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262615); }//����ˮ�±�2#��ˮ�ù��Ϸ���
        static WarGrey::SCADA::Alarms* Alarm262617() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262617); }//����ˮ�±�ϵͳ�ۺϱ���
        static WarGrey::SCADA::Alarms* Alarm262618() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262618); }//����ˮ�±�ˮ�µ�������¶ȱ���
        static WarGrey::SCADA::Alarms* Alarm262619() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262619); }//����ˮ�±�ˮ�µ������¶ȱ���
        static WarGrey::SCADA::Alarms* Alarm262620() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262620); }//����ˮ�±�ˮ�µ��©ˮ����
        static WarGrey::SCADA::Alarms* Alarm262621() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262621); }//����ˮ�±�ˮ�µ��ѹ���
        static WarGrey::SCADA::Alarms* Alarm262622() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262622); }//����ˮ�±�ˮ�µ����ϵͳ����
        static WarGrey::SCADA::Alarms* Alarm262623() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(262623); }//����ˮ�±�ˮ�µ������ϵͳ����
        static WarGrey::SCADA::Alarms* Alarm13436300() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436300); }//����нǶȹ���0
        static WarGrey::SCADA::Alarms* Alarm13436301() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436301); }//�Ұ��нǶȹ���0
        static WarGrey::SCADA::Alarms* Alarm13436689() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436689); }//����ѹ��С��3bar,���нʳ����ܶ���0
        static WarGrey::SCADA::Alarms* Alarm13436936() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436936); }//5#վET200M-��PLCͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436937() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436937); }//6#վET200M-��PLC���1��ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436938() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436938); }//7#վET200M-��PLC���2��-1ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436939() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436939); }//8#վET200M-��PLC���2��-2ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436940() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436940); }//9#վET200M-��PLC���3��ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436941() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436941); }//10#վET200M-��PLC���4��-1ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436942() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436942); }//11#վET200M-��PLC���4��-2ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436943() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436943); }//12#վET200M-���迣̨ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436944() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436944); }//13#վET201M-���迣̨ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436945() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436945); }//21#վ-����ܽʳ�������ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436946() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436946); }//22#վ-����нʳ�������ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436947() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436947); }//23#վ-���ͷ�ʳ�������ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436948() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436948); }//24#վ-����ܽʳ�������ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436949() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436949); }//25#վ-�Ұ��нʳ�������ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436950() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436950); }//26#վ-�Ұ�ͷ�ʳ�������ͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436951() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436951); }//27#վ-PMS DP/DP COUPLERͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436952() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436952); }//31#վ-����ڱ�DP/DP COUPLERͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436953() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436953); }//32#վ-���ѹ��ˮ��DP/DP COUPLERͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436954() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436954); }//33#վ-�Ҳ��ڱ�DP/DP COUPLERͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436955() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436955); }//34#վ-�Ҹ�ѹ��ˮ��DP/DP COUPLERͨѶ����
        static WarGrey::SCADA::Alarms* Alarm13436959() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436959); }//PLC��Դģ���ع���0
        static WarGrey::SCADA::Alarms* Alarm13436967() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436967); }//����÷�ˮ��ѹ���쳣0
        static WarGrey::SCADA::Alarms* Alarm13436968() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436968); }//����÷�ˮ�������쳣0
        static WarGrey::SCADA::Alarms* Alarm13436969() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436969); }//����÷�ˮ��ѹ���쳣0
        static WarGrey::SCADA::Alarms* Alarm13436970() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436970); }//����÷�ˮ�������쳣0
        static WarGrey::SCADA::Alarms* Alarm13436971() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436971); }//����ڱó������¶ȸ߱�����>58�㣩0
        static WarGrey::SCADA::Alarms* Alarm13436972() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436972); }//����ڱó�����ѹ���ͱ���(<0.4bar)0
        static WarGrey::SCADA::Alarms* Alarm13436973() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436973); }//�Ҳ��ڱó������¶ȸ߱�����>58�㣩0
        static WarGrey::SCADA::Alarms* Alarm13436974() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13436974); }//�Ҳ��ڱó�����ѹ���ͱ���(<0.4bar)0
        static WarGrey::SCADA::Alarms* Alarm13437016() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437016); }//����1#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437017() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437017); }//����2#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437018() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437018); }//����3#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437019() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437019); }//����4#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437020() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437020); }//����5#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437021() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437021); }//����6#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437022() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437022); }//����7#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437023() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437023); }//����1#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437024() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437024); }//����2#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437025() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437025); }//����3#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437026() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437026); }//����4#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437027() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437027); }//����5#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437028() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437028); }//����6#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437029() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437029); }//����7#�������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437030() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437030); }//�����ˮբ���г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437031() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437031); }//��������ˮ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437032() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437032); }//���ϲ��˲�����������ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437033() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437033); }//���˲������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437034() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437034); }//��������ˮ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437035() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437035); }//���ϲ��˲�����������ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437036() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437036); }//�Ҳ��˲��������̰ң��г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437037() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437037); }//�Ҳ��˲����������ң��г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437038() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437038); }//���ϰ�ͷ�ʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437039() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437039); }//���ϰ�ͷ�ʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437040() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437040); }//���ϰ��нʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437041() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437041); }//���ϰ��нʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437042() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437042); }//������ܽʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437043() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437043); }//������ܽʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437044() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437044); }//���ϰ�ͷ�ʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437045() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437045); }//���ϰ�ͷ�ʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437046() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437046); }//���ϰ��нʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437047() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437047); }//���ϰ��нʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437048() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437048); }//������ܽʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437049() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437049); }//������ܽʳ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437056() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437056); }//��1#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437057() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437057); }//��2#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437058() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437058); }//��3#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437059() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437059); }//��4#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437060() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437060); }//��5#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437061() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437061); }//��6#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437062() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437062); }//��7#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437063() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437063); }//��1#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437064() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437064); }//��2#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437065() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437065); }//��3#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437066() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437066); }//��4#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437067() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437067); }//��5#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437068() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437068); }//��6#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437069() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437069); }//��7#������г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437070() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437070); }//���������Һλ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437071() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437071); }//���������Һλ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437072() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437072); }//���������Һλ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437073() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437073); }//���������Һλ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437074() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437074); }//���ϲ��ڱ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437075() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437075); }//���ϲ��ڱ��ų�ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437076() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437076); }//���ϲ��ڱ�����˷�ˮѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437077() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437077); }//���ϲ��ڱ���˷�ˮѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437078() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437078); }//���ϲ��ڱ�ɰˮ����������ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437079() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437079); }//�Ҹ�ѹ��ˮ�ó���ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437080() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437080); }//��������ˮ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437081() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437081); }//�������ڳ�ˮ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437082() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437082); }//�����ڳ���ѹ�� ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437083() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437083); }//���ϲ��ڱ�����ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437084() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437084); }//���ϲ��ڱ��ų�ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437085() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437085); }//���ϲ��ڱ�����˷�ˮѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437086() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437086); }//���ϲ��ڱ���˷�ˮѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437087() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437087); }//���ϲ��ڱ�ɰˮ����������ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437088() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437088); }//���ѹ��ˮ�ó���ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437089() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437089); }//��������ˮ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437090() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437090); }//�������ڳ�ˮ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437091() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437091); }//�����ڳ���ѹ�� ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437092() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437092); }//���ϲ��ڱ�����˷�ˮ�������߱���
        static WarGrey::SCADA::Alarms* Alarm13437093() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437093); }//���ϲ��ڱ���˷�ˮ�������߱���
        static WarGrey::SCADA::Alarms* Alarm13437094() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437094); }//���ϲ��ڱ�����˷�ˮ�������߱���
        static WarGrey::SCADA::Alarms* Alarm13437095() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437095); }//���ϲ��ڱ���˷�ˮ�������߱���
        static WarGrey::SCADA::Alarms* Alarm13437096() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437096); }//���ѹ��ˮ���������߱���
        static WarGrey::SCADA::Alarms* Alarm13437097() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437097); }//�Ҹ�ѹ��ˮ���������߱���
        static WarGrey::SCADA::Alarms* Alarm13437098() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437098); }//��������բ���г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437099() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437099); }//��������բ���г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437100() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437100); }//��������Ͳ�г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437101() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437101); }//��������Ͳ�г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437102() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437102); }//���š�բ��ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437103() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437103); }//���ͱ�ѹѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437104() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437104); }//���ű�ѹ����ѹ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437105() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437105); }//��������ѹ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437106() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437106); }//Һѹ���������¶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437107() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437107); }//���ͷ�ͱ���ѹ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437108() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437108); }//������ͱ���ѹ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437109() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437109); }//������ͱ���ѹ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437110() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437110); }//�Ұ�ͷ�ͱ���ѹ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437111() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437111); }//�Ұ����ͱ���ѹ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437112() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437112); }//������ͱ���ѹ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437113() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437113); }//��Ҵ��ͱ���ѹ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437114() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437114); }//�ҰҴ��ͱ���ѹ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437115() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437115); }//�Ҵ��������¶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437116() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437116); }//�����ϴ�ֱ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437117() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437117); }//������ˮƽ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437118() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437118); }//�����´�ֱ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437119() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437119); }//������ˮƽ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437120() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437120); }//���ϰҹ�����1 ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437121() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437121); }//���ϰҹ�����2 ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437122() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437122); }//���ϰ�ͷ�г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437123() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437123); }//���ϰ�ͷѹ����߱���
        static WarGrey::SCADA::Alarms* Alarm13437124() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437124); }//��ˮ�±�������ն��߱���
        static WarGrey::SCADA::Alarms* Alarm13437125() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437125); }//����ˮ�±÷�ˮѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437126() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437126); }//����ˮ�±��ų�ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437127() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437127); }//���ö��߱���
        static WarGrey::SCADA::Alarms* Alarm13437128() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437128); }//�����ϴ�ֱ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437129() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437129); }//������ˮƽ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437130() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437130); }//�����´�ֱ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437131() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437131); }//������ˮƽ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437132() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437132); }//���ϰҹ�����1 ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437133() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437133); }//���ϰҹ�����2 ���߱���
        static WarGrey::SCADA::Alarms* Alarm13437134() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437134); }//���ϰ�ͷ�г̶��߱���
        static WarGrey::SCADA::Alarms* Alarm13437135() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437135); }//���ϰ�ͷѹ����߱���
        static WarGrey::SCADA::Alarms* Alarm13437136() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437136); }//��ˮ�±�������ն��߱���
        static WarGrey::SCADA::Alarms* Alarm13437137() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437137); }//����ˮ�±÷�ˮѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437138() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437138); }//����ˮ�±��ų�ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437139() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437139); }//���ö��߱���
        static WarGrey::SCADA::Alarms* Alarm13437140() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437140); }//��������1#��ˮ�õ��ת�ٶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437141() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437141); }//��������1#��ˮ�õ���������߱���
        static WarGrey::SCADA::Alarms* Alarm13437142() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437142); }//��������2#��ˮ�õ��ת�ٶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437143() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437143); }//��������2#��ˮ�õ���������߱���
        static WarGrey::SCADA::Alarms* Alarm13437144() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437144); }//��������3#��ˮ�õ��ת�ٶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437145() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437145); }//��������3#��ˮ�õ���������߱���
        static WarGrey::SCADA::Alarms* Alarm13437146() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437146); }//�Ҳ������1#��ˮ�õ��ת�ٶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437147() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437147); }//�Ҳ������1#��ˮ�õ���������߱���
        static WarGrey::SCADA::Alarms* Alarm13437148() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437148); }//�Ҳ������2#��ˮ�õ��ת�ٶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437149() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437149); }//�Ҳ������2#��ˮ�õ���������߱���
        static WarGrey::SCADA::Alarms* Alarm13437150() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437150); }//�Ҳ������3#��ˮ�õ��ת�ٶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437151() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437151); }//�Ҳ������3#��ˮ�õ���������߱���
        static WarGrey::SCADA::Alarms* Alarm13437152() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437152); }//��������1#��ˮ�õ������U�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437153() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437153); }//��������1#��ˮ�õ������V�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437154() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437154); }//��������1#��ˮ�õ������W�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437155() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437155); }//��������2#��ˮ�õ������U�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437156() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437156); }//��������2#��ˮ�õ������V�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437157() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437157); }//��������2#��ˮ�õ������W�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437158() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437158); }//��������3#��ˮ�õ������U�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437159() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437159); }//��������3#��ˮ�õ������V�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437160() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437160); }//��������3#��ˮ�õ������W�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437161() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437161); }//�Ҳ������1#��ˮ�õ������U�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437162() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437162); }//�Ҳ������1#��ˮ�õ������V�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437163() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437163); }//�Ҳ������1#��ˮ�õ������W�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437164() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437164); }//�Ҳ������2#��ˮ�õ������U�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437165() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437165); }//�Ҳ������2#��ˮ�õ������V�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437166() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437166); }//�Ҳ������2#��ˮ�õ������W�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437167() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437167); }//�Ҳ������3#��ˮ�õ������U�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437168() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437168); }//�Ҳ������3#��ˮ�õ������V�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437169() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437169); }//�Ҳ������3#��ˮ�õ������W�¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437176() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437176); }//����ó�����������¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437177() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437177); }//����ó����������ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437178() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437178); }//����ó�����������¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437179() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437179); }//����ó����������ѹ�����߱���
        static WarGrey::SCADA::Alarms* Alarm13437180() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437180); }//���ϲ����������󻬵�Ԫ�����¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437181() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437181); }//���ϲ����������󻬵�Ԫ�����¶ȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437184() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437184); }//������ܽʳ�ң���ٶ��趨���߱���
        static WarGrey::SCADA::Alarms* Alarm13437185() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437185); }//���ϰ��нʳ�ң���ٶ��趨���߱���
        static WarGrey::SCADA::Alarms* Alarm13437186() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437186); }//���ϰ�ͷ�ʳ�ң���ٶ��趨���߱���
        static WarGrey::SCADA::Alarms* Alarm13437188() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437188); }//���ϲ��ڱ�/ˮ�±õ��ң���ٶ��趨���߱���
        static WarGrey::SCADA::Alarms* Alarm13437189() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437189); }//�Ҹ�ѹ��ˮ�õ��ң���ٶ��趨���߱���
        static WarGrey::SCADA::Alarms* Alarm13437192() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437192); }//������ܽʳ�ң���ٶ��趨���߱���
        static WarGrey::SCADA::Alarms* Alarm13437193() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437193); }//���ϰ�ͷ�ʳ�ң���ٶ��趨���߱���
        static WarGrey::SCADA::Alarms* Alarm13437194() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437194); }//���ϰ��нʳ�ң���ٶ��趨���߱���
        static WarGrey::SCADA::Alarms* Alarm13437195() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437195); }//���ö��߱���
        static WarGrey::SCADA::Alarms* Alarm13437196() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437196); }//���ϲ��ڱ�/ˮ�±õ��ң���ٶ��趨���߱���
        static WarGrey::SCADA::Alarms* Alarm13437197() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437197); }//���ѹ��ˮ�õ��ң���ٶ��趨���߱���
        static WarGrey::SCADA::Alarms* Alarm13437198() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437198); }//�迣̨��ʾ��������߱���
        static WarGrey::SCADA::Alarms* Alarm13437199() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437199); }//����̨��ʾ��������߱���
        static WarGrey::SCADA::Alarms* Alarm13437200() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437200); }//����ˮ�±��ܶȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437201() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437201); }//����ˮ�±����ٶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437202() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437202); }//����ˮ�±��ܶȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437203() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437203); }//����ˮ�±����ٶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437204() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437204); }//���ϲ��ڱ��ܶȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437205() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437205); }//���ϲ��ڱ����ٶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437206() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437206); }//���ϲ��ڱ��ܶȶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437207() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437207); }//���ϲ��ڱ����ٶ��߱���
        static WarGrey::SCADA::Alarms* Alarm13437248() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437248); }//����ϰҹܽǶȹ��󱨾�0
        static WarGrey::SCADA::Alarms* Alarm13437249() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437249); }//����°ҹܽǶȹ��󱨾�0
        static WarGrey::SCADA::Alarms* Alarm13437250() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437250); }//������°ҹܼнǽǶȹ��󱨾�0
        static WarGrey::SCADA::Alarms* Alarm13437251() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437251); }//�����ܸ�˿���·ų��ȹ��󱨾�0
        static WarGrey::SCADA::Alarms* Alarm13437252() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437252); }//�Ұ��ϰҹܽǶȹ��󱨾�0
        static WarGrey::SCADA::Alarms* Alarm13437253() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437253); }//�Ұ��°ҹܽǶȹ��󱨾�0
        static WarGrey::SCADA::Alarms* Alarm13437254() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437254); }//�Ұ����°ҹܼнǽǶȹ��󱨾�0
        static WarGrey::SCADA::Alarms* Alarm13437255() { return WarGrey::SCADA::Tongue<WarGrey::SCADA::Alarms>::UnsafeTongue(13437255); }//�Ұ���ܸ�˿���·ų��ȹ��󱨾�0

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

