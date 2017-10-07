package baci.program;

import baci.interpreter.*;
/**
 * InstructionCompareString
 * @author: David Strite
 */
public class InstructionCompareString extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionCompareString(Program program, int x, int y) {
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
	top--;
	currProc.setTop(top);
	stack.set(top,string1.compareTo(string2));
}
/**
 * Gets the description of this instruction
 * @return String
 */
protected String getDescription() {
	return "COMPARE_STRING, pop(1), s[t] = strcmp(&s[s[oldt]],&s[s[oldt-1]])";
}
}
