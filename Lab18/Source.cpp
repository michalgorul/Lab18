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

       // AudioFile<float> a;
       // bool loadedOK = a.load(inputFilePath);

        /** If you hit this assert then the file path above
         probably doesn't refer to a valid audio file */

        //---------------------------------------------------------------
        // 3. Let's apply a gain to every audio sample

        float gain = 0.5f;


        // 1. Create an AudioBuffer 
// (BTW, AudioBuffer is just a vector of vectors)
        AudioFile<double> audioFile;
        bool loadedOK = audioFile.load(inputFilePath);
        assert(loadedOK);

        AudioFile<double>::AudioBuffer buffer;

        // 2. Set to (e.g.) two channels
        buffer.resize(2);

        // 3. Set number of samples per channel
        buffer[0].resize(audioFile.getNumSamplesPerChannel());
        buffer[1].resize(audioFile.getNumSamplesPerChannel());

        // 4. do something here to fill the buffer with samples, e.g.

// then...

        int numChannels = 2;
        int numSamplesPerChannel = 44100;
        float sampleRate = 44100.f;
        //float frequency = 440.f;


        int ECHO = audioFile.getNumSamplesPerChannel();

        int echooffset = 44100; // chenge to delay more example int echooffset=1024*6;
        int echooffsetplay = 44100;


        for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
        {

            float sample = (audioFile.samples[0][i] + audioFile.samples[1][i]) / 2;
            

            buffer[0][echooffset] = audioFile.samples[0][i] * 0.5;
            buffer[1][echooffset] = audioFile.samples[1][i] * 0.5;
            
            if (echooffset++ >= ECHO - 1)
                echooffset = 0;

            audioFile.samples[0][i] += buffer[0][echooffsetplay] * 0.4;// amount to mix into dry
            audioFile.samples[1][i] += buffer[1][echooffsetplay] * 0.4;// amount to mix into dry

            if (echooffsetplay++ >= ECHO - 1)
                echooffsetplay = 0;

 
        }

        audioFile.setAudioBuffer(buffer);

        //---------------------------------------------------------------
        // 4. Write audio file to disk

        std::string outputFilePath = "quieter-audio-filer.wav"; // change this to somewhere useful for you
        audioFile.save(outputFilePath, AudioFileFormat::Aiff);
    }
}
