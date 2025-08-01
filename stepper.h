#ifndef STEPPER_H
    #define STEPPER_H

        #include <stdio.h>
        #include <stdlib.h>
        #include "pico/stdlib.h"
        #include "hardware/dma.h"
        #include "hardware/pio.h"
        #include "hardware/timer.h"
        #include "hardware/clocks.h"
        #include "hardware/uart.h"
        #include "stepper.pio.h"
        
        #define LEFT -1
        #define RIGHT 1

        #define BUFFER_SIZE 5000
        typedef struct {
            uint32_t steps;
            uint32_t freq; 
        } MovementCommand;


        typedef struct {
            int StepsToTake;
            bool activedir;
            int dir;
            float ActiveAngle; 
            int DIR_PIN;
            int STEP_PIN;
            int ENABLE_PIN;
            int HOME_PIN;
            int FULLSTEP;
            int directionchangeDelayCounter;
            PIO stm_pio;
            int stm_sm;
            MovementCommand buffer[BUFFER_SIZE];
            int buffer_head;   // posición de lectura
            int buffer_tail;   // posición de escritura
            int buffer_length; // número de elementos válidos
            bool running;
            float actual_speed;
            int dma_chan;
            int current_transfer_count;  // Para contar pasos en DMA
        } STEPPER_DRV;


        void setupGPIO(STEPPER_DRV * stepper);
        void setupStepper(STEPPER_DRV * stepper, uint8_t enable_pin, uint8_t dir_pin, uint8_t step_pin);
        void setupPIO(STEPPER_DRV * stepper);
        void pio0_irq0_handler();

        
        void setFrec( STEPPER_DRV * stepper,float desired_freq);
        void moveSteps( STEPPER_DRV * stepper, int steps );
        void setDirection( STEPPER_DRV * stepper,int direction);
        void moveStepsAtSpeed(STEPPER_DRV * stepper, int steps, float frec);
        void enqueueMovement(STEPPER_DRV* stepper, int steps, int freq);
        void startNextMove(STEPPER_DRV* stepper);

        uint8_t generateAccelerationRamp(STEPPER_DRV *stepper, float t_accel, float v_start, float accel);
        uint8_t generateSmoothAccelerationRamp(STEPPER_DRV *stepper, float t_accel, float v_start, float accel);

        void generateTrapezoidalProfile(STEPPER_DRV *stepper, int total_steps, float v_start, float v_max, float v_end, float accel);
        void generateSmoothSProfile(STEPPER_DRV *stepper, int total_steps, float v_start, float v_max, float v_end, float accel) ;
        void generateMotionProfile(STEPPER_DRV *stepper, int total_steps, float v_start, float v_max, float v_end, float accel, bool use_s_curve);

#endif