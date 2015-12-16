

# Introduction #

## What is repulse? ##
Repulse is a MIDI Jack real time sampler machine that can be used to play drum kits or other sounds.<br>
It has 8 voices of polyphony and supports a rich set of parameters per voice.<br>
Repulse does not contain a sequencer, use any sequencer of your choice to control the sampler, try <a href='http://www.filter24.org/seq24'>Seq42</a> or <a href='http://www.rosegardenmusic.com'>Rosegarden</a>.<br>
<br>
<h2>Voice controllers</h2>
This are some of the parameters per voice:<br>
<ul><li><i>Start time:</i> delays the start time of the sound the given miliseconds.<br>
</li><li><i>Transpose:</i> transpose the sound the given octaves.<br>
</li><li><i>Time Stretch:</i> stretch the sound by the given percentage (the pitch does not change).<br>You have different time stretch types to choose from.<br>
</li><li><i>Staturation:</i> introduce analog like saturation into the sound.<br>
</li><li><i>Filtering:</i> you have five types to choose from with controllable cutoff and resonance:<br>
<ul><li>Low pass.<br>
</li><li>High pass.<br>
</li><li>Band pass 1.<br>
</li><li>Band pass 2.<br>
</li><li>Notch<br>
</li></ul></li><li><i>Envelope:</i> you have three envelopes to choose from, with a tuneable decay time:<br>
<ul><li>Infinite: does not apply any envelope.<br>
</li><li>Trigger: also known as one shot, starts the decay envelope on note on event.<br>
</li><li>Gate: starts the decay after the note off event.<br>
</li></ul></li><li><i>Panning:</i> pan the voice to the left or the right in the stereo output.<br>
</li><li><i>Volume:</i> adjusts the output volume of the voice.</li></ul>

<h2>Controller modulation</h2>
Almost any parameter can be modulated jointly by note velocity and a random source.<br>
<ul><li>Modulating by <b>note velocity</b> you can create a velocity reactive sampler.<br>Other parameters can be modulated by velocity to achieve subtle effects.<br>
</li><li>Modulating different parameters by the <b>random source</b>, with small values, induces an analogue feeling to the machine.</li></ul>

You can apply random modulation to all the parameters at the same time by a minuscule value to achieve an analogue effect.<br>
Or you can turn the random modulations up to get strange effects.<br>
The velocity modulation can applied to uncommon parameters at the same time, like the cutoff frequency, to get interesting sounds.<br>
<br>
<h2>MIDI support</h2>
You can drive any parameter that you can see in the GUI with a midi controller. You can check the <a href='Controllers.md'>Controllers</a> page to see the detailed controller mappings.<br>
The MIDI support is quite good with repulse responding to the program change, all controllers/notes off, omni, poly, volume and plenty of other custom events.<br>
Currently the supported MIDI driver is ALSA, Jack will be added in the future.<br>
<br>
<h1>Installing</h1>
<h2>Dependencies</h2>
<ul><li>ALSA <a href='http://www.alsa-project.org'>http://www.alsa-project.org</a>
</li><li>Jack <a href='http://jackaudio.org'>http://jackaudio.org</a>
</li><li>SndFile <a href='http://www.mega-nerd.com/libsndfile'>http://www.mega-nerd.com/libsndfile</a>
</li><li>Secret Rabbit (libsamplerate) <a href='http://www.mega-nerd.com/SRC'>http://www.mega-nerd.com/SRC</a></li></ul>

<h2>Compiling</h2>

After downloading you can compile a binary like this:<br>
<pre><code>cd repulse<br>
./build.sh<br>
</code></pre>

<h2>Executing</h2>

You can go on and execute repulse like this:<br>
<pre><code>./repulse -c patch.xml<br>
</code></pre>

In general you can execute:<br>
<pre><code>repulse [-c] [-n jack_client_name] &lt;patch_file&gt;<br>
</code></pre>

This are the repulse command line switches:<br>
<ul><li><i>-c</i>: autoconnect the repulse stereo output to the first physical output.<br>
</li><li><i>-n jack_client_name</i>: the client name in the Jack environment.</li></ul>

By default the engine name is repulse and the machine does not autoconnect its outputs.<br>
So if you dont use the -c switch you have to manually connect the outputs.<br>
Repulse searches for wave files relative to the binary location folder.<br>
<br>
<h2>Downloading the SVN version</h2>

Use this command to anonymously check out the latest project source code:<br>
<pre><code>svn checkout http://repulse.googlecode.com/svn/trunk/ repulse<br>
</code></pre>

<h1>Curses GUI</h1>
<img src='http://img219.imageshack.us/img219/8738/screenv001.png' />

<h2>Keyboard controls</h2>
<ul><li><i>Arrow keys:</i> moves the cursor in the spread-sheet like grid.<br>
</li><li><i>A key:</i> tune the selected cell parmeter up.<br>
</li><li><i>Z key:</i> tune the selected cell parmeter down.<br>
</li><li><i>S key:</i> tune the selected cell parmeter up, finer increment.<br>
</li><li><i>X key:</i> tune the selected cell parmeter down, finer decrement.<br>
</li><li><i>+ key:</i> increment the selected preset number.<br>
</li><li><i>- key:</i> decrement the selected preset number.<br>
</li><li><i>CONTROL + N:</i> create a new preset.<br>
</li><li><i>CONTROL + B:</i> save the current preset.<br>
</li><li><i>CONTROL + R:</i> recall the currently selected preset.<br>
</li><li><i>CONTROL + L:</i> local on/off toggle.<br>
<ul><li>When active the machine will interpret its control surface events and MIDI events.<br>
</li><li>When deactivated the machine will interpret only MIDI events.<br>
</li></ul></li><li><i>CONTROL + W:</i> alternate wheel toggle.<br>
<ul><li>when activated the pitch bend wheel controls the engine's time stretch value.<br>
</li><li>When deactivated the pitch bend wheel controls the engine's transpose value.<br>
</li></ul></li><li><i>CONTROL + O:</i> omni on/off toggle.<br>
<ul><li>When activated the machine will receive MIDI events in any channel.<br>
</li><li>When deactivated the machine will receive MIDI events only the base channel (10).<br>
</li></ul></li><li><i>CONTROL + V:</i> Mono on/off toggle.<br>
<ul><li>When activated the machine operates in mono and does not mix-down the stereo channel.<br>
</li><li>When deactivated the machine operates in mono and does the mix-down of the stereo channel.<br>
</li></ul></li><li><i>CONTROL + E:</i> quits the software.<br>
</li><li><i>CONTROL + T:</i> changes the MIDI base note, by default it is 60 (middle c).<br>
</li><li><i>CONTROL + J:</i> changes the MIDI base channel, by default it is the channel 10.</li></ul>

<h2>Engine DSP controllers</h2>
<ul><li><i>Transpose:</i> apply a pitch transposition by the given octaves to all voices.<br>
</li><li><i>Stretch:</i> time-stretch (does not change pitch) all voices by the given percentage.<br>
</li><li><i>Volume:</i> decrease or increase the voice gain.</li></ul>

<h2>Voice DSP controllers</h2>
<ul><li><i>Start:</i> advance the sample start point by the given miliseconds.<br>
</li><li><i>Soft Start:</i> apply a quick attack envelope to the sound start.<br>
</li><li><i>Transpose:</i> apply a pitch transposition by the given octaves.<br>
</li><li><i>Transpose Velocity:</i> affect the transpose value by the note's velocity (by this percentage).<br>
</li><li><i>Transpose Random:</i> affect the transpose value by a random source (by this percentage).<br>
</li><li><i>Stretch:</i> time-stretch (does not change pitch) the voice by the given percentage.<br>
</li><li><i>Stretch Velocity:</i> affect the stretch value by the note's velocity (by this percentage).<br>
</li><li><i>Stretch Type:</i> choose from a set of time-stretch types to suit your sound's needs.<br>
<ul><li><i>Auto:</i> should suit any sound type automatically.<br>
</li><li><i>Speech:</i> this type is well suited for speech sounds.<br>
</li><li><i>Type 1:</i> alternative mode to choose from.<br>
</li><li><i>Type 2:</i> alternative mode to choose from.<br>
</li></ul></li><li><i>Over Drive:</i> overdrives the sound by the given amount, you have to activate the Saturation toggle.<br>
</li><li><i>Saturate:</i> activates the saturation of the sound by the given Over Drive amount.<br>
</li><li><i>Filter Frequency:</i> middle point frequency for the different filters.<br>
</li><li><i>Filter:</i> activate or deactivate the filter.<br>
</li><li><i>Filter Type</i>: there are five types of filter to choose from.<br>
<ul><li><i>Low Pass</i> filter.<br>
</li><li><i>High Pass</i> filter.<br>
</li><li><i>Band Pass 1</i> filter.<br>
</li><li><i>Band Pass 2</i> filter (an alternative implementation).<br>
</li><li><i>Notch</i> filter.<br>
</li></ul></li><li><i>Filter Resonance:</i> controls the filter resonance value.<br>
</li><li><i>Filter Velocity:</i> affect the filter frequency value by the note's velocity (by this percentage).<br>
</li><li><i>Filter Random:</i> affect the filter frequency value by a random source (by this percentage).<br>
</li><li><i>Decay:</i> sets the envelope decay time in seconds<br>
</li><li><i>Decay Envelope:</i> there are three envelope types to choose from.<br>
<ul><li><i>Infinite:</i> no envelope applied when this type is selected, the voice 'just' plays.<br>
</li><li><i>Trigger:</i> the decay envelope starts after a note on event.<br>
</li><li><i>Gate:</i> the decay envelope starts after a note off event.<br>
</li></ul></li><li><i>Panning:</i> displaces the voice to the left or right of the stereo mix.<br>
</li><li><i>Panning Velocity:</i> affect the panning value by the note's velocity (by this percentage).<br>
</li><li><i>Panning Random:</i> affect the panning value by a random source (by this percentage).<br>
</li><li><i>Volume:</i> decrease or increase the voice gain.<br>
</li><li><i>Volume Velocity:</i> affect the volume value by the note's velocity (by this percentage).<br>
</li><li><i>Mute:</i> mute the selected voice.<br>
</li><li><i>Solo:</i> solo the selected voice.<br>
</li><li><i>Join:</i> stops the voice 7 when the voice 8 is started and viceversa.<br>
.<br>
<h1>MIDI controllers</h1></li></ul>

<h2>Engine controllers</h2>
This are the controllers for the repulse engine, it listens to this controllers on the base channel 10 and 11<br>
Only the engine controllers are listened on any channel (omni).<br>
<ul><li>You can send a <i>Program Change</i> to select between the first 128 presets in the machine.<br>
</li><li>Sliding the <i>Pitch Wheel</i> MIDI controller it is possible to apply a pitch transposition to all the voices.<br>
</li><li>You can switch the <i>Wheel</i> toggle to make the <i>Pitch Wheel</i> control the overall time-stretch.</li></ul>

<table><thead><th> <b>Name</b>              </th><th> <b>#</b> </th><th> <b>Description</b> </th></thead><tbody>
<tr><td> Program Change           </td><td> -        </td><td> Select a program between the 128 first contained in the machine. </td></tr>
<tr><td> Pitch Wheel              </td><td> -        </td><td> Apply a pitch transposition by the given octaves to all the voices. </td></tr>
<tr><td> Linked                   </td><td> 2        </td><td> Stops the voice 7 when the voice 8 is started and viceversa (on/off). </td></tr>
<tr><td> Rranspose                </td><td> 4        </td><td> Apply a pitch transposition by the given octaves to all the voices (signed). </td></tr>
<tr><td> Stretch                  </td><td> 5        </td><td> Apply time-stretch by the given percentage to all the voices (signed). </td></tr>
<tr><td> Volume                   </td><td> 7        </td><td> Decrease or increase the voice gain. </td></tr>
<tr><td> All Sound Off            </td><td> 120      </td><td> Turns all the voices off (on/off). </td></tr>
<tr><td> All Controllers Off      </td><td> 121      </td><td> Resets all the machine parameters to a default state (on/off). </td></tr>
<tr><td> Local Keyboard           </td><td> 122      </td><td> When activated the machine will interpret MIDI events and local surface events (on/off). </td></tr>
<tr><td> All Notes Off            </td><td> 123      </td><td> Turns all the voices off and sends a note off event to each voice (on/off). </td></tr>
<tr><td> Omni Mode Off            </td><td> 124      </td><td> Listen MIDI events only on channels 10 and 11 (on/off). </td></tr>
<tr><td> Omni Mode On             </td><td> 125      </td><td> Listen MIDI events on any channel (on/off). </td></tr>
<tr><td> Mono Operation           </td><td> 126      </td><td> Disable the stereo mix-down (on/off). </td></tr>
<tr><td> Poly Operation           </td><td> 127      </td><td> Enable the stereo mix-down (on/off). </td></tr></tbody></table>

<h2>Voice controllers</h2>
Almost every parameter in the machine can be controlled via MIDI.<br>
<br>The number of controllers spans two midi channels, base channels 10 and 11.<br>
In MIDI channel 10 you have the common controllers, in MIDI channel 11 you can find the modulation controllers.<br>
The omni switch does not affect the voice controllers, you have to send them in their own channel (base channel 10 and 11).<br>
The engine related controllers can received in omni (view the previous table).<br>

<ul><li>The machine always receives voice controllers on channels 10 and 11. The MIDI base channel can be changed via the GUI.<br>
</li><li>The machine receives note on message for the MIDI notes 60 to 67. The MIDI base note can be changed via the GUI.</li></ul>

This is the full list of voice (and engine) controllers:<br>
<br>
<h3>Channel 10</h3>
<table><thead><th>  <b>Controller</b> </th><th> <b>Engine</b> </th><th> <b>Voice 01</b> </th><th> <b>Voice 02</b> </th><th> <b>Voice 03</b> </th><th> <b>Voice 04</b> </th><th> <b>Voice 05</b> </th><th> <b>Voice 06</b> </th><th> <b>Voice 07</b> </th><th> <b>Voice 08</b> </th></thead><tbody>
<tr><td> Engine Linked      </td><td> 2             </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Engine Transpose   </td><td> 4             </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Engine Stretch     </td><td> 5             </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Engine Volume      </td><td> 7             </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Sound Start        </td><td> -             </td><td> 8               </td><td> 22              </td><td> 36              </td><td> 50              </td><td> 64              </td><td> 78              </td><td> 92              </td><td> 106             </td></tr>
<tr><td> Sound Start Soft   </td><td> -             </td><td> 9               </td><td> 23              </td><td> 37              </td><td> 51              </td><td> 65              </td><td> 79              </td><td> 93              </td><td> 107             </td></tr>
<tr><td> Sound Transpose    </td><td> -             </td><td> 10              </td><td> 24              </td><td> 38              </td><td> 52              </td><td> 66              </td><td> 80              </td><td> 94              </td><td> 108             </td></tr>
<tr><td> Sound Stretch      </td><td> -             </td><td> 11              </td><td> 25              </td><td> 39              </td><td> 53              </td><td> 67              </td><td> 81              </td><td> 95              </td><td> 109             </td></tr>
<tr><td> Sound Over Drive   </td><td> -             </td><td> 12              </td><td> 26              </td><td> 40              </td><td> 54              </td><td> 68              </td><td> 82              </td><td> 96              </td><td> 110             </td></tr>
<tr><td> Sound Over Drive Active </td><td> -             </td><td> 13              </td><td> 27              </td><td> 41              </td><td> 55              </td><td> 69              </td><td> 83              </td><td> 97              </td><td> 111             </td></tr>
<tr><td> Sound Filter Frequency </td><td> -             </td><td> 14              </td><td> 28              </td><td> 42              </td><td> 56              </td><td> 70              </td><td> 84              </td><td> 98              </td><td> 112             </td></tr>
<tr><td> Sound Filter Active </td><td> -             </td><td> 15              </td><td> 29              </td><td> 43              </td><td> 57              </td><td> 71              </td><td> 85              </td><td> 99              </td><td> 113             </td></tr>
<tr><td> Sound Filter Resonance </td><td> -             </td><td> 16              </td><td> 30              </td><td> 44              </td><td> 58              </td><td> 72              </td><td> 86              </td><td> 100             </td><td> 114             </td></tr>
<tr><td> Sound Decay        </td><td> -             </td><td> 17              </td><td> 31              </td><td> 45              </td><td> 59              </td><td> 73              </td><td> 87              </td><td> 101             </td><td> 115             </td></tr>
<tr><td> Sound Panning      </td><td> -             </td><td> 18              </td><td> 32              </td><td> 46              </td><td> 60              </td><td> 74              </td><td> 88              </td><td> 102             </td><td> 116             </td></tr>
<tr><td> Sound Volume       </td><td> -             </td><td> 19              </td><td> 33              </td><td> 47              </td><td> 61              </td><td> 75              </td><td> 89              </td><td> 103             </td><td> 117             </td></tr>
<tr><td> Engine All Sound Off </td><td> 120           </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Engine All Controllers Off </td><td> 121           </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Engine Local Keyboard </td><td> 122           </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Engine All Notes Off </td><td> 123           </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Engine Omni Mode Off </td><td> 124           </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Engine Omni Mode On </td><td> 125           </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Engine Mono Operation </td><td> 126           </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr>
<tr><td> Engine Poly Operation </td><td> 127           </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td><td> -               </td></tr></tbody></table>

<h3>Channel 11</h3>
<table><thead><th> <b>Controller</b> </th><th> <b>Engine</b> </th><th> <b>Voice 01</b> </th><th> <b>Voice 02</b> </th><th> <b>Voice 03</b> </th><th> <b>Voice 04</b> </th><th> <b>Voice 05</b> </th><th> <b>Voice 06</b> </th><th> <b>Voice 07</b> </th><th> <b>Voice 08</b> </th></thead><tbody>
<tr><td> Sound Transpose Velocity </td><td> -             </td><td> 8               </td><td> 22              </td><td> 36              </td><td> 50              </td><td> 64              </td><td> 78              </td><td> 92              </td><td> 106             </td></tr>
<tr><td> Sound Transpose Random </td><td> -             </td><td> 9               </td><td> 23              </td><td> 37              </td><td> 51              </td><td> 65              </td><td> 79              </td><td> 93              </td><td> 107             </td></tr>
<tr><td> Sound Stretch Velocity </td><td> -             </td><td> 10              </td><td> 24              </td><td> 38              </td><td> 52              </td><td> 66              </td><td> 80              </td><td> 94              </td><td> 108             </td></tr>
<tr><td> Sound Stretch Type </td><td> -             </td><td> 11              </td><td> 25              </td><td> 39              </td><td> 53              </td><td> 67              </td><td> 81              </td><td> 95              </td><td> 109             </td></tr>
<tr><td> Sound Filter Type </td><td> -             </td><td> 12              </td><td> 26              </td><td> 40              </td><td> 54              </td><td> 68              </td><td> 82              </td><td> 96              </td><td> 110             </td></tr>
<tr><td> Sound Filter Velocity </td><td> -             </td><td> 13              </td><td> 27              </td><td> 41              </td><td> 55              </td><td> 69              </td><td> 83              </td><td> 97              </td><td> 111             </td></tr>
<tr><td> Sound Filter Random </td><td> -             </td><td> 14              </td><td> 28              </td><td> 42              </td><td> 56              </td><td> 70              </td><td> 84              </td><td> 98              </td><td> 112             </td></tr>
<tr><td> Sound Decay Type  </td><td> -             </td><td> 15              </td><td> 29              </td><td> 43              </td><td> 57              </td><td> 71              </td><td> 85              </td><td> 99              </td><td> 113             </td></tr>
<tr><td> Sound Panning Velocity </td><td> -             </td><td> 16              </td><td> 30              </td><td> 44              </td><td> 58              </td><td> 72              </td><td> 86              </td><td> 100             </td><td> 114             </td></tr>
<tr><td> Sound Panning Random </td><td> -             </td><td> 17              </td><td> 31              </td><td> 45              </td><td> 59              </td><td> 73              </td><td> 87              </td><td> 101             </td><td> 115             </td></tr>
<tr><td> Sound Volume Velocity </td><td> -             </td><td> 18              </td><td> 32              </td><td> 46              </td><td> 60              </td><td> 74              </td><td> 88              </td><td> 102             </td><td> 116             </td></tr>
<tr><td> Sound Muted       </td><td> -             </td><td> 19              </td><td> 33              </td><td> 47              </td><td> 61              </td><td> 75              </td><td> 89              </td><td> 103             </td><td> 117             </td></tr>
<tr><td> Sound Soloed      </td><td> -             </td><td> 20              </td><td> 34              </td><td> 48              </td><td> 62              </td><td> 76              </td><td> 90              </td><td> 104             </td><td> 118             </td></tr></tbody></table>

<h2>Modulation grid</h2>
This table shows the modulation capabilities, and which parameter is modulated by each of the two possible sources:<br>
<br>
<table><thead><th> <b>Parameter</b> </th><th> <b>Note</b><br>Velocity</th><th> <b>Random</b><br>Source</th></thead><tbody>
<tr><td> Transpose        </td><td> x                      </td><td> x                      </td></tr>
<tr><td> Stretch          </td><td> x                      </td><td>                        </td></tr>
<tr><td> Filter Frequency </td><td> x                      </td><td> x                      </td></tr>
<tr><td> Panning          </td><td> x                      </td><td> x                      </td></tr>
<tr><td> Volume           </td><td> x                      </td><td>                        </td></tr></tbody></table>

<br>

<h1>Editing patches</h1>
The patch editing (creating, renaming, deleting) capabilities are quite small.<br>
You can create a patch file by saving a text file with this content:<br>
<br>
<pre><code>&lt;Repulse&gt;<br>
    &lt;Waves&gt;<br>
        &lt;Wave file="909/BT7A0DA.WAV" /&gt;<br>
        &lt;Wave file="909/RIDED2.WAV" /&gt;<br>
        &lt;Wave file="909/ST7T7S7.WAV" /&gt;<br>
        &lt;Wave file="909/HANDCLP1.WAV" /&gt;<br>
        &lt;Wave file="909/RIM63.WAV" /&gt;<br>
        &lt;Wave file="909/LT0D0.WAV" /&gt;<br>
        &lt;Wave file="909/HHCDA.WAV" /&gt;<br>
        &lt;Wave file="909/HHODA.WAV" /&gt;<br>
    &lt;/Waves&gt;<br>
&lt;/Repulse&gt;<br>
</code></pre>

And introducing a <b>file</b> value that corresponds to the location of your wave file.<br>
The machine will look for wave files relative to its directory, you can enter absolute paths too.<br>
Check the <a href='EditingPatches.md'>Editing Patches</a> page for more information on how to edit the xml file.<br>
<br>
<h1>Tips and tricks</h1>

There is an extensive list of ideas you can test in the <a href='TipsAndTricks.md'>Tips And Tricks</a> page.<br>
Be sure to check this page to get some interesting ideas on how to use the machine.<br>
<br>
<h1>Utilised libraries</h1>
This software depends on a number of great libraries:<br>
<br>
<ul><li><a href='http://www.alsa-project.org'>ALSA</a> Audio and MIDI functionality.<br>
</li><li><a href='http://jackaudio.org'>Jack</a> Real-time, low latency audio and MIDI.<br>
</li><li><a href='http://www.mega-nerd.com/libsndfile'>SndFile</a> An unified sound file access library.<br>
</li><li><a href='http://www.mega-nerd.com/SRC'>Secret Rabbit Code</a> A samplerate conversion library (libsamplerate),<br>
</li><li><a href='http://www.surina.net/soundtouch'>SoundTouch</a> SOLA time stretching library.<br>
</li><li><a href='http://www.grinninglizard.com/tinyxml'>TinyXML</a> A small XML library.<br>
</li><li><a href='http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT'>SFMT</a> A fast <a href='http://en.wikipedia.org/wiki/Mersenne_twister'>Merssene Twister</a> implementation.</li></ul>

<h1>Team</h1>

This is the great team the makes repulse possible:<br>
<br>
<ul><li>Jaume Aguilera: adviser and tester.<br>
</li><li>Juan Carlos Rodrigo: lead coder and tester.