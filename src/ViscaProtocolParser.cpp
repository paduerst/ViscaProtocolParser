#include "ViscaProtocolParser.h"
#include "ViscaProtocolParserVersion.h"
#include <string.h>


cr::visca::ViscaProtocolParser::ViscaProtocolParser() :
    m_inputDataCounter(0),
    m_inputPacketType(cr::visca::ViscaPackets::UNDEFINED_PACKET),
    m_lastEncodedCommand(cr::visca::ViscaPackets::UNDEFINED_PACKET)
{
    memset(m_inputPacketData, 0, 128);
}


cr::visca::ViscaProtocolParser::~ViscaProtocolParser()
{
    
}


bool cr::visca::ViscaProtocolParser::encodeCommand(
            cr::visca::ViscaPackets commandId,
            uint8_t* packet,
            uint32_t& packetSize,
            uint32_t cameraAddress,
            uint32_t param1,
            uint32_t param2,
            uint32_t param3,
            uint32_t param4,
            uint32_t param5,
            uint32_t param6,
            uint32_t param7,
            uint32_t param8,
            uint32_t param9,
            uint32_t param10)
{
    // Check camera address.
    if (cameraAddress < 1 || cameraAddress > 7)
        return false;

    // Remember last encoded command for REPLY packets decoding.
    m_lastEncodedCommand = commandId;

    // Check command ID.
    switch (commandId)
    {
    case cr::visca::ViscaPackets::COMMAND_AddressSet:
        packet[0] = 0x88;
        packet[1] = 0x30;
        packet[2] = (uint8_t)cameraAddress;
        packet[3] = 0xFF;
        packetSize = 4;
        return true;

    case cr::visca::ViscaPackets::COMMAND_IF_Clear_Broadcast:
        packet[0] = 0x88;
        packet[1] = 0x01;
        packet[2] = 0x00;
        packet[3] = 0x01;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::COMMAND_IF_Clear:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x00;
        packet[3] = 0x01;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CommandCancel:
        // Check socet number.
        if (param1 != 1 && param1 != 2)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = (0x20 | (uint8_t)param1);
        packet[2] = 0xFF;
        packetSize = 3;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Power_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x00;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Power_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x00;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Zoom_Stop:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x07;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Zoom_Tele:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x07;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Zoom_Wide:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x07;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Zoom_Tele_Variable:
        // Check speed value.
        if (param1 > 7)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x07;
        packet[4] = (0x20 | (uint8_t)param1);
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Zoom_Wide_Variable:
        // Check speed value.
        if (param1 > 7)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x07;
        packet[4] = (0x30 | (uint8_t)param1);
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Zoom_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x47;
        packet[4] = (uint8_t)(param1 >> 12) & 0x0F;
        packet[5] = (uint8_t)(param1 >> 8) & 0x0F;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_DZoom_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x06;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_DZoom_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x06;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case visca::ViscaPackets::COMMAND_CAM_DZoom_Combine_Mode:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x36;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_DZoom_Separate_Mode:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x36;
        packet[4] = 0x01;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_DZoom_Stop:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x06;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_DZoom_Tele_Variable:
        // Check speed value.
        if (param1 > 7)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x06;
        packet[4] = (0x20 | (uint8_t)param1);
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_DZoom_Wide_Variable:
        // Check speed value.
        if (param1 > 7)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x06;
        packet[4] = (0x30 | (uint8_t)param1);
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_DZoom_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x46;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Stop:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x08;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Far:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x08;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Near:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x08;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Far_Variable:
        // Check speed value.
        if (param1 > 7)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x08;
        packet[4] = (0x20 | (uint8_t)param1);
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Near_Variable:
        // Check speed value.
        if (param1 > 7)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x08;
        packet[4] = (0x30 | (uint8_t)param1);
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x48;
        packet[4] = (uint8_t)(param1 >> 12) & 0x0F;
        packet[5] = (uint8_t)(param1 >> 8) & 0x0F;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Auto_Focus_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x38;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Manual_Focus_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x38;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Auto_Manual:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x38;
        packet[4] = 0x10;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_One_Push_Trigger:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x18;
        packet[4] = 0x01;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Full_Scan_One_Push_Trigger:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x18;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Focus_Near_Limit:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x28;
        packet[4] = (uint8_t)(param1 >> 12) & 0x0F;
        packet[5] = (uint8_t)(param1 >> 8) & 0x0F;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AFSensitivity_Normal:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x58;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AFSensitivity_Low:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x58;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AFMode_Normal_AF:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x57;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AFMode_Interval_AF:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x57;
        packet[4] = 0x01;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AFMode_Zoom_Trigger_AF:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x57;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AFMode_Active_or_Interval_Time:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x27;
        packet[4] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[5] = (uint8_t)param1 & 0x0F;
        packet[6] = (uint8_t)(param2 >> 4) & 0x0F;
        packet[7] = (uint8_t)param2 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_IRCorrection_Standard:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x11;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_IRCorrection_IR_Light:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x11;
        packet[4] = 0x01;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ZoomFocus_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x47;
        packet[4] = (uint8_t)(param1 >> 12) & 0x0F;
        packet[5] = (uint8_t)(param1 >> 8) & 0x0F;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = (uint8_t)(param2 >> 12) & 0x0F;
        packet[9] = (uint8_t)(param2 >> 8) & 0x0F;
        packet[10] = (uint8_t)(param2 >> 4) & 0x0F;
        packet[11] = (uint8_t)param2 & 0x0F;
        packet[12] = 0xFF;
        packetSize = 13;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Initialize_Lens:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x19;
        packet[4] = 0x01;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Initialize_Camera:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x19;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_Auto:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_Indoor:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0x01;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_Outdoor:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_One_Push_WB:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_ATW:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0x04;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_Manual:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0x05;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_One_Push_Trigger:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x10;
        packet[4] = 0x05;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_Outdoor_Auto:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0x06;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_Sodium_Lamp_Auto:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0x07;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_Sodium_Lamp:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0x08;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WB_Sodium_Lamp_Outdoor_Auto:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0x09;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_RGain_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x03;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_RGain_Up:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x03;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_RGain_Down:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x03;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_RGain_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x43;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_BGain_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x04;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_BGain_Up:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x04;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_BGain_Down:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x04;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_BGain_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x44;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AE_Full_Auto:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x39;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AE_Manual:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x39;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AE_Shutter_Priority:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x39;
        packet[4] = 0x0A;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AE_Iris_Priority:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x39;
        packet[4] = 0x0B;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AutoSlowShutter_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x5A;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AutoSlowShutter_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x5A;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Shutter_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0A;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Shutter_Up:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0A;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Shutter_Down:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0A;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Shutter_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x4A;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Iris_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0B;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Iris_Up:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0B;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Iris_Down:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0B;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Iris_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x4B;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Gain_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0C;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Gain_Up:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0C;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Gain_Down:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0C;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Gain_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x4C;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Gain_Limit:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x2C;
        packet[4] = (uint8_t)param1 & 0x0F;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExpComp_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3E;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case visca::ViscaPackets::COMMAND_CAM_ExpComp__Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3E;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExpComp_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0E;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExpComp_Up:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0E;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExpComp_Down:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x0E;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExpComp_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x4E;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_BackLight_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x33;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_BackLight_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x33;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_SpotAE_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x59;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_SpotAE_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x59;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_SpotAE_Position:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x29;
        packet[4] = (uint8_t)(param1 >> 12) & 0x0F;
        packet[5] = (uint8_t)(param1 >> 8) & 0x0F;
        packet[6] = (uint8_t)(param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AE_Response_Direct:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x5D;
        packet[4] = (uint8_t)param1;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_VE_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3D;
        packet[4] = 0x06;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_VE_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3D;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_VE_Set_Parameter:
        // Check Display brightness level.
        if (param1 > 6)
            return false;
        // Check Brightness compensation selection.
        if (param2 > 3)
            return false;
        // Check Compensation leve.
        if (param3 > 2)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x2D;
        packet[4] = 00;
        packet[5] = (uint8_t)param1;
        packet[6] = (uint8_t)param2;
        packet[7] = (uint8_t)param3;
        packet[8] = 0x00;
        packet[9] = 0x00;
        packet[10] = 0x00;
        packet[11] = 0x00;
        packet[12] = 0xFF;
        packetSize = 13;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WD_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3D;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WD_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3D;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_WD_Set_Parameter:
        // Check Display brightness level.
        if (param1 > 6)
            return false;
        // Check Brightness compensation selection.
        if (param2 > 3)
            return false;
        // Check Compensation level.
        if (param3 > 2)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x2D;
        packet[4] = 00;
        packet[5] = (uint8_t)param1;
        packet[6] = (uint8_t)param2;
        packet[7] = (uint8_t)param3;
        packet[8] = 0x00;
        packet[9] = 0x00;
        packet[10] = 0x00;
        packet[11] = 0x00;
        packet[12] = 0xFF;
        packetSize = 13;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Defog_On:
        // Check defog level.
        if (param1 < 1 || param1 > 3)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x37;
        packet[4] = 0x02;
        packet[5] = (uint8_t)param1;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Defog_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x37;
        packet[4] = 0x03;
        packet[5] = 0x00;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Aperture_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x02;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Aperture_Up:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x02;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Aperture_Down:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x02;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Aperture_Direct:
        // Check aperture gain.
        if (param1 > 0x0F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x42;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = (uint8_t)param1;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_NR_Noise_Reduction:
        // Check setting.
        if (param1 != 0x00)
        {
            if (param1 != 0x7F)
            {
                if (param1 < 1 || param1 > 5)
                {
                    return false;
                }
            }
        }
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x53;
        packet[4] = (uint8_t)param1;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_NR_2D_3D_Independent_Setting:
        // Check setting.
        if (param1 > 5)
            return false;
        if (param2 > 5)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x53;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Gamma:
        // Check setting.
        if (param1 != 0 && param1 != 1)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x5B;
        packet[4] = (uint8_t)param1;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_HighSensitivity_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x5E;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_HighSensitivity_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x5E;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_LR_Reverse_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x61;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_LR_Reverse_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x61;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Freeze_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x62;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Freeze_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x62;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PictureEffect_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x63;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PictureEffect_Black_and_White:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x63;
        packet[4] = 0x04;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MinShutter_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x12;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MinShutter_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x12;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MinShutter_Limit:
        // Check setting.
        if (param1 < 0x05 || param1 > 0x14)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x13;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PictureFlip_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x66;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PictureFlip_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x66;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ICR_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x01;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ICR_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x01;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AutoICR_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x51;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AutoICR_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x51;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AutoICR_Threshold:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x21;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AutoICRAlarmReply_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x31;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_AutoICRAlarmReply_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x31;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Stabilizer_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x34;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Stabilizer_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x34;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Stabilizer_Hold:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x34;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Memory_Reset:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3F;
        packet[4] = 0x00;
        packet[5] = (uint8_t)param1;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Memory_Set:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3F;
        packet[4] = 0x01;
        packet[5] = (uint8_t)param1;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Memory_Recall:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3F;
        packet[4] = 0x02;
        packet[5] = (uint8_t)param1;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Custom_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3F;
        packet[4] = 0x00;
        packet[5] = 0x7F;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Custom_Set:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3F;
        packet[4] = 0x01;
        packet[5] = 0x7F;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Custom_Recall:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x3F;
        packet[4] = 0x02;
        packet[5] = 0x7F;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MemSave_Write:
        // Check setting.
        if (param1 > 0x07)
            return false;
        if (param2 > 0xFFFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x23;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)((uint16_t)param2 >> 12) & 0x0F;
        packet[6] = (uint8_t)((uint16_t)param2 >> 8) & 0x0F;
        packet[7] = (uint8_t)((uint16_t)param2 >> 4) & 0x0F;
        packet[8] = (uint8_t)param2 & 0x0F;
        packet[9] = 0xFF;
        packetSize = 10;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_Up:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x01;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0x03;
        packet[7] = 0x01;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_Down:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x01;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0x03;
        packet[7] = 0x02;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_Left:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x01;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0x01;
        packet[7] = 0x03;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_Right:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x01;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0x02;
        packet[7] = 0x03;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_UpLeft:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x01;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0x01;
        packet[7] = 0x01;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_UpRight:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x01;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0x02;
        packet[7] = 0x01;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_DownLeft:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x01;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0x01;
        packet[7] = 0x02;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_DownRight:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x01;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0x02;
        packet[7] = 0x02;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_Stop:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x01;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0x03;
        packet[7] = 0x03;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_AbsolutePosition:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x02;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = (uint8_t)(param3 >> 12) & 0x0F;
        packet[7] = (uint8_t)(param3 >> 8) & 0x0F;
        packet[8] = (uint8_t)(param3 >> 4) & 0x0F;
        packet[9] = (uint8_t)param3 & 0x0F;
        packet[10] = (uint8_t)(param4 >> 12) & 0x0F;
        packet[11] = (uint8_t)(param4 >> 8) & 0x0F;
        packet[12] = (uint8_t)(param4 >> 4) & 0x0F;
        packet[13] = (uint8_t)param4 & 0x0F;
        packet[14] = 0xFF;
        packetSize = 15;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_RelativePosition:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x03;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = (uint8_t)(param3 >> 12) & 0x0F;
        packet[7] = (uint8_t)(param3 >> 8) & 0x0F;
        packet[8] = (uint8_t)(param3 >> 4) & 0x0F;
        packet[9] = (uint8_t)param3 & 0x0F;
        packet[10] = (uint8_t)(param4 >> 12) & 0x0F;
        packet[11] = (uint8_t)(param4 >> 8) & 0x0F;
        packet[12] = (uint8_t)(param4 >> 4) & 0x0F;
        packet[13] = (uint8_t)param4 & 0x0F;
        packet[14] = 0xFF;
        packetSize = 15;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_Home:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x04;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::COMMAND_PanTiltDrive_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x06;
        packet[3] = 0x05;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Display_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x15;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Display_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x15;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Display_On_Off_Switch:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x15;
        packet[4] = 0x10;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MultiLineTitle_Title_Set_1:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        if (param2 > 0xFF)
            return false;
        if (param3 > 0xFF)
            return false;
        if (param4 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x73;
        packet[4] = (uint8_t)param1 | 0x10;
        packet[5] = 0x00;
        packet[6] = (uint8_t)param2;
        packet[7] = (uint8_t)param3;
        packet[8] = (uint8_t)param4;
        packet[9] = 0x00;
        packet[10] = 0x00;
        packet[11] = 0x00;
        packet[12] = 0x00;
        packet[13] = 0x00;
        packet[14] = 0x00;
        packet[15] = 0xFF;
        packetSize = 16;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MultiLineTitle_Title_Set_2:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        if (param2 > 0xFF)
            return false;
        if (param3 > 0xFF)
            return false;
        if (param4 > 0xFF)
            return false;
        if (param5 > 0xFF)
            return false;
        if (param6 > 0xFF)
            return false;
        if (param7 > 0xFF)
            return false;
        if (param8 > 0xFF)
            return false;
        if (param9 > 0xFF)
            return false;
        if (param10 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x73;
        packet[4] = (uint8_t)param1 | 0x20;
        packet[5] = 0x00;
        packet[6] = (uint8_t)param2;
        packet[7] = (uint8_t)param3;
        packet[8] = (uint8_t)param4;
        packet[9] = (uint8_t)param5;
        packet[10] = (uint8_t)param6;
        packet[11] = (uint8_t)param7;
        packet[12] = (uint8_t)param8;
        packet[13] = (uint8_t)param9;
        packet[14] = (uint8_t)param10;
        packet[15] = 0xFF;
        packetSize = 16;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MultiLineTitle_Title_Set_3:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        if (param2 > 0xFF)
            return false;
        if (param3 > 0xFF)
            return false;
        if (param4 > 0xFF)
            return false;
        if (param5 > 0xFF)
            return false;
        if (param6 > 0xFF)
            return false;
        if (param7 > 0xFF)
            return false;
        if (param8 > 0xFF)
            return false;
        if (param9 > 0xFF)
            return false;
        if (param10 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x73;
        packet[4] = (uint8_t)param1 | 0x30;
        packet[5] = 0x00;
        packet[6] = (uint8_t)param2;
        packet[7] = (uint8_t)param3;
        packet[8] = (uint8_t)param4;
        packet[9] = (uint8_t)param5;
        packet[10] = (uint8_t)param6;
        packet[11] = (uint8_t)param7;
        packet[12] = (uint8_t)param8;
        packet[13] = (uint8_t)param9;
        packet[14] = (uint8_t)param10;
        packet[15] = 0xFF;
        packetSize = 16;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MultiLineTitle_Title_Clear:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x74;
        packet[4] = (uint8_t)param1 | 0x10;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MultiLineTitle_On:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x74;
        packet[4] = (uint8_t)param1 | 0x20;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MultiLineTitle_Off:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x74;
        packet[4] = (uint8_t)param1 | 0x30;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Mute_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x75;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Mute_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x75;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_Mute_On_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x75;
        packet[4] = 0x10;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PrivacyZone_SetMask:
        // Check setting.
        if (param1 > 0xFF)
            return false;
        if (param2 != 0x00 && param2 != 0x01)
            return false;
        if (param3 > 0xFF)
            return false;
        if (param4 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x76;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = ((uint8_t)param3 >> 4) & 0x0F;
        packet[7] = (uint8_t)param3 & 0x0F;
        packet[8] = ((uint8_t)param4 >> 4) & 0x0F;
        packet[9] = (uint8_t)param4 & 0x0F;
        packet[10] = 0xFF;
        packetSize = 11;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PrivacyZone_Display:
        // Check setting.
        if (param1 != 0x00 && param1 != 0x01)
            return false;
        if (param2 != 0x00 && param2 != 0x01)
            return false;
        if (param3 != 0x00 && param3 != 0x01)
            return false;
        if (param4 != 0x00 && param4 != 0x01)
            return false;
        if (param5 != 0x00 && param5 != 0x01)
            return false;
        if (param6 != 0x00 && param6 != 0x01)
            return false;
        if (param7 != 0x00 && param7 != 0x01)
            return false;
        if (param8 != 0x00 && param8 != 0x01)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x77;
        packet[4] = (((uint8_t)param1 << 4) & 0xF0) | param2;
        packet[5] = (((uint8_t)param3 << 4) & 0xF0) | param4;
        packet[6] = (((uint8_t)param5 << 4) & 0xF0) | param6;
        packet[7] = (((uint8_t)param7 << 4) & 0xF0) | param8;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PrivacyZone_SetMaskColor:
        // Check setting.
        if (param1 != 0x00 && param1 != 0x01)
            return false;
        if (param2 != 0x00 && param2 != 0x01)
            return false;
        if (param3 != 0x00 && param3 != 0x01)
            return false;
        if (param4 != 0x00 && param4 != 0x01)
            return false;
        if (param5 != 0x00 && param5 != 0x01)
            return false;
        if (param6 != 0x00 && param6 != 0x01)
            return false;
        if (param7 != 0x00 && param7 != 0x01)
            return false;
        if (param8 != 0x00 && param8 != 0x01)
            return false;
        if (param9 < 0x00 || param9 > 0xFF)
            return false;
        if (param10 < 0x00 || param10 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x78;
        packet[4] = (((uint8_t)param1 << 4) & 0xF0) | param2;
        packet[5] = (((uint8_t)param3 << 4) & 0xF0) | param4;
        packet[6] = (((uint8_t)param5 << 4) & 0xF0) | param6;
        packet[7] = (((uint8_t)param7 << 4) & 0xF0) | param8;
        packet[8] = (uint8_t)param9;
        packet[9] = (uint8_t)param10;
        packet[10] = 0xFF;
        packetSize = 11;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PrivacyZone_SetPanTiltAngle:
        // Check setting.
        if (param1 > 0xFFF)
            return false;
        if (param2 > 0xFFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x79;
        packet[4] = (uint8_t)(((uint16_t)param1 >> 8) & 0x000F);
        packet[5] = (uint8_t)(((uint16_t)param1 >> 4) & 0x000F);
        packet[6] = (uint8_t)((uint16_t)param1 & 0x000F);
        packet[7] = (uint8_t)(((uint16_t)param2 >> 8) & 0x000F);
        packet[8] = (uint8_t)(((uint16_t)param2 >> 4) & 0x000F);
        packet[9] = (uint8_t)((uint16_t)param2 & 0x000F);
        packet[10] = 0xFF;
        packetSize = 11;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PrivacyZone_SetPTZMask:
        // Check settings.
        if (param1 > 0xFF)
            return false;
        if (param2 > 0xFFF)
            return false;
        if (param3 > 0xFFF)
            return false;
        if (param4 > 0xFFFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x7B;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)(((uint16_t)param2 >> 8) & 0x000F);
        packet[6] = (uint8_t)(((uint16_t)param2 >> 4) & 0x000F);
        packet[7] = (uint8_t)((uint16_t)param2 & 0x000F);
        packet[8] = (uint8_t)(((uint16_t)param3 >> 8) & 0x000F);
        packet[9] = (uint8_t)(((uint16_t)param3 >> 4) & 0x000F);
        packet[10] = (uint8_t)((uint16_t)param3 & 0x000F);
        packet[11] = (uint8_t)(((uint16_t)param4 >> 12) & 0x000F);
        packet[12] = (uint8_t)(((uint16_t)param4 >> 8) & 0x000F);
        packet[13] = (uint8_t)(((uint16_t)param4 >> 4) & 0x000F);
        packet[14] = (uint8_t)((uint16_t)param4 & 0x000F);
        packet[15] = 0xFF;
        packetSize = 16;
        return false;

    case cr::visca::ViscaPackets::COMMAND_CAM_PrivacyZone_Non_InterlockMask:
        // Check parameters.
        if (param1 > 0xFF)
            return false;
        if (param2 > 0xFF)
            return false;
        if (param3 > 0xFF)
            return false;
        if (param4 > 0xFF)
            return false;
        if (param5 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x6F;
        packet[4] = (uint8_t)param1;
        packet[5] = ((uint8_t)param2 >> 4) & 0x0F;
        packet[6] = (uint8_t)param2 & 0x0F;
        packet[7] = ((uint8_t)param3 >> 4) & 0x0F;
        packet[8] = (uint8_t)param3 & 0x0F;
        packet[9] = ((uint8_t)param4 >> 4) & 0x0F;
        packet[10] = (uint8_t)param4 & 0x0F;
        packet[11] = ((uint8_t)param5 >> 4) & 0x0F;
        packet[12] = (uint8_t)param5 & 0x0F;
        packet[13] = 0xFF;
        packetSize = 14;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PrivacyZone_CenterLineOff:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x7C;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_PrivacyZone_CenterLineOn:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x7C;
        packet[4] = 0x04;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_IDWrite:
        // Check setting.
        if (param1 > 0xFFFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x22;
        packet[4] = (uint8_t)(((uint16_t)param1 >> 12) & 0x000F);
        packet[5] = (uint8_t)(((uint16_t)param1 >> 8) & 0x000F);
        packet[6] = (uint8_t)(((uint16_t)param1 >> 4) & 0x000F);
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MD_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1B;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MD_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1B;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MD_Function_Set:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        if (param2 > 0x0F)
            return false;
        if (param3 > 0xFF)
            return false;
        if (param4 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1C;
        packet[4] = (uint8_t)param1 & 0x0F;
        packet[5] = (uint8_t)param2 & 0x0F;
        packet[6] = ((uint8_t)param3 >> 4) & 0x0F;
        packet[7] = (uint8_t)param3 & 0x0F;
        packet[8] = ((uint8_t)param4 >> 4) & 0x0F;
        packet[9] = (uint8_t)param4 & 0x0F;
        packet[10] = 0xFF;
        packetSize = 11;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_MD_Window_Set:
        // Check setting.
        if (param1 > 0x0F)
            return false;
        if (param2 > 0x10)
            return false;
        if (param3 > 0x0E)
            return false;
        if (param4 < 0x01 || param4 > 0x11)
            return false;
        if (param5 < 0x01 || param5 > 0x0F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1D;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = (uint8_t)param3;
        packet[7] = (uint8_t)param4;
        packet[8] = (uint8_t)param5;
        packet[9] = 0xFF;
        packetSize = 10;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ContinuousZoomPosReply_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x69;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ContinuousZoomPosReply_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x69;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ZoomPosReplyIntervalTimeSet:
        // Check setting.
        if (param1 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x6A;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ContinuousFocusPosReply_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x16;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ContinuousFocusPosReply_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x16;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_FocusPosReplyIntervalTimeSet:
        // Check setting.
        if (param1 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1A;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[7] = (uint8_t)param1 & 0x0F;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_RegisterValue:
        // Check parameters.
        if (param1 > 0x7F)
            return false;
        if (param2 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x24;
        packet[4] = (uint8_t)param1;
        packet[5] = ((uint8_t)param2 >> 4) & 0x0F;
        packet[6] = (uint8_t)param2 & 0x0F;
        packet[7] = 0xFF;
        packetSize = 8;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ChromaSuppress:
        // Check parameters.
        if (param1 > 0x03)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x5F;
        packet[4] = (uint8_t)param1;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ColorGain_Direct:
        // Check parameters.
        if (param1 > 0x0E)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x49;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = (uint8_t)param1;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ColorHue_Direct:
        // Check parameters.
        if (param1 > 0x0E)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x4F;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = (uint8_t)param1;
        packet[8] = 0xFF;
        packetSize = 9;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_GammaOffset_Direct:
        // Check parameters.
        if (param1 != 0x00 && param1 != 0x01)
            return false;
        if (param2 > 0x40)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1E;
        packet[4] = 0x00;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = (uint8_t)param1;
        packet[8] = ((uint8_t)param2 >> 4) & 0x0F;
        packet[9] = (uint8_t)param2 & 0x0F;
        packet[10] = 0xFF;
        packetSize = 11;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ContrastAdjLevel_Direct:
        // Check parameters.
        if (param1 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x05;
        packet[3] = 0x5D;
        packet[4] = 0x01;
        packet[5] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[6] = (uint8_t)param1 & 0x0F;
        packet[7] = 0xFF;
        packetSize = 8;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExExpComp_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x0E;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = 0xFF;
        packetSize = 8;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExExpComp_Up:
        // Check parameters.
        if (param1 > 0x7F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x0E;
        packet[5] = 0x02;
        packet[6] = (uint8_t)param1;
        packet[7] = 0xFF;
        packetSize = 8;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExExpComp_Down:
        // Check parameters.
        if (param1 > 0x7F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x0E;
        packet[5] = 0x03;
        packet[6] = (uint8_t)param1;
        packet[7] = 0xFF;
        packetSize = 8;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExExpComp_Direct:
        // Check parameters.
        if (param1 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x4E;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[8] = (uint8_t)param1 & 0x0F;
        packet[9] = 0xFF;
        packetSize = 10;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExAperture_Reset:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x02;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = 0xFF;
        packetSize = 8;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExAperture_Up:
        // Check parameters.
        if (param1 > 0x7F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x02;
        packet[5] = 0x02;
        packet[6] = (uint8_t)param1;
        packet[7] = 0xFF;
        packetSize = 8;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExAperture_Down:
        // Check parameters.
        if (param1 > 0x7F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x02;
        packet[5] = 0x03;
        packet[6] = (uint8_t)param1;
        packet[7] = 0xFF;
        packetSize = 8;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExAperture_Direct:
        // Check parameters.
        if (param1 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x42;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[8] = (uint8_t)param1 & 0x0F;
        packet[9] = 0xFF;
        packetSize = 10;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExAutoICR_Threshold:
        // Check parameters.
        if (param1 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x21;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[8] = (uint8_t)param1 & 0x0F;
        packet[9] = 0xFF;
        packetSize = 10;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExAutoICR_Level:
        // Check parameters.
        if (param1 > 0x1C)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x21;
        packet[5] = 0x01;
        packet[6] = 0x00;
        packet[7] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[8] = (uint8_t)param1 & 0x0F;
        packet[9] = 0xFF;
        packetSize = 10;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExColorGain_Direct:
        // Check parameters.
        if (param1 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x49;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[8] = (uint8_t)param1 & 0x0F;
        packet[9] = 0xFF;
        packetSize = 10;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_ExColorHue_Direct:
        // Check parameters.
        if (param1 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x4F;
        packet[5] = 0x00;
        packet[6] = 0x00;
        packet[7] = ((uint8_t)param1 >> 4) & 0x0F;
        packet[8] = (uint8_t)param1 & 0x0F;
        packet[9] = 0xFF;
        packetSize = 10;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_HLC_Parameter_Set:
        /// Check parameters.
        if (param1 > 3)
            return false;
        if (param2 > 3)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x14;
        packet[4] = (uint8_t)param1;
        packet[5] = (uint8_t)param2;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_FlickerReduction_On:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x32;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::COMMAND_CAM_FlickerReduction_Off:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x01;
        packet[2] = 0x04;
        packet[3] = 0x32;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    /*
    ENQUIRY COMMANDS.
    */
    case cr::visca::ViscaPackets::INQUIRY_CAM_PowerInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x00;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ZoomPosInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x47;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_DZoomModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x06;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_DZoom_C_S_ModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x36;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_DZoomPosInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x46;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FocusModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x38;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FocusPosInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x48;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FocusNearLimitInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x28;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AFSensitivityInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x58;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AFModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x57;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AFTimeSettingInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x27;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_IRCorrectionInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x11;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_WBModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x35;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_RGainInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x43;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_BGainInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x44;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AEModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x39;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AutoSlowShutterInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x5A;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ShutterPosInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x4A;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_IrisPosInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x4B;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_GainPosInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x4C;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_GainLimitInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x2C;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExpCompModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x3E;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExpCompPosInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x4E;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_BackLightModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x33;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_SpotAEModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x59;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_SpotAEPosInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x29;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_VEModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x3D;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_VEParameterInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x2D;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_WDModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x3D;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_WDParameterInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x2D;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AEResponseInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x5D;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_DefogInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x37;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ApertureInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x42;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_NRInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x53;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_NR2D3Dinq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x05;
        packet[3] = 0x53;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;
        
    case cr::visca::ViscaPackets::INQUIRY_CAM_GammaInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x5B;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_HighSensitivityInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x5E;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_LR_ReverseModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x61;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FreezeModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x62;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PictureEffectModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x63;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PictureFlipModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x66;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ICRModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x01;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AutoICRModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x51;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AutoICRThresholdInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x21;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AutoICRAlarmReplyInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x31;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MemoryInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x3F;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MemSaveInq:
        // Check parameters.
        if (param1 > 0x07)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x23;
        packet[4] = (uint8_t)param1;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_DisplayModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x15;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_StabilizerModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x34;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MuteModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x75;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PrivacyDisplayInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x77;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PrivacyPanTiltInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x79;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PrivacyPTZInq:
        // Check parameters.
        if (param1 > 0xFF)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x7B;
        packet[4] = (uint8_t)param1;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PrivacyMonitorInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x6F;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_IDInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x22;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_VersionInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x00;
        packet[3] = 0x02;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MDModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1B;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MDFunctionInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1C;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MDWindowInq:
        // Check parameters.
        if (param1 > 3)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1D;
        packet[4] = (uint8_t)param1;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ContinuousZoomPosReplyModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x69;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ZoomPosReplyIntervalTimeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x6A;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ContinuousFocusPosReplyModeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x16;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FocusReplyIntervalTimeInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1A;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_RegisterValueInq:
        // Check parameters.
        if (param1 > 0x7F)
            return false;
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x24;
        packet[4] = (uint8_t)param1;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ChromaSuppressInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x5F;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ColorGainInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x49;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ColorHueInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x4F;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_TempInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x4F;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_GammaOffsetInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1E;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ContrastAdjLevelInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x05;
        packet[3] = 0x5D;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExExpCompPosInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x4E;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExApertureInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x42;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExColorGainInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x49;
        packet[5] = 0x00;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExColorHueInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x4F;
        packet[5] = 0x00;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExAutoICRThresholdInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x21;
        packet[5] = 0x00;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExAutoICROnLevelInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x1F;
        packet[4] = 0x21;
        packet[5] = 0x01;
        packet[6] = 0xFF;
        packetSize = 7;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MinShutterInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x12;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MinShutterLimitInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x13;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_HLCInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x14;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FlickerReductionInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x04;
        packet[3] = 0x32;
        packet[4] = 0xFF;
        packetSize = 5;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_LenseControlSystemInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x7E;
        packet[3] = 0x7E;
        packet[4] = 0x00;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_CameraControlSystemInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x7E;
        packet[3] = 0x7E;
        packet[4] = 0x01;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_OtherDataInq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x7E;
        packet[3] = 0x7E;
        packet[4] = 0x02;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_ExtendedFunction_1_Inq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x7E;
        packet[3] = 0x7E;
        packet[4] = 0x03;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_ExtendedFunction_2_Inq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x7E;
        packet[3] = 0x7E;
        packet[4] = 0x04;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;

    case cr::visca::ViscaPackets::INQUIRY_ExtendedFunction_3_Inq:
        packet[0] = (0x80 | (uint8_t)cameraAddress);
        packet[1] = 0x09;
        packet[2] = 0x7E;
        packet[3] = 0x7E;
        packet[4] = 0x05;
        packet[5] = 0xFF;
        packetSize = 6;
        return true;    
    
    default:
        return false;
    }

    return false;
}


cr::visca::ViscaPackets cr::visca::ViscaProtocolParser::decodeData(
            uint8_t next_byte,
            uint32_t cameraAddress,
            uint32_t& param1,
            uint32_t& param2,
            uint32_t& param3,
            uint32_t& param4,
            uint32_t& param5,
            uint32_t& param6,
            uint32_t& param7,
            uint32_t& param8,
            uint32_t& param9,
            uint32_t& param10)
{
    param1 = 0;
    param2 = 0;
    param3 = 0;
    param4 = 0;
    param5 = 0;
    param6 = 0;
    param7 = 0;
    param8 = 0;
    param9 = 0;
    param10 = 0;

    // Check camera address.
    if (cameraAddress < 1 || cameraAddress > 7)
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    // Check decoding stage.
    if (m_inputDataCounter == 0)
    {
        m_inputPacketType = cr::visca::ViscaPackets::UNDEFINED_PACKET;

        // Check first byte.
        uint8_t first_byte = ((uint8_t)(cameraAddress + 8) << 4) & 0xF0;
        if (first_byte != next_byte)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;

        // Copy data to buffer.
        m_inputPacketData[m_inputDataCounter] = next_byte;
        // Update counter.
        m_inputDataCounter = 1;

        return cr::visca::ViscaPackets::UNDEFINED_PACKET;
    }
    else
    {
        // Check packet index.
        if (m_inputDataCounter == 1)
        {
            // Check Acknowledge packet.
            if ((next_byte & 0xF0) == 0x40)
            {
                // Set packet type.
                m_inputPacketType = cr::visca::ViscaPackets::ACKNOWLEDGE;
                // Copy data.
                m_inputPacketData[m_inputDataCounter] = next_byte;
                // Update counter.
                ++m_inputDataCounter;

                return cr::visca::ViscaPackets::UNDEFINED_PACKET;
            }

            // Check Completion (Commands) or Reply data (Inquiries).
            if ((next_byte & 0xF0) == 0x50)
            {
                // Set packet type.
                m_inputPacketType = cr::visca::ViscaPackets::COMPLETION_COMMANDS;
                // Copy data.
                m_inputPacketData[m_inputDataCounter] = next_byte;
                // Update counter.
                ++m_inputDataCounter;

                return cr::visca::ViscaPackets::UNDEFINED_PACKET;
            }

            // Check error messages.
            if ((next_byte & 0xF0) == 0x60)
            {
                // Set packet type.
                m_inputPacketType = cr::visca::ViscaPackets::ERROR_Message_Length;
                // Copy data.
                m_inputPacketData[m_inputDataCounter] = next_byte;
                // Update counter.
                ++m_inputDataCounter;

                return cr::visca::ViscaPackets::UNDEFINED_PACKET;
            }

            // Check Alarm and reply packets.
            if (next_byte == 0x07)
            {
                m_inputPacketType = cr::visca::ViscaPackets::REPLY;
                // Copy data.
                m_inputPacketData[m_inputDataCounter] = next_byte;
                // Update counter.
                ++m_inputDataCounter;

                return cr::visca::ViscaPackets::UNDEFINED_PACKET;
            }

            // Reset counter.
            m_inputDataCounter = 0;

            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        }
        else
        {
            // Check packet type.
            switch (m_inputPacketType)
            {
            case cr::visca::ViscaPackets::ACKNOWLEDGE:
            {
                // Check last byte.
                if (next_byte != 0xFF)
                {
                    // Reset counter.
                    m_inputDataCounter = 0;

                    return cr::visca::ViscaPackets::UNDEFINED_PACKET;
                }

                // Return socket number.
                param1 = 0x0F & m_inputPacketData[1];

                // Reset counter.
                m_inputDataCounter = 0;

                // Check packet type.
                return cr::visca::ViscaPackets::ACKNOWLEDGE;
            }

            case cr::visca::ViscaPackets::COMPLETION_COMMANDS:
            {
                // Check last byte.
                if (next_byte == 0xFF)
                {
                    // Return socket number.
                    param1 = 0x0F & m_inputPacketData[1];

                    // Reset counter.
                    m_inputDataCounter = 0;

                    // Check packet type.
                    return cr::visca::ViscaPackets::COMPLETION_COMMANDS;
                }

                // Check second byte.
                if (m_inputPacketData[1] != 0x50)
                {
                    // Reset counter.
                    m_inputDataCounter = 0;

                    return cr::visca::ViscaPackets::UNDEFINED_PACKET;
                }

                m_inputPacketType = cr::visca::ViscaPackets::COMPLETION_INQUIRY;

                m_inputPacketData[m_inputDataCounter] = next_byte;

                // Update counter.
                ++m_inputDataCounter;

                return cr::visca::ViscaPackets::UNDEFINED_PACKET;
            }

            case cr::visca::ViscaPackets::COMPLETION_INQUIRY:
            {
                // Check last byte.
                if (next_byte == 0xFF)
                {
                    // Add data to bufer.
                    m_inputPacketData[m_inputDataCounter] = next_byte;
                    ++m_inputDataCounter;

                    // Decode reply.
                    return decodeReply(m_inputPacketData,
                                       m_inputDataCounter,
                                       param1,
                                       param2,
                                       param3,
                                       param4,
                                       param5,
                                       param6,
                                       param7,
                                       param8,
                                       param9,
                                       param10);
                }

                // Check reply size.
                if (m_inputDataCounter > 15)
                {
                    // Reset counter.
                    m_inputDataCounter = 0;

                    return cr::visca::ViscaPackets::UNDEFINED_PACKET;
                }

                m_inputPacketData[m_inputDataCounter] = next_byte;

                // Update counter.
                ++m_inputDataCounter;

                return cr::visca::ViscaPackets::UNDEFINED_PACKET;
            }

            case cr::visca::ViscaPackets::ERROR_Message_Length:
            {
                // Check message type.
                if (m_inputDataCounter < 3)
                {
                    m_inputPacketData[m_inputDataCounter] = next_byte;

                    // Update counter.
                    ++m_inputDataCounter;

                    return cr::visca::ViscaPackets::UNDEFINED_PACKET;
                }
                else
                {
                    if (next_byte != 0xFF)
                    {
                        // Reset counter.
                        m_inputDataCounter = 0;

                        return cr::visca::ViscaPackets::UNDEFINED_PACKET;
                    }

                    // Check error type.
                    switch (m_inputPacketData[2])
                    {
                    case 0x01:
                        // Reset counter.
                        m_inputDataCounter = 0;
                        // Return result.
                        return cr::visca::ViscaPackets::ERROR_Message_Length;
                    case 0x02:
                        // Reset counter.
                        m_inputDataCounter = 0;
                        // Return result.
                        return cr::visca::ViscaPackets::ERROR_Syntax;
                    case 0x03:
                        // Reset counter.
                        m_inputDataCounter = 0;
                        // Return result.
                        return cr::visca::ViscaPackets::ERROR_Command_Buffer_Full;
                    case 0x04:
                        // Reset counter.
                        m_inputDataCounter = 0;
                        // Return result.
                        return cr::visca::ViscaPackets::ERROR_Command_Canceled;
                    case 0x05:
                        // Reset counter.
                        m_inputDataCounter = 0;
                        // Return result.
                        return cr::visca::ViscaPackets::ERROR_No_Socket;
                    case 0x41:
                        // Reset counter.
                        m_inputDataCounter = 0;
                        // Return result.
                        return cr::visca::ViscaPackets::ERROR_Command_Not_Executable;
                    default:
                        // Reset counter.
                        m_inputDataCounter = 0;
                        // Return result.
                        return cr::visca::ViscaPackets::UNDEFINED_PACKET;
                    }

                    return cr::visca::ViscaPackets::UNDEFINED_PACKET;
                }
            }

            case cr::visca::ViscaPackets::REPLY:
            {
                // Check last byte.
                if (next_byte == 0xFF)
                {
                    // Add data to bufer.
                    m_inputPacketData[m_inputDataCounter] = next_byte;
                    ++m_inputDataCounter;

                    // Check packet size.
                    if (m_inputDataCounter == 6)
                    {
                        // Check AutoICRAlarmReply On.
                        if (m_inputPacketData[2] == 0x04 &&
                            m_inputPacketData[3] == 0x31 &&
                            m_inputPacketData[4] == 0x02)
                        {
                            // Reset counter.
                            m_inputDataCounter = 0;

                            return visca::ViscaPackets::REPLY_CAM_AutoICRAlarmReply_On;
                        }

                        // Check AutoICRAlarmReply Off.
                        if (m_inputPacketData[2] == 0x04 &&
                            m_inputPacketData[3] == 0x31 &&
                            m_inputPacketData[4] == 0x03)
                        {
                            // Reset counter.
                            m_inputDataCounter = 0;

                            return visca::ViscaPackets::REPLY_CAM_AutoICRAlarmReply_Off;
                        }

                        // Check MD Alarm.
                        if (m_inputPacketData[2] == 0x04 &&
                            m_inputPacketData[3] == 0x1B)
                        {
                            param1 = m_inputPacketData[4] & 0x0F;

                            // Reset counter.
                            m_inputDataCounter = 0;

                            return visca::ViscaPackets::REPLY_CAM_MD_Alarm;
                        }

                        // Reset counter.
                        m_inputDataCounter = 0;

                        return visca::ViscaPackets::UNDEFINED_PACKET;
                    }
                    else
                    {
                        if (m_inputDataCounter == 11)
                        {
                            // Check Continious zoom position reply.
                            if (m_inputPacketData[2] == 0x04 && m_inputPacketData[3] == 0x69)
                            {
                                param1 = (uint32_t)(((m_inputPacketData[4] << 4) & 0xF0) | (m_inputPacketData[5] & 0x0F));
                                param2 = (uint32_t)(((m_inputPacketData[6] << 4) & 0xF0) | (m_inputPacketData[7] & 0x0F)) * 256 +
                                          (uint32_t)(((m_inputPacketData[8] << 4) & 0xF0) | (m_inputPacketData[9] & 0x0F));

                                return visca::ViscaPackets::REPLY_CAM_ContinuousZoomPos;
                            }

                            // Check Continious focus position reply.
                            if (m_inputPacketData[2] == 0x04 && m_inputPacketData[3] == 0x16)
                            {
                                param1 = (uint32_t)(((m_inputPacketData[6] << 4) & 0xF0) | (m_inputPacketData[7] & 0x0F)) * 256 +
                                          (uint32_t)(((m_inputPacketData[8] << 4) & 0xF0) | (m_inputPacketData[9] & 0x0F));

                                return visca::ViscaPackets::REPLY_CAM_ContinuousFocusPos;
                            }

                            // Reset counter.
                            m_inputDataCounter = 0;

                            return visca::ViscaPackets::UNDEFINED_PACKET;
                        }
                        else
                        {
                            // Reset counter.
                            m_inputDataCounter = 0;

                            return visca::ViscaPackets::UNDEFINED_PACKET;
                        }
                    }
                }

                // Check reply size.
                if (m_inputDataCounter > 10)
                {
                    // Reset counter.
                    m_inputDataCounter = 0;

                    return visca::ViscaPackets::UNDEFINED_PACKET;
                }

                m_inputPacketData[m_inputDataCounter] = next_byte;

                // Update counter.
                ++m_inputDataCounter;

                return visca::ViscaPackets::UNDEFINED_PACKET;
            }

            default:
                break;
            }

            // Reset counter.
            m_inputDataCounter = 0;

            // Check packet type.
            return visca::ViscaPackets::UNDEFINED_PACKET;

        }

    }

    return visca::ViscaPackets::UNDEFINED_PACKET;
}


cr::visca::ViscaPackets cr::visca::ViscaProtocolParser::decodeReply(
    uint8_t* packetData,
    uint32_t packetSize,
    uint32_t& param1,
    uint32_t& param2,
    uint32_t& param3,
    uint32_t& param4,
    uint32_t& param5,
    uint32_t& param6,
    uint32_t& param7,
    uint32_t& param8,
    uint32_t& param9,
    uint32_t& param10)
{
    param1 = 0;
    param2 = 0;
    param3 = 0;
    param4 = 0;
    param5 = 0;
    param6 = 0;
    param7 = 0;
    param8 = 0;
    param9 = 0;
    param10 = 0;

    // Reset data counter.
    m_inputDataCounter = 0;

    // Check last encoded command.
    switch (m_lastEncodedCommand)
    {
    /*
    ENQUIRY COMMANDS.
    */
    case cr::visca::ViscaPackets::INQUIRY_CAM_PowerInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_Power_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_Power_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ZoomPosInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = (uint32_t)(((m_inputPacketData[2] << 4) & 0xF0) | (m_inputPacketData[3] & 0x0F)) * 256 +
                  (uint32_t)(((m_inputPacketData[4] << 4) & 0xF0) | (m_inputPacketData[5] & 0x0F));
        return cr::visca::ViscaPackets::REPLY_CAM_ZoomPos;

    case cr::visca::ViscaPackets::INQUIRY_CAM_DZoomModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_DZoomMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_DZoomMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_DZoom_C_S_ModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x00)
            return cr::visca::ViscaPackets::REPLY_CAM_DZoom_Combine_Mode;
        if (packetData[2] == 0x01)
            return cr::visca::ViscaPackets::REPLY_CAM_DZoom_Separate_Mode;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_DZoomPosInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_DZoomPos;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FocusModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_FocusMode_Auto_Focus;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_FocusMode_Manual_Focus;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FocusPosInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = (uint32_t)(((m_inputPacketData[2] << 4) & 0xF0) | (m_inputPacketData[3] & 0x0F)) * 256 +
                  (uint32_t)(((m_inputPacketData[4] << 4) & 0xF0) | (m_inputPacketData[5] & 0x0F));
        return cr::visca::ViscaPackets::REPLY_CAM_FocusPos;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FocusNearLimitInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        param3 = packetData[4] & 0x0F;
        param4 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_FocusNearLimit;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AFSensitivityInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_AFSensitivity_Normal;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_AFSensitivity_Low;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AFModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x00)
            return cr::visca::ViscaPackets::REPLY_CAM_AFMode_Normal_AF;
        if (packetData[2] == 0x01)
            return cr::visca::ViscaPackets::REPLY_CAM_AFMode_Interval_AF;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_AFMode_Zoom_Trigger_AF;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AFTimeSettingInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        param3 = packetData[4] & 0x0F;
        param4 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_AFTimeSetting;

    case cr::visca::ViscaPackets::INQUIRY_CAM_IRCorrectionInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x00)
            return cr::visca::ViscaPackets::REPLY_CAM_IRCorrection_Standard;
        if (packetData[2] == 0x01)
            return cr::visca::ViscaPackets::REPLY_CAM_IRCorrection_IR_Light;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_WBModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x00)
            return cr::visca::ViscaPackets::REPLY_CAM_WBMode_Auto;
        if (packetData[2] == 0x01)
            return cr::visca::ViscaPackets::REPLY_CAM_WBMode_Indoor;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_WBMode_Outdoor;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_WBMode_One_Push_WB;
        if (packetData[2] == 0x04)
            return cr::visca::ViscaPackets::REPLY_CAM_WBMode_ATW;
        if (packetData[2] == 0x05)
            return cr::visca::ViscaPackets::REPLY_CAM_WBMode_Manual;
        if (packetData[2] == 0x06)
            return cr::visca::ViscaPackets::REPLY_CAM_WBMode_Outdoor_Auto;
        if (packetData[2] == 0x07)
            return cr::visca::ViscaPackets::REPLY_CAM_WBMode_Sodium_Lamp_Auto;
        if (packetData[2] == 0x08)
            return cr::visca::ViscaPackets::REPLY_CAM_WBMode_Sodium_Lamp;
        if (packetData[2] == 0x09)
            return cr::visca::ViscaPackets::REPLY_CAM_WBMode_Sodium_Lamp_Outdoor_Auto;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_RGainInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_RGain;

    case cr::visca::ViscaPackets::INQUIRY_CAM_BGainInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_BGain;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AEModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x00)
            return cr::visca::ViscaPackets::REPLY_CAM_AEMode_Full_Auto;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_AEMode_Manual;
        if (packetData[2] == 0x0A)
            return cr::visca::ViscaPackets::REPLY_CAM_AEMode_Shutter_Priority;
        if (packetData[2] == 0x0B)
            return cr::visca::ViscaPackets::REPLY_CAM_AEMode_Iris_Priority;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AutoSlowShutterInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_AutoSlowShutter_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_AutoSlowShutter_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ShutterPosInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ShutterPos;

    case cr::visca::ViscaPackets::INQUIRY_CAM_IrisPosInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_IrisPos;

    case cr::visca::ViscaPackets::INQUIRY_CAM_GainPosInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_GainPos;

    case cr::visca::ViscaPackets::INQUIRY_CAM_GainLimitInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_GainLimit;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExpCompModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_ExpCompMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_ExpCompMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExpCompPosInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ExpCompPos;

    case cr::visca::ViscaPackets::INQUIRY_CAM_BackLightModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_BackLightMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_BackLightMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_SpotAEModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_SpotAEMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_SpotAEMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_SpotAEPosInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        param3 = packetData[4] & 0x0F;
        param4 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_SpotAEPos;

    case cr::visca::ViscaPackets::INQUIRY_CAM_VEModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_VEMode_Off;
        if (packetData[2] == 0x06)
            return cr::visca::ViscaPackets::REPLY_CAM_VEMode_On;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_VEParameterInq:
        if (packetSize != 11)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0 || packetData[8] != 0 || packetData[9] != 0)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[3] & 0x0F;
        param2 = packetData[4] & 0x0F;
        param3 = packetData[5] & 0x0F;
        param4 = packetData[6] & 0x0F;
        param5 = packetData[7] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_VEParameter;

    case cr::visca::ViscaPackets::INQUIRY_CAM_WDModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_WDMode_WD_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_WDMode_WD_and_VE_Off;
        if (packetData[2] == 0x06)
            return cr::visca::ViscaPackets::REPLY_CAM_WDMode_VE_On;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_WDParameterInq:
        if (packetSize != 11)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0 || packetData[8] != 0 || packetData[9] != 0)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[3] & 0x0F;
        param2 = packetData[4] & 0x0F;
        param3 = packetData[5] & 0x0F;
        param4 = packetData[6] & 0x0F;
        param5 = packetData[7] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_WDParameter;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AEResponseInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] < 0x01 || packetData[2] > 0x30)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];
        return cr::visca::ViscaPackets::REPLY_CAM_AEResponse;

    case cr::visca::ViscaPackets::INQUIRY_CAM_DefogInq:
        if (packetSize != 5)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x03 && packetData[3] == 0x00)
            return cr::visca::ViscaPackets::REPLY_CAM_Defog_Off;
        if (packetData[2] == 0x02)
        {
            if (packetData[3] != 0x01 && packetData[3] != 0x02 && packetData[4] != 0x03)
                return cr::visca::ViscaPackets::UNDEFINED_PACKET;
            param1 = packetData[3];
            return cr::visca::ViscaPackets::REPLY_CAM_Defog_Level;
        }
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ApertureInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_Aperture;

    case cr::visca::ViscaPackets::INQUIRY_CAM_NRInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];
        return cr::visca::ViscaPackets::REPLY_CAM_NR;

    case cr::visca::ViscaPackets::INQUIRY_CAM_NR2D3Dinq:
        if (packetSize != 5)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] > 0x05 || packetData[3] > 0x05)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];
        param2 = packetData[3];
        return cr::visca::ViscaPackets::REPLY_CAM_NR2D3D;

    case cr::visca::ViscaPackets::INQUIRY_CAM_GammaInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] > 0x01)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];
        return cr::visca::ViscaPackets::REPLY_CAM_Gamma;

    case cr::visca::ViscaPackets::INQUIRY_CAM_HighSensitivityInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_HighSensitivity_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_HighSensitivity_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_LR_ReverseModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_LR_ReverseMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_LR_ReverseMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FreezeModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_FreezeMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_FreezeMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PictureEffectModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x00)
            return cr::visca::ViscaPackets::REPLY_CAM_PictureEffectMode_Off;
        if (packetData[2] == 0x04)
            return cr::visca::ViscaPackets::REPLY_CAM_PictureEffectMode_Black_and_White;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PictureFlipModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_PictureFlipMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_PictureFlipMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ICRModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_ICRMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_ICRMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AutoICRModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_AutoICRMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_AutoICRMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AutoICRThresholdInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_AutoICRThreshold;

    case cr::visca::ViscaPackets::INQUIRY_CAM_AutoICRAlarmReplyInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_AutoICRAlarmReply_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_AutoICRAlarmReply_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MemoryInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];
        return cr::visca::ViscaPackets::REPLY_CAM_Memory;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MemSaveInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        param3 = packetData[4] & 0x0F;
        param4 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_MemSave;

    case cr::visca::ViscaPackets::INQUIRY_CAM_DisplayModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_DisplayMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_DisplayMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_StabilizerModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_StabilizerMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_StabilizerMode_Off;
        if (packetData[2] == 0x00)
            return cr::visca::ViscaPackets::REPLY_CAM_StabilizerMode_Hold;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MuteModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_MuteMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_MuteMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PrivacyDisplayInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];
        param2 = packetData[3];
        param3 = packetData[4];
        param4 = packetData[5];
        return cr::visca::ViscaPackets::REPLY_CAM_PrivacyDisplay;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PrivacyPanTiltInq:
        if (packetSize != 9)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        param3 = packetData[4] & 0x0F;
        param4 = packetData[5] & 0x0F;
        param5 = packetData[6] & 0x0F;
        param6 = packetData[7] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_PrivacyPanTilt;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PrivacyPTZInq:
        if (packetSize != 13)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        param3 = packetData[4] & 0x0F;
        param4 = packetData[5] & 0x0F;
        param5 = packetData[6] & 0x0F;
        param6 = packetData[7] & 0x0F;
        param7 = packetData[8] & 0x0F;
        param8 = packetData[9] & 0x0F;
        param9 = packetData[10] & 0x0F;
        param10 = packetData[11] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_PrivacyPTZ;

    case cr::visca::ViscaPackets::INQUIRY_CAM_PrivacyMonitorInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];
        param2 = packetData[3];
        param3 = packetData[4];
        param4 = packetData[5];
        return cr::visca::ViscaPackets::REPLY_CAM_PrivacyMonitor;

    case cr::visca::ViscaPackets::INQUIRY_CAM_IDInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        param3 = packetData[4] & 0x0F;
        param4 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ID;

    case cr::visca::ViscaPackets::INQUIRY_CAM_VersionInq:
        if (packetSize != 10)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x20)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4];
        param2 = packetData[5];
        param3 = packetData[6];
        param4 = packetData[7];
        param5 = packetData[8];
        return cr::visca::ViscaPackets::REPLY_CAM_Version;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MDModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_MDMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_MDMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MDFunctionInq:
        if (packetSize != 9)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        param3 = packetData[4] & 0x0F;
        param4 = packetData[5] & 0x0F;
        param5 = packetData[6] & 0x0F;
        param6 = packetData[7] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_MDFunction;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MDWindowInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];
        param2 = packetData[3] & 0x0F;
        param3 = packetData[4];
        param4 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_MDWindow;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ContinuousZoomPosReplyModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_ContinuousZoomPosReplyMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_ContinuousZoomPosReplyMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ZoomPosReplyIntervalTimeInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ZoomPosReplyIntervalTime;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ContinuousFocusPosReplyModeInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_ContinuousFocusPosReplyMode_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_ContinuousFocusPosReplyMode_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FocusReplyIntervalTimeInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_FocusReplyIntervalTime;

    case cr::visca::ViscaPackets::INQUIRY_CAM_RegisterValueInq:
        if (packetSize != 5)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_RegisterValue;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ChromaSuppressInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];
        return cr::visca::ViscaPackets::REPLY_CAM_ChromaSuppress;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ColorGainInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00 || packetData[4] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ColorGain;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ColorHueInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00 || packetData[4] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ColorHue;

    case cr::visca::ViscaPackets::INQUIRY_CAM_TempInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_Temp;

    case cr::visca::ViscaPackets::INQUIRY_CAM_GammaOffsetInq:
        if (packetSize != 9)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00 || packetData[4] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[5] & 0x0F;
        param2 = packetData[6] & 0x0F;
        param3 = packetData[7] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_GammaOffset;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ContrastAdjLevelInq:
        if (packetSize != 5)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ContrastAdjLevel;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExExpCompPosInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ExExpCompPos;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExApertureInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_Aperture;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExColorGainInq:
        if (packetSize != 5)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ExColorGain;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExColorHueInq:
        if (packetSize != 5)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ExColorHue;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExAutoICRThresholdInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ExAutoICRThreshold;

    case cr::visca::ViscaPackets::INQUIRY_CAM_ExAutoICROnLevelInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_ExAutoICROnLevel;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MinShutterInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_MinShutter_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_MinShutter_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_CAM_MinShutterLimitInq:
        if (packetSize != 7)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] != 0x00 || packetData[3] != 0x00)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[4] & 0x0F;
        param2 = packetData[5] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_MinShutterLimit;

    case cr::visca::ViscaPackets::INQUIRY_CAM_HLCInq:
        if (packetSize != 5)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3] & 0x0F;
        return cr::visca::ViscaPackets::REPLY_CAM_HLC;

    case cr::visca::ViscaPackets::INQUIRY_CAM_FlickerReductionInq:
        if (packetSize != 4)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        if (packetData[2] == 0x02)
            return cr::visca::ViscaPackets::REPLY_CAM_FlickerReduction_On;
        if (packetData[2] == 0x03)
            return cr::visca::ViscaPackets::REPLY_CAM_FlickerReduction_Off;
        return cr::visca::ViscaPackets::UNDEFINED_PACKET;

    case cr::visca::ViscaPackets::INQUIRY_LenseControlSystemInq:
    {
        if (packetSize != 16)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        uint8_t zoom_pos_1 = packetData[2] & 0x0F;
        uint8_t zoom_pos_2 = packetData[3] & 0x0F;
        uint8_t zoom_pos_3 = packetData[4] & 0x0F;
        uint8_t zoom_pos_4 = packetData[5] & 0x0F;
        param1 = (uint32_t)(((zoom_pos_1 << 4) & 0xF0) + zoom_pos_2) * 256 + (uint32_t)(((zoom_pos_3 << 4) & 0xF0) + zoom_pos_4);
        uint8_t focus_near_limit_1 = packetData[6] & 0x0F;
        uint8_t focus_near_limit_2 = packetData[7] & 0x0F;
        param2 = ((focus_near_limit_1 << 4) & 0xF0) + focus_near_limit_2;
        uint8_t focus_pos_1 = packetData[8] & 0x0F;
        uint8_t focus_pos_2 = packetData[9] & 0x0F;
        uint8_t focus_pos_3 = packetData[10] & 0x0F;
        uint8_t focus_pos_4 = packetData[11] & 0x0F;
        param3 = (uint32_t)(((focus_pos_1 << 4) & 0xF0) + focus_pos_2) * 256 + (uint32_t)(((focus_pos_3 << 4) & 0xF0) + focus_pos_4);
        param4 = packetData[13];
        param5 = packetData[14];

        return cr::visca::ViscaPackets::REPLY_LenseControlSystem;
    }

    case cr::visca::ViscaPackets::INQUIRY_CameraControlSystemInq:
    {
        if (packetSize != 16)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        uint8_t r_gain_1 = packetData[2] & 0x0F;
        uint8_t r_gain_2 = packetData[3] & 0x0F;
        param1 = ((r_gain_1 << 4) & 0xF0) + r_gain_2;
        uint8_t b_gain_1 = packetData[4] & 0x0F;
        uint8_t b_gain_2 = packetData[5] & 0x0F;
        param2 = ((b_gain_1 << 4) & 0xF0) + b_gain_2;
        param3 = packetData[6] & 0x0F;
        param4 = packetData[7] & 0x0F;
        param5 = packetData[8] & 0x0F;
        param6 = packetData[9];
        param7 = packetData[10];
        param8 = packetData[11];
        param9 = packetData[12];
        param10 = packetData[14];

        return cr::visca::ViscaPackets::REPLY_CameraControlSystem;
    }

    case cr::visca::ViscaPackets::INQUIRY_OtherDataInq:
    {
        if (packetSize != 16)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2] & 0x0F;
        param2 = packetData[3];
        param3 = packetData[4];
        param4 = packetData[5];
        uint8_t camera_id_1 = packetData[8] & 0x0F;
        uint8_t camera_id_2 = packetData[9] & 0x0F;
        uint8_t camera_id_3 = packetData[10] & 0x0F;
        uint8_t camera_id_4 = packetData[11] & 0x0F;
        param5 = (uint32_t)(((camera_id_1 << 4) & 0xF0) + camera_id_2) * 256 + (uint32_t)(((camera_id_3 << 4) & 0xF0) + camera_id_4);
        param6 = packetData[12];

        return cr::visca::ViscaPackets::REPLY_OtherData;
    }

    case cr::visca::ViscaPackets::INQUIRY_ExtendedFunction_1_Inq:
    {
        if (packetSize != 16)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        uint8_t d_zoom_pos_1 = packetData[2] & 0x0F;
        uint8_t d_zoom_pos_2 = packetData[3] & 0x0F;
        param1 = ((d_zoom_pos_1 << 4) & 0xF0) + d_zoom_pos_2;
        uint8_t af_activation_time_1 = packetData[4] & 0x0F;
        uint8_t af_activation_time_2 = packetData[5] & 0x0F;
        param2 = ((af_activation_time_1 << 4) & 0xF0) + af_activation_time_2;
        uint8_t af_interval_time_1 = packetData[6] & 0x0F;
        uint8_t af_interval_time_2 = packetData[7] & 0x0F;
        param3 = ((af_interval_time_1 << 4) & 0xF0) + af_interval_time_2;
        param4 = packetData[8];
        param5 = packetData[9];
        param6 = packetData[10];
        param7 = packetData[11];
        param8 = packetData[12];
        param9 = packetData[13];
        param10 = packetData[14];

        return cr::visca::ViscaPackets::REPLY_ExtendedFunction_1;
    }

    case cr::visca::ViscaPackets::INQUIRY_ExtendedFunction_2_Inq:
    {
        if (packetSize != 16)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];
        param2 = packetData[4];
        param3 = packetData[5];
        param4 = packetData[6];
        param5 = packetData[7];
        param6 = packetData[8];

        return cr::visca::ViscaPackets::REPLY_ExtendedFunction_2;
    }

    case cr::visca::ViscaPackets::INQUIRY_ExtendedFunction_3_Inq:
    {
        if (packetSize != 16)
            return cr::visca::ViscaPackets::UNDEFINED_PACKET;
        param1 = packetData[2];

        return cr::visca::ViscaPackets::REPLY_ExtendedFunction_3;
    }

    default:
        break;
    }

    // Reset data counter.
    m_inputDataCounter = 0;

    return cr::visca::ViscaPackets::UNDEFINED_PACKET;
}



std::string cr::visca::ViscaProtocolParser::getVersion()
{
    return std::to_string(VISCA_PROTOCOL_PARSER_MAJOR_VERSION) + "." +
           std::to_string(VISCA_PROTOCOL_PARSER_MINOR_VERSION) + "." +
           std::to_string(VISCA_PROTOCOL_PARSER_PATCH_VERSION);
}





