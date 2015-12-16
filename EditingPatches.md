# Editing patches #
Tip: you can reload the XML file by pressing CONTROL + U while repulse is running.

## Creating a patch File ##
You can create a preset by saving a text file with this content:

```
<Repulse>
    <Waves>
        <Wave file="909/BT7A0DA.WAV" />
        <Wave file="909/RIDED2.WAV" />
        <Wave file="909/ST7T7S7.WAV" />
        <Wave file="909/HANDCLP1.WAV" />
        <Wave file="909/RIM63.WAV" />
        <Wave file="909/LT0D0.WAV" />
        <Wave file="909/HHCDA.WAV" />
        <Wave file="909/HHODA.WAV" />
    </Waves>
</Repulse>
```

And introducing a file value that corresponds to the location of your wave files.<br>
The machine will look for wave files relative to the XML file directory, you can enter absolute paths too.<br>

<h2>Deleting patches</h2>

You can delete a whole preset by removing its XML block:<br>
<pre><code>&lt;Presets&gt;<br>
    &lt;Preset name="Preset 1"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
    &lt;Preset name="Preset 2"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
&lt;/Presets&gt;<br>
</code></pre>

Becomes:<br>
<pre><code>&lt;Presets&gt;<br>
    &lt;Preset name="Preset 1"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
&lt;/Presets&gt;<br>
</code></pre>

<h2>Renaming patches</h2>

You can rename a preset by changing its <b>name</b> attribute:<br>
<pre><code>    &lt;Preset name="Preset 1"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
</code></pre>

Becomes:<br>
<pre><code>    &lt;Preset name="My Preset"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
</code></pre>

<h2>Swapping patches</h2>

The first 128 patches found in the XML are assignable via the standard midi <a href='Controllers.md'>controller</a> program change.<br>Sometimes is convenient to shuffle the first 128 pressets around.<br>
You can do this by simply swapping the preset tags:<br>
<pre><code>&lt;Presets&gt;<br>
    &lt;Preset name="Preset 1"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
    &lt;Preset name="Preset 2"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
    &lt;Preset name="Preset 3"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
&lt;/Presets&gt;<br>
</code></pre>

Becomes:<br>
<pre><code>&lt;Presets&gt;<br>
    &lt;Preset name="Preset 2"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
    &lt;Preset name="Preset 1"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
    &lt;Preset name="Preset 3"&gt;&lt;!-- other tags --&gt;&lt;/Preset&gt;<br>
&lt;/Presets&gt;<br>
</code></pre>