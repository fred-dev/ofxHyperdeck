# ofxHyperdeck

openFrameworks addon for controlling Blackmagic HyperDeck recorders over the HyperDeck Ethernet protocol (TCP port 9993): play, record, stop, shuttle, jog, clip and timecode navigation, slot and input selection. Replies and notifications from the deck are parsed, so you can read the transport state, current timecode and clip list.

```cpp
ofxHyperdeck deck;
deck.setup("192.168.1.50");   // in setup()
deck.update();                // every frame; reconnects if the deck drops off
deck.play();
deck.gotoClip(2);
deck.requestClips();          // results appear in deck.getClips()
```

Every reply also fires `responseEvent` with the code, text and `key: value` fields, and `send()` takes any raw protocol command.

## Example

`example/` connects to a deck, shows its status, timecode and clips, and maps the keyboard to transport commands. Set `HYPERDECK_IP` in `example/src/ofApp.cpp`, then generate the project with projectGenerator.

## Requirements

- openFrameworks 0.12 or later
- Core addon `ofxNetwork`
- Remote control enabled on the deck

Written in 2014, rewritten in 2026 for current openFrameworks and the current protocol.
