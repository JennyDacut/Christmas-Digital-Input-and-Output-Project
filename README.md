# Christmas-Digital-Input-and-Output-Project

This project reacts to combinations of buttons to play different songs and control lights. It demonstrates how to use digital inputs (buttons) to trigger complex digital outputs (PWM-like sound and LED control).

🎄 Concept: The Logic Gates of Christmas
The core of this project is Digital Logic. It doesn't just look at one button; it looks at the combination of buttons pressed together. This is like a "secret code" or a logic gate (AND, OR, XOR).

Input: 4 Buttons (Left, Right, Up, Down).

Output:
Sound: A buzzer playing different melodies.
Light: Red and Yellow LEDs turning on/off.
Matrix: An external LED matrix (optional display).
🎮 How It Works (The "Modes")
The code defines two main states: Red Mode (red_hi) and Yellow Mode (yellow_hi).
Red Mode (Rudolph):
Trigger: Pressing specific button combinations (e.g., Button 4 AND Button 2).

Effect:

Music: Plays "Rudolph the Red-Nosed Reindeer."
LED: The Red LED lights up.
Yellow Mode (Deck the Halls):
Trigger: Pressing a different combination (e.g., Button 4 XOR Button 3).

Effect:

Music: Plays "Deck the Halls."
LED: The Yellow LED lights up.
Both Modes (Jingle Bells):
Trigger: Pressing the "Up" button (Button 4) alone acts as an override, turning BOTH modes on.

Effect:
Music: Plays "Jingle Bells."
LED: Both Red and Yellow LEDs light up (making Orange!).

🎵 The "Music Engine"
The buzzer isn't playing an MP3 file; the code is generating the sound waves manually.
Frequency: The code knows that "Middle C" is 262 Hertz.
Duration: It turns the buzzer pin High and Low 262 times a second for a specific duration (e.g., 500ms).
Song: A song is just a list of these frequencies and durations played in order.

💡 Why This Project Matters

This project teaches three fundamental embedded systems concepts:
GPIO Input: Reading the state of multiple buttons at once.

Boolean Logic: Using code to make decisions based on multiple inputs (AND, OR, NOT logic).

Bit-Banging Audio: Creating sound by manually toggling a pin at specific frequencies, which is how early video game consoles made music!
