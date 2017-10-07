package baci.program;

import baci.interpreter.*;
/**
 * InstructionStoreSemaphore
 * @author: David Strite
 */
public class InstructionStoreSemaphore extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionStoreSemaphore(Program program, int x, int y) {
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
	Address address = stack.getAddress(top-1);
	address.getProcess().getStack().set(address.getIndex(),stack.get(top));
	top-=2;
	currProc.setTop(top);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "STORE_SEM, s[s[t-1]]=sem_check(s[t]), pop(2)";
}
}
