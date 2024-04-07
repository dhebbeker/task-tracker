# Debouncing Approaches

Obtaining the state of switches and buttons generally requires to debounce the input signal because of contact bouncing.
The waveform of an input may be smoothed physically using a low-pass filter.
Coping with bouncing programmatically may allow a simplified circuit thus reducing complexity and material cost.

Debouncing in software follows in general one of these approaches:

1. steady sampling of the signals and waiting for either
  1. an average value to cross a threshold
  2. a sequence of continuous states to reach a minimum length
2. observing processor interrupts triggered by state changes of digital inputs
  1. after a state change, ignore further changes for a specified time before adopting the next change
  2. after a state change, wait for the state to be stable for a specified time before adopting that change

Steady sampling has disadvantages:

- if the sample rate is too low, the reaction may be significantly delayed
- if the number of samples is too low, a state change may be prematurely detected (state changes while still bouncing)
- constantly polling the state
  - may bind processing time
  - may require context switching
  - may be solved (more) efficiently using the processor's hardware timers and counters (availability depends on the MPU)

Interrupt based sampling only consumes processing time if actual changes occur.
This is especially advantageous with buttons, which are seldomly actuated, compared to other activities.

Also interrupt based sampling is not flawless.
The specific disadvantages depend on the chosen approach and the concrete implementation.

In any case it must be considered that detecting a steady state change of a button should probably be executed with a higher priority than the execution of the processing of that event.
For example periodically sampling or reacting on an digital input should interrupt time-insensitive processing.
But the actual processing of a finally adopted state change may take longer and should take place with lower priority.
Thus some kind of synchronization mechanism (or interrupt masking) will usually be necessary.

Hereafter two different approaches based on hardware interrupts will be discussed.


## Interrupt based Debouncing

Interrupts called by the processor as a reaction on a change of a digital input may be used to detect state changes of switches and buttons.
In contrast to debouncing approaches which sample with fixed periods, interrupt based debouncing usually uses the time relative the the last state change to detect steady states.
Either by discarding changes which occur shortly after a previously adopted state change.
Or by waiting for the last change to prevail long enough.


### Post-change delay

Approaches which ignore changes which occur in a short period after a previously accepted change are easy to implement.
One simply has to compare the timestamp of the last accepted change with the current time.
If that difference is below a threshold, the state change will be ignored.

\include{doc} bouncing-classic-normal.puml

The timing diagram above shows how, when the button is pressed, a rising edge will be detected.
But also bouncing will occur for some time.
A threshold defines a window (highlighted section) beginning with the first event in which following state changes will be ignored.
After that window, if the button is released the falling edge, and therefor the state change, will immediately be detected.

The debounced signal will be active for the time between button pressed and released.

\include{doc} bouncing-classic-too-short.puml

A problem arises in case the button press (the pulse) is too short.
The first state change will be detected.
But if bouncing finishes within the window for ignoring changes, the final state (released) will not be detected.
The debounced state will remain "active" until further state changes.

As the time a button bounces may differ on each actuation, defining a threshold value totally avoiding this problem may not be possible.


### Pre-change delay

A different interrupt based approach waits for the input signal to be stable before adopting its change.
This can be achieved with a delay which will be restarted each time a state change occurs.
If the delay elapses without further changes the state is considered as stable.
The current state is then read to check for a state change.

The duration of the delay can be much shorter than the window in which state changes will be ignored from the previous approach.
In contrast to defining the expected time after which a signal will be stable it defines the minimum time a signal may not change to be accepted as stable.
Thus the duration is more a requirement then a heuristic.

\include{doc} bouncing-delay-normal.puml

In the timing diagram above is shown that the debouncer will wait while the digital input bounces.
After bouncings stops the delay time will elapse.
Only then the state of the digital input is read to detect the state change.

The debounced signal will be active for a duration which depends on the bouncing while releasing the button.
If bouncing takes the same time while pressing and releasing, the active duration will be the same as the pressed duration.

The debounced state will be delayed by the bouncing time and the defined startup delay.

\include{doc} bouncing-delay-too-short.puml

An advantage to the previous approach can be seen with short pulses.
If the button is pressed and then released while still bouncing (or withing the delay), no state change will be adopted.
Too short button presses do not lead to adopt only one of two (or more) state changes.
This is the safer approach.

