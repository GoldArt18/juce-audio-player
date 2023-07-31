#pragma once

#include <JuceHeader.h>
#include "MoisesWaveform.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
enum TransportState
{
    Stopped,
    Starting,
    Playing,
    Stopping
};
//public juce::FileDragAndDropTarget, public juce::Timer
class MainComponent  : public juce::AudioAppComponent, public juce::ChangeListener
{
public:
    //==============================================================================
    MainComponent()
        : state (TransportState::Stopped), moisesWaveform(formatManager)
    {
        setSize (450, 400);
        
       // thumbnail.addChangeListener (this);

        addAndMakeVisible (&openButton);
        openButton.setButtonText ("Open...");
        openButton.onClick = [this] { openButtonClicked(); };
        //openButton.onStateChange = handleClick;
        
        addAndMakeVisible (&playButton);
        playButton.setButtonText ("Play");
        
        playButton.onClick = [this] { playButtonClicked(); };
        playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
        playButton.setEnabled (false);
    //
        addAndMakeVisible (&stopButton);
        stopButton.setButtonText ("Stop");
        stopButton.onClick = [this] { stopButtonClicked(); };
        stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
        stopButton.setEnabled (false);
    
        //setSize (300, 200);
    //
        formatManager.registerBasicFormats();       // [1]
        transportSource.addChangeListener (this);   // [2]
    
        setAudioChannels (0, 2);
        
        addAndMakeVisible(&moisesWaveform);
        
    }
        
    ~MainComponent() override
    {
        shutdownAudio();
    }

    //==============================================================================
    
    
    
//    void paint (juce::Graphics& g) override
//    {
//        // (Our component is opaque, so we must completely fill the background with a solid colour)
//        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//        juce::Rectangle<int> thumbnailBounds (25, 225, getWidth() - 50, getHeight() - 250); //change size of wave paint
//         
//        if (thumbnail.getNumChannels() == 0)
//            paintIfNoFileLoaded (g, thumbnailBounds);
//        else
//            paintIfFileLoaded (g, thumbnailBounds);
//    }
//    
//    void paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
//    {
//        g.setColour (juce::Colours::darkgrey);
//        g.fillRect (thumbnailBounds);
//        g.setColour (juce::Colours::white);
//        g.drawFittedText ("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
////        g.fillAll (juce::Colours::white);
////        
////        g.setColour (juce::Colours::mediumblue);
////        
////        if (thumbnail.getTotalLength() > 0)
////        {
////            int heightPerChannel = (getHeight() - 4) / thumbnail.getNumChannels();
////            
////            for (int i = 0; i < thumbnail.getNumChannels(); ++i)
////            {
////                thumbnail.drawChannel (g, 2, 2 + heightPerChannel * i,
////                                       getWidth() - 4, heightPerChannel,
////                                       startTime, endTime,
////                                       i, 1.0f);
////            }
////            
////            g.setColour(juce::Colours::tomato);
////            g.drawRect(mouseDownX ,1 , mouseUpX - mouseDownX, getHeight()-2, 1);
////        }
//    }
//
//    
//    void paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
//        {
//            g.setColour (juce::Colours::white);
//            g.fillRect (thumbnailBounds);
//     
//            g.setColour (juce::Colours::blue);                               // [8]
//            thumbnail.drawChannels (g,                                      // [9]
//                                    thumbnailBounds,
//                                    0.0,                                    // start time
//                                    thumbnail.getTotalLength(),             // end time
//                                    1.0f);                                  // vertical zoom
//        auto audioPosition = (float)transportSource.getCurrentPosition();
//          auto drawPosition =
//            (audioPosition / thumbnail.getTotalLength()) * (float)thumbnailBounds.getWidth() +
//            (float)thumbnailBounds.getX();
//        g.setColour(juce::Colours::black);
//          g.drawLine(
//            drawPosition,
//            (float)thumbnailBounds.getY(),
//            drawPosition,
//            (float)thumbnailBounds.getBottom(),
//            2.0f
//          );
//
//          if (transportSource.isPlaying()) {
//            repaint();
//          }
//        }

    void resized() override
    {
        // call set bounds
        //setSize(450, 400);
        openButton.setBounds(75, 25, 300, 50);
        playButton.setBounds(75, 87.5, 300, 50);
        stopButton.setBounds(75, 150, 300, 50);
        
        juce::Rectangle<int> thumbnailBounds (25, 225, getWidth() - 50, getHeight() - 250); //change size of wave paint
        
        // This is called when the MainComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
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
    void mouseDown (const juce::MouseEvent& e) override
        {
            mouseDrag (e);
        }

        void mouseDrag (const juce::MouseEvent& e) override
        {
//            if (canMoveTransport())
//                transportSource.setPosition (jmax (0.0, xToTime ((float) e.x)));
            DBG("dragged");
        }

        void mouseUp (const juce::MouseEvent&) override
        {
            transportSource.start();
        }

       
    

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
                case Stopped:                           // [3]
                    stopButton.setEnabled (false);
                    playButton.setEnabled (true);
                   // transportSource.setPosition (0.0);
                    break;

                case Starting:                          // [4]
                    playButton.setEnabled (false);
                    transportSource.start();
                    break;

                case Playing:                           // [5]
                    stopButton.setEnabled (true);
                    break;

                case Stopping:                          // [6]
                    transportSource.stop();
                    break;
            }
        }
    }
    
   
    
    void openButtonClicked()
    {
        chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                       juce::File::getSpecialLocation(juce::File::SpecialLocationType::userMusicDirectory),
                                                       "*.wav, *.mp3");                     // [7]
        auto chooserFlags = juce::FileBrowserComponent::openMode
                          | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)     // [8]
        {
            auto file = fc.getResult();

            if (file != juce::File{})                                                // [9]
            {
                auto* reader = formatManager.createReaderFor (file);                 // [10]

                if (reader != nullptr)
                {
                    openButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
                    openButton.setButtonText(file.getFileName());
                    DBG("selected file " << file.getFullPathName());
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);   // [11]
                    transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);       // [12]
                    playButton.setEnabled (true);                                                      // [13]
                    readerSource.reset (newSource.release());
                    //thumbnail.setSource (new juce::FileInputSource (file));     // [14] add to other class later
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
