#include "microphone/globals.h"
#include "screen/globals.h"
#include "sprites.h"
#include "states/globals.h"

uint16_t PCM_Buffer[PCM_BUFFER_LEN / 2];
BSP_AUDIO_Init_t MicParams;

size_t AUDIO_RECORDING_LENGTH = 10; // Audio recording length in ms (sensitivity)
size_t TARGET_AUDIO_BUFFER_NB_SAMPLES = (size_t) AUDIO_SAMPLING_FREQUENCY * ((double) AUDIO_RECORDING_LENGTH / 1000);
int16_t *TARGET_AUDIO_BUFFER = (int16_t*)calloc(TARGET_AUDIO_BUFFER_NB_SAMPLES, sizeof(int16_t));
size_t TARGET_AUDIO_BUFFER_IX = 0;

float max_amp_change = 20;
float max_in_decibels = 10000;

// callback that gets invoked when TARGET_AUDIO_BUFFER is full
void target_audio_buffer_full() {
    int32_t ret;
    uint32_t state;

    ret = BSP_AUDIO_IN_GetState(AUDIO_INSTANCE, &state);
    if (ret != BSP_ERROR_NONE) {
        printf("Cannot start recording: Error getting audio state (%d)\n", ret);
        return;
    }
    if (state != AUDIO_IN_STATE_STOP) {
        printf("Cannot process data: Still recording\n");
        return;
    }

    float new_max_amplitude = -std::numeric_limits<float>::infinity();;
    int16_t *buf = (int16_t*)TARGET_AUDIO_BUFFER;
    for (size_t ix = 0; ix < TARGET_AUDIO_BUFFER_NB_SAMPLES; ix++) {
        if (abs(buf[ix]) > new_max_amplitude) new_max_amplitude = abs(buf[ix]);
    }
    // Equation found here: https://microsoft.public.win32.programmer.directx.audio.narkive.com/xXQQgJG4/how-to-convert-wav-samples-to-decibels
    float new_max_in_decibels = 20 * log10 (abs(new_max_amplitude));
    // float new_min_in_decibels = 20 * log10 (abs(new_min_amplitude));

    // Check for jumpscare
    if (new_max_in_decibels - max_in_decibels >= max_amp_change) {
        printf("SCARED\n");
        gSensiPet.update_state(Action::SCARED);
        printf("MAX: %f => %f\n", new_max_amplitude, new_max_in_decibels);
        printf("MAX_DIFF: %f\n\n", new_max_in_decibels - max_in_decibels);
    }

    // Update Values
    max_in_decibels = new_max_in_decibels;
    // TODO: track multiple statistics like median, min, max
}

/**
* @brief  Half Transfer user callback, called by BSP functions.
* @param  None
* @retval None
*/
void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance) {

    uint32_t buffer_size = PCM_BUFFER_LEN / 2; /* Half Transfer */
    uint32_t nb_samples = buffer_size / sizeof(int16_t); /* Bytes to Length */

    if ((TARGET_AUDIO_BUFFER_IX + nb_samples) > TARGET_AUDIO_BUFFER_NB_SAMPLES) {
        return;
    }

    /* Copy first half of PCM_Buffer from Microphones onto Fill_Buffer */
    memcpy(((uint8_t*)TARGET_AUDIO_BUFFER) + (TARGET_AUDIO_BUFFER_IX * 2), PCM_Buffer, buffer_size);
    TARGET_AUDIO_BUFFER_IX += nb_samples;

    if (TARGET_AUDIO_BUFFER_IX >= TARGET_AUDIO_BUFFER_NB_SAMPLES) {
        // pause audio stream
        int32_t ret = BSP_AUDIO_IN_Stop(AUDIO_INSTANCE);
        if (ret != BSP_ERROR_NONE) {
            printf("Error Audio Stop (%d)\n", ret);
        }
        gSensiPet.get_eq()->call(target_audio_buffer_full);
        return;
    }
}

/**
* @brief  Transfer Complete user callback, called by BSP functions.
* @param  None
* @retval None
*/
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance) {

    uint32_t buffer_size = PCM_BUFFER_LEN / 2; /* Half Transfer */
    uint32_t nb_samples = buffer_size / sizeof(int16_t); /* Bytes to Length */

    if ((TARGET_AUDIO_BUFFER_IX + nb_samples) > TARGET_AUDIO_BUFFER_NB_SAMPLES) {
        return;
    }

    /* Copy second half of PCM_Buffer from Microphones onto Fill_Buffer */
    memcpy(((uint8_t*)TARGET_AUDIO_BUFFER) + (TARGET_AUDIO_BUFFER_IX * 2),
        ((uint8_t*)PCM_Buffer) + (nb_samples * 2), buffer_size);
    TARGET_AUDIO_BUFFER_IX += nb_samples;

    if (TARGET_AUDIO_BUFFER_IX >= TARGET_AUDIO_BUFFER_NB_SAMPLES) {
        // pause audio stream
        int32_t ret = BSP_AUDIO_IN_Stop(AUDIO_INSTANCE);
        if (ret != BSP_ERROR_NONE) {
            printf("Error Audio Stop (%d)\n", ret);
        }
        gSensiPet.get_eq()->call(target_audio_buffer_full);
        return;
    }
}

/**
  * @brief  Manages the BSP audio in error event.
  * @param  Instance Audio in instance.
  * @retval None.
  */
void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance) {
    printf("BSP_AUDIO_IN_Error_CallBack\n");
}

void start_recording() {
    int32_t ret;
    uint32_t state;

    ret = BSP_AUDIO_IN_GetState(AUDIO_INSTANCE, &state);
    if (ret != BSP_ERROR_NONE) {
        printf("Cannot start recording: Error getting audio state (%d)\n", ret);
        return;
    }
    if (state == AUDIO_IN_STATE_RECORDING) {
        printf("Cannot start recording: Already recording\n");
        return;
    }

    // reset audio buffer location
    TARGET_AUDIO_BUFFER_IX = 0;

    ret = BSP_AUDIO_IN_Record(AUDIO_INSTANCE, (uint8_t *) PCM_Buffer, PCM_BUFFER_LEN);
    if (ret != BSP_ERROR_NONE) {
        printf("Error Audio Record (%d)\n", ret);
        return;
    }
}

int init_microphone()
{
    if (!TARGET_AUDIO_BUFFER) {
        printf("Failed to allocate TARGET_AUDIO_BUFFER buffer\n");
        return 0;
    }

    // set up the microphone
    MicParams.BitsPerSample = 16;
    MicParams.ChannelsNbr = AUDIO_CHANNELS;
    MicParams.Device = AUDIO_IN_DIGITAL_MIC1;
    MicParams.SampleRate = AUDIO_SAMPLING_FREQUENCY;
    MicParams.Volume = 32;

    int32_t ret = BSP_AUDIO_IN_Init(AUDIO_INSTANCE, &MicParams);

    if (ret != BSP_ERROR_NONE) {
        printf("Error Audio Init (%d)\r\n", ret);
        return 1;
    } else {
        printf("OK Audio Init\t(Audio Freq=%d)\r\n", AUDIO_SAMPLING_FREQUENCY);
    }

    return 0;
}