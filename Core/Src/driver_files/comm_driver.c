/*
 * comm_driver.c
 *
 *  Created on: Jun 3, 2025
 *      Author: User
 */

#include <comm_driver.h>


#ifdef FECU
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;
#define SPI &hspi1

//FECU CODE

#define SPI_CS_GPIO_PORT  GPIOA
#define SPI_CS_PIN        GPIO_PIN_3
#define SPI_QUEUE_SIZE  16  // Small queue for VCU (adjust as needed)
#define SPI_TIMEOUT_MS  50 // Timeout for SPI operations

typedef enum {
    MSG_FLOAT_WITH_TYPE = 0,  // 1-byte type + 4-byte float
    MSG_RAW_16BIT       = 1   // 2-byte raw uint16_t (no prefix)
} SPI_MessageType;

typedef struct {
    SPI_MessageType msg_type;
    union {
        struct {
            uint8_t val_type;
            float value;
        } float_msg;
        uint16_t raw_16bit;
    } payload;
} SPI_Message;

volatile SPI_Message SPI_queue[SPI_QUEUE_SIZE];
volatile uint8_t queue_head = 0;
volatile uint8_t queue_tail = 0;
volatile uint8_t is_transmitting = 0;

void SPI_CS_Enable(void) {
    HAL_GPIO_WritePin(SPI_CS_GPIO_PORT, SPI_CS_PIN, GPIO_PIN_RESET); // Active LOW
}

void SPI_CS_Disable(void) {
    HAL_GPIO_WritePin(SPI_CS_GPIO_PORT, SPI_CS_PIN, GPIO_PIN_SET); // Inactive HIGH
}


// For sending floats with type prefix
HAL_StatusTypeDef Transmit_SensorVals(uint8_t val_type, float value) {
    if (queue_tail + 1 % SPI_QUEUE_SIZE == queue_head) return HAL_BUSY;

    __disable_irq();
    SPI_queue[queue_tail].msg_type = MSG_FLOAT_WITH_TYPE;
    SPI_queue[queue_tail].payload.float_msg.val_type = val_type;
    SPI_queue[queue_tail].payload.float_msg.value = value;
    queue_tail = (queue_tail + 1) % SPI_QUEUE_SIZE;
    __enable_irq();

    if (!is_transmitting) return Process_SPI_Queue();
    return HAL_OK;
}

// For sending raw 16-bit values
HAL_StatusTypeDef Transmit_ErrorCode(uint16_t value) {
    if (queue_tail + 1 % SPI_QUEUE_SIZE == queue_head) return HAL_BUSY;

    __disable_irq();
    SPI_queue[queue_tail].msg_type = MSG_RAW_16BIT;
    SPI_queue[queue_tail].payload.raw_16bit = value;
    queue_tail = (queue_tail + 1) % SPI_QUEUE_SIZE;
    __enable_irq();

    if (!is_transmitting) return Process_SPI_Queue();
    return HAL_OK;
}

//Add to Queue if busy

HAL_StatusTypeDef Process_SPI_Queue(void) {
    if (queue_head == queue_tail) {
        is_transmitting = 0;
        return HAL_OK;
    }

    is_transmitting = 1;
    __attribute__((aligned(4))) static uint8_t tx_buffer[5];
    uint8_t data_size;

    switch(SPI_queue[queue_head].msg_type) {
        case MSG_FLOAT_WITH_TYPE:
            tx_buffer[0] = SPI_queue[queue_head].payload.float_msg.val_type;
            memcpy(&tx_buffer[1], (const void*)&SPI_queue[queue_head].payload.float_msg.value, 4);
            data_size = 5;
            break;

        case MSG_RAW_16BIT:
            memcpy(tx_buffer, (const void*)&SPI_queue[queue_head].payload.raw_16bit, 2);
            data_size = 2;
            break;

        default:
            return HAL_ERROR;
    }

    queue_head = (queue_head + 1) % SPI_QUEUE_SIZE;

    SPI_CS_Enable();
    HAL_StatusTypeDef status = HAL_SPI_Transmit_DMA(SPI, tx_buffer, data_size);
    if (status != HAL_OK) {
        SPI_CS_Disable();
        is_transmitting = 0;
    }
    return status;
}

// --- SPI Transmission Complete Callback ---
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi == SPI) {
        // Process next message (if any)
    	SPI_CS_Disable();
        Process_SPI_Queue();
    }
}

// --- SPI Error Handling ---
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
	if (hspi == SPI) {
		HAL_SPI_Abort(SPI);  // Force-release SPI bus
	    is_transmitting = 0;
	    Process_SPI_Queue();  // Retry
	}
}


#elif RECU




//RECU CODE




#else
#error "Both FECU and RECU is not defined"
#endif

