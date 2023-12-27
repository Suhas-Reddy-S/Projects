(https://github.com/ECEN5813/assignment-7-Suhas-Reddy-S/assets/143859218/b4273414-659c-49ad-a95f-7d7c0dcc5fe2)[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/y8YVwySe)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=12947357&assignment_repo_type=AssignmentRepo)
# PES Assignment 7: Waveforms

The Project contains all the necessary files required in the source directory:
waveforms.h and waveforms.c which contains functions to initialize Buffers for all the waveforms, fp_trig_sin.c contains a sample sin function which uses 5 terms of taylor series.
dma.c has functions to wirte the analog output to DAC. 
adc.c has functions to read analog input from DAC output and waveform analysis.

Video Showcasing the waveforms switching after 2 seconds is present in WaveFormSequence.mp4 . It reccords more than six cycles to show that there is no tearing at wrap point. It also show the frequency and pk-pk Voltages.


Below file shows the output
![UART _OUTPUT](https://github.com/ECEN5813/assignment-7-Suhas-Reddy-S/assets/143859218/a8cf089a-0d0a-4392-b9d8-f7cf1a2a1c1e)



Below Picture Shows Sine to Triangle Tranisition
![Sine_Triangle](https://github.com/ECEN5813/assignment-7-Suhas-Reddy-S/assets/143859218/dff9e818-f584-47f8-8060-23d1b7a006ab)


Below Picture Shows Square to Sine Tranisition
![Square_Sine](https://github.com/ECEN5813/assignment-7-Suhas-Reddy-S/assets/143859218/4deda22d-e495-41e2-94af-fc59c6a1531a)


Below Picture Shows Triangle to Square Tranisition
![Triangle_Square](https://github.com/ECEN5813/assignment-7-Suhas-Reddy-S/assets/143859218/fe6cb4cf-6d88-4611-a5c8-c414bcac735c)
