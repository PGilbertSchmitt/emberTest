#JASON
These are the two tests.

## Sound Ramp Test
A test of the ability to change volume over time using the vs1053 chip. The three volume labels are clearly marked, so you can modify them if you feel the need. You can change the length of time that it plays for and whether or not it's the daytime via the variables totalTime and dayTime respectively. The length of time that the sound takes to ramp up/down and to change from full to half volume is changed via the preprocessor macros defined as PERIOD and SWITCH respectively. The way it's set right now is:

1. Ramp up to full volume for two seconds
2. Play for six seconds at full volume
3. Ramp down to half volume for two seconds
4. Play at half volume for the remaining amount of time (18 seconds)
5. Ramp down to zero volume for two seconds

## Strain Test
Prints a number between 0 and 255 (I think) every half second. That number is the signal being returned by the strain guage. I think it gets lower the more weight there is, I forget. All I know is you can use that to figure out the readings that you want, and I can simply plug that data into the final program to use as the weight threshold.
