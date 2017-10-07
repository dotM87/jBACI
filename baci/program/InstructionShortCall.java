package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionShorCall
 * @author: David Strite
 */
public class InstructionShortCall extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionShortCall(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if pcode address is invalid
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	int y = getY();
	Program program = getProgram();
	program.validatePcodeAddress(y);
	BaciProcess currProc = interpreter.getCurrentProcess();
	interpreter.setShortCallAddress(currProc.getCurrentPcode());
	currProc.setCurrentPcode(y);
	program.buildVariables(currProc.getVariableModel(),program.getPcode(currProc.getCurrentPcode()).getBlockIndex(),currProc.getBottom());
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "SHORTCALL to "+getY()+", shortcall_reg = pc, pc = "+getY();
}
}
