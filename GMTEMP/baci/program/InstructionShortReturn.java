package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionShortReturn
 * @author: David Strite
 */
public class InstructionShortReturn extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionShortReturn(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if pcode address is invalid
 */
public void doInstruction(Interpreter interpreter) throws BaciException{
	int index = interpreter.getShortCallAddress();
	Program program = getProgram();
	program.validatePcodeAddress(index);
	BaciProcess currProc = interpreter.getCurrentProcess();
	currProc.setCurrentPcode(index);
	program.buildVariables(currProc.getVariableModel(),program.getPcode(currProc.getCurrentPcode()).getBlockIndex(),currProc.getBottom());
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "SHORTRET, pc = shortcall_reg";
}
}
