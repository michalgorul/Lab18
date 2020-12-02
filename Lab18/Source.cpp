#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "AudioFile.h"


void loadAudioFileAndProcessSamples();
void echoEffect(AudioFile<double> &audioFile, float &gain);
void fourEchoEffect(AudioFile<double>& audioFile, float& gain);
void fourEchoEffectCleaner(AudioFile<double>& audioFile, float& gain);
void reverbffect(AudioFile<double>& audioFile, float& decay);
void infiniteEcho(AudioFile<double>& audioFile, float gain);
void overdriveEffect(AudioFile<double>& audioFile);


int main()
{
    loadAudioFileAndProcessSamples();

    return 0;
}

void loadAudioFileAndProcessSamples()
{
    std::cout << "**********************" << std::endl;
    std::cout << "Load Audio File and Process Samples" << std::endl;
    std::cout << "**********************" << std::endl << std::endl;

    const std::string inputFilePath = "Alesis-Fusion-Clean-Guitar-C3.wav";

    AudioFile<double> audioFile;
    bool loadedOK = audioFile.load(inputFilePath);
    assert(loadedOK);

    float gain = 0.5f;     
    float decay = 0.45f;     

    //echoEffect(audioFile, gain);
    //fourEchoEffect(audioFile, gain);
    //fourEchoEffectCleaner(audioFile, gain);
    //infiniteEcho(audioFile, gain);
    //reverbffect(audioFile, decay);
    overdriveEffect(audioFile);

    std::string outputFilePath = "OVERDRIVE.wav";
    audioFile.save(outputFilePath);
}

void echoEffect(AudioFile<double> &audioFile, float &gain)
{
    int echoOffset = 44100;                                     // 1 sec offset

    AudioFile<double>::AudioBuffer buffer;                      // declaring a buffer

    buffer.resize(2);                                           // buffer has also two channels

    buffer[0].resize(audioFile.getNumSamplesPerChannel());      // buffer size is the same as input
    buffer[1].resize(audioFile.getNumSamplesPerChannel());

    for (int i = 0; i < audioFile.getNumSamplesPerChannel() - 1; i++)
    {
        buffer[0][echoOffset] = audioFile.samples[0][i] * gain;     // buffer plus offset equals value
        buffer[1][echoOffset] = audioFile.samples[1][i] * gain;     // of input at the same place, but
                                                                    // it is twice quieter
        echoOffset++;

        if (echoOffset >= audioFile.getNumSamplesPerChannel() - 1 || // if offset is bigger than
            echoOffset > 44100 * 2)                                  // input size - break
            break;  
    }

    for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
    {
        audioFile.samples[0][i] += buffer[0][i];                    // adding buffer to input file
        audioFile.samples[1][i] += buffer[1][i];                    // to get echo effect
    }   
}

void fourEchoEffectCleaner(AudioFile<double>& audioFile, float& gain)
{
    const int offset = 44100;
    int echoOffset = 44100;                                         // 1 sec offset

    AudioFile<double>::AudioBuffer buffer;

    buffer.resize(2);                                               // buffer has also two channels

    buffer[0].resize(audioFile.getNumSamplesPerChannel());          // buffer size is the same as input
    buffer[1].resize(audioFile.getNumSamplesPerChannel());
    
    for (int i = 0; i < audioFile.getNumSamplesPerChannel() - 1; i++)
    {
        echoOffset = offset + i;
        for (int j = 1; j < 5; j++)                                     // a loop to get a 4-echo
        {
            if (echoOffset >= audioFile.getNumSamplesPerChannel() - 1)  // if offset is bigger than
                break;                                                  // input size - break

            buffer[0][echoOffset] += audioFile.samples[0][i] * gain;    // buffer plus offset equals value
            buffer[1][echoOffset] += audioFile.samples[1][i] * gain;    // of input at the same place plus itself,
                                                                        // but it is twice quieter
            echoOffset += offset;                                       
        }
    }

    for (int i = 0; i < audioFile.getNumSamplesPerChannel() - 1; i++)
    {
        audioFile.samples[0][i] += buffer[0][i];                        // adding buffer to input file
        audioFile.samples[1][i] += buffer[1][i];                        // to get four-echo effect
    }
}

void infiniteEcho(AudioFile<double> &audioFile, float gain) {

    const int offset = 22050;
    int echoOffset = 22050;     // 0.5 sec offset

    AudioFile<double>::AudioBuffer buffer;

    buffer.resize(2);                                               // buffer has also two channels

    buffer[0].resize(audioFile.getNumSamplesPerChannel());          // buffer size is the same as input
    buffer[1].resize(audioFile.getNumSamplesPerChannel());

    for (int i = 0; i < audioFile.getNumSamplesPerChannel() - 1; i++)
    {
        gain = 0.5f;                                                    // gain is as at the begining       
        echoOffset = offset + i;

        for (int j = 1; j < 10; j++)                                    // loop to get infinite-echo
        {
            if (echoOffset >= audioFile.getNumSamplesPerChannel() - 1)
                break;

            buffer[0][echoOffset] += audioFile.samples[0][i] * gain;    // buffer plus offset equals value
            buffer[1][echoOffset] += audioFile.samples[1][i] * gain;    // of input at the same place plus itself,
                                                                        // but with next iteration it gets quieter
            echoOffset += offset;                                       // and quieter
            gain *= 0.7f;

        }
    }

    for (int i = 0; i < audioFile.getNumSamplesPerChannel() - 1; i++)
    {
        audioFile.samples[0][i] += buffer[0][i];                        // adding buffer to input file
        audioFile.samples[1][i] += buffer[1][i];                        // to get infinite-echo effect
    }
}

void fourEchoEffect(AudioFile<double>& audioFile, float& gain)
{
    int echooffset1 = 44100;         // 1 sec
    int echooffset2 = 44100 *2;     // 2 sec
    int echooffset3 = 44100 * 3;     // 3 sec
    int echooffset4 = 44100 * 4;     // 4 sec

    AudioFile<double>::AudioBuffer buffer1;
    buffer1.resize(2);
    buffer1[0].resize(audioFile.getNumSamplesPerChannel());
    buffer1[1].resize(audioFile.getNumSamplesPerChannel());

    AudioFile<double>::AudioBuffer buffer2;
    buffer2.resize(2);
    buffer2[0].resize(audioFile.getNumSamplesPerChannel());
    buffer2[1].resize(audioFile.getNumSamplesPerChannel());

    AudioFile<double>::AudioBuffer buffer3;
    buffer3.resize(2);
    buffer3[0].resize(audioFile.getNumSamplesPerChannel());
    buffer3[1].resize(audioFile.getNumSamplesPerChannel());

    AudioFile<double>::AudioBuffer buffer4;
    buffer4.resize(2);
    buffer4[0].resize(audioFile.getNumSamplesPerChannel());
    buffer4[1].resize(audioFile.getNumSamplesPerChannel());

    for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
    {
        buffer1[0][echooffset1] = audioFile.samples[0][i] * gain;
        buffer1[1][echooffset1] = audioFile.samples[1][i] * gain;

        if (echooffset1++ >= audioFile.getNumSamplesPerChannel() - 1)
            echooffset1 = 0;

        buffer2[0][echooffset2] = audioFile.samples[0][i] * gain;
        buffer2[1][echooffset2] = audioFile.samples[1][i] * gain;

        if (echooffset2++ >= audioFile.getNumSamplesPerChannel() - 1)
            echooffset2 = 0;

        buffer3[0][echooffset3] = audioFile.samples[0][i] * gain;
        buffer3[1][echooffset3] = audioFile.samples[1][i] * gain;

        if (echooffset3++ >= audioFile.getNumSamplesPerChannel() - 1)
            echooffset3 = 0;

        buffer4[0][echooffset4] = audioFile.samples[0][i] * gain;
        buffer4[1][echooffset4] = audioFile.samples[1][i] * gain;

        if (echooffset4++ >= audioFile.getNumSamplesPerChannel() - 1)
            echooffset4 = 0;
    }

    for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
    {
        audioFile.samples[0][i] += buffer1[0][i] + buffer2[0][i] + buffer3[0][i] + buffer4[0][i];
        audioFile.samples[1][i] += buffer1[1][i] + buffer2[1][i] + buffer3[1][i] + buffer4[0][i];
    }
}

void reverbffect(AudioFile<double>& audioFile, float& decay)
{
    int echoOffset = 3000;     // 68 ms

    for (int i = 0; i < audioFile.getNumSamplesPerChannel() - echoOffset - 1; i++)
    {
        audioFile.samples[0][echoOffset + i] += audioFile.samples[0][i] * decay;
        audioFile.samples[1][echoOffset + i] += audioFile.samples[1][i] * decay;
    }

}

void overdriveEffect(AudioFile<double>& audioFile)
{

    // PARAMETER SETTING

    float boost = 0.2;                     // user may specify boost 
                            
    float gain = ((boost / 100) * 100) + 1; // Equation 01 is used to rescaling input signal 
                                            // by adjusting the �Boost� value. After we get the gain,
                                            // we can multiply gain to the input signal to get the boosted signal.

    float drive = 50.0;                     // user may specify boost 
                                            // range of the drive parameter �Drive� is 0 < drive < 100
                                            // In short, 0% �Drive� value lead to a copied
                                            // output while 100 % produce a square - liked output.

    float a = sin(((drive + 1) / 101) * (M_PI / 2));

    float k = 2 * a / (1 - a);


    // x = (1 + k) * x / (1 + k * abs(x));        // to zrobi� z ka�dym elementem w tablicy


    // INPUT SIGNAL PROCESSING

    for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
    {
        audioFile.samples[0][i] *= gain;        // Manipulate the input signal by the gain function
        audioFile.samples[1][i] *= gain;
    }

    // processed the gain of input signal, and return a gained signal


    //  GAINED SIGNAL PROCESSING

    for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
    {
        // Manipulate the Mgained signal by calling drive function.

        audioFile.samples[0][i] = (1 + k) * audioFile.samples[0][i] / (1 + k * abs(audioFile.samples[0][i]));
        audioFile.samples[1][i] = (1 + k) * audioFile.samples[0][i] / (1 + k * abs(audioFile.samples[0][i]));
    }

    // To distort the gained signal, and return an distorted output signal.

    // DISTORTED SIGNAL PROCESSING

    // poczyta� to: http://www.exstrom.com/journal/sigproc/

    // Defines a butterworth lowpass filter with center freq. 0.08 * sampling frequency
    // Note: this processor only works with 'double'
    // this filter is used as anti-aliasing filter from 10kHz sampling rate down to 4kHz

    //const int NZEROS = 6;
    //const int NPOLES = 6;
    //const double GAIN = 9.339780497e+03;

    //double xv[NZEROS + 1] = { 0.0 }, yv[NPOLES + 1] = { 0.0 };

    //for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
    //{
    //    xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4]; xv[4] = xv[5]; xv[5] = xv[6];
    //    xv[6] = audioFile.samples[0][i] / GAIN;
    //    xv[6] = audioFile.samples[1][i] / GAIN;

    //    yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4]; yv[4] = yv[5]; yv[5] = yv[6];
    //    yv[6] = (xv[0] + xv[6]) + 6.0 * (xv[1] + xv[5]) + 15.0 * (xv[2] + xv[4])
    //        + 20.0 * xv[3]
    //        + (-0.1396600417 * yv[0]) + (1.1086708553 * yv[1])
    //        + (-3.7230194289 * yv[2]) + (6.7850160254 * yv[3])
    //        + (-7.0995038188 * yv[4]) + (4.0616439992 * yv[5]);

    //    audioFile.samples[0][i] = yv[6];
    //    audioFile.samples[1][i] = yv[6];
   
    //}
}

