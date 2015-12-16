

# Tips and Tricks #

## Use the overdrive to get an analogue feeling ##
![http://img98.imageshack.us/img98/4528/overdrive.png](http://img98.imageshack.us/img98/4528/overdrive.png)<br>
In this case the <i>overdrive</i> of the first voice is incremented and the <i>saturation</i> is activated. The first voice contains a bass drum sound.<br>

<h2>Create a velocity responsive sampler</h2>
<img src='http://img834.imageshack.us/img834/1143/velocity.png' /><br>
By incrementing all the <i>volume velocity</i> modulations you can create a fully velocity responsive sampler. This is interesting if you want to edit the velocity in your sequencer, or use an external midi controller with pads.<br>

<h2>Try different envelopes</h2>
<img src='http://img219.imageshack.us/img219/5118/envelopez.png' /><br>
We have applied a gate decay <i>envelope type</i> (turns the envelope on after the note off event) to the ride cymbal (voice 2), with a short <i>decay</i>.<br>

<h2>Join the voices 7 and 8</h2>
<img src='http://img262.imageshack.us/img262/62/linked.png' /><br>
Emulates the voice response of an old drum machine. Turning on the voice 7, turns off the voice 8, and viceversa. If you insert hi-hat like samples in the last two voices you can emulate a machine with one voice of polyphony for two samples. Having two samples <i>linked</i> to one voice<br>

<h2>Filter the metal section</h2>
<img src='http://img408.imageshack.us/img408/5984/filterh.png' /><br>
You can make the metal section lighter or heavier by activating the <i>filter</i> option and selecting a <i>low pass</i>, or any other, filter and adjusting the <i>frequency</i> a bit.<br>

<h2>Tune the samples</h2>
<img src='http://img801.imageshack.us/img801/8207/transpose.png' /><br>
This time we have <i>transposed</i> down the ride cymbal and the hi-hats by one octave, and <i>transposed</i> up the bass drum by one octave. This way new registers of the samples can be explored.<br>

<h2>Randomize some parameters</h2>
<img src='http://img153.imageshack.us/img153/7692/randomize.png' /><br>
You can achieve a real analogue feeling by slightly coupling the note velocity and random source to multiple parameters, this way the machine never really does the same thing two times... As you can see we have slightly altered the random value on the voices 2, 7 and 8. In addition you have all the other parameters changed highlighted.<br>

<h2>Reload the patch file</h2>
You can edit the patch XML file and change any parameter while repulse is running. Change the <i>file</i> attribute of the <i>Wave</i> tag and press <i>CONTROL + U</i>, repulse will reload the XML file contents and update the wave files and any other parameter changed in the XML file; but more important you dont have to quit repulse losing all the connection work. You can edit the XML patch file to <a href='EditingPatches.md'>rename, delete or shuffle</a> patches.<br>
<br>
<h2>Swap patches</h2>
The first 128 patches found in the patch file are assignable via the standard midi <a href='Controllers.md'>controller</a> program change.<br>Sometimes is convenient to shuffle the first 128 presets around. You can do this by <a href='EditingPatches.md'>editing the patch file</a>.