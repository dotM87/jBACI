package baci.program;

import baci.interpreter.*;
/**
 * InstructionCoend
 * @author: David Strite
 */
public class InstructionCoend extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionCoend(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	interpreter.setInCobeginBlock(false);
	//Suspend main process
	BaciProcess mainProc = interpreter.getProcess(0);
	mainProc.setActive(false);
	mainProc.setSuspend(null);
}
/**
 * Gets the description of this instruction
 * @return String
 */
protected String getDescription() {
	return "COEND";
}
}
