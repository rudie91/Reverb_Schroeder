# Reverb_Schroeder
An implementation of a reverberator on OMAP L138 DSP board with TI's Code Composer Studio (written in C). <br/>
The theory implemented in this system is that from M.R Schroeder's two papers: “Colorless” Artificial Reverberation and Natural Sounding Artificial Reverberation. <br/>
gainslider.gel controls the gain of the first comb filter of the system. <br/>
tauslider.gel controls the delay time of the first comb filter. <br/>
The parameters in Reverb.c are based on a roomwhich is 30 * 50 * 10 meters big with 85% sound reflection. <br/>
Equations used to compute them are the followings.<br/>
<a href="https://www.codecogs.com/eqnedit.php?latex=T&space;=&space;0.163*\frac{V}{\alpha&space;S}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?T&space;=&space;0.163*\frac{V}{\alpha&space;S}" title="T = 0.163*\frac{V}{\alpha S}" /></a>
 , where V is volume, S is surface area and alpha is absoption rate. <br/>
<a href="https://www.codecogs.com/eqnedit.php?latex=\tau&space;=&space;\frac{l}{c}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\tau&space;=&space;\frac{l}{c}" title="\tau = \frac{l}{c}" /></a>
 , where l is the shortest length of a room and c is speed of sound. <br/>
<a href="https://www.codecogs.com/eqnedit.php?latex=g&space;=&space;10^{\frac{-3\tau}{T}}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?g&space;=&space;10^{\frac{-3\tau}{T}}" title="g = 10^{\frac{-3\tau}{T}}" /></a>
 , where g is the gain of the first comb filter and tau is the delay time of the same filter.
