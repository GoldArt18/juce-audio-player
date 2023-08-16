/*
  ==============================================================================

    MoisesWaveform.h
    Created: 11 Aug 2023 12:26:40pm
    Author:  Liam Campbell

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MoisesWaveform  : public juce::Component, public juce::FileDragAndDropTarget, public juce::Timer
{
public:
    MoisesWaveform(juce::AudioFormatManager& formatManager,
                   juce::AudioTransportSource& source,
                   juce::Slider& slider)
      : transportSource(source),
        zoomSlider(slider),
        thumbnailCache(5),
        thumbnail(512, formatManager, thumbnailCache) // Correct order
//
//    ,juce::AudioFormatManager& formatManager,
//    juce::AudioTransportSource& source): transportSource (source),thumbnail (512, formatManager, thumbnailCache)                           // [4]
//    thumbnail (512, formatManager, thumbnailCache)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
    }

    ~MoisesWaveform() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

         
        if (thumbnail.getNumChannels() == 0)
            paintIfNoFileLoaded (g, getLocalBounds());
        else
            paintIfFileLoaded (g, getLocalBounds());
        
//        juce::Rectangle<int> thumbnailBounds (10, 100, getWidth() - 20, getHeight() - 120);
//         
//                if (thumbnail.getNumChannels() == 0)
//                    paintIfNoFileLoaded (g, thumbnailBounds);
//                else
//                    paintIfFileLoaded (g, thumbnailBounds);
    }
    
    void paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
        {
            g.setColour (juce::Colours::white);
            g.fillRect (thumbnailBounds);

            g.setColour (juce::Colours::blue);
            auto audioLength = (float) thumbnail.getTotalLength();
            thumbnail.drawChannels(g, thumbnailBounds, 0.0, audioLength, 1.0f);
             
            g.setColour (juce::Colours::green);
            auto audioPosition = (float) transportSource.getCurrentPosition();
            auto drawPosition = (audioPosition / audioLength) * (float) thumbnailBounds.getWidth() + (float) thumbnailBounds.getX();
            g.drawLine(drawPosition, (float) thumbnailBounds.getY(), drawPosition, (float) thumbnailBounds.getBottom(), 2.0f);
        }

    
    
      void paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
    {
        g.setColour (juce::Colours::darkgrey);
        g.fillRect (thumbnailBounds);
        g.setColour (juce::Colours::white);
        g.drawFittedText ("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
    }
        
    
    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoisesWaveform)
    
    juce::AudioThumbnailCache thumbnailCache;  // Declaration order matters!
    juce::AudioThumbnail thumbnail;
    juce::AudioTransportSource& transportSource;
    juce::Slider& zoomSlider;
    
    
};



