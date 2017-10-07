package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionJump
 * @author: David Strite
 */
public class InstructionJump extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionJump(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if the pcode address is invalid
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	getProgram().validatePcodeAddress(getY());
	interpreter.getCurrentProcess().setCurrentPcode(getY());
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "JUMP to "+getY();
}
}
