package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionCopyRawString
 * @author: David Strite
 */
public class InstructionCopyRawString extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionCopyRawString(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	Address address = stack.getAddress(currProc.getTop());
	address.getProcess().getStack().set(address.getIndex(),getProgram().getString(getY()));
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "COPY_RAWSTRING from stab["+getY()+"] to s[s[t]], pop(1)";
}
}
