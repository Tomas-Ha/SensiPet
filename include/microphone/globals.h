#pragma once

#include "stm32l475e_iot01_audio.h"
#include <cstdlib>

// Microphone globals
extern uint16_t PCM_Buffer[PCM_BUFFER_LEN / 2];
extern BSP_AUDIO_Init_t MicParams;

// Place to store final audio (alloc on the heap), here two seconds...
extern size_t AUDIO_RECORDING_LENGTH;
extern size_t TARGET_AUDIO_BUFFER_NB_SAMPLES;
extern int16_t *TARGET_AUDIO_BUFFER;
extern size_t TARGET_AUDIO_BUFFER_IX;

extern float max_amp_change;
extern float avg_amplitude;
extern float max_amplitude;
extern float avg_in_decibels;
extern float max_in_decibels;

extern void target_audio_buffer_full();
extern void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance);
extern void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance);
extern void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance);
extern void start_recording();
extern int init_microphone();