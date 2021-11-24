#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : forwardFFT(fftOrder),
                                 window(fftSize,
                                 juce::dsp::WindowingFunction<float>::blackman),
                                 state(Stopped),
                                 openButton("Open"),
                                 playButton("Play"),
                                 stopButton("Stop")
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1280, 1280);
    setAudioChannels(0, 2);
    Timer::startTimerHz(60);
    
    centrePoint.setX(getWidth() / 2);
    centrePoint.setY(getHeight() / 2);
    
    openButton.onClick = [this] { openButtonClicked(); };
    addAndMakeVisible(&openButton);
    
    playButton.onClick = [this] { playButtonClicked();};
    addAndMakeVisible(&playButton);
    
    stopButton.onClick = [this] { stopButtonClicked(); };
    addAndMakeVisible(&stopButton);
    
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    
    addAndMakeVisible(fftCircleComponent);
    
    addAndMakeVisible(logoComponent);
    logoComponent.toFront(&fftCircleComponent);
    
    for (int i = 0; i < FFTCircleData::scopeSize; i++)
    {
        mainScopeData[i] = 0;
    }

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    circleDataPtr = nullptr;
    stopTimer();
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    
    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    transportSource.getNextAudioBlock(bufferToFill);
    
    if (bufferToFill.buffer->getNumChannels() > 0)
    {
        auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);
        
        for (auto i = 0; i < bufferToFill.numSamples; i++)
        {
            circleDataPtr->processAudioBlock(channelData[i]);
        }
    }
}

void MainComponent::releaseResources()
{
    transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (Colours::black);

}

void MainComponent::resized()
{
    openButton.setBounds(10, 10, getWidth() - 20, 30);
    playButton.setBounds(10, 50, getWidth() - 20, 30);
    stopButton.setBounds(10, 90, getWidth() - 20, 30);
    
    fftCircleComponent.setBounds(0, 120, getWidth(), getHeight() - 120);
    logoComponent.setBounds((getWidth() / 2) - 300, (getHeight() / 2 ) - 240, 600, 600);
}

void MainComponent::timerCallback()
{
    if (circleDataPtr->nextFFTBlockReady)
    {
        circleDataPtr->drawNextFrameOfSpectrum();
        circleDataPtr->nextFFTBlockReady = false;
        
        for (int i = 0; i < beatDetector.scopeSize; i++)
        {
            beatDetected = beatDetector.detectBeat(circleDataPtr->getScopeDataAtIndex(i));
            //DBG("Index: " << i << " .Value: " << circleDataPtr->getScopeDataAtIndex(i));
            
            logoComponent.beatDetected = beatDetector.detectBeat(circleDataPtr->getScopeDataAtIndex(i));
        }
        fftCircleComponent.repaint();
        logoComponent.repaint();
        
        for (int i = 0; i < FFTCircleData::scopeSize; i++)
        {
            mainScopeData[i] = circleDataPtr->getScopeDataAtIndex(i);
        }
    }
    
    // Go through each particle and update their position
    for (int i = particlesTopLeft.size(); --i >= 0;)
    {
        particlesTopLeft.operator[](i)->beatDetected = beatDetected;
        if (! particlesTopLeft.getUnchecked(i)->step())
            particlesTopLeft.remove(i);
    }
    for (int i = particlesTopRight.size(); --i >= 0;)
    {
        particlesTopRight.operator[](i)->beatDetected = beatDetected;
        if (! particlesTopRight.getUnchecked(i)->step())
            particlesTopRight.remove(i);
    }
    for (int i = particlesBottomLeft.size(); --i >= 0;)
    {
        particlesBottomLeft.operator[](i)->beatDetected = beatDetected;
        if (! particlesBottomLeft.getUnchecked(i)->step())
            particlesBottomLeft.remove(i);
    }
    for (int i = particlesBottomRight.size(); --i >= 0;)
    {
        particlesBottomRight.operator[](i)->beatDetected = beatDetected;
        if (! particlesBottomRight.getUnchecked(i)->step())
            particlesBottomRight.remove(i);
    }
    
    
    
    // Randomly generate new particles
    for (int i = 0; i < beatDetector.scopeSize; i++)
    {
        if  (beatDetector.detectBeat(circleDataPtr->getScopeDataAtIndex(i)) == true)
        {
            if (Random::getSystemRandom().nextInt(100) < 4){
                    addAndMakeVisible(particlesTopLeft.add(new ParticleComponent (centrePoint, 2.0f, -3.0f)));
            
                    for (int i = 0; i < particlesTopLeft.size(); i++)
                    {
                        particlesTopLeft.operator[](i)->toBehind(&fftCircleComponent);
                    }
                }
        }
    }
    
    for (int i = 0; i < beatDetector.scopeSize; i++)
    {
        if  (beatDetector.detectBeat(circleDataPtr->getScopeDataAtIndex(i)) == true)
        {
        if (Random::getSystemRandom().nextInt(100) < 4)
            {
                addAndMakeVisible(particlesTopRight.add(new ParticleComponent (centrePoint, -2.0f, -3.0f)));
                for (int i = 0; i < particlesTopRight.size(); i++)
                {
                    particlesTopRight.operator[](i)->toBehind(&fftCircleComponent);
                }
            }
        }
    }
    
    for (int i = 0; i < beatDetector.scopeSize; i++)
    {
        if  (beatDetector.detectBeat(circleDataPtr->getScopeDataAtIndex(i)) == true)
        {
            if (Random::getSystemRandom().nextInt(100) < 4)
            {
                addAndMakeVisible(particlesBottomLeft.add(new ParticleComponent (centrePoint, -2.0f, 3.0f)));
                for (int i = 0; i < particlesBottomLeft.size(); i++)
                {
                    particlesBottomLeft.operator[](i)->toBehind(&fftCircleComponent);
                }
            }
        }
    }
    
    for (int i = 0; i < beatDetector.scopeSize; i++)
    {
        if  (beatDetector.detectBeat(circleDataPtr->getScopeDataAtIndex(i)) == true)
        {
        if (Random::getSystemRandom().nextInt(100) < 4)
            {
                addAndMakeVisible(particlesBottomRight.add(new ParticleComponent (centrePoint, 2.0f, 3.0f)));
                for (int i = 0; i < particlesBottomRight.size(); i++)
                {
                    particlesBottomRight.operator[](i)->toBehind(&fftCircleComponent);
                }
            }
        }
    }
}

void MainComponent::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState(Playing);
        else if ((state == Stopping) || (state == Playing))
            changeState(Stopped);
        else if (Pausing == state)
            changeState(Paused);
    }
}

void MainComponent::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser> ("Select a Wave or MP3 file to play...",
                                                   juce::File{},
                                                   "*.wav;*.mp3");
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
                auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                playButton.setEnabled (true);
                readerSource.reset (newSource.release());
            }
        }
    });
}

void MainComponent::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused)) {
        changeState (Starting);
    }
    else if (state == Playing)
        changeState(Pausing);
    
}

void MainComponent::stopButtonClicked()
{
    if (state == Paused) {
        changeState (Stopped);
    }
    else
        changeState(Stopping);
    
}

void MainComponent::changeState (TransportState newState)
{
    if (state != newState)
    {
        state = newState;
        
        switch (state) {
            case Stopped:
                playButton.setButtonText("Play");
                stopButton.setButtonText("Stop");
                stopButton.setEnabled(false);
                transportSource.setPosition(0.0);
                break;
                
            case Starting:
                transportSource.start();
                break;
                
            case Playing:
                playButton.setButtonText("Pause");
                stopButton.setButtonText("Stop");
                stopButton.setEnabled(true);
                break;
                
            case Pausing:
                transportSource.stop();
                
            case Paused:
                playButton.setButtonText("Resume");
                stopButton.setButtonText("Return to Zero");
                
            case Stopping:
                transportSource.stop();
                break;
        }
    }
}
