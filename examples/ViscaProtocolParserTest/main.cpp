#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>
#include "ViscaProtocolParser.h"
#include "SerialPort.h"


int main(void)
{
    std::cout << "VISCA protocol parser v" <<
                 cr::visca::ViscaProtocolParser::getVersion() << " test" <<
                 std::endl;
    std::cout << "SerialPort class v" <<
                 cr::clib::SerialPort::getVersion() << std::endl << std::endl;

    // Dialog to set serial port name.
    std::string serialPortName = "";
    std::cout << "Enter serial port name: ";
    std::cin >> serialPortName;

    // Dialog to set serial port baudrate.
    int serialPortBaudrate = 0;
    std::cout << "Enter serial port baudrate (bps): ";
    std::cin >> serialPortBaudrate;

    // Dialog to enter camera address (0-7).
    int cameraAddress = 1;
    std::cout << "Enter camera address [1:7], 1 - default: ";
    std::cin >> cameraAddress;

    // Init serial port.
    cr::clib::SerialPort serialPort;
    if (!serialPort.open(serialPortName.c_str(), serialPortBaudrate, 500))
    {
        std::cout << "ERROR: " << serialPortName << " not open. Exit..." <<
                     std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }
    std::cout << "Serial port " << serialPortName <<
                 " open. Wait response timeout by default 500 msec" <<
                 std::endl;

    // Init variables.
    const int bufferSize = 1024;
    uint8_t dataBuffer[bufferSize];
    memset(dataBuffer, 0, bufferSize);
    cr::visca::ViscaProtocolParser protocolParser;
    uint32_t param1, param2, param3, param4, param5 = 0;
    uint32_t param6, param7, param8, param9, param10 = 0;

    // Clear serial port buffer.
    serialPort.readData(dataBuffer, bufferSize);

    // Main loop.
    while (true)
    {
        std::cout << "Type of command:" << std::endl;
        std::cout << "1 - Zoom tele" << std::endl;
        std::cout << "2 - Zoom wide" << std::endl;
        std::cout << "3 - Zoom stop" << std::endl;
        std::cout << "4 - Focus far" << std::endl;
        std::cout << "5 - Focus near" << std::endl;
        std::cout << "6 - Focus stop" << std::endl;
        std::cout << "7 - Inqury zoom position" << std::endl;
        std::cout << "8 - Inqury focus position" << std::endl;
        std::cout << "9 - Enable autofocus mode" << std::endl;
        std::cout << "0 - Disable autofocus mode" << std::endl;
        std::cout << "Command type: " << std::endl;
        int commandType = 0;
        std::cin >> commandType;

        // Check command type.
        if (commandType < 0 || commandType > 9)
        {
            std::cout << "Incorrect command type" << std::endl << std::endl;
            continue;
        }

        switch (commandType)
        {
        case 0: // Disable autofocus mode.
        {
            // Encode command.
            uint32_t packetSize = 0;
            protocolParser.encodeCommand(
                        cr::visca::ViscaPackets::COMMAND_CAM_Focus_Auto_Manual,
                        dataBuffer, packetSize, cameraAddress);

            // Send command.
            if (serialPort.sendData(dataBuffer, packetSize) != packetSize)
                std::cout <<
                   "ERROR: Command COMMAND_CAM_Focus_Auto_Manual not sent" <<
                             std::endl;
            else
                std::cout <<
                   "Command COMMAND_CAM_Focus_Auto_Manual has beed sent" <<
                             std::endl;
        }
            break;

        case 1: // Zoom tele.
        {
            // Encode command.
            uint32_t packetSize = 0;
            protocolParser.encodeCommand(
                        cr::visca::ViscaPackets::COMMAND_CAM_Zoom_Tele,
                        dataBuffer, packetSize, cameraAddress);

            // Send command.
            if (serialPort.sendData(dataBuffer, packetSize) != packetSize)
                std::cout <<
                   "ERROR: Command COMMAND_CAM_Zoom_Tele not sent" <<
                             std::endl;
            else
                std::cout <<
                   "Command COMMAND_CAM_Zoom_Tele has beed sent" <<
                             std::endl;
        }
            break;

        case 2: // Zoom wide.
        {
            // Encode command.
            uint32_t packetSize = 0;
            protocolParser.encodeCommand(
                        cr::visca::ViscaPackets::COMMAND_CAM_Zoom_Wide,
                        dataBuffer, packetSize, cameraAddress);

            // Send command.
            if (serialPort.sendData(dataBuffer, packetSize) != packetSize)
                std::cout <<
                   "ERROR: Command COMMAND_CAM_Zoom_Wide not sent" <<
                             std::endl;
            else
                std::cout <<
                   "Command COMMAND_CAM_Zoom_Wide has beed sent" <<
                             std::endl;
        }
            break;

        case 3: // Zoom stop.
        {
            // Encode command.
            uint32_t packetSize = 0;
            protocolParser.encodeCommand(
                        cr::visca::ViscaPackets::COMMAND_CAM_Zoom_Stop,
                        dataBuffer, packetSize, cameraAddress);

            // Send command.
            if (serialPort.sendData(dataBuffer, packetSize) != packetSize)
                std::cout <<
                   "ERROR: Command COMMAND_CAM_Zoom_Stop not sent" <<
                             std::endl;
            else
                std::cout <<
                   "Command COMMAND_CAM_Zoom_Stop has beed sent" <<
                             std::endl;
        }
            break;

        case 4: // Focus far.
        {
            // Encode command.
            uint32_t packetSize = 0;
            protocolParser.encodeCommand(
                        cr::visca::ViscaPackets::COMMAND_CAM_Focus_Far,
                        dataBuffer, packetSize, cameraAddress);

            // Send command.
            if (serialPort.sendData(dataBuffer, packetSize) != packetSize)
                std::cout <<
                   "ERROR: Command COMMAND_CAM_Focus_Far not sent" <<
                             std::endl;
            else
                std::cout <<
                   "Command COMMAND_CAM_Focus_Far has beed sent" <<
                             std::endl;
        }
            break;

        case 5: // Focus near.
        {
            // Encode command.
            uint32_t packetSize = 0;
            protocolParser.encodeCommand(
                        cr::visca::ViscaPackets::COMMAND_CAM_Focus_Near,
                        dataBuffer, packetSize, cameraAddress);

            // Send command.
            if (serialPort.sendData(dataBuffer, packetSize) != packetSize)
                std::cout <<
                   "ERROR: Command COMMAND_CAM_Focus_Near not sent" <<
                             std::endl;
            else
                std::cout <<
                   "Command COMMAND_CAM_Focus_Near has beed sent" <<
                             std::endl;
        }
            break;

        case 6: // Focus stop.
        {
            // Encode command.
            uint32_t packetSize = 0;
            protocolParser.encodeCommand(
                        cr::visca::ViscaPackets::COMMAND_CAM_Focus_Stop,
                        dataBuffer, packetSize, cameraAddress);

            // Send command.
            if (serialPort.sendData(dataBuffer, packetSize) != packetSize)
                std::cout <<
                   "ERROR: Command COMMAND_CAM_Focus_Stop not sent" <<
                             std::endl;
            else
                std::cout <<
                   "Command COMMAND_CAM_Focus_Stop has beed sent" <<
                             std::endl;
        }
            break;

        case 7: // Inqury zoom position.
        {
            // Encode command.
            uint32_t packetSize = 0;
            protocolParser.encodeCommand(
                        cr::visca::ViscaPackets::INQUIRY_CAM_ZoomPosInq,
                        dataBuffer, packetSize, cameraAddress);

            // Send command.
            if (serialPort.sendData(dataBuffer, packetSize) != packetSize)
                std::cout <<
                   "ERROR: Command INQUIRY_CAM_ZoomPosInq not sent" <<
                             std::endl;
            else
                std::cout <<
                   "Command INQUIRY_CAM_ZoomPosInq has beed sent" <<
                             std::endl;
        }
            break;

        case 8: // Inqury focus position.
        {
            // Encode command.
            uint32_t packetSize = 0;
            protocolParser.encodeCommand(
                        cr::visca::ViscaPackets::INQUIRY_CAM_FocusPosInq,
                        dataBuffer, packetSize, cameraAddress);

            // Send command.
            if (serialPort.sendData(dataBuffer, packetSize) != packetSize)
                std::cout <<
                   "ERROR: Command INQUIRY_CAM_FocusPosInq not sent" <<
                             std::endl;
            else
                std::cout <<
                   "Command INQUIRY_CAM_FocusPosInq has beed sent" <<
                             std::endl;
        }
            break;

        case 9: // Enable autofocus mode.
        {
            // Encode command.
            uint32_t packetSize = 0;
            protocolParser.encodeCommand(
                        cr::visca::ViscaPackets::COMMAND_CAM_Focus_Auto_Focus_On,
                        dataBuffer, packetSize, cameraAddress);

            // Send command.
            if (serialPort.sendData(dataBuffer, packetSize) != packetSize)
                std::cout <<
                   "ERROR: Command COMMAND_CAM_Focus_Auto_Manual not sent" <<
                             std::endl;
            else
                std::cout <<
                   "Command COMMAND_CAM_Focus_Auto_Manual has beed sent" <<
                             std::endl;
        }
            break;

        default:
            break;
        }


        // Read data from serial port.
        int numberOfBytes = serialPort.readData(dataBuffer, bufferSize);

        // Decode data.
        if (numberOfBytes <= 0)
        {
            std::cout << "No response" << std::endl;
        }
        else
        {
            // Show input bytes.
            std::cout << "Read from serial port " << numberOfBytes <<" bytes: ";
            for (int i = 0; i < numberOfBytes; ++i)
                std::cout << std::hex << (int)dataBuffer[i] << " ";
            std::cout << std::dec << std::endl;

            // Decode bytes.
            for (int i = 0; i < numberOfBytes; ++i)
            {
                switch(protocolParser.decodeData(dataBuffer[i],
                                                 cameraAddress,
                                                 param1, param2, param3, param4,
                                                 param5, param6, param7, param8,
                                                 param9, param10))
                {
                case cr::visca::ViscaPackets::ACKNOWLEDGE:
                    std::cout << "Input ACKNOWLEDGE" << std::endl;
                    break;
                case cr::visca::ViscaPackets::COMPLETION_COMMANDS:
                    std::cout << "Input COMPLETION_COMMANDS" << std::endl;
                    break;
                case cr::visca::ViscaPackets::COMPLETION_INQUIRY:
                    std::cout << "Input COMPLETION_INQUIRY" << std::endl;
                    break;
                case cr::visca::ViscaPackets::REPLY_CAM_ZoomPos:
                    std::cout << "Input ZOOM POSITION: " << param1 << std::endl;
                    break;
                case cr::visca::ViscaPackets::REPLY_CAM_FocusPos:
                    std::cout << "Input FOCUS POSITION: " << param1 << std::endl;
                    break;
                default:
                    break;
                }
            }
        }

    }

    return 1;
}
