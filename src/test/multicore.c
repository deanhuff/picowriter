#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/irq.h"
#include "hardware/adc.h"
 
// Core 1 interrupt Handler
void core1_interrupt_handler() {

    // Receive Raw Value, Convert and Print Temperature Value
    while (multicore_fifo_rvalid()){
        uint16_t raw = multicore_fifo_pop_blocking();
        const float conversion_factor = 3.3f / (1 << 12);
        float result = raw * conversion_factor;
        float temp = 27 - (result - 0.706)/0.001721;
        printf("Temp = %f C\n", temp);        
    }

    multicore_fifo_clear_irq(); // Clear interrupt
}

// Core 1 Main Code
void core1_entry() {
    // Configure Core 1 Interrupt
    multicore_fifo_clear_irq();
    irq_set_exclusive_handler(SIO_IRQ_PROC1, core1_interrupt_handler);

    irq_set_enabled(SIO_IRQ_PROC1, true);

    // Infinte While Loop to wait for interrupt
    while (1){
        tight_loop_contents();
    }
}

// Core 0 Main Code
int main(void){
    stdio_init_all();

    multicore_launch_core1(core1_entry); // Start core 1 - Do this before any interrupt configuration

    // Configure the ADC
    adc_init();
    adc_set_temp_sensor_enabled(true); // Enable on board temp sensor
    adc_select_input(4);

    // Primary Core 0 Loop
    while (1) {
        uint16_t raw = adc_read();
        multicore_fifo_push_blocking(raw);
        sleep_ms(1000);
    }
}
