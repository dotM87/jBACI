package baci.program;

import baci.interpreter.*;
/**
 * InstructionWhichProc
 * @author: David Strite
 */
public class InstructionWhichProc extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionWhichProc(Program program, int x, int y) {
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
	top++;
	currProc.setTop(top);
	stack.set(top,currProc.getIndex());
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "WHICHPROC, push # current process";
}
}
