package baci.program;

import baci.interpreter.*;
/**
 * InstructionWait
 * @author: David Strite
 */
public class InstructionWait extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionWait(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();

	int top = currProc.getTop();
	Address address = stack.getAddress(top);
	top--;
	currProc.setTop(top);
	int semValue = address.getProcess().getStack().getInt(address.getIndex());
	if(semValue > 0) {
		address.getProcess().getStack().set(address.getIndex(),semValue-1);
	} else {
		currProc.setSuspend(address);
		currProc.setActive(false);
		interpreter.setNumInstBeforeSwap(0);
	}
	
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "WAIT(s[s[t]]), pop(1)";
}
}
