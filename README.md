# vmcli

## License

This is for free non-commercial or educational use only. Please see https://www.vb-audio.com/Services/developers.htm for more information on the license restrictions regarding the VoiceMeeterRemote API that is used by this project.

## Usage

This tool allows you to set different VoiceMeeter properties from the command line:

```
vmcli <property setting> ...
```

The properties are described in the API documentation [here](https://download.vb-audio.com/Download_CABLE/VoicemeeterRemoteAPI.pdf).

## Examples

### Setting the gain of the first input to 0:

```
vmcli Strip[0].Gain=0
```

### Increasing the gain by 10:

```
vmcli Strip[0].Gain+=10
```

### Decreasing the gain by 10:

```
vmcli Strip[0].Gain-=10
```

### Toggling properties:

```
vmcli !Strip[0].Mute
```

### Multiple settings can be changed at once:

```
vmcli Strip[0].Mute=0 Strip[0].Gain=10 Strip[1].Mute=1
```

### Getting values:

```
vmcli Strip[0].Gain
```

Result:

```
Strip[0].Gain=0.000
```

