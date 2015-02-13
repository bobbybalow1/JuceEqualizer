/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>


//==============================================================================
PluginAudioProcessor::PluginAudioProcessor()
{
    uf1Freq = DEFAULT_U_F1_FREQ;
    uf2Freq = DEFAULT_U_F2_FREQ;
    uf3Freq = DEFAULT_U_F3_FREQ;
    uf1Q = uf2Q = uf3Q = DEFAULT_U_FILTER_Q;
    uf1GainDb = uf2GainDb = uf3GainDb = DEFAULT_U_FILTER_GAIN_DB;
    uOutputGainDb = DEFAULT_U_OUTPUT_GAIN_DB;
    
    af1Freq = DEFAULT_A_F1_FREQ;
    af2Freq = DEFAULT_A_F2_FREQ;
    af3Freq = DEFAULT_A_F3_FREQ;
    af1Q = af2Q = af3Q = DEFAULT_A_FILTER_Q;
    af1GainDb = af2GainDb = af3GainDb = DEFAULT_A_FILTER_GAIN_DB;
    aOutputGain = DEFAULT_A_OUTPUT_GAIN;
    
    filter1     = *new stk::BiQuad();
    filter2     = *new stk::BiQuad();
    filter3     = *new stk::BiQuad();
}

PluginAudioProcessor::~PluginAudioProcessor()
{
}

//==============================================================================
const String PluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int PluginAudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float PluginAudioProcessor::getParameter (int index)                    // Must return 0-1.f
{
    switch (index) {
        case outputGainParam:   return uOutputGainDb;
        case f1FreqParam:       return uf1Freq;
        case f1GainParam:       return uf1GainDb;
        case f1QParam:          return uf1Q;
        case f2FreqParam:       return uf2Freq;
        case f2GainParam:       return uf2GainDb;
        case f2QParam:          return uf2Q;
        case f3FreqParam:       return uf3Freq;
        case f3GainParam:       return uf3GainDb;
        case f3QParam:          return uf3Q;
        default:                return 0.0f;
    }
}

void PluginAudioProcessor::setParameter (int index, float newValue)     // newValue always 0-1.f
{
    switch (index) {
        case outputGainParam:
            uOutputGainDb = newValue;
            aOutputGain = powf(10, (-24 + (uOutputGainDb) * 48)/ 20);
            break;

        case f1FreqParam:
            uf1Freq = newValue;
            af1Freq = 20 + (uf1Freq * 19980);
            calculateFilter1Coefficients();
            break;
        case f1QParam:
            uf1Q = newValue;
            af1Q = 0.1f + (uf1Q * 9.9f);
            calculateFilter1Coefficients();
            break;
        case f1GainParam:
            uf1GainDb = newValue;
            af1GainDb = -24 + (uf1GainDb * 48);
            calculateFilter1Coefficients();
            break;

        case f2FreqParam:
            uf2Freq = newValue;
            af2Freq = 20 + (uf2Freq * 19980);
            calculateFilter2Coefficients();
            break;
        case f2QParam:
            uf2Q = newValue;
            af2Q = 0.1f + (uf2Q * 9.9f);
            calculateFilter2Coefficients();
            break;
        case f2GainParam:
            uf2GainDb = newValue;
            af2GainDb = -24 + (uf2GainDb * 48);
            calculateFilter2Coefficients();
            break;
            
        case f3FreqParam:
            uf3Freq = newValue;
            af3Freq = 20 + (uf3Freq * 19980);
            calculateFilter3Coefficients();
            break;
        case f3QParam:
            uf3Q = newValue;
            af3Q = 0.1f + (uf3Q * 9.9f);
            calculateFilter3Coefficients();
            break;
        case f3GainParam:
            uf3GainDb = newValue;
            af3GainDb = -24 + (uf3GainDb * 48);
            calculateFilter3Coefficients();
            break;
    }
}

float PluginAudioProcessor::getParameterDefaultValue (int index)        // Must return 0-1.f
{
    switch (index) {
        case outputGainParam:       return DEFAULT_U_OUTPUT_GAIN_DB;
        case f1FreqParam:           return DEFAULT_U_F1_FREQ;
        case f1QParam:              return DEFAULT_U_FILTER_Q;
        case f1GainParam:           return DEFAULT_U_FILTER_GAIN_DB;
        case f2FreqParam:           return DEFAULT_U_F2_FREQ;
        case f2QParam:              return DEFAULT_U_FILTER_Q;
        case f2GainParam:           return DEFAULT_U_FILTER_GAIN_DB;
        case f3FreqParam:           return DEFAULT_U_F3_FREQ;
        case f3QParam:              return DEFAULT_U_FILTER_Q;
        case f3GainParam:           return DEFAULT_U_FILTER_GAIN_DB;
        default:                    return 0.0f;
    }
}

const String PluginAudioProcessor::getParameterName (int index)
{
    switch (index) {
        case outputGainParam:   return String("Output Gain");
        case f1FreqParam:       return String("Filter1 Freq");
        case f1QParam:          return String("Filter1 Q");
        case f1GainParam:       return String("Filter1 Gain");
        case f2FreqParam:       return String("Filter2 Freq");
        case f2QParam:          return String("Filter2 Q");
        case f2GainParam:       return String("Filter2 Gain");
        case f3FreqParam:       return String("Filter3 Freq");
        case f3QParam:          return String("Filter3 Q");
        case f3GainParam:       return String("Filter3 Gain");
        default:                return String::empty;
    }
}

const String PluginAudioProcessor::getParameterText (int index)
{
    switch (index) {
        case outputGainParam:   return String((-24 + (uOutputGainDb) * 48), 1) + "db";
        case f1FreqParam:       return String(af1Freq, 0) + "Hz";
        case f1QParam:          return String(af1Q, 2);
        case f1GainParam:       return String(af1GainDb, 1) + "db";
        case f2FreqParam:       return String(af2Freq, 0) + "Hz";
        case f2QParam:          return String(af2Q, 2);
        case f2GainParam:       return String(af2GainDb, 1) + "db";
        case f3FreqParam:       return String(af3Freq, 0) + "Hz";
        case f3QParam:          return String(af3Q, 2);
        case f3GainParam:       return String(af3GainDb, 1) + "db";
        default:                return String(getParameter(index), 2);
    }
}

const String PluginAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PluginAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool PluginAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool PluginAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool PluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double PluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PluginAudioProcessor::setCurrentProgram (int index)
{
}

const String PluginAudioProcessor::getProgramName (int index)
{
    return String();
}

void PluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    fs = sampleRate;
    calculateFilter1Coefficients();
    calculateFilter2Coefficients();
    calculateFilter3Coefficients();
}

void PluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // If sampleRate changes and not previously detected
    if (fs != getSampleRate()) {
        fs = getSampleRate();
        calculateFilter1Coefficients();
        calculateFilter2Coefficients();
        calculateFilter3Coefficients();
    }
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        leftChannel[i]  = aOutputGain * filter3.tick(filter2.tick(filter1.tick(leftChannel[i])));
        rightChannel[i] = aOutputGain * filter3.tick(filter2.tick(filter1.tick(rightChannel[i])));
    }
}

//==============================================================================
bool PluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PluginAudioProcessor::createEditor()
{
    return new PluginAudioProcessorEditor (*this);
}

//==============================================================================
void PluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginAudioProcessor();
}

void PluginAudioProcessor::calculateFilter1Coefficients(/*filter_type_enum*/)
{
    // Low Shelf Filter
    // Frequency occurs left of slope
    // Isn't balanced when attenuating...
    af1A = powf(10.f, af1GainDb/40.f);
    af1w0 = 2.f * M_PI * af1Freq / fs;
    af1Alpha = sinf(af1w0)/(2.f * af1Q);
    
    tmp1 =      af1A * ( (af1A+1.f) - (af1A-1.f)*cosf(af1w0) + 2.f*sqrtf(af1A)*af1Alpha );
    tmp2 =  2.f*af1A * ( (af1A-1.f) - (af1A+1.f)*cosf(af1w0)                            );
    tmp3 =      af1A * ( (af1A+1.f) - (af1A-1.f)*cosf(af1w0) - 2.f*sqrtf(af1A)*af1Alpha );
    
    tmp4 =               (af1A+1.f) + (af1A-1.f)*cosf(af1w0) + 2.f*sqrtf(af1A)*af1Alpha;
    tmp5 =      -2.f * ( (af1A-1.f) + (af1A+1.f)*cosf(af1w0)                            );
    tmp6 =               (af1A+1.f) + (af1A-1.f)*cosf(af1w0) - 2.f*sqrtf(af1A)*af1Alpha;
    
    f1b0 = tmp1/tmp4; f1b1 = tmp2/tmp4; f1b2 = tmp3/tmp4;
    f1a1 = tmp5/tmp4; f1a2 = tmp6/tmp4;
    
    filter1.setCoefficients(f1b0, f1b1, f1b2, f1a1, f1a2);
}

void PluginAudioProcessor::calculateFilter2Coefficients(/*filter_type_enum*/)
{
    // Peaking Filter
    // Frequecy is skewed in this equation!!! (Peak isn't provided freq)
    af2A = powf(10.f, af2GainDb/40.f);
    af2w0 = 2.f * M_PI * af2Freq / fs;
    af2Alpha = sinf(af2w0)/(2.f * af2Q);
    
    tmp1 =  1.f + af2Alpha * af2A;
    tmp2 =  -2.f * cosf(af2w0);
    tmp3 =  1.f - af2Alpha * af2A;
    
    tmp4 =  1.f + af2Alpha / af2A;
    tmp5 =  tmp2;
    tmp6 =  1.f - af2Alpha / af2A;
    
    f2b0 = tmp1/tmp4; f2b1 = tmp2/tmp4; f2b2 = tmp3/tmp4;
    f2a1 = tmp5/tmp4; f2a2 = tmp6/tmp4;
    
    filter2.setCoefficients(f2b0, f2b1, f2b2, f2a1, f2a2);
}

void PluginAudioProcessor::calculateFilter3Coefficients(/*filter_type_enum*/)
{
    // High Shelf Filter
    // Frequency occurs left of slope
    // Isn't balanced when amplifying...
    af3A = powf(10.f, af3GainDb/40.f);
    af3w0 = 2.f * M_PI * af3Freq / fs;
    af3Alpha = sinf(af3w0)/(2.f * af3Q);
    
    tmp1 =      af3A * ( (af3A+1.f) + (af3A-1.f)*cosf(af3w0) + 2.f*sqrtf(af3A)*af3Alpha );
    tmp2 = -2.f*af3A * ( (af3A-1.f) + (af3A+1.f)*cosf(af3w0)                            );
    tmp3 =      af3A * ( (af3A+1.f) + (af3A-1.f)*cosf(af3w0) - 2.f*sqrtf(af3A)*af3Alpha );
    
    tmp4 =               (af3A+1.f) - (af3A-1.f)*cosf(af3w0) + 2.f*sqrtf(af3A)*af3Alpha;
    tmp5 =       2.f * ( (af3A-1.f) - (af3A+1.f)*cosf(af3w0)                            );
    tmp6 =               (af3A+1.f) - (af3A-1.f)*cosf(af3w0) - 2.f*sqrtf(af3A)*af3Alpha;
    
    f3b0 = tmp1/tmp4; f3b1 = tmp2/tmp4; f3b2 = tmp3/tmp4;
    f3a1 = tmp5/tmp4; f3a2 = tmp6/tmp4;
    
    filter3.setCoefficients(f3b0, f3b1, f3b2, f3a1, f3a2);
}

