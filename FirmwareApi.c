#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Define return values for get_pwr_status
#define STATUS_ON 1
#define STATUS_OFF 0
#define STATUS_ERROR -1


// Define handle for UART communication
static HANDLE hSerial;

// Initialize UART communication
__declspec(dllexport) bool initialize_uart(const char *com_port, DWORD baud_rate) 
{
    hSerial = CreateFile
    (
        com_port,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) 
    {
        return false;                                               // Unable to open COM port
    }

    // Configure UART settings
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) 
    {
        CloseHandle(hSerial);
        return false;
    }

    dcbSerialParams.BaudRate = baud_rate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) 
    {
        CloseHandle(hSerial);
        return false;
    }

    return true;
}

// Send a command to firmware
__declspec(dllexport) bool send_command(uint8_t cmd_id, uint8_t *payload, uint8_t payload_length) 
{
    if (hSerial == INVALID_HANDLE_VALUE) 
    {
        return false;                                                                   // UART not initialized
    }

    // Prepare command buffer
    uint8_t buffer[256];
    buffer[0] = cmd_id;
    buffer[1] = payload_length;
    if (payload_length > 0) 
    {
        memcpy(&buffer[2], payload, payload_length);
    }

    // Write command to UART
    DWORD bytes_written;
    if (!WriteFile(hSerial, buffer, payload_length + 2, &bytes_written, NULL)) 
    {
        return false;
    }

    return bytes_written == payload_length + 2;
}

// Receive a response from firmware
__declspec(dllexport) bool receive_response(uint8_t *response_buffer, uint8_t buffer_size, uint8_t *response_length) 
{
    if (hSerial == INVALID_HANDLE_VALUE) 
    {
        printf("UART not initialized\n");
        return false;
    }

    DWORD total_bytes_read = 0;
    DWORD bytes_read;
    DWORD timeout_ms = 500; // Timeout for the entire read operation
    DWORD start_time = GetTickCount();

    while (total_bytes_read < buffer_size) 
    {
        if (!ReadFile(hSerial, response_buffer + total_bytes_read, buffer_size - total_bytes_read, &bytes_read, NULL)) 
        {
            printf("ReadFile failed\n");
            return false;
        }

        if (bytes_read == 0) 
        {
            // Check for timeout
            if (GetTickCount() - start_time > timeout_ms) 
            {
                printf("ReadFile timeout\n");
                break;
            }
            continue;
        }

        total_bytes_read += bytes_read;
    }

    printf("Total bytes read: %lu\n", total_bytes_read);
    for (DWORD i = 0; i < total_bytes_read; i++) 
    {
        printf("Byte %lu: 0x%02X\n", i, response_buffer[i]);
    }

    *response_length = (uint8_t)total_bytes_read;
    return total_bytes_read > 0;
}



// Power on/off the coffee machine
__declspec(dllexport) bool toggle_power() 
{
    return send_command(0x01, NULL, 0);
}

// Toggle strong brew mode
__declspec(dllexport) bool toggle_strong_brew() 
{
    return send_command(0x02, NULL, 0);
}

// Get Power Status
__declspec(dllexport) int get_pwr_status(void)
{
    if (!send_command(0x05, NULL, 0)) {
        printf("Failed to send command\n");
        return STATUS_ERROR;
    }

    uint8_t response_buffer[1];
    uint8_t response_length;

    if (!receive_response(response_buffer, sizeof(response_buffer), &response_length)) {
        printf("Failed to receive response\n");
        return STATUS_ERROR;
    }

    if (response_length != 1) {
        printf("Invalid response length: %d\n", response_length);
        return STATUS_ERROR;
    }

    printf("Response received: 0x%02X\n", response_buffer[0]);
    if (response_buffer[0] == 0x01) {
        return STATUS_ON;
    } else if (response_buffer[0] == 0x00) {
        return STATUS_OFF;
    }

    printf("Unexpected response: 0x%02X\n", response_buffer[0]);
    return STATUS_ERROR;
}



// Set the water level
__declspec(dllexport) bool set_water_level(uint8_t water_level) 
{
    uint8_t payload[1] = { water_level };
    return send_command(0x04, payload, 1);
}

// Close UART communication
__declspec(dllexport) void close_uart() 
{
    if (hSerial != INVALID_HANDLE_VALUE) 
    {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}
