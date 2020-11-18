

08.10.2020	1st Lesson, powerup of Apus with skeleton C-project

15.10.2020	2nd Lesson, added signaling-GPIOs to all Tasks

19.10.2020	wrapping GPIO-Functions into abstractions, added basic course-protocol

21.10.2020	separate FW-projects for part A and B of the first assignment

22.10.2020	Poti works a little bit, MandelBort breaks after half image

23.10.2020	separate INTERVAL defines for every Task

05.11.2020	2nd lesson: copies of 1st project and implementation of an RTOS

12.11.2020	dummy tasks A, B and C to fill Registrs with detectable Data


18.11.2020	...


	// tryGPIO();
	
	// Hinweise von Langer aus der letzten Ü:
	//	PSP porcess stack pointer
	//	MSP master stack pointer
	//	PndSV interrupt for context switch
	//	pTask enthält zB stackpointer
	//	TimeSlice = wieviel Slices möcht ich für den Task
	// bei HardFaults: Stack, Speicher, Register anschauen, Screenshots machen beim Debuggen, wie is vorher/nacher
	//	sysTick auf 1 ms
	//	sysTick setzt PenSV (handelt auch Interrupts, die ich nicht behandle zB SPIs oä)
	//	-> interrupt
	//	-> APUS Scheduler
		
	//	Context Switch, sichern:
	//		SPH SPL
	//		Register R4...R11
	//		PC
		
	//	befüllen:
	//		xPSR 						enthält zB T-Switch
	//		ProgramCounter	zum rückspringen

	// 0xFFFFFFFD ins LR beim rücksprung?

	
So hab grad gepushed. es funktioniert jetzt alles bis auf kleine schönheitsfehler. Steht eh alles im Commit

ok hab grad bei den kollegen nachgefragt und wir machen noch was nicht richtig.
das CONTROL reg müsste auf 2 gestellt werden
dann muss ma den psp setzen und den stack befüllen. Also die pRoutine und das LR müssen am stack sein genauso wie alle register damit der TaskSwitch dann funktioniert.

vl kannst du des mal probieren
wenn mas ned hinbekommen können wirs ja zur not auch so abgeben :sweat_smile:


Mögliche Lösung:
Eine Funktion InitStack die bei taskCreate aufgerufen wird.
Wenn der stack beim ersten Aufruf korrekt ist sollt der Rest auch funktionieren. 
Also CONTROL muss dann noch auf 2 gestellt werden.
Als LR einfach den scheduler	
Eventuell muss dann im scheduler noch a Kleinigkeit geändert werden aber des muss ma schauen.

Änderungen:
  - PendSV handler funktioniert
  - Tasks: Counter, Key, Watch, ... eingetragen und getestet
  - Mandelbrot neu implementiert und getestet

TODO:
  - Text "ManBr" wird nur am anfang ausgegeben dadurch wird die
	Farbe nicht geändert bei button User0
  - Interval für Funktionen einstellen
  - R4 wird im Scheduler für den Rücksprung( scheduler -> start)
    verwendet und ist damit nicht korrekt beim aufruf des nächsten Tasks
    (ignorieren?)
  - Doku