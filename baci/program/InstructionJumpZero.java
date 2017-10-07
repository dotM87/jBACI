package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionJumpZero
 * @author: David Strite
 */
public class InstructionJumpZero extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionJumpZero(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if the pcode address is invalid
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	int top = currProc.getTop();
	Stack stack = currProc.getStack();
	if(stack.getInt(top)==0) {
		getProgram().validatePcodeAddress(getY());
		currProc.setCurrentPcode(getY());
	}
	currProc.setTop(top-1);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "JZER s[t] to "+getY()+", pop(1)";
}
}
