#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :
                                 state(Stopped),
                                 openButton("Open"),
                                 playButton("Play"),
                                 stopButton("Stop")
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1920, 1080);
    setAudioChannels(0, 2);
    Timer::startTimerHz(30);
    
    centrePoint.setX(getWidth() / 2);
    centrePoint.setY(getHeight() / 2);
    
    openButton.onClick = [this] { openButtonClicked(); };
    addAndMakeVisible(&openButton);
    
    playButton.onClick = [this] { playButtonClicked();};
    addAndMakeVisible(&playButton);
    
    stopButton.onClick = [this] { stopButtonClicked(); };
    addAndMakeVisible(&stopButton);
    
    //addAndMakeVisible(menuComponent);
    
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    
    addAndMakeVisible(fftCircleComponent);
    
    addAndMakeVisible(logoComponent);
    
    backgroundImage = ImageCache::getFromMemory(BinaryData::backgroundImage2_jpeg, BinaryData::backgroundImage2_jpegSize);
    backgroundImage.multiplyAllAlphas(0.5f);
    

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
            circleDataPtr->processFFTSampleBySample(channelData[i]);
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
    
    backgroundImageScaled = backgroundImage.rescaled(getWidth(), getHeight());
    g.drawImageAt(backgroundImageScaled, 0, 0);

}

void MainComponent::resized()
{
    openButton.setBounds(600, 10, 50, 30);
    playButton.setBounds(600, 50, 50, 30);
    stopButton.setBounds(600, 90, 50, 30);
    
    //menuComponent.setBounds(0, 0, 250, 200);
    
    fftCircleComponent.setBounds(0, 120, getWidth(), getHeight() - 120);
    logoComponent.setBounds((getWidth() / 2) - 200, (getHeight() / 2 ) - 140, 400, 400);
    repaint();
}

void MainComponent::timerCallback()
{
    centrePoint.setX(getWidth() / 2);
    centrePoint.setY(getHeight() / 2);
    
    if (circleDataPtr->nextFFTBlockReady)
    {
        circleDataPtr->drawNextFrameOfSpectrum();
        circleDataPtr->nextFFTBlockReady = false;
        
        for (int i = 0; i < beatDetector.getScopeSizeForLevelDetector(); i++)
        {
            levelDectectorValue = beatDetector.detectFreqsOverCertainLevel(circleDataPtr->scopeData[i]);
        }
        fftCircleComponent.repaint();
        logoComponent.beatDetected = levelDectectorValue;
        logoComponent.repaint();
    }
    
    updateParticlePositions(particlesTopLeft);
    updateParticlePositions(particlesTopRight);
    updateParticlePositions(particlesBottomLeft);
    updateParticlePositions(particlesBottomRight);
    
    if  (levelDectectorValue == true)
    {
        randomlyGenerateNewParticles(particlesTopLeft, 50, 2.0f, -3.0f, centrePoint);
        randomlyGenerateNewParticles(particlesTopRight, 50, -2.0f, -3.0f, centrePoint);
        randomlyGenerateNewParticles(particlesBottomLeft, 50, -2.0f, 3.0f, centrePoint);
        randomlyGenerateNewParticles(particlesBottomRight, 50, 2.0f, 3.0f, centrePoint);
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

void MainComponent::updateParticlePositions(OwnedArray<ParticleComponent>& particleArray)
{
    for (int i = particleArray.size(); --i >= 0;)
    {
        particleArray.operator[](i)->beatDetected = levelDectectorValue;
        if (! particleArray.getUnchecked(i)->step())
            particleArray.remove(i);
    }
}

void MainComponent::randomlyGenerateNewParticles(OwnedArray<ParticleComponent>& particleArray, int amountOfParticles, float xDir, float yDir, Point<float> newCentrePoint)
{
    if (Random::getSystemRandom().nextInt(100) < amountOfParticles)
    {
            addAndMakeVisible(particleArray.add(new ParticleComponent (newCentrePoint, xDir, yDir)));
    
            for (int i = 0; i < particleArray.size(); i++)
            {
                particleArray.operator[](i)->toBehind(&fftCircleComponent);
            }
    }
}
