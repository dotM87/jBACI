package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionRevive
 * @author: David Strite
 */
public class InstructionRevive extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionRevive(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if process number is invalid
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();

	int top = currProc.getTop();
	int index = stack.getInt(top);

	top--;
	currProc.setTop(top);

	BaciProcess selectedProc;

	try {
		selectedProc= interpreter.getProcess(index);
	} catch (IndexOutOfBoundsException e) {
		throw new BaciException("Invalid process number for REVIVE");
	}

	if(selectedProc.getIdentifierIndex()<0) {
		throw new BaciException("Invalid process number for REVIVE");
	}

	selectedProc.setActive(true);
	selectedProc.setSuspend(null);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "REVIVE process # s[t]";
}
}
