# MIDI controllers #

## Engine controllers ##
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
The omni switch does not affect the voice controllers, you have to send them in their own channel.<br>
The engine related controllers can received in omni.<br>
This is the full list of voice (and engine) controllers.<br>
<br>
<ul><li>The machine always receives voice controllers on channels 10 and 11. The MIDI base channel can be changed via the GUI.<br>
</li><li>The machine receives note on message for the MIDI notes 60 to 67. The MIDI base note can be changed via the GUI.</li></ul>

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
<tr><td> Sound Soloed      </td><td> -             </td><td> 20              </td><td> 34              </td><td> 48              </td><td> 62              </td><td> 76              </td><td> 90              </td><td> 104             </td><td> 118             </td></tr>