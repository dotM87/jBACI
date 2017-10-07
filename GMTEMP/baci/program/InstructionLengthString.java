package baci.program;

import baci.interpreter.*;
/**
 * InstructionLengthString
 * @author: David Strite
 */
public class InstructionLengthString extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionLengthString(Program program, int x, int y) {
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

	String string = address.getProcess().getStack().getString(address.getIndex());
	stack.set(top,string.length());
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "LENGTH_STRING, s[t] = strlen(&s[s[t]]";
}
}
