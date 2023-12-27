#ifndef DMA_H
#define DMA_H

#include <stdint.h>

// Function to initialize DMA for audio playback
void Init_DMA_For_Playback(uint16_t *source, uint32_t count);

// Function to start DMA playback
void Start_DMA_Playback(void);

// Callback function to switch between different audio waveforms
void CallBack_Function(void);

// Function to initialize and play audio tones using DMA and a DAC
void Play_Tone_with_DMA(void);

#endif  // DMA_H
