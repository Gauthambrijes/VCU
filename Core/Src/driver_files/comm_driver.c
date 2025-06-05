/*
 * comm_driver.c
 *
 *  Created on: Jun 3, 2025
 *      Author: User
 */

#include <comm_driver.h>
#include <config.h>
#include <string.h>


#ifdef FECU
extern I2C_HandleTypeDef hi2c1;
#define I2C &hi2c1
//static HAL_StatusTypeDef Process_I2C_Queue(void);

//FECU CODE
#define I2C_QUEUE_SIZE  10  // Small queue for VCU (adjust as needed)
#define I2C_TIMEOUT_MS  50 // Timeout for I2C operations

typedef struct {
    uint8_t val_type;
    float   value;
} I2C_Message;

volatile I2C_Message i2c_queue[I2C_QUEUE_SIZE];
volatile uint8_t queue_head = 0;
volatile uint8_t queue_tail = 0;
volatile uint8_t is_transmitting = 0;


HAL_StatusTypeDef Transmit_SensorVal_to_Rpi(uint8_t val_type, const float *value) {
    // Check for NULL pointers
    if (value == NULL) return HAL_ERROR;

    // Check if queue is full
    uint8_t next_tail = (queue_tail + 1) % I2C_QUEUE_SIZE;
    if (next_tail == queue_head) return HAL_BUSY; // Queue full

    // Add message to queue
    i2c_queue[queue_tail].val_type = val_type;
    i2c_queue[queue_tail].value = *value;
    queue_tail = next_tail;

    // Start transmission if idle
    if (!is_transmitting) {
        return Process_I2C_Queue();
    }

    return HAL_OK;
}

//Add to Queue if busy

HAL_StatusTypeDef Process_I2C_Queue(void) {
    if (queue_head == queue_tail) {
        is_transmitting = 0; // Queue empty
        return HAL_OK;
    }

    is_transmitting = 1;

    // Static buffer (DMA-safe)
    static uint8_t tx_buffer[5];
    tx_buffer[0] = i2c_queue[queue_head].val_type;
    memcpy(&tx_buffer[1], (const void*)&i2c_queue[queue_head].value, sizeof(float));

    // Move queue head (circular buffer)
    queue_head = (queue_head + 1) % I2C_QUEUE_SIZE;

    // Start DMA transmission (with timeout)
    return HAL_I2C_Master_Transmit_DMA(I2C, SLAVE_ADDRESS, tx_buffer, 5);
}

// --- I2C Transmission Complete Callback ---
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c == I2C) {
        // Process next message (if any)
        Process_I2C_Queue();
    }
}

// --- I2C Error Handling ---
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c == I2C) {
        is_transmitting = 0; // Reset transmission flag
        Process_I2C_Queue(); // Retry next message
    }
}

#elif RECU




//RECU CODE




#else
#error "Both FECU and RECU is not defined"
#endif

