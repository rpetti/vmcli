@echo off
cd /D %~dp0 
SETLOCAL EnableDelayedExpansion
PATH=C:\MyPrograms\MIDI\sendmidi-windows-1.0.15;C:\MyPrograms\vmcli;%PATH%;
vmcli Strip[0].Mute=1.000 Bus[0].Mute=0.000 Strip[0].A1=0.000 Strip[0].A2=1.000 Strip[0].A3=1.000 Strip[0].A4=0.000 Strip[0].A5=0.000 Strip[0].B1=0.000 Strip[0].B2=1.000 Strip[0].B3=1.000
vmcli Strip[1].Mute=1.000 Bus[1].Mute=1.000 Strip[1].A1=0.000 Strip[1].A2=1.000 Strip[1].A3=1.000 Strip[1].A4=0.000 Strip[1].A5=0.000 Strip[1].B1=0.000 Strip[1].B2=1.000 Strip[1].B3=1.000
vmcli Strip[2].Mute=1.000 Bus[2].Mute=1.000 Strip[2].A1=0.000 Strip[2].A2=1.000 Strip[2].A3=1.000 Strip[2].A4=0.000 Strip[2].A5=0.000 Strip[2].B1=0.000 Strip[2].B2=1.000 Strip[2].B3=1.000
vmcli Strip[3].Mute=1.000 Bus[3].Mute=1.000 Strip[3].A1=0.000 Strip[3].A2=1.000 Strip[3].A3=1.000 Strip[3].A4=0.000 Strip[3].A5=0.000 Strip[3].B1=0.000 Strip[3].B2=1.000 Strip[3].B3=1.000
vmcli Strip[4].Mute=1.000 Bus[4].Mute=1.000 Strip[4].A1=0.000 Strip[4].A2=1.000 Strip[4].A3=1.000 Strip[4].A4=0.000 Strip[4].A5=0.000 Strip[4].B1=0.000 Strip[4].B2=1.000 Strip[4].B3=1.000
vmcli Strip[5].Mute=0.000 Bus[5].Mute=0.000 Strip[5].A1=1.000 Strip[5].A2=1.000 Strip[5].A3=1.000 Strip[5].A4=0.000 Strip[5].A5=0.000 Strip[5].B1=0.000 Strip[5].B2=0.000 Strip[5].B3=0.000
vmcli Strip[6].Mute=0.000 Bus[6].Mute=0.000 Strip[6].A1=0.000 Strip[6].A2=1.000 Strip[6].A3=1.000 Strip[6].A4=0.000 Strip[6].A5=0.000 Strip[6].B1=0.000 Strip[6].B2=0.000 Strip[6].B3=1.000
vmcli Strip[7].Mute=0.000 Bus[7].Mute=0.000 Strip[7].A1=0.000 Strip[7].A2=0.000 Strip[7].A3=0.000 Strip[7].A4=0.000 Strip[7].A5=0.000 Strip[7].B1=0.000 Strip[7].B2=0.000 Strip[7].B3=0.000

exit
