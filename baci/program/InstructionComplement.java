package baci.program;

import baci.interpreter.*;
/**
 * InstructionComplement
 * @author: David Strite
 */
public class InstructionComplement extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionComplement(Program program, int x, int y) {
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
	int value = stack.getInt(top)==1?0:1;
	stack.set(top,value);
}
/**
 * Gets the description of this instruction
 * @return String
 */
protected String getDescription() {
	return "COMPLEMENT s[t]";
}
}
