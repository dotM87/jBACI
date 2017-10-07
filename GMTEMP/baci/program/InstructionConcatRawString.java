package baci.program;

import baci.interpreter.*;
/**
 * InstructionConcatRawString
 * @author: David Strite
 */
public class InstructionConcatRawString extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionConcatRawString(Program program, int x, int y) {
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
	address.getProcess().getStack().set(address.getIndex(),string1+string2);
	top--;
	currProc.setTop(top);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "CONCAT_RAWSTRING at stab["+getY()+"] to string at s[s[t]], pop(1)";
}
}
