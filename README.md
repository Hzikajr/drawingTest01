# drawingTest01
Drawing experiment with oF for a music "Alphabet" by Fantastic Plastic Machine
(DEMO : https://vimeo.com/124597185)

## You need something to play with this repository
1. Music file of "Alphabet" (by Fantastic Plastic Machine)
   (YouTube? SoundCloud? fine! But these are illegal maybe:)
2. Download/clone some addons from github
<dl>
  <dt>ofxTrueTypeFontUC</dt>
  <dd>https://github.com/hironishihara/ofxTrueTypeFontUC</dd>
  <dt>ofxPostProcessing</dt>
  <dd>https://github.com/satcy/ofxPostProcessing</dd>
  <dt>ofxTriangle</dt>
  <dd>https://github.com/microcosm/ofxTriangle</dd>
</dl>
3. Need application "SoundFlower" to get audio output
  
  https://code.google.com/p/soundflower/downloads/detail?name=Soundflower-1.6.6b.dmg
  
  >Soundflower is a MacOS system extension that allows applications to pass audio to other applications. Soundflower is easy to use, it simply presents itself as an audio device, allowing any audio application to send and receive audio with no other support needed.
  >
  >Initial support for the development and maintenance of Soundflower was provided by Cycling '74. Soundflower is an open source project, if you are interested in contributing please feel free to get involved.
  >
  >Code for the Soundflower project is now hosted at
  >
  >https://github.com/Cycling74 

4. Launch SoundFlower app, change audio output to "SoundFlower (2ch)" from both SoundFlower menu bar and System Preference app

## How to play with this repository
1. Clone this repo to the directory: <your_of_directory>/apps/myApps/
2. Copy one Japanese font and one English font that are your favorite from your PC to drawingTest01/bin/data
   (rename of the font files required. Japanese-> "jp.otf" / English->"en.ttf")
   (if you want to use fonts that have different extentions or filename, please change filenames in the codes)
3. Open drawingTest01/animation01.xcodeproj
4. Run the program

## Key commands
<dl>
  <dt>q, w, e, r, t, y, u : visual effects switching</dt>
  <dt>1 : move to next lyrics word</dt>
  <dt>z : show/hide sound wave</dt>
  <dt>x : show/hide colorful polygon</dt>
  <dt>c : show/hide lyrics</dt>
  <dt>v : triangle particles spread</dt>
  <dt>b : show/hide grids</dt>
  <dt>n : show/hide rhymeWire</dt>
</dl>
