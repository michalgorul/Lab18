#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "AudioFile.h"

//=======================================================================
namespace examples
{
    void writeSineWaveToAudioFile();
    void loadAudioFileAndPrintSummary();
    void loadAudioFileAndProcessSamples();
    void echoEffect(AudioFile<double> &audioFile, float &gain);
    void fourEchoEffect(AudioFile<double>& audioFile, float& gain);
    void fourEchoEffectCleaner(AudioFile<double>& audioFile, float& gain);
    void reverbffect(AudioFile<double>& audioFile, float& decay);
}

//=======================================================================
int main()
{
    //---------------------------------------------------------------
    /** Writes a sine wave to an audio file */
    //examples::writeSineWaveToAudioFile();

    //---------------------------------------------------------------
    /** Loads an audio file and prints key details to the console*/
    //examples::loadAudioFileAndPrintSummary();

    //---------------------------------------------------------------
    /** Loads an audio file and processess the samples */
    examples::loadAudioFileAndProcessSamples();

    return 0;
}

//=======================================================================
namespace examples
{
    //=======================================================================
    void writeSineWaveToAudioFile()
    {
        //---------------------------------------------------------------
        std::cout << "**********************" << std::endl;
        std::cout << "Running Example: Write Sine Wave To Audio File" << std::endl;
        std::cout << "**********************" << std::endl << std::endl;

        //---------------------------------------------------------------
        // 1. Let's setup our AudioFile instance

        AudioFile<float> a;
        a.setNumChannels(2);
        a.setNumSamplesPerChannel(44100);

        //---------------------------------------------------------------
        // 2. Create some variables to help us generate a sine wave

        const float sampleRate = 44100.f;
        const float frequencyInHz = 440.f;

        //---------------------------------------------------------------
        // 3. Write the samples to the AudioFile sample buffer

        for (int i = 0; i < a.getNumSamplesPerChannel(); i++)
        {
            for (int channel = 0; channel < a.getNumChannels(); channel++)
            {
                a.samples[channel][i] = sin((static_cast<float> (i) / sampleRate) * frequencyInHz * 2.f * M_PI);
            }
        }

        //---------------------------------------------------------------
        // 4. Save the AudioFile

        std::string filePath = "sine-wave.wav"; // change this to somewhere useful for you
        a.save("sine-wave.wav", AudioFileFormat::Wave);
    }

    //=======================================================================
    void loadAudioFileAndPrintSummary()
    {
        //---------------------------------------------------------------
        std::cout << "**********************" << std::endl;
        std::cout << "Running Example: Load Audio File and Print Summary" << std::endl;
        std::cout << "**********************" << std::endl << std::endl;

        //---------------------------------------------------------------
        // 1. Set a file path to an audio file on your machine
        const std::string filePath = "test-audio.wav";

        //---------------------------------------------------------------
        // 2. Create an AudioFile object and load the audio file

        AudioFile<float> a;
        bool loadedOK = a.load(filePath);

        /** If you hit this assert then the file path above
         probably doesn't refer to a valid audio file */
        assert(loadedOK);

        //---------------------------------------------------------------
        // 3. Let's print out some key details

        std::cout << "Bit Depth: " << a.getBitDepth() << std::endl;
        std::cout << "Sample Rate: " << a.getSampleRate() << std::endl;
        std::cout << "Num Channels: " << a.getNumChannels() << std::endl;
        std::cout << "Length in Seconds: " << a.getLengthInSeconds() << std::endl;
        std::cout << std::endl;
    }

    //=======================================================================
    void loadAudioFileAndProcessSamples()
    {
        //---------------------------------------------------------------
        std::cout << "**********************" << std::endl;
        std::cout << "Running Example: Load Audio File and Process Samples" << std::endl;
        std::cout << "**********************" << std::endl << std::endl;

        //---------------------------------------------------------------
        // 1. Set a file path to an audio file on your machine
        const std::string inputFilePath = "test-audio.wav";

        //---------------------------------------------------------------
        // 2. Create an AudioFile object and load the audio file

        AudioFile<double> audioFile;
        bool loadedOK = audioFile.load(inputFilePath);
        assert(loadedOK);

        /** If you hit this assert then the file path above
         probably doesn't refer to a valid audio file */

        //---------------------------------------------------------------
        // 3. Let's apply a gain to every audio sample

        float gain = 0.5f;     
        float decay = 0.45f;     

        // 4. do something here to fill the buffer with samples, e.g. echo

        //examples::echoEffect(audioFile, gain);

        //examples::fourEchoEffect(audioFile, gain);

        examples::fourEchoEffectCleaner(audioFile, gain);

        //examples::reverbffect(audioFile, decay);

        //---------------------------------------------------------------
        // 5. Write audio file to disk

        std::string outputFilePath = "four-echo.wav"; // change this to somewhere useful for you
        audioFile.save(outputFilePath, AudioFileFormat::Aiff);
    }

    void echoEffect(AudioFile<double> &audioFile, float &gain)
    {
        int echoOffset = 44100;     // 1 sec

        AudioFile<double>::AudioBuffer buffer;

        // 2. Set to (e.g.) two channels
        buffer.resize(2);

        // 3. Set number of samples per channel
        buffer[0].resize(audioFile.getNumSamplesPerChannel());
        buffer[1].resize(audioFile.getNumSamplesPerChannel());

        for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
        {

            buffer[0][echoOffset] = audioFile.samples[0][i] * gain;
            buffer[1][echoOffset] = audioFile.samples[1][i] * gain;

            if (echoOffset++ >= audioFile.getNumSamplesPerChannel() - 1)
                echoOffset = 0;
        }

        for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
        {

            audioFile.samples[0][i] += buffer[0][i];
            audioFile.samples[1][i] += buffer[1][i];

        }
    }

    void fourEchoEffectCleaner(AudioFile<double>& audioFile, float& gain)
    {
        int echoOffset = 22050;     // 1 sec

        AudioFile<double>::AudioBuffer buffer;

        // 2. Set to (e.g.) two channels
        buffer.resize(2);

        // 3. Set number of samples per channel
        buffer[0].resize(audioFile.getNumSamplesPerChannel());
        buffer[1].resize(audioFile.getNumSamplesPerChannel());

        for (int i = 0; i < audioFile.getNumSamplesPerChannel() - 1 - (4*echoOffset); i++)
        {

            for (int j = 1; j < 5; j++)
            {
                buffer[0][(echoOffset * j) + i] = audioFile.samples[0][i] * gain;
                buffer[1][(echoOffset * j) + i] = audioFile.samples[1][i] * gain;
            }

           
        }

        for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
        {

            audioFile.samples[0][i] += buffer[0][i];
            audioFile.samples[1][i] += buffer[1][i];

        }
    }

    void fourEchoEffect(AudioFile<double>& audioFile, float& gain)
    {
        int echooffset1 = 22050;     // 0.5 sec
        int echooffset2 = 44100;     // 1 sec
        int echooffset3 = 66150;     // 1.5 sec
        int echooffset4 = 88200;     // 2 sec

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

            buffer2[0][echooffset2] = audioFile.samples[0][i] * gain * 0.8;
            buffer2[1][echooffset2] = audioFile.samples[1][i] * gain * 0.8;

            if (echooffset2++ >= audioFile.getNumSamplesPerChannel() - 1)
                echooffset2 = 0;

            buffer3[0][echooffset3] = audioFile.samples[0][i] * gain * 0.6;
            buffer3[1][echooffset3] = audioFile.samples[1][i] * gain * 0.6;

            if (echooffset3++ >= audioFile.getNumSamplesPerChannel() - 1)
                echooffset3 = 0;

            buffer4[0][echooffset4] = audioFile.samples[0][i] * gain * 0.4;
            buffer4[1][echooffset4] = audioFile.samples[1][i] * gain * 0.4;

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
}
