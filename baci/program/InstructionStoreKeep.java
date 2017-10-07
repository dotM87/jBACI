package baci.program;

import baci.interpreter.*;
/**
 * InstructionStoreKeep
 * @author: David Strite
 */
public class InstructionStoreKeep extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionStoreKeep(Program program, int x, int y) {
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
	address.getProcess().getStack().set(address.getIndex(),stack.getInt(top));
	stack.set(top-1,stack.getInt(top));
	currProc.setTop(top-1);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "STORE_KEEP, s[s[t-1]]=s[t], s[t-1]=s[t], pop(1)";
}
}
