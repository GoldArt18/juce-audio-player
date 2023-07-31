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
class MoisesWaveform  : public juce::Component
{
public:
    MoisesWaveform(juce::AudioFormatManager& formatManager):thumbnailCache (5),                            // [4]
    thumbnail (512, formatManager, thumbnailCache)
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
    }
    
    void paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
    {
        g.setColour (juce::Colours::darkgrey);
        g.fillRect (thumbnailBounds);
        g.setColour (juce::Colours::white);
        g.drawFittedText ("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
//        g.fillAll (juce::Colours::white);
//
//        g.setColour (juce::Colours::mediumblue);
//
//        if (thumbnail.getTotalLength() > 0)
//        {
//            int heightPerChannel = (getHeight() - 4) / thumbnail.getNumChannels();
//
//            for (int i = 0; i < thumbnail.getNumChannels(); ++i)
//            {
//                thumbnail.drawChannel (g, 2, 2 + heightPerChannel * i,
//                                       getWidth() - 4, heightPerChannel,
//                                       startTime, endTime,
//                                       i, 1.0f);
//            }
//
//            g.setColour(juce::Colours::tomato);
//            g.drawRect(mouseDownX ,1 , mouseUpX - mouseDownX, getHeight()-2, 1);
//        }
    }

    
    void paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
        {
            g.setColour (juce::Colours::white);
            g.fillRect (thumbnailBounds);
     
            g.setColour (juce::Colours::blue);                               // [8]
            thumbnail.drawChannels (g,                                      // [9]
                                    thumbnailBounds,
                                    0.0,                                    // start time
                                    thumbnail.getTotalLength(),             // end time
                                    1.0f);                                  // vertical zoom
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

//          if (transportSource.isPlaying()) {
//            repaint();
//          }
        }
    
    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoisesWaveform)
    
    juce::AudioThumbnailCache thumbnailCache;                  // [1]
    juce::AudioThumbnail thumbnail;
    
    
};



