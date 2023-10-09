# vmcli

## Examples
Here are a couple of CMD scripts as examples of what you can do with vmcli.
They are scripts that i use myself.
I have placed vmcli.exe in C:\MyPrograms\vmcli, the scripts adds that to their PATH.

### mutem.cmd

If Strip 1 is (un)muted, (un)mute strips 2 and 3 too.

### apply_OL_meeting_routing.cmd

Sets up the routing and mute state of all strips and buses, for when I go into on-line meetings.

Routes hardware strip 1 to B2 and B3. Routes viritual AUX to A2, A3 (two pairs of headphones) and B3.  B3 is my recording source.
Mutes all inputs except hardware strip 1 (my microphone) and virtual AUX (where Skype, Zoom, Teams, Discord etc. come in).

### apply_DEFAULT_routing.cmd

Reverts the above to the settings I normally use, when not in an on-line meeting.

### apply_OL_meeting_settings.cmd

Sets up the routing and gainlayers (SEL) I use when in an on-line meeting.

### apply_DEFAULT_settings.cmd

Sets up the routing and gainlayers (SEL) I normally use.

### MacroButton_mutem.xml

A Macrobutton definition that executes the script C:\MyPrograms\vmcli\mutem.cmd

![Load it into a button.](/examples/mb_load.png)


