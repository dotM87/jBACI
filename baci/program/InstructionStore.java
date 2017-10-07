package baci.program;

import baci.interpreter.*;
/**
 * InstructionStore
 * @author: David Strite
 */
public class InstructionStore extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionStore(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	int top = currProc.getTop();
	Stack stack = currProc.getStack();
	Address address = stack.getAddress(top-1);
	address.getProcess().getStack().set(address.getIndex(),stack.getInt(top));
	currProc.setTop(top-2);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "STORE, s[s[t-1]]=s[t], pop(2)";
}
}
