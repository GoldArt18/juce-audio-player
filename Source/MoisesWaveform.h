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
class MoisesWaveform  : public juce::Component, public juce::FileDragAndDropTarget, public juce::Timer, public juce::ChangeListener
{
public:
    MoisesWaveform(juce::AudioFormatManager& formatManager,
                   juce::AudioTransportSource& source)
                   
    : thumbnailCache(5),
    thumbnail(512, formatManager, thumbnailCache),
    transportSource(source)
    
    
    {
        
        thumbnail.addChangeListener (this);
        

        
        currentPositionMarker.setFill (juce::Colours::white.withAlpha (0.85f));
        addAndMakeVisible (currentPositionMarker);
        startTimerHz(60);
        visibleRange = juce::Range<double>(0.0f, thumbnail.getTotalLength());//get size after loaded
    }
    
    void changeListenerCallback (juce::ChangeBroadcaster* source){
        repaint();
    }
    
    void setRange (juce::Range<double> newRange)
        {
            visibleRange = newRange;
            
            repaint();
        }
    
    double xToTime (const float x) const
    {
        DBG("Width of the Component: " + juce::String(getWidth()));
        DBG("VisibleRange Start: " + juce::String(visibleRange.getStart()));
        DBG("VisibleRange Length: " + juce::String(visibleRange.getLength()));
        
        double time = (x / (float) getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
        DBG("Clicked at X: " + juce::String(x) + " which corresponds to time: " + juce::String(time));
        return time;
    }



    ~MoisesWaveform() override
    {
    }

    void timerCallback() override{
        repaint();
    }
    
    bool isInterestedInFileDrag(const juce::StringArray &files) override {
        return true;
    }
    
    void filesDropped(const juce::StringArray &files, int x, int y) override{
        
    }
    
    bool setSource (juce::InputSource *newSource) {
        return thumbnail.setSource(newSource);
    }
    
    void paint (juce::Graphics& g) override
    {
        
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

        juce::Rectangle<int> thumbnailBounds (10, 100, getWidth() - 20, getHeight() - 120);
        
        if (thumbnail.getNumChannels() == 0)
            paintIfNoFileLoaded (g, getLocalBounds());
        else
            paintIfFileLoaded (g, getLocalBounds());
        
    }
    
    
    
    void paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
        {
            g.setColour (juce::Colours::black);
            g.fillRect (thumbnailBounds);

            g.setColour (juce::Colours::aqua);
            auto audioLength = (float) thumbnail.getTotalLength();
            thumbnail.drawChannels(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 1.0f);
             
            g.setColour (juce::Colours::blue);
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
        
    void mouseDown (const juce::MouseEvent& e) override
    {
        
            mouseDrag(e);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        
            transportSource.setPosition(juce::jmax(0.0, xToTime((float)e.x)));
            DBG("dragged");
            repaint();
        
    }


        void mouseUp (const juce::MouseEvent&) override
        {
            DBG("up");
            //transportSource.start();
        }
    
    void updateAfterLoading()
    {
        visibleRange = juce::Range<double>(0.0f, thumbnail.getTotalLength());
        DBG("Thumbnail Total Length after loading: " + juce::String(thumbnail.getTotalLength()));
        repaint();
    }
    


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoisesWaveform)
    
    juce::AudioThumbnailCache thumbnailCache;  // Declaration order matters!
    juce::AudioThumbnail thumbnail;
    juce::AudioTransportSource& transportSource;
    
    juce::DrawableRectangle currentPositionMarker;
    
    juce::Range<double> visibleRange;
    
};



