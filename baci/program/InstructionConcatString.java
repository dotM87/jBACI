package baci.program;

import baci.interpreter.*;
/**
 * InstructionConcatString
 * @author: David Strite
 */
public class InstructionConcatString extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionConcatString(Program program, int x, int y) {
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

	Address address1 = stack.getAddress(top-1);
	Address address2 = stack.getAddress(top);

	String string1 = address1.getProcess().getStack().getString(address1.getIndex());
	String string2 = address2.getProcess().getStack().getString(address2.getIndex());
	address1.getProcess().getStack().set(address1.getIndex(),string1+string2);
	top-=2;
	currProc.setTop(top);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "CONCAT_STRING at s[s[t]] to string at s[s[t-1]], pop(2)";
}
}
