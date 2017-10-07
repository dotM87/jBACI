package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import baci.program.*;
import java.awt.event.*;
/**
 * Action button to step one source line
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class SourceStepButton extends BaciActionButton {
/**
 * Construct new SourceStepButton
 */
public SourceStepButton() {
	super(Config.StepSource, Config.StepSourceMN, Config.StepSourceAC);
}
/**
 * Step one line of source
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
	Interpreter interpreter = getDebugger().getInterpreter();
/*
	BaciProcess currProc = interpreter.getCurrentProcess();
	Program program = interpreter.getProgram();
	int currPcode = currProc.getCurrentPcode();
	PcodeInstruction instruction = program.getPcode(currPcode);
	int fileIndex = instruction.getFileIndex();
	int lineIndex = instruction.getFileLineIndex()+1;
	int index = -1;
	if (instruction instanceof InstructionCobegin) // To avoid problems at Coend
		interpreter.setStopOnSwap(true);
	while(index < 0) {
		index = program.getMappingFirst(fileIndex,lineIndex);
		lineIndex++;
	}
	interpreter.getBreakpointManager().addTempBreakpoint(index,currProc);
 */
	interpreter.setSourceStepping(true);
	interpreter.setRunning(true);
}
}
