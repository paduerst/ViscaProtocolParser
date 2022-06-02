#pragma once
#include <cstdint>
#include <string>
#include "ViscaProtocolParserDataStructures.h"


namespace  cr
{
namespace visca
{
    /**
     * @brief VISCA protocol parser class.
     */
    class ViscaProtocolParser
    {
    public:

        /**
         * @brief Class constructor.
         */
        ViscaProtocolParser();

        /**
         * @brief Class destructor.
         */
        ~ViscaProtocolParser();

        /**
         * @brief Encode COMMAND or ENQUIRY COMMAND.
         * @param commandId ID of command.
         * @param packet Pointer to packet data buffer. Should be >= 24.
         * @param packetSize Output packet size.
         * @param camera_address Camera address.
         * @param param1 Parameter 1. The meanimg depends from ID of command.
         * @param param2 Parameter 2. The meanimg depends from ID of command.
         * @param param3 Parameter 3. The meanimg depends from ID of command.
         * @param param4 Parameter 4. The meanimg depends from ID of command.
         * @param param5 Parameter 5. The meanimg depends from ID of command.
         * @param param6 Parameter 6. The meanimg depends from ID of command.
         * @param param7 Parameter 7. The meanimg depends from ID of command.
         * @param param8 Parameter 8. The meanimg depends from ID of command.
         * @param param9 Parameter 9. The meanimg depends from ID of command.
         * @param param10 Parameter 10. The meanimg depends from ID of command.
         * @return true if the commend was encoded.
         * @return false in case any errors.
         */
        bool encodeCommand(
            visca::ViscaPackets commandId,
            uint8_t* packet,
            uint32_t& packetSize,
            uint32_t cameraAddress,
            uint32_t param1 = 0,
            uint32_t param2 = 0,
            uint32_t param3 = 0,
            uint32_t param4 = 0,
            uint32_t param5 = 0,
            uint32_t param6 = 0,
            uint32_t param7 = 0,
            uint32_t param8 = 0,
            uint32_t param9 = 0,
            uint32_t param10 = 0);

        /**
         * @brief Method to decode input data byte-by-byte.
         * @param nextByte Next byte of input data.
         * @param cameraAddress Camera address.
         * @param param1 Parameter 1. The meanimg depends from ID of command.
         * @param param2 Parameter 2. The meanimg depends from ID of command.
         * @param param3 Parameter 3. The meanimg depends from ID of command.
         * @param param4 Parameter 4. The meanimg depends from ID of command.
         * @param param5 Parameter 5. The meanimg depends from ID of command.
         * @param param6 Parameter 6. The meanimg depends from ID of command.
         * @param param7 Parameter 7. The meanimg depends from ID of command.
         * @param param8 Parameter 8. The meanimg depends from ID of command.
         * @param param9 Parameter 9. The meanimg depends from ID of command.
         * @param param10 Parameter 10. The meanimg depends from ID of command.
         * @return visca::ViscaPackets type.
         */
        visca::ViscaPackets decodeData(
            uint8_t nextByte,
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
            uint32_t& param10);

        /**
         * @brief GetVersion
         * @return String of current calss version.
         */
        static std::string getVersion();

    private:

        uint32_t m_inputDataCounter;
        uint8_t m_inputPacketData[128];
        visca::ViscaPackets m_inputPacketType;
        visca::ViscaPackets m_lastEncodedCommand;

        visca::ViscaPackets decodeReply(
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
            uint32_t& param10);
            
    };
}
}
