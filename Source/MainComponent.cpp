#include "MainComponent.h"
void handleClick(){
    DBG("HandleClicked");
}
//==============================================================================
MainComponent::MainComponent()
//: state (Stopped)
{
    setSize (600, 400);
    

    addAndMakeVisible (&openButton);
    openButton.setButtonText ("Open...");
    openButton.onClick = [this] { DBG("Clicked"); };
    openButton.onStateChange = handleClick;
    
    addAndMakeVisible (&playButton);
    playButton.setButtonText ("Play");
//    playButton.onClick = [this] { playButtonClicked(); };
//    playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
//    playButton.setEnabled (false);
//
//    addAndMakeVisible (&stopButton);
//    stopButton.setButtonText ("Stop");
//    stopButton.onClick = [this] { stopButtonClicked(); };
//    stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
//    stopButton.setEnabled (false);
//
//    setSize (300, 200);
//
//    formatManager.registerBasicFormats();       // [1]
//    transportSource.addChangeListener (this);   // [2]
//
//    setAudioChannels (0, 2);

}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // call set bounds
    setSize(450, 400);
    openButton.setBounds(75, 25, 300, 50);
    playButton.setBounds(75, 100, 300, 50);
    
    
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
