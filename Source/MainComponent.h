#pragma once

#include <JuceHeader.h>
#include "MoisesWaveform.h"


enum TransportState
{
    Stopped,
    Starting,
    Playing,
    Stopping
};

class MainComponent  : public juce::AudioAppComponent, public juce::ChangeListener
{
public:
    //==============================================================================
    MainComponent()
        : state (TransportState::Stopped), moisesWaveform(formatManager, transportSource)
    {
        setSize (450, 400);
        
       

        addAndMakeVisible (&openButton);
        openButton.setButtonText ("Open...");
        openButton.onClick = [this] { openButtonClicked(); };
        
        
        addAndMakeVisible (&playButton);
        playButton.setButtonText ("Play");
        
        playButton.onClick = [this] { playButtonClicked(); };
        playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
        playButton.setEnabled (false);
    
        addAndMakeVisible (&stopButton);
        stopButton.setButtonText ("Stop");
        stopButton.onClick = [this] { stopButtonClicked(); };
        stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
        stopButton.setEnabled (false);
    
        
        formatManager.registerBasicFormats();
        transportSource.addChangeListener (this);
    
        setAudioChannels (0, 2);
        
        addAndMakeVisible(&moisesWaveform);
        
    }
        
    ~MainComponent() override
    {
        transportSource.removeChangeListener(this);
        shutdownAudio();
    }

    

    void resized() override
    {
        
        openButton.setBounds(75, 25, 300, 50);
        playButton.setBounds(75, 87.5, 300, 50);
        stopButton.setBounds(75, 150, 300, 50);
        
        juce::Rectangle<int> thumbnailBounds (25, 225, getWidth() - 50, getHeight() - 250); //change size of wave paint
        
        
        moisesWaveform.setBounds(thumbnailBounds);

    }
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource.get() == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }

        transportSource.getNextAudioBlock (bufferToFill);
    }

    void releaseResources() override
    {
        transportSource.releaseResources();
    }
    
    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource) transportSourceChanged();
        //if (source == &thumbnail)       thumbnailChanged();
        if (source == &transportSource)
        {
            if (transportSource.isPlaying())
                changeState (Playing);
            else
                changeState (Stopped);
        }
    }
    
    void transportSourceChanged()
    {
        changeState (transportSource.isPlaying() ? Playing : Stopped);
    }
    void thumbnailChanged()
        {
            repaint();
        }
    
    

    
    
//    void mouseDown(const MouseEvent& e)
//        {
//            mouseDownX = e.x;
//            startSel = startTime + (endTime - startTime) * (double) e.x / (double) getWidth();
//        }
//
//        void mouseDrag(const MouseEvent& e)
//        {
//            mouseUpX = e.x;
//            endSel = startTime + (endTime - startTime) * (double) e.x / (double) getWidth();
//            repaint();
//        }
    

       
    

private:
    //==============================================================================
    // Your private member variables go here...
    //add components here
    
    void changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;

            switch (state)
            {
                case Stopped:
                    stopButton.setEnabled (false);
                    playButton.setEnabled (true);
                   // transportSource.setPosition (0.0);
                    break;

                case Starting:
                    playButton.setEnabled (false);
                    transportSource.start();
                    break;

                case Playing:
                    stopButton.setEnabled (true);
                    break;

                case Stopping:
                    transportSource.stop();
                    break;
            }
        }
    }
    
   
    
    void openButtonClicked()
    {
        chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                       juce::File::getSpecialLocation(juce::File::SpecialLocationType::userMusicDirectory),
                                                       "*.wav, *.mp3, *.m4p");
        auto chooserFlags = juce::FileBrowserComponent::openMode
                          | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

            if (file != juce::File{})
            {
                auto* reader = formatManager.createReaderFor (file);

                if (reader != nullptr)
                {
                    openButton.setColour(juce::TextButton::buttonColourId, juce::Colours::aqua);
                    openButton.setButtonText(file.getFileName());
                    DBG("selected file " << file.getFullPathName());
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                    transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                    playButton.setEnabled (true);
                    readerSource.reset (newSource.release());
                    moisesWaveform.setSource (new juce::FileInputSource (file));
                    moisesWaveform.updateAfterLoading();
                    transportSource.setGain(0.5f);//volume of sound
                }
            }
        });
    }
    
    void playButtonClicked()
    {
        changeState (Starting);
    }

    void stopButtonClicked()
    {
        changeState (Stopping);
    }
    
private:
    std::unique_ptr<juce::FileChooser> chooser;
    
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    
    
    
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;//dont move
    TransportState state;
//    juce::AudioThumbnailCache thumbnailCache;                  // [1]
//    juce::AudioThumbnail thumbnail;
    MoisesWaveform moisesWaveform;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
