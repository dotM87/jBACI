package baci.program;

import baci.interpreter.*;
/**
 * InstructionValueAt
 * @author: David Strite
 */
public class InstructionValueAt extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionValueAt(Program program, int x, int y) {
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
	stack.set(top,address.getProcess().getStack().get(address.getIndex()));
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "VALUE_AT, s[t]=s[s[t]]";
}
}
