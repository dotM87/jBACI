package baci.program;

import baci.interpreter.*;
/**
 * InstructionCompareRawString
 * @author: David Strite
 */
public class InstructionCompareRawString extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionCompareRawString(Program program, int x, int y) {
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

	String string1 = address.getProcess().getStack().getString(address.getIndex());
	String string2 = getProgram().getString(getY());
	stack.set(top,string1.compareTo(string2));
}
/**
 * Gets the description of this instruction
 * @return String
 */
protected String getDescription() {
	return "COMPARE_RAWSTRING, s[t] = strcmp(&stab["+getY()+"],&s[s[t]])";
}
}
